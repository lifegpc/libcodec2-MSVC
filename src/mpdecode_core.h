/*
  FILE...: mpdecode_core.h
  AUTHOR.: David Rowe
  CREATED: Sep 2016

  C-callable core functions for MpDecode, so they can be used for
  Octave and C programs.  Also some convenience functions to help use
  the C-callable LDPC decoder in C programs.
*/

#ifndef __MPDECODE_CORE__
#define __MPDECODE_CORE__

#include <stdint.h>

#include "comp.h"
#include "dllexport.h"

struct LDPC {
    int max_iter;
    int dec_type;
    int q_scale_factor;
    int r_scale_factor;
    int CodeLength;
    int NumberParityBits;
    int NumberRowsHcols;
    int max_row_weight;
    int max_col_weight;

    /* these two are fixed to code params */
    int ldpc_data_bits_per_frame;
    int ldpc_coded_bits_per_frame;

    /* these three may vary if we don't use all data bits in code */
    int data_bits_per_frame;
    int coded_bits_per_frame;
    int coded_syms_per_frame;

    uint16_t *H_rows;
    uint16_t *H_cols;
};

CODEC2_PUBLIC void encode(struct LDPC *ldpc, unsigned char ibits[], unsigned char pbits[]);

CODEC2_PUBLIC int run_ldpc_decoder(struct LDPC *ldpc, uint8_t out_char[], float input[], int *parityCheckCount);

CODEC2_PUBLIC void sd_to_llr(float llr[], double sd[], int n);
CODEC2_PUBLIC void Demod2D(float symbol_likelihood[], COMP r[], COMP S_matrix[], float EsNo, float fading[], float mean_amp, int number_symbols);
CODEC2_PUBLIC void Somap(float bit_likelihood[], float symbol_likelihood[], int number_symbols);
CODEC2_PUBLIC void symbols_to_llrs(float llr[], COMP rx_qpsk_symbols[], float rx_amps[], float EsNo, float mean_amp, int nsyms);

CODEC2_PUBLIC void ldpc_print_info(struct LDPC *ldpc);


#endif
