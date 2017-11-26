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
#include <cstdio>
#include <cstring>
#include <inttypes.h>
#include <openwsp/types.h>

extern "C" {
#include "libavcodec/avcodec.h"
}
#include <openwsp/audioformat.h>
#include "audcodec.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

static class FFMpegImpl : public IAudioCodec {
public:
    const char *getname() const {
        return "FFmpeg/libavcodec audio decoders";
    }
    const char *getshortname() const {
        return "FFmpeg";
    }
    const fileformat_t *getdlls() const {
        static fileformat_t dlls[MAX_AD_DLLS] = {
            {0x55,"mp3"},
            {0x5500736d, "mp3"},
            {0xffffffff,NULL},
        };
        return dlls;
    }

    FFMpegImpl() {
        avcodec_inited = false;
    }

    int preinit(sh_audio_t *sh_audio) {
        sh_audio->audio_out_minsize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
        return 1;
    }

    int init(sh_audio_t *sh_audio) {
        int x;
        AVCodecContext *lavc_context;
        AVCodec *lavc_codec;

        ws_log(log::M_DECAUDIO,log::VERBOSE)
            << "FFmpeg's libavcodec audio codec"
            << endlog;

        if(!avcodec_inited){
            avcodec_register_all();
            avcodec_inited = true;
        }

        lavc_codec = (AVCodec *)avcodec_find_decoder_by_name(sh_audio->codec->dll);

        if(!lavc_codec){
            ws_log::write(log::M_DECAUDIO,log::ERR, MSGTR_MissingLAVCcodec,sh_audio->codec->dll);
            return 0;
        }

        lavc_context = avcodec_alloc_context();
    #if 0
        lavc_context = avcodec_alloc_context3(lavc_codec);
    #endif
        sh_audio->context=lavc_context;

        lavc_context->sample_rate = sh_audio->samplerate;
        lavc_context->bit_rate = sh_audio->i_bps * 8;
        if(sh_audio->wf) {
            lavc_context->channels = sh_audio->wf->nChannels;
            lavc_context->sample_rate = sh_audio->wf->nSamplesPerSec;
            lavc_context->bit_rate = sh_audio->wf->nAvgBytesPerSec * 8;
            lavc_context->block_align = sh_audio->wf->nBlockAlign;
            //lavc_context->bits_per_sample = sh_audio->wf->wBitsPerSample;
        }
        lavc_context->request_channels = audio_output_channels;
        lavc_context->codec_tag = sh_audio->format; //FOURCC
        lavc_context->codec_id = lavc_codec->id; // not sure if required, imho not --A'rpi

        /* alloc extra data */
        if (sh_audio->wf && sh_audio->wf->cbSize > 0) {
            lavc_context->extradata = (uint8_t*)av_mallocz(sh_audio->wf->cbSize + FF_INPUT_BUFFER_PADDING_SIZE);
            lavc_context->extradata_size = sh_audio->wf->cbSize;
            memcpy(lavc_context->extradata, (char *)sh_audio->wf + sizeof(WAVEFORMATEX),
                   lavc_context->extradata_size);
        }

        /* for QDM2 */
        if (sh_audio->codecdata_len && sh_audio->codecdata && !lavc_context->extradata)
        {
            lavc_context->extradata = (uint8_t*)av_malloc(sh_audio->codecdata_len);
            lavc_context->extradata_size = sh_audio->codecdata_len;
            memcpy(lavc_context->extradata, (char *)sh_audio->codecdata,
                   lavc_context->extradata_size);
        }

        /* open it */
        if (avcodec_open(lavc_context, lavc_codec) < 0) {
    #if 0
        if (avcodec_open2(lavc_context, lavc_codec, NULL) < 0) {
    #endif
            ws_log::write(log::M_DECAUDIO,log::ERR, MSGTR_CantOpenCodec);
            return 0;
        }

        ws_log(log::M_DECAUDIO,log::VERBOSE)
            << "INFO: libavcodec init OK!"
            << endlog;

        if(sh_audio->format==0x3343414D){
            /* MACE 3:1 */
            sh_audio->ds->ss_div = 2*3; // 1 samples/packet
            sh_audio->ds->ss_mul = 2*sh_audio->wf->nChannels; // 1 byte*ch/packet
        } else
        if(sh_audio->format==0x3643414D){
            /* MACE 6:1 */
            sh_audio->ds->ss_div = 2*6; // 1 samples/packet
            sh_audio->ds->ss_mul = 2*sh_audio->wf->nChannels; // 1 byte*ch/packet
        }

        /* Decode at least 1 byte:  (to get header filled) */
        x = decode(sh_audio,(unsigned char*)sh_audio->a_buffer,1,sh_audio->a_buffer_size);
        if(x>0) sh_audio->a_buffer_len=x;

        sh_audio->channels=lavc_context->channels;
        sh_audio->samplerate=lavc_context->sample_rate;
        sh_audio->i_bps=lavc_context->bit_rate/8;

        if(sh_audio->wf){
            /* If the decoder uses the wrong number of channels all is lost anyway.
             * sh_audio->channels=sh_audio->wf->nChannels; */
            if (sh_audio->wf->nSamplesPerSec)
                sh_audio->samplerate=sh_audio->wf->nSamplesPerSec;
            if (sh_audio->wf->nAvgBytesPerSec)
                sh_audio->i_bps=sh_audio->wf->nAvgBytesPerSec;
        }
        sh_audio->samplesize = 2;

        return 1;
    }

    int uninit(sh_audio_t *sh) {
        AVCodecContext *lavc_context = (AVCodecContext*)sh->context;

        if (avcodec_close(lavc_context) < 0) {
            ws_log(log::M_DECAUDIO, log::ERR)
                << MSGTR_CantCloseCodec
                << endlog;
        }

        av_freep(&lavc_context->extradata);
        av_freep(&lavc_context);

        return 1;
    }

    int control(sh_audio_t *sh, int cmd, void* arg, ...) {
        AVCodecContext *lavc_context = (AVCodecContext*)sh->context;

        switch(cmd) {
            case ADCTRL_RESYNC_STREAM:
                avcodec_flush_buffers(lavc_context);
                return ADCONTROL_TRUE;
        }

        return ADCONTROL_UNKNOWN;
    }

    int decode(sh_audio_t *sh, unsigned char *buf, int minlen, int maxlen) {
    #if 0
        unsigned char *packet_data = NULL;
        int packet_size = 0;
        int len=-1;

        AVPacket packet;
        AVFrame *frame = avcodec_alloc_frame();

        while(len < minlen) {
            int dec_len = maxlen;
            double pts;

            packet_size = ds_get_packet_pts(sh->ds, &packet_data, &pts);
            if(packet_size<=0) break; /* error */
            if (pts != MP_NOPTS_VALUE) {
                sh->pts = pts;
                sh->pts_bytes = 0;
            }

            packet.data = packet_data;
            packet.size = packet_size;

            int gotFrames = 0;
            int y = avcodec_decode_audio4(sh->context, frame, &gotFrames, &packet);

            if(y<0){ ws_log(MSGT_DECAUDIO,MSGL_V,"lavc_audio: error\n");break; }
            if(y<packet_size) sh->ds->buffer_pos+=y - packet_size;  /* put back data (HACK!) */

            dec_len = frame->linesize[0];
            if(dec_len>0) {
                memcpy(buf, frame->data[0], dec_len);

                if(len<0) len=dec_len; else len+=dec_len;
                buf+=dec_len;
                maxlen -= dec_len;
                sh->pts_bytes += dec_len;

            }
            ws_log(log::M_DECAUDIO,log::DBG2)
                << "Decoded "
                << y << " -> " << dec_len
                << endlog;
        }

        av_free(frame);

        return len;
    #else
        unsigned char *start=NULL;
        int y,len=-1;
        while(len<minlen){
            int len2=maxlen;
            double pts;
            int x=ds_get_packet_pts(sh->ds,&start, &pts);
            if(x<=0) break; // error
            if (pts != MP_NOPTS_VALUE) {
                sh->pts = pts;
                sh->pts_bytes = 0;
            }
            y=avcodec_decode_audio2((AVCodecContext*)sh->context,(int16_t*)buf,&len2,start,x);
        //  printf("return:%d samples_out:%d bitstream_in:%d sample_sum:%d\n", y, len2, x, len); fflush(stdout);
            if(y<0){
                ws_log(log::M_DECAUDIO, log::VERBOSE)
                    << "lavc_audio: error"
                    << endlog;
                break;
            }

            if(y<x) sh->ds->buffer_pos+=y-x;  // put back data (HACK!)
            if(len2>0){
        //    len=len2;break;
              if(len<0) len=len2; else len+=len2;
              buf+=len2;
              maxlen -= len2;
              sh->pts_bytes += len2;
            }
            ws_dbg(MSGT_DECAUDIO,MSGL_DBG2,"Decoded %d -> %d  \n",y,len2);
        }
      return len;
    #endif
    }

private:
    bool avcodec_inited;

#if 0
    void dump_codec_ctx(AVCodecContext *pCodecCtx) {
        printf("\nSelected Codec [%s].\n", pCodecCtx->codec->long_name);
        printf("========================================================\n");
        printf("->bit_rate = %d\n", pCodecCtx->bit_rate);
        printf("->sample_rate = %d\n", pCodecCtx->sample_rate);
        printf("->channels = %d\n", pCodecCtx->channels);
        printf("->format = %d\n", pCodecCtx->sample_fmt);
        printf("->frame_size = %d\n", pCodecCtx->frame_size);
        printf("->frame_number = %d\n", pCodecCtx->frame_number);
        printf("->delay = %d\n", pCodecCtx->delay);
        printf("->frame_bits = %d\n", pCodecCtx->frame_bits);
        printf("========================================================\n");
    }
#endif
} audio_codec_ffmpeg_instance;

//
// extern
//
IAudioCodec *audio_codec_ffmpeg = static_cast<IAudioCodec*>(&audio_codec_ffmpeg_instance);


} // namespace openwsp
