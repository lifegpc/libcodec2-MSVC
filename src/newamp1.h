/*---------------------------------------------------------------------------*\

  FILE........: newamp1.h
  AUTHOR......: David Rowe
  DATE CREATED: Jan 2017

  Quantisation functions for the sinusoidal coder, using "newamp1"
  algorithm that resamples variable rate L [Am} to a fixed rate K then
  VQs.

\*---------------------------------------------------------------------------*/

/*
  Copyright David Rowe 2017

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

#ifndef __NEWAMP1__
#define __NEWAMP1__

#define NEWAMP1_N_INDEXES    4  /* Number of indexes to pack: vq1, vq2, energy, Wo */
#define NEWAMP1_PHASE_NFFT 128  /* size of FFT used for phase synthesis            */
#define NEWAMP1_K           20  /* rate K vector length                            */

#include "codec2_fft.h"
#include "comp.h"
#include "dllexport.h"

CODEC2_PUBLIC void interp_para(float y[], float xp[], float yp[], int np, float x[], int n);
CODEC2_PUBLIC float ftomel(float fHz);
CODEC2_PUBLIC void mel_sample_freqs_kHz(float rate_K_sample_freqs_kHz[], int K, float mel_start, float mel_end);
CODEC2_PUBLIC void resample_const_rate_f(C2CONST *c2const, MODEL *model, float rate_K_vec[], float rate_K_sample_freqs_kHz[], int K);
CODEC2_PUBLIC float rate_K_mbest_encode(int *indexes, float *x, float *xq, int ndim, int mbest_entries);
CODEC2_PUBLIC void post_filter_newamp1(float vec[], float sample_freq_kHz[], int K, float pf_gain);
CODEC2_PUBLIC void interp_Wo_v(float Wo_[], int L_[], int voicing_[], float Wo1, float Wo2, int voicing1, int voicing2);
CODEC2_PUBLIC void resample_rate_L(C2CONST *c2const, MODEL *model, float rate_K_vec[], float rate_K_sample_freqs_kHz[], int K);
CODEC2_PUBLIC void determine_phase(C2CONST *c2const, COMP H[], MODEL *model, int Nfft, codec2_fft_cfg fwd_cfg, codec2_fft_cfg inv_cfg);
CODEC2_PUBLIC void newamp1_model_to_indexes(C2CONST *c2const,
                              int    indexes[], 
                              MODEL *model, 
                              float  rate_K_vec[], 
                              float  rate_K_sample_freqs_kHz[], 
                              int    K,
                              float *mean,
                              float  rate_K_vec_no_mean[], 
                              float  rate_K_vec_no_mean_[],
                              float *se,
                              float *eq,
                              int    eq_en);
CODEC2_PUBLIC void newamp1_indexes_to_rate_K_vec(float  rate_K_vec_[],
                                   float  rate_K_vec_no_mean_[],
                                   float  rate_K_sample_freqs_kHz[], 
                                   int    K,
                                   float *mean_,
                                   int    indexes[],
                                   float user_rate_K_vec_no_mean_[],
                                   int post_filter_en);
CODEC2_PUBLIC void newamp1_interpolate(float interpolated_surface_[], float left_vec[], float right_vec[], int K);

void newamp1_indexes_to_model(C2CONST *c2const,
                              MODEL  model_[],
                              COMP   H[],
                              float  interpolated_surface_[],
                              float  prev_rate_K_vec_[],
                              float  *Wo_left,
                              int    *voicing_left,
                              float  rate_K_sample_freqs_kHz[], 
                              int    K,
                              codec2_fft_cfg fwd_cfg, 
                              codec2_fft_cfg inv_cfg,
                              int    indexes[],
                              float user_rate_K_vec_no_mean_[],
                              int post_filter_en);

#endif
