/*---------------------------------------------------------------------------*\

  FILE........: nlp.c
  AUTHOR......: David Rowe
  DATE CREATED: 23/3/93

  Non Linear Pitch (NLP) estimation functions.

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

#ifndef __NLP__
#define __NLP__

#include "comp.h"
#include "dllexport.h"

CODEC2_PUBLIC void *nlp_create(C2CONST *c2const);
CODEC2_PUBLIC void nlp_destroy(void *nlp_state);
CODEC2_PUBLIC float nlp(void *nlp_state, float Sn[], int n,
	  float *pitch_samples, COMP Sw[], COMP W[], float *prev_f0);

#endif
