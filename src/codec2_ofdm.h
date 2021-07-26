/*---------------------------------------------------------------------------*\

  FILE........: codec2_ofdm.h
  AUTHORS.....: David Rowe & Steve Sampson
  DATE CREATED: June 2017

  External user references to the modem library.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2017 David Rowe

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

#ifndef CODEC2_OFDM_H
#define CODEC2_OFDM_H

/* Includes */
    
#include <complex.h>
#include <stdbool.h>
#include <stdint.h>
  
#include "comp.h"
#include "modem_stats.h"
#include "dllexport.h"

/* Defines */

#ifdef __cplusplus
extern "C" {
#endif

#define OFDM_AMP_SCALE (2E5*1.1491/1.06)   /* use to scale to 16 bit short */
#define OFDM_CLIP (32767*0.35)             /* experimentally derived constant to reduce PAPR to about 8dB */

#define UN_SYNC      0  /* Used with the ofdm_set_sync() */
#define AUTO_SYNC    1
#define MANUAL_SYNC  2

#define AUTO_PHASE_EST   0 
#define LOCKED_PHASE_EST 1

#define LOW_BW       0
#define HIGH_BW      1

struct OFDM_CONFIG;
struct OFDM;

/* create and destroy modem states */

CODEC2_PUBLIC struct OFDM *ofdm_create(const struct OFDM_CONFIG * config);
CODEC2_PUBLIC void ofdm_destroy(struct OFDM *);

/* signal processing */

CODEC2_PUBLIC void ofdm_mod(struct OFDM *, COMP *, const int *);
CODEC2_PUBLIC void ofdm_demod(struct OFDM *, int *, COMP *);
CODEC2_PUBLIC void ofdm_demod_shorts(struct OFDM *, int *, short *, float);
CODEC2_PUBLIC int  ofdm_sync_search(struct OFDM *, COMP *);
CODEC2_PUBLIC int  ofdm_sync_search_shorts(struct OFDM *, short *, float);
CODEC2_PUBLIC void ofdm_sync_state_machine(struct OFDM *, uint8_t *);

/* getters */
    
CODEC2_PUBLIC struct OFDM_CONFIG *ofdm_get_config_param(void);
CODEC2_PUBLIC int ofdm_get_nin(struct OFDM *);
CODEC2_PUBLIC int ofdm_get_samples_per_frame(void);
CODEC2_PUBLIC int ofdm_get_max_samples_per_frame(void);
CODEC2_PUBLIC int ofdm_get_bits_per_frame(void);
CODEC2_PUBLIC void ofdm_get_demod_stats(struct OFDM *ofdm, struct MODEM_STATS *stats);
CODEC2_PUBLIC int ofdm_get_phase_est_bandwidth_mode(struct OFDM *ofdm);

/* option setters */

CODEC2_PUBLIC void ofdm_set_verbose(struct OFDM *, int);
CODEC2_PUBLIC void ofdm_set_timing_enable(struct OFDM *, bool);
CODEC2_PUBLIC void ofdm_set_foff_est_enable(struct OFDM *, bool);
CODEC2_PUBLIC void ofdm_set_phase_est_enable(struct OFDM *, bool);
CODEC2_PUBLIC void ofdm_set_phase_est_bandwidth_mode(struct OFDM *ofdm, int val);
CODEC2_PUBLIC void ofdm_set_off_est_hz(struct OFDM *, float);
CODEC2_PUBLIC void ofdm_set_sync(struct OFDM *, int);
CODEC2_PUBLIC void ofdm_set_tx_bpf(struct OFDM *, bool);
CODEC2_PUBLIC void ofdm_set_dpsk(struct OFDM *ofdm, bool val);
    
CODEC2_PUBLIC void ofdm_print_info(struct OFDM *);

#ifdef __cplusplus
}
#endif

#endif

