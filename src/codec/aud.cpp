/** @file
 * Openwsp - audio decoding.
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
#include "aud.h"

#include "audcodec.h"
#include "demuxer/stheader.h"


namespace openwsp {

////////////////////////////////////////////////////////////////////////////////

void resync_audio_stream(sh_audio_t *sh_audio) {
    sh_audio->a_in_buffer_len = 0;        /* clear audio input buffer */

    if(!sh_audio->inited) return;
    sh_audio->ad_driver->control(sh_audio,ADCTRL_RESYNC_STREAM,NULL);
}

void skip_audio_frame(sh_audio_t *sh_audio) {
    if(!sh_audio->inited) return;
    if(sh_audio->ad_driver->control(sh_audio,ADCTRL_SKIP_FRAME,NULL)==ADCONTROL_TRUE) return;

    /* default skip code: */
    ds_fill_buffer(sh_audio->ds);  /* skip block */
}

int decode_audio(sh_audio_t *sh_audio,unsigned char *buf,int minlen,int maxlen) {
    IAudioCodec* mpadec = sh_audio->ad_driver;

    if(!sh_audio->inited) return -1; /* no codec */

    /* FIXME: don't drop initial decoded data in a_buffer! */
    return mpadec->decode(sh_audio, buf, minlen, maxlen);
}

} // namespace openwsp
