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

#ifndef MP3HEADER_H_
#define MP3HEADER_H_

namespace openwsp {

int getmp3Header(unsigned char* hbuf,int* chans, int* freq, int* spf, int* mpa_layer, int* br);

#define decodemp3Header(hbuf)  getmp3Header(hbuf,NULL,NULL,NULL,NULL,NULL)

static inline int checkmp3Header(unsigned int head){
    if( (head & 0x0000e0ff) != 0x0000e0ff ||  
        (head & 0x00fc0000) == 0x00fc0000) return 0;
    if(decodemp3Header((unsigned char*)(&head))<=0) return 0;
    return 1;
}

} // namespace openwsp

#endif //!defined(MP3HEADER_H_)
