/*---------------------------------------------------------------------------*\

  FILE........: freedv_rx.c
  AUTHOR......: David Rowe
  DATE CREATED: August 2014

  Demo receive program for FreeDV API functions, some side information
  written to freedv_rx_log.txt

  Example usage (all one line):

    $ cd codec2-dev/build_linux/src
    $ ./freedv_tx 1600 ../../raw/ve9qrp_10s.raw - | ./freedv_rx 1600 - - | aplay -f S16

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2014 David Rowe

  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1, as
  published by the Free Software Foundation.  This program is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "freedv_api.h"
#include "modem_stats.h"

#include "codec2.h"

#define NDISCARD 5                /* BER measure optionally discards first few frames after sync */

struct my_callback_state {
    FILE *ftxt;
};

void my_put_next_rx_char(void *callback_state, char c) {
    struct my_callback_state* pstate = (struct my_callback_state*)callback_state;
    if (pstate->ftxt != NULL) {
        fprintf(pstate->ftxt, "text msg: %c\n", c);
    }
}

void my_put_next_rx_proto(void *callback_state,char *proto_bits){
    struct my_callback_state* pstate = (struct my_callback_state*)callback_state;
    if (pstate->ftxt != NULL) {
        fprintf(pstate->ftxt, "proto chars: %.*s\n",2, proto_bits);
    }
}

/* Called when a packet has been received */
void my_datarx(void *callback_state, unsigned char *packet, size_t size) {
    struct my_callback_state* pstate = (struct my_callback_state*)callback_state;
    if (pstate->ftxt != NULL) {
        size_t i;
	
	fprintf(pstate->ftxt, "data (%zd bytes): ", size);
	for (i = 0; i < size; i++) {
	    fprintf(pstate->ftxt, "0x%02x ", packet[i]);
	}
	fprintf(pstate->ftxt, "\n");
    }
}

/* Called when a new packet can be send */
void my_datatx(void *callback_state, unsigned char *packet, size_t *size) {
    /* This should not happen while receiving.. */
    fprintf(stderr, "datarx callback called, this should not happen!\n");    
    *size = 0;
}

