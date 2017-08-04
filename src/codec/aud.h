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

#ifndef DECODEAUDIO_H_
#define DECODEAUDIO_H_

#include "demuxer/stheader.h"

namespace openwsp {

void resync_audio_stream(sh_audio_t *sh_audio);
void skip_audio_frame(sh_audio_t *sh_audio);
int decode_audio(sh_audio_t *sh_audio,unsigned char *buf,int minlen,int maxlen);

} // namespace openwsp

#endif //!defined(DECODEAUDIO_H_)
