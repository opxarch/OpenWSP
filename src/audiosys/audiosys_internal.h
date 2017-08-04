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

#ifndef AUDIOSYS_INTERNAL_H_
#define AUDIOSYS_INTERNAL_H_

namespace openwsp {

// prototypes:
static int init(int rate, int channels, int format, int flags);
static int uninit(int flags);
static void reset(void);
static int get_space(void);
static int get_delay(void);
static void audio_pause(void);
static void audio_resume(void);
static int control(int cmd, void* arg, ...);
static int write(char *data, int length, int flags);

#define AO_EXTERN(x) ao_functions_t audio_out_##x =\
{\
    &info,\
    init,\
    uninit,\
    reset, \
    get_space,\
    get_delay,\
    audio_pause,\
    audio_resume,\
    control,\
    write\
};

} // namespace openwsp

#endif //!defined(AUDIOSYS_INTERNAL_H_)
