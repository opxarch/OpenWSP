/** @file
 * OpenWSP - Audio Thread.
 */

/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
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

#include <openwsp/thread.h>
#include <openwsp/err.h>
#include <openwsp/timer.h>

#include "stream/stream.h"
#include "audiosys/audiosystem.h"
#include "codec/audcodec.h"
#include "codec/aud.h"
#include "config.h"
#include "openwsp.h"

#include "threads.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

ThreadAudio::ThreadAudio()
    : demuxer(0),
      stream(0),
      sh_audio(0),
      ds_audio(0),
      audio_out(0),
      file_format(0),
      state(ARS_TERMINATING),
      audio_delay(0),
      audio_clock(0.0f),
      audio_clock_length(0),
      m_write_len(0),
      m_write_total(0),
      streamCacheSize(0),
      streamCacheMinPercent(0),
      streamCacheSeekMinPercent(0),
      volumeLeft(0),
      volumeRight(0)
{
}

ThreadAudio::~ThreadAudio() {

}

////////////////////////////////////////////////////////////////////////////////

int ThreadAudio::init(void *opaque) {
    /*
     * Register configurations
     */
    config().insert(new configElement("streamCacheSize", 320, &streamCacheSize));
    config().insert(new configElement("streamCacheMinPercent", 20, &streamCacheMinPercent));
    config().insert(new configElement("streamCacheSeekMinPercent", 50, &streamCacheSeekMinPercent));
    config().insert(new configElement("mainVolumeLeft", 100, &volumeLeft));
    config().insert(new configElement("mainVolumeRight", 100, &volumeRight));

    return this->WSThread::create(opaque);
}

int ThreadAudio::uninit() {
    return WINF_SUCCEEDED;
}

/////////////////////////////////////////////////////////////////////

// Begin event looping

/**
 * Entry of I/O thread context.
 * @param opaque pointer data.
 * @return status code.
 */
int ThreadAudio::run(void *opaque) {
    int rc;
    WSEvent *event;

    for (;;) {
        /*
         Process the Audio events
         */
        rc = this->EventPump().pop(&event);
        if (WS_SUCCESS(rc)) {
            event->run(this);
            event->release(MakeLocator());
        }

        if (this->cancelRaised()) {
            return WINF_TERMINATED;
        }

        /*
         * Processing the root state machine
         */
        switch(state) {
        case ARS_IDLE: {
            usecSleep(0);
        }
        break;

        case ARS_LOADING: {
            rc = this->onCreateContext(filenameToLoad.c_str());
            if (WS_SUCCESS(rc)) {
                state = ARS_PROCESSING;
            } else {
                state = ARS_TERMINATING;
            }
        }
        break;

        case ARS_PROCESSING: {
            rc = this->onRunProcess(this);
            if (rc == WINF_TERMINATED || WS_FAILURE(rc)) {
                state = ARS_TERMINATING;
            }
        }
        break;

        case ARS_STALLING: {
            pause_audio(audio_out);
            state = ARS_STALLED;
        }
        break;

        case ARS_STALLED: {
            usecSleep(0); /* idle */
        }
        break;

        case ARS_RESUMING: {
            resume_audio(audio_out);
            state = ARS_PROCESSING;
        }
        break;

        case ARS_TERMINATING: {
            rc = this->onEndContext();
            state = ARS_IDLE;
        }
        }
    }
    return WINF_SUCCEEDED;
}

// End event looping

/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
// for Audio Thread context only
////////////////////////////////////////////////////////////////////

/**
 * initiate the audio chain (such as codecs and devices)
 * @return status code
 */
