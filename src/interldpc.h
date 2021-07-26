/*---------------------------------------------------------------------------*\

  FILE........: interldpc.h
  AUTHOR......: David Rowe
  DATE CREATED: April 2018

  Helper functions for interleaved LDPC modems.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2018 David Rowe

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

#ifndef __INTERLDPC__
#define __INTERLDPC__

#include <stdint.h>

#include "comp.h"
#include "mpdecode_core.h"
#include "ofdm_internal.h"
#include "dllexport.h"

/* CRC type function, used to compare QPSK vectors when debugging */

CODEC2_PUBLIC COMP test_acc(COMP v[], int n);
CODEC2_PUBLIC void printf_n(COMP v[], int n);
CODEC2_PUBLIC void set_up_hra_112_112(struct LDPC *ldpc, struct OFDM_CONFIG *);
CODEC2_PUBLIC void set_up_hra_504_396(struct LDPC *ldpc, struct OFDM_CONFIG *c);
CODEC2_PUBLIC void set_data_bits_per_frame(struct LDPC *ldpc, int new_data_bits_per_frame, int bps);
CODEC2_PUBLIC void ldpc_encode_frame(struct LDPC *ldpc, int codeword[], unsigned char tx_bits_char[]);
CODEC2_PUBLIC void qpsk_modulate_frame(COMP tx_symbols[], int codeword[], int n);
CODEC2_PUBLIC void interleaver_sync_state_machine(struct OFDM *ofdm, struct LDPC *ldpc, struct OFDM_CONFIG *config,
                                    COMP codeword_symbols_de[],
                                    float codeword_amps_de[],
                                    float EsNo, int interleave_frames,
                                    int *inter, int *parityCheckCount, int *Nerrs_coded);
CODEC2_PUBLIC int count_uncoded_errors(struct LDPC *ldpc, struct OFDM_CONFIG *config, int Nerrs_raw[], int interleave_frames, COMP codeword_symbols_de[]);
CODEC2_PUBLIC int count_errors(uint8_t tx_bits[], uint8_t rx_bits[], int n);
CODEC2_PUBLIC void ofdm_ldpc_interleave_tx(struct OFDM *ofdm, struct LDPC *ldpc, _Fcomplex tx_sams[], uint8_t tx_bits[], uint8_t txt_bits[], int interleave_frames, struct OFDM_CONFIG *config);
CODEC2_PUBLIC void build_modulated_uw(struct OFDM *ofdm, _Fcomplex tx_symbols[], uint8_t txt_bits[], struct OFDM_CONFIG *config);

#endif