int main(int argc, char *argv[]) {
    FILE                      *fin, *fout, *ftxt;
    struct freedv             *freedv;
    int                        nin, nout, frame = 0;
    struct my_callback_state   my_cb_state;
    struct MODEM_STATS         stats;
    int                        mode;
    int                        sync;
    float                      snr_est;
    float                      clock_offset;
    int                        use_codecrx, use_testframes, interleave_frames, verbose, discard, use_complex, use_dpsk;
    struct CODEC2             *c2 = NULL;
    int                        i;

    
    if (argc < 4) {
        char f2020[80] = {0};
        #ifdef __LPCNET__
        sprintf(f2020,"|2020");
        #endif     
	printf("usage: %s 1600|700|700B|700C|700D|2400A|2400B|800XA%s InputModemSpeechFile OutputSpeechRawFile\n"
               " [--testframes] [--interleaver depth] [--codecrx] [-v] [--discard] [--usecomplex] [--dpsk]\n", argv[0],f2020);
	printf("e.g    %s 1600 hts1a_fdmdv.raw hts1a_out.raw\n", argv[0]);
	exit(1);
    }

    mode = -1;
    if (!strcmp(argv[1],"1600"))
        mode = FREEDV_MODE_1600;
    if (!strcmp(argv[1],"700"))
        mode = FREEDV_MODE_700;
    if (!strcmp(argv[1],"700B"))
        mode = FREEDV_MODE_700B;
    if (!strcmp(argv[1],"700C"))
        mode = FREEDV_MODE_700C;
    if (!strcmp(argv[1],"700D"))
        mode = FREEDV_MODE_700D;
    if (!strcmp(argv[1],"2400A"))
        mode = FREEDV_MODE_2400A;
    if (!strcmp(argv[1],"2400B"))
        mode = FREEDV_MODE_2400B;
    if (!strcmp(argv[1],"800XA"))
        mode = FREEDV_MODE_800XA;
    #ifdef __LPCNET__
    if (!strcmp(argv[1],"2020"))
        mode = FREEDV_MODE_2020;
    #endif
    assert(mode != -1);

    if (strcmp(argv[2], "-")  == 0) fin = stdin;
    else if ( (fin = fopen(argv[2],"rb")) == NULL ) {
	fprintf(stderr, "Error opening input raw modem sample file: %s: %s.\n",
         argv[2], strerror(errno));
	exit(1);
    }

    if (strcmp(argv[3], "-") == 0) fout = stdout;
    else if ( (fout = fopen(argv[3],"wb")) == NULL ) {
	fprintf(stderr, "Error opening output speech sample file: %s: %s.\n",
         argv[3], strerror(errno));
	exit(1);
    }

    use_codecrx = 0; use_testframes = 0; interleave_frames = 1; verbose = 0; discard = 0; use_complex = 0; use_dpsk = 0;

    if (argc > 4) {
        for (i = 4; i < argc; i++) {
            if (strcmp(argv[i], "--testframes") == 0) {
                use_testframes = 1;
            }
            if (strcmp(argv[i], "--codecrx") == 0) {
                int c2_mode;

                if (mode == FREEDV_MODE_700)  {
		    c2_mode = CODEC2_MODE_700;
		} else if ((mode == FREEDV_MODE_700B)|| (mode == FREEDV_MODE_800XA)) {
                    c2_mode = CODEC2_MODE_700B;
                } else if ((mode == FREEDV_MODE_700C)|| (mode == FREEDV_MODE_700D)) {
                    c2_mode = CODEC2_MODE_700C;
                } else {
                    c2_mode = CODEC2_MODE_1300;
                }
                use_codecrx = 1;

                c2 = codec2_create(c2_mode);
                assert(c2 != NULL);
            }

            if (strcmp(argv[i], "--interleave") == 0) {
                interleave_frames = atoi(argv[i+1]);
            }
            if (strcmp(argv[i], "-v") == 0) {
                verbose = 1;
            }
            if (strcmp(argv[i], "-vv") == 0) {
                verbose = 2;
            }
            if (strcmp(argv[i], "--discard") == 0) {
                discard = 1;
            }
            if (strcmp(argv[i], "--usecomplex") == 0) {
                fprintf(stderr, "using complex!\n");
                use_complex = 1;
            }
            if (strcmp(argv[i], "--dpsk") == 0) {
                use_dpsk = 1;
            }
        }
    }

    if ((mode == FREEDV_MODE_700D) || (mode == FREEDV_MODE_2020)) {
        struct freedv_advanced adv;
        adv.interleave_frames = interleave_frames;
        freedv = freedv_open_advanced(mode, &adv);
    }
    else {
        freedv = freedv_open(mode);
    }
    assert(freedv != NULL);

    freedv_set_test_frames(freedv, use_testframes);
    freedv_set_verbose(freedv, verbose);

    freedv_set_snr_squelch_thresh(freedv, -100.0);
    freedv_set_squelch_en(freedv, 0);
    freedv_set_dpsk(freedv, use_dpsk);

    short* speech_out = (short*)malloc(freedv_get_n_speech_samples(freedv) * sizeof(short));
    short* demod_in = (short*)malloc(freedv_get_n_max_modem_samples(freedv) * sizeof(short));

    ftxt = fopen("freedv_rx_log.txt","wt");
    assert(ftxt != NULL);
    my_cb_state.ftxt = ftxt;
    freedv_set_callback_txt(freedv, &my_put_next_rx_char, NULL, &my_cb_state);
    freedv_set_callback_protocol(freedv, &my_put_next_rx_proto, NULL, &my_cb_state);
    freedv_set_callback_data(freedv, my_datarx, my_datatx, &my_cb_state);

    /* Note we need to work out how many samples demod needs on each
       call (nin).  This is used to adjust for differences in the tx and rx
       sample clock frequencies.  Note also the number of output
       speech samples is time varying (nout). */

    nin = freedv_nin(freedv);
    while(fread(demod_in, sizeof(short), nin, fin) == nin) {
        frame++;
        
        if (use_codecrx == 0) {
            /* usual case: use the freedv_api to do everything: speech decoding, demodulating */
            if (use_complex) {
                /* exercise the complex version of the API (useful
                   for testing 700D which has a different code path for
                   short samples) */
                COMP* demod_in_complex = (COMP*)malloc(nin * sizeof(COMP));
                for(int i=0; i<nin; i++) {
                    demod_in_complex[i].real = (float)demod_in[i];
                    demod_in_complex[i].imag = 0.0;
                }
                nout = freedv_comprx(freedv, speech_out, demod_in_complex);
                free(demod_in_complex);
           }
            else {
                // most common interface - real shorts in, real shorts out
                nout = freedv_rx(freedv, speech_out, demod_in);
            }
        } else {
            /* demo of codecrx mode - separate demodulation and speech decoding */
            int bits_per_codec_frame = codec2_bits_per_frame(c2);
            int bytes_per_codec_frame = (bits_per_codec_frame + 7) / 8;
            int codec_frames = freedv_get_n_codec_bits(freedv) / bits_per_codec_frame;
            int samples_per_frame = codec2_samples_per_frame(c2);
            unsigned char* encoded = (unsigned char*)malloc(bytes_per_codec_frame * codec_frames * sizeof(unsigned char));

            /* Use the freedv_api to demodulate only */
            nout = freedv_codecrx(freedv, encoded, demod_in);

            /* decode the speech ourself (or send it to elsewhere, e.g. network) */
            if (nout) {
                unsigned char *enc_frame = encoded;
                short *speech_frame = speech_out;
                
                nout = 0;
                for (i = 0; i < codec_frames; i++) {
                    codec2_decode(c2, speech_frame, enc_frame);
                    enc_frame += bytes_per_codec_frame;
                    speech_frame += samples_per_frame;
                    nout += samples_per_frame;
                }
            }
            free(encoded);
        }

        nin = freedv_nin(freedv);

        fwrite(speech_out, sizeof(short), nout, fout);
        freedv_get_modem_stats(freedv, &sync, &snr_est);
        freedv_get_modem_extended_stats(freedv, &stats);
        int total_bit_errors = freedv_get_total_bit_errors(freedv);
        clock_offset = stats.clock_offset;

       if (discard && (sync == 0)) {
            // discard BER results if we get out of sync, helps us get sensible BER results
            freedv_set_total_bits(freedv, 0); freedv_set_total_bit_errors(freedv, 0);
            freedv_set_total_bits_coded(freedv, 0); freedv_set_total_bit_errors_coded(freedv, 0);
        }

        /* log some side info to the txt file */

        if (ftxt != NULL) {
            fprintf(ftxt, "frame: %d  demod sync: %d  nin: %d demod snr: %3.2f dB  bit errors: %d clock_offset: %f\n",
                    frame, sync, nin, snr_est, total_bit_errors, clock_offset);
       }

	/* if this is in a pipeline, we probably don't want the usual
           buffering to occur */

        if (fout == stdout) fflush(stdout);
        if (fin == stdin) fflush(stdin);
    }

    if (freedv_get_test_frames(freedv)) {
        int Tbits = freedv_get_total_bits(freedv);
        int Terrs = freedv_get_total_bit_errors(freedv);
        float uncoded_ber = (float)Terrs/Tbits;
        fprintf(stderr, "BER......: %5.4f Tbits: %5d Terrs: %5d\n", 
		(double)uncoded_ber, Tbits, Terrs);
        if ((mode == FREEDV_MODE_700D) || (mode == FREEDV_MODE_2020)) {
            int Tbits_coded = freedv_get_total_bits_coded(freedv);
            int Terrs_coded = freedv_get_total_bit_errors_coded(freedv);
            float coded_ber = (float)Terrs_coded/Tbits_coded;
            fprintf(stderr, "Coded BER: %5.4f Tbits: %5d Terrs: %5d\n",
                    (double)coded_ber, Tbits_coded, Terrs_coded);

            free(speech_out);
            free(demod_in);

            /* set return code for Ctest */
            if ((uncoded_ber < 0.1) && (coded_ber < 0.01))
                return 0;
            else
                return 1;
        }
    }

    fclose(ftxt);
    freedv_close(freedv);
    fclose(fin);
    fclose(fout);
    
    free(speech_out);
    free(demod_in);

    return 0;
}

