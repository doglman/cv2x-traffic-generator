/*
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

/******************************************************************************
 *  File:         prach.h
 *
 *  Description:  Physical random access channel.
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10 Sec. 5.7
 *****************************************************************************/

#ifndef SRSRAN_PRACH_H
#define SRSRAN_PRACH_H

#include "srsran/config.h"
#include "srsran/phy/common/phy_common.h"
#include "srsran/phy/dft/dft.h"
#include <complex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SRSRAN_PRACH_MAX_LEN (2 * 24576 + 21024) // Maximum Tcp + Tseq

/** Generation and detection of RACH signals for uplink.
 *  Currently only supports preamble formats 0-3.
 *  Does not currently support high speed flag.
 *  Based on 3GPP TS 36.211 version 10.7.0 Release 10.
 */

typedef struct SRSRAN_API {
  // Parameters from higher layers (extracted from SIB2)
  uint32_t config_idx;
  uint32_t f;            // preamble format
  uint32_t rsi;          // rootSequenceIndex
  bool     hs;           // highSpeedFlag
  uint32_t zczc;         // zeroCorrelationZoneConfig
  uint32_t N_ifft_ul;    // IFFT size for uplink
  uint32_t N_ifft_prach; // IFFT size for PRACH generation

  uint32_t max_N_ifft_ul;

  // Working parameters
  uint32_t N_zc;  // PRACH sequence length
  uint32_t N_cs;  // Cyclic shift size
  uint32_t N_seq; // Preamble length
  float    T_seq; // Preamble length in seconds
  float    T_tot; // Total sequence length in seconds
  uint32_t N_cp;  // Cyclic prefix length

  // Generated tables
  cf_t     seqs[64][839];     // Our set of 64 preamble sequences
  cf_t     dft_seqs[64][839]; // DFT-precoded seqs
  uint32_t root_seqs_idx[64]; // Indices of root seqs in seqs table
  uint32_t N_roots;           // Number of root sequences used in this configuration

  // Containers
  cf_t*  ifft_in;
  cf_t*  ifft_out;
  cf_t*  prach_bins;
  cf_t*  corr_spec;
  float* corr;

  // PRACH IFFT
  srsran_dft_plan_t fft;
  srsran_dft_plan_t ifft;

  // ZC-sequence FFT and IFFT
  srsran_dft_plan_t zc_fft;
  srsran_dft_plan_t zc_ifft;

  cf_t* signal_fft;
  float detect_factor;

  uint32_t deadzone;
  float    peak_values[65];
  uint32_t peak_offsets[65];
  uint32_t            num_ra_preambles;
  srsran_tdd_config_t tdd_config;
  uint32_t            current_prach_idx;

} srsran_prach_t;

typedef struct SRSRAN_API {
  int      nof_sf;
  uint32_t sf[5];
} srsran_prach_sf_config_t;

typedef enum SRSRAN_API {
  SRSRAN_PRACH_SFN_EVEN = 0,
  SRSRAN_PRACH_SFN_ANY,
} srsran_prach_sfn_t;

typedef struct {
  uint32_t            config_idx;
  uint32_t            root_seq_idx;
  uint32_t            zero_corr_zone;
  uint32_t            freq_offset;
  uint32_t            num_ra_preambles;
  bool                hs_flag;
  srsran_tdd_config_t tdd_config;
} srsran_prach_cfg_t;

typedef struct SRSRAN_API {
  uint32_t f;
  uint32_t t0;
  uint32_t t1;
  uint32_t t2;
} srsran_prach_tdd_loc_t;

typedef struct SRSRAN_API {
  uint32_t               nof_elems;
  srsran_prach_tdd_loc_t elems[6];
} srsran_prach_tdd_loc_table_t;

SRSRAN_API uint32_t srsran_prach_get_preamble_format(uint32_t config_idx);

SRSRAN_API srsran_prach_sfn_t srsran_prach_get_sfn(uint32_t config_idx);

SRSRAN_API bool srsran_prach_tti_opportunity(srsran_prach_t* p, uint32_t current_tti, int allowed_subframe);

SRSRAN_API bool
srsran_prach_tti_opportunity_config_fdd(uint32_t config_idx, uint32_t current_tti, int allowed_subframe);

SRSRAN_API bool srsran_prach_tti_opportunity_config_tdd(uint32_t  config_idx,
                                                        uint32_t  tdd_ul_dl_config,
                                                        uint32_t  current_tti,
                                                        uint32_t* prach_idx);

SRSRAN_API uint32_t srsran_prach_f_ra_tdd(uint32_t config_idx,
                                          uint32_t tdd_ul_dl_config,
                                          uint32_t current_tti,
                                          uint32_t prach_idx,
                                          uint32_t prach_offset,
                                          uint32_t n_rb_ul);

SRSRAN_API uint32_t srsran_prach_f_id_tdd(uint32_t config_idx, uint32_t tdd_ul_dl_config, uint32_t prach_idx);

SRSRAN_API uint32_t srsran_prach_nof_f_idx_tdd(uint32_t config_idx, uint32_t tdd_ul_dl_config);

SRSRAN_API void srsran_prach_sf_config(uint32_t config_idx, srsran_prach_sf_config_t* sf_config);

SRSRAN_API int srsran_prach_init(srsran_prach_t* p, uint32_t max_N_ifft_ul);

SRSRAN_API int srsran_prach_set_cell_fdd(srsran_prach_t* p,
                                         uint32_t        N_ifft_ul,
                                         uint32_t        config_idx,
                                         uint32_t        root_seq_index,
                                         bool            high_speed_flag,
                                         uint32_t        zero_corr_zone_config);

SRSRAN_API int srsran_prach_set_cell_tdd(srsran_prach_t*      p,
                                         uint32_t             N_ifft_ul,
                                         uint32_t             config_idx,
                                         uint32_t             root_seq_index,
                                         bool                 high_speed_flag,
                                         uint32_t             zero_corr_zone_config,
                                         srsran_tdd_config_t* tdd_config);

SRSRAN_API int srsran_prach_set_cfg(srsran_prach_t* p, srsran_prach_cfg_t* cfg, uint32_t nof_prb);

SRSRAN_API int srsran_prach_gen(srsran_prach_t* p, uint32_t seq_index, uint32_t freq_offset, cf_t* signal);

SRSRAN_API int srsran_prach_detect(srsran_prach_t* p,
                                   uint32_t        freq_offset,
                                   cf_t*           signal,
                                   uint32_t        sig_len,
                                   uint32_t*       indices,
                                   uint32_t*       ind_len);

SRSRAN_API int srsran_prach_detect_offset(srsran_prach_t* p,
                                          uint32_t        freq_offset,
                                          cf_t*           signal,
                                          uint32_t        sig_len,
                                          uint32_t*       indices,
                                          float*          t_offsets,
                                          float*          peak_to_avg,
                                          uint32_t*       ind_len);

SRSRAN_API void srsran_prach_set_detect_factor(srsran_prach_t* p, float factor);

SRSRAN_API int srsran_prach_free(srsran_prach_t* p);

SRSRAN_API int srsran_prach_print_seqs(srsran_prach_t* p);

#endif // SRSRAN_PRACH_H
