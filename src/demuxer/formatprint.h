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

#ifndef FMTPRINT_H_
#define FMTPRINT_H_

#include "msheader.h"
#include "log.h"

namespace openwsp {

void print_wave_header(WAVEFORMATEX *h, log::level verbose_level);
//void print_video_header(BITMAPINFOHEADER *h, log::level verbose_level);

} // namespace openwsp

#endif //!defined(FMTPRINT_H_)
