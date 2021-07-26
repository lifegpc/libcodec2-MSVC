/*
  Copyright (C) 2018 James C. Ahlstrom

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

#ifndef __FILTER__
#define __FILTER__

#include "dllexport.h"
#include <complex.h>

#if !defined(_MSC_VER) && !defined(_Fcomplex)
#define _Fcomplex complex float
#endif

struct quisk_cfFilter {        // Structure to hold the static data for FIR filters
    float * dCoefs;    // real filter coefficients
    _Fcomplex* cpxCoefs;   // complex filter coefficients
    int nBuf;          // dimension of cBuf
    int nTaps;         // dimension of dSamples, cSamples, dCoefs
    int decim_index;   // index of next sample for decimation
    _Fcomplex* cSamples;   // storage for old samples
    _Fcomplex* ptcSamp;    // next available position in cSamples
    _Fcomplex* cBuf;       // auxillary buffer for interpolation
} ;

CODEC2_PUBLIC extern int quisk_cfInterpDecim(_Fcomplex*, int, struct quisk_cfFilter *, int, int);
CODEC2_PUBLIC extern void quisk_filt_cfInit(struct quisk_cfFilter *, float *, int);
CODEC2_PUBLIC extern void quisk_filt_destroy(struct quisk_cfFilter *);
CODEC2_PUBLIC extern void quisk_cfTune(struct quisk_cfFilter *, float);
CODEC2_PUBLIC extern void quisk_ccfFilter(_Fcomplex*, _Fcomplex*, int, struct quisk_cfFilter *);

CODEC2_PUBLIC extern float quiskFilt120t480[480];
CODEC2_PUBLIC extern float filtP550S750[160];

#endif
