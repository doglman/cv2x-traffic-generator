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

#ifndef RLCPCAP_H
#define RLCPCAP_H

#include "srsran/common/pcap.h"
#include <stdint.h>

namespace srsran {

class rlc_pcap
{
public:
  rlc_pcap()
  {
    enable_write = false;
    ue_id        = 0;
    pcap_file    = NULL;
  };
  void enable(bool en);
  void open(const char* filename, uint32_t ue_id = 0);
  void close();

  void set_ue_id(uint16_t ue_id);

  void write_dl_am_ccch(uint8_t* pdu, uint32_t pdu_len_bytes);
  void write_ul_am_ccch(uint8_t* pdu, uint32_t pdu_len_bytes);

private:
  bool     enable_write;
  FILE*    pcap_file;
  uint32_t ue_id;
  void     pack_and_write(uint8_t* pdu,
                          uint32_t pdu_len_bytes,
                          uint8_t  mode,
                          uint8_t  direction,
                          uint8_t  priority,
                          uint8_t  seqnumberlength,
                          uint16_t ueid,
                          uint16_t channel_type,
                          uint16_t channel_id);
};

} // namespace srsran

#endif // RLCPCAP_H
