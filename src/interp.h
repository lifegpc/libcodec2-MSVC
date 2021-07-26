/*---------------------------------------------------------------------------*\

  FILE........: interp.h
  AUTHOR......: David Rowe
  DATE CREATED: 9/10/09

  Interpolation of 20ms frames to 10ms frames.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2009 David Rowe

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

#ifndef __INTERP__
#define __INTERP__

#include "kiss_fft.h"
#include "dllexport.h"

CODEC2_PUBLIC void interpolate(MODEL *interp, MODEL *prev, MODEL *next);
CODEC2_PUBLIC void interpolate_lsp(kiss_fft_cfg  fft_dec_cfg,
		     MODEL *interp, MODEL *prev, MODEL *next,
		     float *prev_lsps, float  prev_e,
		     float *next_lsps, float  next_e,
		     float *ak_interp, float *lsps_interp, float Wo_min);
CODEC2_PUBLIC void interp_Wo(MODEL *interp, MODEL *prev, MODEL *next, float Wo_min);
CODEC2_PUBLIC void interp_Wo2(MODEL *interp, MODEL *prev, MODEL *next, float weight, float Wo_min);
CODEC2_PUBLIC float interp_energy(float prev, float next);
CODEC2_PUBLIC float interp_energy2(float prev, float next, float weight);
CODEC2_PUBLIC void interpolate_lsp_ver2(float interp[], float prev[],  float next[], float weight, int order);

#endif
