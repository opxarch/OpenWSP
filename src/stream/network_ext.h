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

#ifndef STREAM_NETWORK_EXT_H_
#define STREAM_NETWORK_EXT_H_

namespace openwsp {

typedef struct stream_s stream_t;

// stream_http.cpp
int http_seek(stream_t *stream, off_t pos);
void fixup_network_stream_cache(stream_t *stream);

int streaming_stop(stream_t *stream);

} // namespace openwsp

#endif // STREAM_NETWORK_EXT_H_
