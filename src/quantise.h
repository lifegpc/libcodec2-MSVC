/*---------------------------------------------------------------------------*\

  FILE........: quantise.h
  AUTHOR......: David Rowe
  DATE CREATED: 31/5/92

  Quantisation functions for the sinusoidal coder.

\*---------------------------------------------------------------------------*/

/*
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

#ifndef __QUANTISE__
#define __QUANTISE__

#include "codec2_fft.h"
#include "comp.h"
#include "dllexport.h"

#define WO_BITS     7
#define WO_LEVELS   (1<<WO_BITS)
#define WO_DT_BITS  3

#define E_BITS      5
#define E_LEVELS    (1<<E_BITS)
#define E_MIN_DB   -10.0
#define E_MAX_DB    40.0

#define LSP_SCALAR_INDEXES    10
#define LSPD_SCALAR_INDEXES    10
#define LSP_PRED_VQ_INDEXES    3
#define LSP_DIFF_FREQ_INDEXES  5
#define LSP_DIFF_TIME_BITS     7

#define LSPDT_ALL   0
#define LSPDT_LOW   1
#define LSPDT_HIGH  2

#define WO_E_BITS   8

#define LPCPF_GAMMA 0.5
#define LPCPF_BETA  0.2

CODEC2_PUBLIC void quantise_init();
CODEC2_PUBLIC float lpc_model_amplitudes(float Sn[], float w[], MODEL *model, int order,
			   int lsp,float ak[]);
CODEC2_PUBLIC void aks_to_M2(codec2_fftr_cfg fftr_fwd_cfg, float ak[], int order, MODEL *model,
	       float E, float *snr, int dump, int sim_pf,
               int pf, int bass_boost, float beta, float gamma, COMP Aw[]);

CODEC2_PUBLIC int   encode_Wo(C2CONST *c2const, float Wo, int bits);
CODEC2_PUBLIC float decode_Wo(C2CONST *c2const, int index, int bits);
CODEC2_PUBLIC int   encode_log_Wo(C2CONST *c2const, float Wo, int bits);
CODEC2_PUBLIC float decode_log_Wo(C2CONST *c2const, int index, int bits);
#if 0
CODEC2_PUBLIC int   encode_Wo_dt(C2CONST *c2const, float Wo, float prev_Wo);
CODEC2_PUBLIC float decode_Wo_dt(C2CONST *c2const, int index, float prev_Wo);
#endif
CODEC2_PUBLIC void  encode_lsps_scalar(int indexes[], float lsp[], int order);
CODEC2_PUBLIC void  decode_lsps_scalar(float lsp[], int indexes[], int order);
CODEC2_PUBLIC void  encode_lspds_scalar(int indexes[], float lsp[], int order);
CODEC2_PUBLIC void  decode_lspds_scalar(float lsp[], int indexes[], int order);
CODEC2_PUBLIC void  encode_lsps_diff_freq_vq(int indexes[], float lsp[], int order);
CODEC2_PUBLIC void  decode_lsps_diff_freq_vq(float lsp_[], int indexes[], int order);
CODEC2_PUBLIC void  encode_lsps_diff_time(int indexes[],
			    float lsp[],
			    float lsp__prev[],
			    int order);
CODEC2_PUBLIC void decode_lsps_diff_time(float lsp_[],
			   int indexes[],
			   float lsp__prev[],
			   int order);

CODEC2_PUBLIC void encode_lsps_vq(int *indexes, float *x, float *xq, int order);
CODEC2_PUBLIC void decode_lsps_vq(int *indexes, float *xq, int order, int stages);

CODEC2_PUBLIC long quantise(const float * cb, float vec[], float w[], int k, int m, float *se);
CODEC2_PUBLIC void lspvq_quantise(float lsp[], float lsp_[], int order);
CODEC2_PUBLIC void lspjnd_quantise(float lsp[], float lsp_[], int order);
CODEC2_PUBLIC void lspdt_quantise(float lsps[], float lsps_[], float lsps__prev[], int mode);
CODEC2_PUBLIC void lspjvm_quantise(float lsps[], float lsps_[], int order);
CODEC2_PUBLIC void lspanssi_quantise(float lsps[], float lsps_[], int order, int mbest_entries);
CODEC2_PUBLIC float lspmelvq_quantise(float *x, float *xq, int order);

CODEC2_PUBLIC float lspmelvq_mbest_encode(int *indexes, float *x, float *xq, int ndim, int mbest_entries);
CODEC2_PUBLIC void lspmelvq_decode(int *indexes, float *xq, int ndim);

CODEC2_PUBLIC void encode_mels_scalar(int mel_indexes[], float mels[], int order);
CODEC2_PUBLIC void decode_mels_scalar(float mels[], int mel_indexes[], int order);

CODEC2_PUBLIC void quantise_WoE(C2CONST *c2const, MODEL *model, float *e, float xq[]);
CODEC2_PUBLIC int  encode_WoE(MODEL *model, float e, float xq[]);
CODEC2_PUBLIC void decode_WoE(C2CONST *c2const, MODEL *model, float *e, float xq[], int n1);

CODEC2_PUBLIC int encode_energy(float e, int bits);
CODEC2_PUBLIC float decode_energy(int index, int bits);

CODEC2_PUBLIC void pack(unsigned char * bits, unsigned int *nbit, int index, unsigned int index_bits);
CODEC2_PUBLIC void pack_natural_or_gray(unsigned char * bits, unsigned int *nbit, int index, unsigned int index_bits, unsigned int gray);
CODEC2_PUBLIC int  unpack(const unsigned char * bits, unsigned int *nbit, unsigned int index_bits);
CODEC2_PUBLIC int  unpack_natural_or_gray(const unsigned char * bits, unsigned int *nbit, unsigned int index_bits, unsigned int gray);

CODEC2_PUBLIC int lsp_bits(int i);
CODEC2_PUBLIC int lspd_bits(int i);
CODEC2_PUBLIC int lspdt_bits(int i);
CODEC2_PUBLIC int lsp_pred_vq_bits(int i);
CODEC2_PUBLIC int mel_bits(int i);
CODEC2_PUBLIC int lspmelvq_cb_bits(int i);

CODEC2_PUBLIC void apply_lpc_correction(MODEL *model);
CODEC2_PUBLIC float speech_to_uq_lsps(float lsp[],
			float ak[],
		        float Sn[],
		        float w[],
		        int m_pitch,
                        int   order
			);
CODEC2_PUBLIC int check_lsp_order(float lsp[], int lpc_order);
CODEC2_PUBLIC void bw_expand_lsps(float lsp[], int order, float min_sep_low, float min_sep_high);
CODEC2_PUBLIC void bw_expand_lsps2(float lsp[], int order);
CODEC2_PUBLIC void locate_lsps_jnd_steps(float lsp[], int order);
CODEC2_PUBLIC float decode_amplitudes(MODEL *model,
			float  ak[],
		        int    lsp_indexes[],
		        int    energy_index,
			float  lsps[],
			float *e);

#endif
