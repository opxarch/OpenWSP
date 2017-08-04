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

#ifndef AUDIOCODEC_H_
#define AUDIOCODEC_H_

#include "demuxer/stheader.h"
#include "stream/stream.h"

namespace openwsp {

#define MAX_AD_DLLS 32

typedef struct fileformat_s {
    unsigned int format;
    const char *display_name;
}fileformat_t;

#include <openwsp/guid.h>

#define CODECS_MAX_FOURCC	32
#define CODECS_MAX_OUTFMT	16
#define CODECS_MAX_INFMT	16


/***************************************************
  *****             Audio output               *****
  ***************************************************/

class IAudioCodec {
public:
    IAudioCodec();
    virtual ~IAudioCodec();

    virtual int preinit(sh_audio_t *sh)=0;
    virtual int init(sh_audio_t *sh)=0;
    virtual int uninit(sh_audio_t *sh)=0;
    virtual int control(sh_audio_t *sh, int cmd, void* arg, ...)=0;
    virtual int decode(sh_audio_t *sh, unsigned char *buf, int minlen, int maxlen)=0;

    virtual const char *getname() const;
    virtual const char *getshortname() const;
    virtual const fileformat_t *getdlls() const;
};

typedef struct codecs_s {
	const char *name;
	const char *short_name;
	const char *drv;
	const char *dll;
	GUID guid;

    /*	short driver; */
	/*short flags;
	short status;
	short cpuflags;*/
} codecs_t;


/* returned value */
#define ADCONTROL_OK 1
#define ADCONTROL_TRUE 1
#define ADCONTROL_FALSE 0
#define ADCONTROL_UNKNOWN -1
#define ADCONTROL_ERROR -2
#define ADCONTROL_NA -3

// fallback if ADCTRL_RESYNC not implemented: sh_audio->a_in_buffer_len=0;
#define ADCTRL_RESYNC_STREAM 1       /* resync, called after seeking! */

// fallback if ADCTRL_SKIP not implemented: ds_fill_buffer(sh_audio->ds);
#define ADCTRL_SKIP_FRAME 2         /* skip block/frame, called while seeking! */

// fallback if ADCTRL_QUERY_FORMAT not implemented: sh_audio->sample_format
#define ADCTRL_QUERY_FORMAT 3 /* test for availabilty of a format */

// fallback: use hw mixer in libao
#define ADCTRL_SET_VOLUME 4 /* set volume (used for mp3lib and liba52) */


extern int audio_output_channels;

/*
 funcs
 */
void list_audio_codecs(void);
int init_best_audio_codec(sh_audio_t *sh_audio);

int init_codec_sh_audio(sh_audio_t *sh_audio);
void uninit_codec_sh_audio(sh_audio_t *sh_audio);

int free_codec(sh_audio_t *sh);
int control_codec(sh_audio_t *sh, int cmd, void* arg, ...);
int decode_codec(sh_audio_t *sh, unsigned char *buf, int minlen, int maxlen);
int seek_codec(sh_audio_t *sh, long seek_sec);

} // namespace openwsp

#endif //!defined(AUDIOCODEC_H_)