int ThreadAudio::onSetupAudioChain(void) {
    AssertThread(THREAD_AUDIO);
    /*
     Initialize codec
     */
    if(!init_best_audio_codec(sh_audio)) {
        sh_audio = 0;
        ds_audio->sh = 0; /* failed to init */
        ds_audio->id = -2;
        return WERR_INIT_BEST_AUDIO_CODEC;
    }

    /*
     Initialize default audio device
     */
    IAudioOutput *audio_dev = init_audio_device("sdl", sh_audio->samplerate, sh_audio->channels, sh_audio->sample_format, 0);

    if(audio_dev == NULL){
        sh_audio = 0;
        ds_audio->sh = 0; /* failed to init */
        ds_audio->id = -2;
        return WERR_INIT_AUDIO_DEVICE;
    }

    audio_out = audio_dev;

    /*
     * Sync the initial volume
     */
    as_control_vol_t vol = {
        .left = CONFIG_GET(volumeLeft),
        .right = CONFIG_GET(volumeRight)
    };

    audio_out->control(AOCONTROL_GET_VOLUME, &vol);

    printf("AUDIO :%s\n", audio_dev->getname());
    return WINF_SUCCEEDED;
}

/**
 * Create a context for audio processing.
 * @param url           The URL string of audio
 * @return status code
 */
int ThreadAudio::onCreateContext(const char *url) {
    AssertThread(THREAD_AUDIO);
    int rc;

    /*
     reset all
     */
    this->onEndContext();
    stream = NULL;
    demuxer = NULL;
    ds_audio = NULL;
    sh_audio = NULL;
    audio_clock = 0;
    audio_delay = 0;
    audio_clock_length = 0;
    m_write_len = 0;
    m_write_total = 0;

    ws_log(log::INFO) << "onCreateContext(): url = " << url << endlog;
    stream = open_stream(url, NULL, &file_format);

    if (!stream) {
        return WERR_OPEN_STREAM;
    }

    switch(file_format) {

    /*
     Handle the play;ist
     */
    case DEMUXER_TYPE_PLAYLIST:
        //!todo add playlist management.

        /* we can't create a audio context there. */
        return WERR_PLAYLIST;
    }

    /*
     * Fix up the cache parameters
     */
    if(CONFIG_GET(streamCacheSize) < 0) {
        /*
         * cache option not set, will use our computed value.
         * buffer in KBytes, *5 because the prefill is 20% of the buffer.
         */
        CONFIG_SET(streamCacheSize, (stream->streaming_ctrl->prebuffer_size/1024)*5);
        if( CONFIG_GET(streamCacheSize) < 64 ) CONFIG_SET(streamCacheSize, 64);  // 16KBytes min buffer
    }

    /*
     * Enable cache sub-system
     */
    if(CONFIG_GET(streamCacheSize) > 0) {
        if(!stream_enable_cache(stream,CONFIG_GET(streamCacheSize)*1024,
                              CONFIG_GET(streamCacheSize)*1024*(CONFIG_GET(streamCacheMinPercent) / 100.0),
                              CONFIG_GET(streamCacheSize)*1024*(CONFIG_GET(streamCacheSeekMinPercent) / 100.0)))
        return WERR_STREAM_ENABLE_CACHE;
    }

    if(stream == NULL) {
        return WERR_OPEN_STREAM;
    }

    int audio_id=-1;
    int video_id=-1;
    int dvdsub_id=-2;

    demuxer = demux_open(stream,file_format,audio_id,video_id,dvdsub_id,(char*)url);

    if (demuxer && demuxer->type==DEMUXER_TYPE_PLAYLIST) {
        /* we can't create a audio context there. */
        return WERR_PLAYLIST;
    }

    if(demuxer != NULL) {
        ds_audio = demuxer->audio;
        sh_audio = (sh_audio_t*)ds_audio->sh;

        if(sh_audio) {
            rc = this->onSetupAudioChain();
            if (WS_FAILURE(rc) || (sh_audio==NULL) || (sh_audio->codec==NULL))
                return rc;

            /*
             * Get some extra information and complete
             */
            audio_clock_length = demuxer_get_time_length(demuxer);
            return WINF_SUCCEEDED;
        }
    }
    return WERR_OPEN_DEMUXER;
}

/**
 * Run the audio processing loop
 * @return status code
 */
