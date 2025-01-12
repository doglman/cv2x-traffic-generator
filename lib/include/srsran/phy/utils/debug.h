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
 *  File:         debug.h
 *
 *  Description:  Debug output utilities.
 *
 *  Reference:
 *****************************************************************************/

#ifndef SRSRAN_DEBUG_H
#define SRSRAN_DEBUG_H

#include "phy_logger.h"
#include "srsran/config.h"
#include <stdio.h>

#define SRSRAN_VERBOSE_DEBUG 2
#define SRSRAN_VERBOSE_INFO 1
#define SRSRAN_VERBOSE_NONE 0

#include <sys/time.h>
SRSRAN_API void get_time_interval(struct timeval* tdata);

#define SRSRAN_DEBUG_ENABLED 1

SRSRAN_API extern int srsran_verbose;
SRSRAN_API extern int handler_registered;

#define SRSRAN_VERBOSE_ISINFO() (srsran_verbose >= SRSRAN_VERBOSE_INFO)
#define SRSRAN_VERBOSE_ISDEBUG() (srsran_verbose >= SRSRAN_VERBOSE_DEBUG)
#define SRSRAN_VERBOSE_ISNONE() (srsran_verbose == SRSRAN_VERBOSE_NONE)

#define PRINT_DEBUG srsran_verbose = SRSRAN_VERBOSE_DEBUG
#define PRINT_INFO srsran_verbose = SRSRAN_VERBOSE_INFO
#define PRINT_NONE srsran_verbose = SRSRAN_VERBOSE_NONE

#define DEBUG(_fmt, ...)                                                                                               \
  do {                                                                                                                 \
    if (SRSRAN_DEBUG_ENABLED && srsran_verbose >= SRSRAN_VERBOSE_DEBUG && !handler_registered) {                       \
      fprintf(stdout, "[DEBUG]: " _fmt, ##__VA_ARGS__);                                                                \
    } else {                                                                                                           \
      srsran_phy_log_print(LOG_LEVEL_DEBUG_S, _fmt, ##__VA_ARGS__);                                                    \
    }                                                                                                                  \
  } while (0)

#define INFO(_fmt, ...)                                                                                                \
  do {                                                                                                                 \
    if (SRSRAN_DEBUG_ENABLED && srsran_verbose >= SRSRAN_VERBOSE_INFO && !handler_registered) {                        \
      fprintf(stdout, "[INFO]: " _fmt, ##__VA_ARGS__);                                                                 \
    } else {                                                                                                           \
      srsran_phy_log_print(LOG_LEVEL_INFO_S, _fmt, ##__VA_ARGS__);                                                     \
    }                                                                                                                  \
  } while (0)

#if CMAKE_BUILD_TYPE == Debug
/* In debug mode, it prints out the  */
#define ERROR(_fmt, ...)                                                                                               \
  do {                                                                                                                 \
    if (!handler_registered) {                                                                                         \
      fprintf(stderr, "\e[31m%s.%d: " _fmt "\e[0m\n", __FILE__, __LINE__, ##__VA_ARGS__);                              \
    } else {                                                                                                           \
      srsran_phy_log_print(LOG_LEVEL_ERROR_S, _fmt, ##__VA_ARGS__);                                                    \
    }                                                                                                                  \
  } while (0)
#else
#define ERROR(_fmt, ...)                                                                                               \
  if (!handler_registered) {                                                                                           \
    fprintf(stderr, "[ERROR in %s]:" _fmt "\n", __FUNCTION__, ##__VA_ARGS__);                                          \
  } else {                                                                                                             \
    srsran_phy_log_print(LOG_LEVEL_ERROR, _fmt, ##__VA_ARGS__);                                                        \
  }    //
#endif /* CMAKE_BUILD_TYPE==Debug */

#endif // SRSRAN_DEBUG_H
