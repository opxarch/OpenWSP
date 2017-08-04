/** @file
 * Openwsp - audio codec.
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
#include "audcodec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <openwsp/misc.h>
#include <openwsp/mem.h>
#include "log.h"

/* used for ac3surround decoder - set using -channels option */
int audio_output_channels = 2;

namespace openwsp {

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
/* used for ac3surround decoder - set using -channels option */
int audio_output_channels = 2;

#if USES(FFMPEG_CODEC)
extern IAudioCodec *audio_codec_ffmpeg;
#endif
#if USES(LIBWMA_CODEC)
extern IAudioCodec *audio_codec_libwma;
#endif
#if USES(PCM_CODEC)
extern IAudioCodec *audio_codec_pcm;
#endif

IAudioCodec* audio_codecs[] =
{
#if USES(FFMPEG_CODEC)
    audio_codec_ffmpeg,
#endif
#if USES(LIBWMA_CODEC)
    audio_codec_libwma,
#endif
#if USES(PCM_CODEC)
    audio_codec_pcm,
#endif
    NULL
};


////////////////////////////////////////////////////////////////////////////////

IAudioCodec::IAudioCodec() {
}

IAudioCodec::~IAudioCodec() {
}

/*
 * void implements
 */
const char *IAudioCodec::getname() const {
    return "NULL";
}
const char *IAudioCodec::getshortname() const {
    return "NULL";
}
const fileformat_t *IAudioCodec::getdlls() const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void list_audio_codecs(void) {
    int i = 0;
    while (audio_codecs[i]) {
        const IAudioCodec *ic = audio_codecs[i++];
        printf("codec: short_name[%s] name(%s)\n", ic->getshortname(), ic->getname());
    }
}

void uninit_codec_sh_audio(sh_audio_t *sh_audio) {
    if(sh_audio->inited){
        //ws_log(MSGT_DECAUDIO,MSGL_V,MSGTR_UninitAudioStr,sh_audio->codec->drv);

        sh_audio->ad_driver->uninit(sh_audio);
        sh_audio->inited=0;
    }

    if(sh_audio->a_out_buffer!=sh_audio->a_buffer) free(sh_audio->a_out_buffer);
    sh_audio->a_out_buffer=NULL;
    sh_audio->a_out_buffer_size=0;
    if(sh_audio->a_buffer) free(sh_audio->a_buffer);
    sh_audio->a_buffer=NULL;
    if(sh_audio->a_in_buffer) free(sh_audio->a_in_buffer);
    sh_audio->a_in_buffer=NULL;
}

/**
 * Allocate the memory that aligned at 16 bytes bound
 */
static void *memalignInner(size_t size) {
    uintptr_t diff;
    void *ptr;
    ptr = malloc(size+16);
    if(!ptr)
        return ptr;
    diff= ((-(long)ptr - 1)&15) + 1;
    *((char*)ptr) = diff;
    return &((char*)ptr)[diff]; // to avoid the warning
}

int init_codec_sh_audio(sh_audio_t *sh_audio) {
    if(!sh_audio->ad_driver->preinit(sh_audio))
    {
        ws_log::write(log::M_DECAUDIO,log::ERR, MSGTR_ADecoderPreinitFailed);
        return 0;
    }

    /* allocate audio in buffer: */
    if(sh_audio->audio_in_minsize>0){
        sh_audio->a_in_buffer_size=sh_audio->audio_in_minsize;

        ws_log::write(log::M_DECAUDIO,log::VERBOSE, MSGTR_AllocatingBytesForInputBuffer,
                sh_audio->a_in_buffer_size);

        //FIXME: align memory alloc
#if 0
        sh_audio->a_in_buffer=(char*)memalign(16, sh_audio->a_in_buffer_size);
#else
        sh_audio->a_in_buffer=(char*)memalignInner(sh_audio->a_in_buffer_size);
#endif
        memset(sh_audio->a_in_buffer,0,sh_audio->a_in_buffer_size);
        sh_audio->a_in_buffer_len=0;
    }

    /* allocate audio out buffer: */
    sh_audio->a_buffer_size=sh_audio->audio_out_minsize+MAX_OUTBURST; /* worst case calc.*/

    ws_log::write(log::M_DECAUDIO,log::VERBOSE, MSGTR_AllocatingBytesForOutputBuffer,
            sh_audio->audio_out_minsize,MAX_OUTBURST,sh_audio->a_buffer_size);

    //FIXME: align memory alloc
#if 0
    sh_audio->a_buffer=(char*)memalign(16, sh_audio->a_buffer_size);
#else
    sh_audio->a_buffer=(char*)memalignInner(sh_audio->a_buffer_size);
#endif
    if(!sh_audio->a_buffer){
        ws_log(log::M_DECAUDIO, log::ERR)
            << MSGTR_CantAllocAudioBuf
            << endlog;
        return 0;
    }
    memset(sh_audio->a_buffer,0,sh_audio->a_buffer_size);
    sh_audio->a_buffer_len=0;

    if(!sh_audio->ad_driver->init(sh_audio)){
        ws_log(log::M_DECAUDIO, log::WARN)
            << MSGTR_ADecoderInitFailed
            << endlog;

        uninit_codec_sh_audio(sh_audio); // free buffers
        return 0;
    }

    sh_audio->inited = 1;

    if(!sh_audio->channels || !sh_audio->samplerate){
        ws_log(log::M_DECAUDIO, log::WARN)
                << MSGTR_UnknownAudio
                << endlog;
        uninit_codec_sh_audio(sh_audio); // free buffers
        return 0;
    }

    if(!sh_audio->o_bps)
        sh_audio->o_bps=sh_audio->channels*sh_audio->samplerate*sh_audio->samplesize;

    ws_log::write(log::M_DECAUDIO, log::INFO, "AUDIO: %d Hz, %d ch, %d, %3.1f kbit/%3.2f%% (ratio: %d->%d)\n",
        sh_audio->samplerate,sh_audio->channels,
        (sh_audio->sample_format),
        sh_audio->i_bps*8*0.001,((float)sh_audio->i_bps/sh_audio->o_bps)*100.0,
        sh_audio->i_bps,sh_audio->o_bps);

    ws_log(log::M_IDENTIFY, log::INFO)
        << "ID_AUDIO_BITRATE=" << sh_audio->i_bps*8 << "\n"
        << "ID_AUDIO_RATE=" << sh_audio->samplerate << "\n"
        << "ID_AUDIO_NCH=" << sh_audio->channels
        << endlog;

    sh_audio->a_out_buffer_size=sh_audio->a_buffer_size;
    sh_audio->a_out_buffer=sh_audio->a_buffer;
    sh_audio->a_out_buffer_len=sh_audio->a_buffer_len;

    return 1;
}

int init_best_audio_codec(sh_audio_t *sh_audio)
{
    sh_audio->codec=NULL;
    sh_audio->ad_driver = NULL;

    int i, x;
    for(i=0;audio_codecs[i];i++) {
        IAudioCodec* audio_codec = audio_codecs[i];

        if(!audio_codec){ /* driver not available (==compiled in) */
            //ws_log(MSGT_DECAUDIO,MSGL_ERR,MSGTR_AudioCodecFamilyNotAvailableStr);

            continue;
        }

        for(x=0;audio_codec->getdlls()[x].display_name;x++) {
            const fileformat_t *ad_format = &(audio_codec->getdlls()[x]);

            /* Matching formats */
            if(ad_format->format == sh_audio->format) {
                sh_audio->codec = (codecs_t*)malloc(sizeof(codecs_t));
                sh_audio->codec->name = audio_codec->getname();
                sh_audio->codec->short_name = audio_codec->getshortname();
                sh_audio->codec->drv = audio_codec->getname();
                sh_audio->codec->dll = ad_format->display_name;

                /* it's available, let's try to init!
                // init() */
                ws_log::write(log::M_DECAUDIO,log::INFO, MSGTR_OpeningAudioDecoder,audio_codec->getshortname(), audio_codec->getname());

                sh_audio->ad_driver = audio_codec;
                if(!init_codec_sh_audio(sh_audio)){
                    ws_log::write(log::M_DECAUDIO,log::INFO, MSGTR_ADecoderInitFailed);

                    free(sh_audio->codec);
                    sh_audio->ad_driver = NULL;
                    continue; // try next...
                }
                return 1;
            }
        }
    }
    return 0;
}


int free_codec(sh_audio_t *sh) {
    IAudioCodec* func = sh->ad_driver;
    if(sh->codec) free(sh->codec);
    return func->uninit(sh);
}
int control_codec(sh_audio_t *sh, int cmd, void* arg, ...) {
    IAudioCodec* func = sh->ad_driver;
    return func->control(sh, cmd, arg);
}
int decode_codec(sh_audio_t *sh, unsigned char *buf, int minlen, int maxlen) {
    IAudioCodec* func = sh->ad_driver;
    return func->decode(sh, buf, minlen, maxlen);
}
int seek_codec(sh_audio_t *sh, long seek_sec) {
    return 0;//func->seek(sh, seek_sec);
}

} // namespace openwsp