int ThreadAudio::onRunProcess(ThreadBase *thread) {
    AssertThread(THREAD_AUDIO);
    int rc;

    /*
     check the context. Be sure that we have initiated all the
     things in audio chain and others needed.
     */
    if (!stream || !demuxer || !ds_audio || !sh_audio) {
      return WERR_FAILED;
    }

    m_write_total = audio_out->get_space();
#if 0
      if (m_write_total)
          ws_log(log::INFO) << "write_total = " << m_write_total << endlog;
#endif

    if (m_write_total) {
        m_write_len = (m_write_total > MAX_OUTBURST)?MAX_OUTBURST:m_write_total;
        m_write_total -= m_write_len;

        /*
         * Fill buffer if needed
         */
        if (sh_audio->a_out_buffer_len < m_write_len) {
            int buflen = sh_audio->a_out_buffer_len;

            /* Decode audio */
            int min = m_write_len - buflen;
            int max = sh_audio->a_out_buffer_size - buflen;
            int ret = decode_audio(sh_audio, (unsigned char*)&sh_audio->a_out_buffer[buflen], min, max);

            if (ret <= 0) { // EOF?
                if (ds_audio->eof) {
                    while(audio_out->get_delay() > .04);

                    sh_audio->a_out_buffer_len = 0;
                    m_write_total = 0;

                    ws_log(log::WARN) << "Audio output truncated at end." << endlog;
                }
                return WINF_TERMINATED;
            }
            sh_audio->a_out_buffer_len += ret;

            return WINF_CONTINUING; // continue
        }

        if (m_write_len > sh_audio->a_out_buffer_len) {
            m_write_len = sh_audio->a_out_buffer_len;
        }
        if (!m_write_len) return WINF_CONTINUING; // continue

        /*
         * The buffer is filled, so now write prepared data
         * to the audio hardware device.
         */
        m_write_len = write_audio(audio_out, sh_audio->a_out_buffer, m_write_len, 0);

        if (m_write_len > 0) {
            sh_audio->a_out_buffer_len -= m_write_len;
            memmove(sh_audio->a_out_buffer,
                    &sh_audio->a_out_buffer[m_write_len],
                    sh_audio->a_out_buffer_len);

            audio_delay = get_delay_audio(audio_out);
            audio_clock += (double)m_write_len / audio_out->m_bps;
        }

    } // if

    return WINF_CONTINUING; // continue
}

/**
 * Terminate the context for audio processing.
 * @return status code
 */
int ThreadAudio::onEndContext() {
    AssertThread(THREAD_AUDIO);
    /* uninit */
    if(sh_audio){
        uninit_codec_sh_audio(sh_audio);
        sh_audio = 0;
    }

    if(demuxer){
        stream = demuxer->stream;
        free_demuxer(demuxer);
        demuxer = 0;
    }

    if(stream) {
        free_stream(stream);
        stream = 0;
    }

    if(audio_out) {
        free_audio(audio_out, 0);
        audio_out = 0;
    }
    return WINF_SUCCEEDED;
}

/////////////////////////////////////////////////////////////////////

int ThreadAudio::onLoadAudio(const char *file) {
    if (state != ARS_IDLE) {
        this->onEndContext();
    }

    filenameToLoad = file;
    state = ARS_LOADING;

    ws_log(log::INFO) << "onLoadAudio(): url = " << file << endlog;

    return WINF_SUCCEEDED;
}

/////////////////////////////////////////////////////////////////////

int ThreadAudio::onGetVolume(int *left, int *right) {
    AssertThread(THREAD_AUDIO);
    if (audio_out) {
        as_control_vol_t vol;
        int rc = audio_out->control(AOCONTROL_GET_VOLUME, &vol);

        if (WS_SUCCESS(rc)) {
            if (left) *left = vol.left;
            if (right) *right = vol.right;
        }
    }

    if (left) *left = CONFIG_GET(volumeLeft);
    if (right) *right = CONFIG_GET(volumeRight);

    return 1;
}

/////////////////////////////////////////////////////////////////////

int ThreadAudio::onSetVolume(int left, int right) {
    AssertThread(THREAD_AUDIO);
    if (audio_out) {
        as_control_vol_t vol;
        vol.left = left;
        vol.right = right;

        audio_out->control(AOCONTROL_GET_VOLUME, &vol);
    }
    CONFIG_SET(volumeLeft, left);
    CONFIG_SET(volumeRight, right);
    return 1;
}

} // namespace openwsp
