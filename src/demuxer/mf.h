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

#ifndef MF_H_
#define MF_H_

namespace openwsp {

extern int    mf_w;
extern int    mf_h;
extern float  mf_fps;
extern char*  mf_type;

typedef struct mf_s {
    int curr_frame;
    int nr_of_files;
    char ** names;
} mf_t;

mf_t* open_mf(char * filename);

} // namespace openwsp

#endif //!defined(MF_H_)
