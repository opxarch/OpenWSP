/** @file
 * Openwsp - audio codec - PCM.
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <openwsp/misc.h>
#include <openwsp/audioformat.h>
#include "audcodec.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

class PCMImpl : public IAudioCodec {
public:
    const char *getname() const {
        return "Uncompressed PCM audio decoder";
    }
    const char *getshortname() const {
        return "pcm";
    }
    const fileformat_t *getdlls() const {
        static fileformat_t dlls[MAX_AD_DLLS] = {
            {0x1,"pcm"},
            {0x0,"pcm"},
            {0x1,"pcm"},
            {0x3,"pcm"},         // IEEE float
            {0xfffe,"pcm"},      // Extended
            {0x20776172,"raw "},  //  (MOV files)
            {0x736f7774,"twos"},  //  (MOV files)
            {0x74776f73,"sowt"},  //  (MOV files)
            {0x32336c66,"fl32"},  //  (MOV files)
            {0x666c3332,"23lf"},  //  (MOV files)
            {0x34366c66,"fl64"},  //  (MOV files)
            {0x666c3634,"46lf"},  //  (MOV files)
            {0x454e4f4e,"NONE"},  //  (MOV files from Kodak CX6320)
            {0x34326e69,"in24"},  //  (MOV files)
            {0x696e3234,"42ni"},  //  (MOV files)
            {0x32336e69,"in32"},  //  (MOV files)
            {0x696e3332,"pcm"},
            {0xffffffff,NULL}
        };
        return dlls;
    }

public:
    int init(sh_audio_t *sh_audio) {
        WAVEFORMATEX *h=sh_audio->wf;
        sh_audio->i_bps=h->nAvgBytesPerSec;
        sh_audio->channels=h->nChannels;
        sh_audio->samplerate=h->nSamplesPerSec;
        sh_audio->samplesize=(h->wBitsPerSample+7)/8;
        sh_audio->sample_format=AF_FORMAT_S16_LE; // default

        switch(sh_audio->format) { /* hardware formats: */
            case 0x0:
            case 0x1: /* Microsoft PCM */
            case 0xfffe: /* Extended */
                switch (sh_audio->samplesize) {
                    case 1: sh_audio->sample_format=AF_FORMAT_U8; break;
                    case 2: sh_audio->sample_format=AF_FORMAT_S16_LE; break;
                    case 3: sh_audio->sample_format=AF_FORMAT_S24_LE; break;
                    case 4: sh_audio->sample_format=AF_FORMAT_S32_LE; break;
                }
                break;
            case 0x3: /* IEEE float */
                sh_audio->sample_format=AF_FORMAT_FLOAT_LE;
                break;
            case 0x6:  sh_audio->sample_format=AF_FORMAT_A_LAW;break;
            case 0x7:  sh_audio->sample_format=AF_FORMAT_MU_LAW;break;
            case 0x11: sh_audio->sample_format=AF_FORMAT_IMA_ADPCM;break;
            case 0x50: sh_audio->sample_format=AF_FORMAT_MPEG2;break;
            case 0x20776172: /* 'raw ' */
                sh_audio->sample_format=AF_FORMAT_S16_BE;
                if(sh_audio->samplesize==1) sh_audio->sample_format=AF_FORMAT_U8;
                break;
            case 0x736F7774: /* 'twos' */
                sh_audio->sample_format=AF_FORMAT_S16_BE;
                break;
            case 0x74776F73: /* 'sowt' */
                if(sh_audio->samplesize==1) sh_audio->sample_format=AF_FORMAT_S8;
                break;
            case 0x32336c66: /* 'fl32', bigendian float32 */
                sh_audio->sample_format=AF_FORMAT_FLOAT_BE;
                sh_audio->samplesize=4;
                break;
            case 0x666c3332: /* '23lf', little endian float32, internal fourCC */
                sh_audio->sample_format=AF_FORMAT_FLOAT_LE;
                sh_audio->samplesize=4;
                break;
            case 0x34326e69: /* 'in24', bigendian int24 */
                sh_audio->sample_format=AF_FORMAT_S24_BE;
                sh_audio->samplesize=3;
                break;
            case 0x696e3234: /* '42ni', little endian int24, internal fourCC */
                sh_audio->sample_format=AF_FORMAT_S24_LE;
                sh_audio->samplesize=3;
                break;
            case 0x32336e69: /* 'in32', bigendian int32 */
                sh_audio->sample_format=AF_FORMAT_S32_BE;
                sh_audio->samplesize=4;
                break;
            case 0x696e3332: /* '23ni', little endian int32, internal fourCC */
                sh_audio->sample_format=AF_FORMAT_S32_LE;
                sh_audio->samplesize=4;
                break;
            default: if(sh_audio->samplesize!=2) sh_audio->sample_format=AF_FORMAT_U8;
        }
        if (!sh_audio->samplesize) /* this would cause hang later */
            sh_audio->samplesize = 2;
        return 1;
    }

    int preinit(sh_audio_t *sh) {
        sh->audio_out_minsize=2048;
        return 1;
    }

    int uninit(sh_audio_t *sh) {
        return 1;
    }

    int control(sh_audio_t *sh, int cmd, void* arg, ...) {
        int skip;
        switch(cmd) {
            case ADCTRL_SKIP_FRAME:
                skip=sh->i_bps/16;
                skip=skip&(~3);

                demux_read_data(sh->ds,NULL,skip);
                return ADCONTROL_TRUE;
        }
        return ADCONTROL_UNKNOWN;
    }

    int decode(sh_audio_t *sh_audio, unsigned char *buf, int minlen, int maxlen) {
        int len = sh_audio->channels*sh_audio->samplesize;
        len = (minlen + len - 1) / len * len;
        if (len > maxlen)
            /* if someone needs hundreds of channels adjust audio_out_minsize
            // based on channels in preinit() */
            return -1;

        len = demux_read_data(sh_audio->ds, buf, len);
        return len;
    }

} audio_codec_pcm_instance;

//
// extern
//
IAudioCodec *audio_codec_pcm = static_cast<IAudioCodec*>(&audio_codec_pcm_instance);


} // namespace openwsp

