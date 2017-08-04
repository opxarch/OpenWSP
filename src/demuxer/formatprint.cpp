/** @file
 * Openwsp - format print.
 */

/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *  please contact with <diyer175@hotmail.com> if you have any problems.
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include "formatprint.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <openwsp/misc.h>

#include "../log.h"
#include "demuxer.h"

namespace openwsp {

void print_wave_header(WAVEFORMATEX *h, log::level verbose_level)
{
  ws_log::write(log::M_HEADER, verbose_level, "======= WAVE Format =======\n");
  ws_log::write(log::M_HEADER, verbose_level, "Format Tag: %d (0x%X)\n",h->wFormatTag,h->wFormatTag);
  ws_log::write(log::M_HEADER, verbose_level, "Channels: %d\n",h->nChannels);
  ws_log::write(log::M_HEADER, verbose_level, "Samplerate: %"PRId32"\n",h->nSamplesPerSec);
  ws_log::write(log::M_HEADER, verbose_level, "avg byte/sec: %"PRId32"\n",h->nAvgBytesPerSec);
  ws_log::write(log::M_HEADER, verbose_level, "Block align: %d\n",h->nBlockAlign);
  ws_log::write(log::M_HEADER, verbose_level, "bits/sample: %d\n",h->wBitsPerSample);
  ws_log::write(log::M_HEADER, verbose_level, "cbSize: %d\n",h->cbSize);
  if(h->wFormatTag==0x55 && h->cbSize>=12){
      MPEGLAYER3WAVEFORMAT* h2=(MPEGLAYER3WAVEFORMAT *)h;
      ws_log::write(log::M_HEADER, verbose_level, "mp3.wID=%d\n",h2->wID);
      ws_log::write(log::M_HEADER, verbose_level, "mp3.fdwFlags=0x%"PRIX32"\n",h2->fdwFlags);
      ws_log::write(log::M_HEADER, verbose_level, "mp3.nBlockSize=%d\n",h2->nBlockSize);
      ws_log::write(log::M_HEADER, verbose_level, "mp3.nFramesPerBlock=%d\n",h2->nFramesPerBlock);
      ws_log::write(log::M_HEADER, verbose_level, "mp3.nCodecDelay=%d\n",h2->nCodecDelay);
  }
  else if (h->cbSize > 0)
  {
    int i;
    uint8_t* p = ((uint8_t*)h) + sizeof(WAVEFORMATEX);
    ws_log(log::M_HEADER, verbose_level) << "Unknown extra header dump: " << endlog;
    for (i = 0; i < h->cbSize; i++)
        ws_log::write(log::M_HEADER, verbose_level, "[%x] ", p[i]);
    ws_log(log::M_HEADER, verbose_level) << endlog;
  }
  ws_log(log::M_HEADER, verbose_level) << "==========================================================================" << endlog;
}

} // namespace openwsp

