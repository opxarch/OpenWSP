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

#ifndef STREAM_CACHE_H_
#define STREAM_CACHE_H_

namespace openwsp {

typedef struct stream_s stream_t;

int stream_enable_cache(stream_t *stream,int size,int min,int seek_limit);
int cache_stream_fill_buffer(stream_t *s);
int cache_stream_seek_long(stream_t *stream,off_t pos);
void cache_uninit(stream_t *s);

} // namespace openwsp

#endif //!defined(STREAM_CACHE_H_)
