/*---------------------------------------------------------------------------*\

  FILE........: fsk.h
  AUTHOR......: Brady O'Brien
  DATE CREATED: 6 January 2016

  C Implementation of 2FSK/4FSK modulator/demodulator, based on octave/fsk_horus.m

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2016 David Rowe

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


#ifndef __C2FSK_H
#define __C2FSK_H
#include <stdint.h>
#include "comp.h"
#include "kiss_fftr.h"
#include "modem_stats.h"
#include "dllexport.h"

#define MODE_2FSK 2
#define MODE_4FSK 4

#define MODE_M_MAX 4

#define FSK_SCALE 16383

struct FSK {
    /*  Static parameters set up by fsk_init */
    int Ndft;               /* buffer size for freq offset est fft */
    int Fs;                 /* sample freq */
    int N;                  /* processing buffer size */
    int Rs;                 /* symbol rate */
    int Ts;                 /* samples per symbol */
    int Nmem;               /* size of extra mem for timing adj */
    int P;                  /* oversample rate for timing est/adj */
    int Nsym;               /* Number of symbols spat out in a processing frame */
    int Nbits;              /* Number of bits spat out in a processing frame */
    int f1_tx;              /* f1 for modulator */
    int fs_tx;              /* Space between TX freqs for modulatosr */
    int mode;               /* 2FSK or 4FSK */
    int est_min;            /* Minimum frequency for freq. estimator */
    int est_max;            /* Maximum frequency for freq. estimaotr */
    int est_space;          /* Minimum frequency spacing for freq. estimator */
    float* hann_table;		/* Precomputed or runtime computed hann window table */
    
    /*  Parameters used by demod */
    COMP phi_c[MODE_M_MAX];
    
    kiss_fft_cfg fft_cfg;   /* Config for KISS FFT, used in freq est */
    float norm_rx_timing;   /* Normalized RX timing */
    
    COMP* samp_old;         /* Tail end of last batch of samples */
    int nstash;             /* How many elements are in there */
    
    float* fft_est;			/* Freq est FFT magnitude */
    
    /* Memory used by demod but not important between demod frames */
    
    /*  Parameters used by mod */
    COMP tx_phase_c;        /* TX phase, but complex */ 
    
    /*  Statistics generated by demod */
    float EbNodB;           /* Estimated EbNo in dB */
    float f_est[MODE_M_MAX];/* Estimated frequencies */
    float ppm;              /* Estimated PPM clock offset */
    
    /*  Parameters used by mod/demod and driving code */
    int nin;                /* Number of samples to feed the next demod cycle */
    int burst_mode;         /* enables/disables 'burst' mode */
    
    /*  modem statistic struct */
    struct MODEM_STATS *stats;
    int normalise_eye;      /* enables/disables normalisation of eye diagram */
};

/*
 * Create an FSK config/state struct from a set of config parameters
 * 
 * int Fs - Sample frequency
 * int Rs - Symbol rate
 * int tx_f1 - '0' frequency
 * int tx_fs - frequency spacing
 */
CODEC2_PUBLIC struct FSK * fsk_create(int Fs, int Rs, int M, int tx_f1, int tx_fs);

/*
 * Create an FSK config/state struct from a set of config parameters
 * 
 * int Fs - Sample frequency
 * int Rs - Symbol rate
 * int tx_f1 - '0' frequency
 * int tx_fs - frequency spacing
 */
CODEC2_PUBLIC struct FSK * fsk_create_hbr(int Fs, int Rs, int P, int M, int tx_f1, int tx_fs);

/* 
 * Set a new number of symbols per processing frame
 */
CODEC2_PUBLIC void fsk_set_nsym(struct FSK *fsk,int nsym);

/*
 * Set the minimum and maximum frequencies at which the freq. estimator can find tones
 */
CODEC2_PUBLIC void fsk_set_est_limits(struct FSK *fsk,int fmin, int fmax);

/* 
 * Clear the estimator states
 */
CODEC2_PUBLIC void fsk_clear_estimators(struct FSK *fsk);

/*
 * Fills MODEM_STATS struct with demod statistics
 */
CODEC2_PUBLIC void fsk_get_demod_stats(struct FSK *fsk, struct MODEM_STATS *stats);

/*
 * Destroy an FSK state struct and free it's memory
 * 
 * struct FSK *fsk - FSK config/state struct to be destroyed
 */
CODEC2_PUBLIC void fsk_destroy(struct FSK *fsk);

/*
 * Modulates Nsym bits into N samples
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * float fsk_out[] - Buffer for N samples of modulated FSK
 * uint8_t tx_bits[] - Buffer containing Nbits unpacked bits
 */
CODEC2_PUBLIC void fsk_mod(struct FSK *fsk, float fsk_out[], uint8_t tx_bits[]);

/*
 * Modulates Nsym bits into N samples
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * float fsk_out[] - Buffer for N samples of "voltage" used to modulate an external VCO
 * uint8_t tx_bits[] - Buffer containing Nbits unpacked bits
 */
CODEC2_PUBLIC void fsk_mod_ext_vco(struct FSK *fsk, float vco_out[], uint8_t tx_bits[]);

/*
 * Modulates Nsym bits into N complex samples
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * comp fsk_out[] - Buffer for N samples of modulated FSK
 * uint8_t tx_bits[] - Buffer containing Nbits unpacked bits
 */
CODEC2_PUBLIC void fsk_mod_c(struct FSK *fsk, COMP fsk_out[], uint8_t tx_bits[]);


/*
 * Returns the number of samples needed for the next fsk_demod() cycle
 *
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * returns - number of samples to be fed into fsk_demod next cycle 
 */
CODEC2_PUBLIC uint32_t fsk_nin(struct FSK *fsk);


/*
 * Demodulate some number of FSK samples. The number of samples to be 
 *  demodulated can be found by calling fsk_nin().
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * uint8_t rx_bits[] - Buffer for Nbits unpacked bits to be written
 * float fsk_in[] - nin samples of modualted FSK
 */
CODEC2_PUBLIC void fsk_demod(struct FSK *fsk, uint8_t rx_bits[],COMP fsk_in[]);

/*
 * Demodulate some number of FSK samples. The number of samples to be 
 *  demodulated can be found by calling fsk_nin().
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * float rx_bits[] - Buffer for Nbits soft decision bits to be written
 * float fsk_in[] - nin samples of modualted FSK
 */
CODEC2_PUBLIC void fsk_demod_sd(struct FSK *fsk, float rx_bits[],COMP fsk_in[]);

/* enables/disables normalisation of eye diagram samples */
  
CODEC2_PUBLIC void fsk_stats_normalise_eye(struct FSK *fsk, int normalise_enable);

/* Set the FSK modem into burst demod mode */

CODEC2_PUBLIC void fsk_enable_burst_mode(struct FSK *fsk,int nsyms);

#endif
