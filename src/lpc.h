/*---------------------------------------------------------------------------*\

  FILE........: lpc.h
  AUTHOR......: David Rowe
  DATE CREATED: 24/8/09

  Linear Prediction functions written in C.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2009-2012 David Rowe

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

#ifndef __LPC__
#define __LPC__

#define LPC_MAX_ORDER 20
#include "dllexport.h"

CODEC2_PUBLIC void pre_emp(float Sn_pre[], float Sn[], float *mem, int Nsam);
CODEC2_PUBLIC void de_emp(float Sn_se[], float Sn[], float *mem, int Nsam);
CODEC2_PUBLIC void hanning_window(float Sn[],	float Wn[], int Nsam);
CODEC2_PUBLIC void autocorrelate(float Sn[], float Rn[], int Nsam, int order);
CODEC2_PUBLIC void levinson_durbin(float R[],	float lpcs[], int order);
CODEC2_PUBLIC void inverse_filter(float Sn[], float a[], int Nsam, float res[], int order);
CODEC2_PUBLIC void synthesis_filter(float res[], float a[], int Nsam,	int order, float Sn_[]);
CODEC2_PUBLIC void find_aks(float Sn[], float a[], int Nsam, int order, float *E);
CODEC2_PUBLIC void weight(float ak[],	float gamma, int order,	float akw[]);

#endif
