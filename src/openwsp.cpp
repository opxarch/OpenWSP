/** @file
 * OpenWSP - the application main logic.
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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <iostream>
#include <assert.h>
#include <exception>
#include <assert.h>
#include <log.h>

#include "openwsp.h"

#include <openwsp/err.h>
#include <openwsp/version.h>
#include <openwsp/misc.h>
#include <openwsp/timer.h>
#include "config.h"
#include "stream/stream.h"
#include "audiosys/audiosystem.h"
#include "codec/audcodec.h"
#include "codec/aud.h"
#include "eventfactory.h"
#include "eventloop.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
static openwsp::Openwsp *g_mainapp   = NULL;  /** the only-one application object */


namespace openwsp {

/*******************************************************************************
*   the Manager Object of the program                                                   *
*******************************************************************************/

Openwsp::Openwsp(int &argc, char **argv)
    : event(this)
{
    stream = 0;
    demuxer = 0;
    ds_audio = 0;
    sh_audio = 0;
    audio_out = 0;
    file_format = 0;
    audio_clock = 0;
    audio_delay = 0;
    audio_clock_length = 0;
    state = PSTATE_STOPPED;
}

Openwsp::~Openwsp() {
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Do the logical initialization.
 * @return status code.
 */
int Openwsp::init() {
    int rc = WERR_FAILED;
    
    /*
     initiate the webservice
     */

    WSList<catalog *> catalogs;

    rc = websrv.init();     UPDATE_RC(rc);

    //ws_log::write(log::M_AUDIOSYS, log::INFO, "1223\ndf\n   ");
    //ws_log(log::M_AUDIOSYS, log::INFO) << "123\nhellow " << "world\n12345\n456" << endlog;

#if 1
    rc = websrv.LoadModule("Qingting FM support API"); UPDATE_RC(rc);
    rc = websrv.connect(); UPDATE_RC(rc);
    rc = websrv.catalogs(&catalogs); UPDATE_RC(rc);

    catalog *c;

    for (catalogs.begin();
            catalogs.end();
            catalogs++)
    {
        rc = catalogs.get(c);
        if (WS_SUCCESS(rc)) {
            std::cout<< "name:" << c->m_name << c->m_id<<std::endl;
        }
    }

    catalogs.RemoveAllPtr();
#endif
    /*
     GUI
     */
    rc = gui.init(); UPDATE_RC(rc);

    /*
     Threads
     */
    rc = mainThread.init(this); UPDATE_RC(rc);
    rc = ioThread.init(this); UPDATE_RC(rc);

    rc = events.init(&mainThread, &ioThread); UPDATE_RC(rc);

    return WINF_SUCCEEDED;
}

/**
 * uninitiate the application.
 * @return status code.
 */
int Openwsp::uninit() {
    int rc;
    rc = events.uninit();
    AssertRC(rc);
    rc = ioThread.uninit();
    AssertRC(rc);
    rc = mainThread.uninit();
    AssertRC(rc);
    rc = gui.uninit();
    AssertRC(rc);
    rc = websrv.uninit();
    AssertRC(rc);
    return WINF_SUCCEEDED;
}

/**
 * initiate the audio chain (such as codecs and devices)
 * @return status code
 */
int Openwsp::initAudioChain(void) {
    /*
     init codec
     */
    if(!init_best_audio_codec(sh_audio)) {
        sh_audio = 0;
        ds_audio->sh = 0; /* failed to init */
        ds_audio->id = -2;
        return WERR_INIT_BEST_AUDIO_CODEC;
    }

    /*
     init default audio device
     */
    IAudioOutput *audio_dev = init_audio_device("dsound", sh_audio->samplerate, sh_audio->channels, sh_audio->sample_format, 0);

    if(audio_dev == NULL){
        sh_audio = 0;
        ds_audio->sh = 0; /* failed to init */
        ds_audio->id = -2;
        return WERR_INIT_AUDIO_DEVICE;
    }

    audio_out = audio_dev;

    printf("AUDIO :%s\n", audio_dev->getname());
    return WINF_SUCCEEDED;
}

/**
 * Create a context for audio processing.
 * @param url           The URL string of audio
 * @return status code
 */
int Openwsp::createAudioProcessContext(const char *url) {
    int rc;

    /*
     reset all
     */
    stream = NULL;
    demuxer = NULL;
    ds_audio = NULL;
    sh_audio = NULL;
    audio_clock = 0;
    audio_delay = 0;
    audio_clock_length = 0;
    state = PSTATE_STOPPED;

    stream = open_stream(url, NULL, &file_format);

    switch(file_format) {

    /*
     Handle the play;ist
     */
    case DEMUXER_TYPE_PLAYLIST:
        //!todo add playlist management.

        /* we can't create a audio context there. */
        return WERR_PLAYLIST;
    }

    if(CFG_stream_cache_size>0) {
        if(!stream_enable_cache(stream,CFG_stream_cache_size*1024,
                              CFG_stream_cache_size*1024*(CFG_stream_cache_min_percent / 100.0),
                              CFG_stream_cache_size*1024*(CFG_stream_cache_seek_min_percent / 100.0)))
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
            rc = initAudioChain();
            if (WS_FAILURE(rc) || (sh_audio==NULL) || (sh_audio->codec==NULL))
                return rc;

            /* Sync volume and get some info */
            //ctl_set_volume(&vol_buff);
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
int Openwsp::runAudioProcess() {
    bool paused = false;
    bool endplay = false;
    int write_len = 0;
    int write_total = 0;

    /*
     check the context. Be sure that we have initiated all the
     things in audio chain and others needed.
     */
    if (!stream || !demuxer || !ds_audio || !sh_audio) {
      return WERR_FAILED;
    }

    /*
     processing runloop.it will keep running until the audio is
     stopped.
     */
    while(!endplay) {

      state = PSTATE_PLAYING;
      write_total = audio_out->get_space();
      if (write_total)
          printf("write_total=%d\n", write_total);

      while (write_total) {
        write_len = (write_total > MAX_OUTBURST)?MAX_OUTBURST:write_total;
        write_total -= write_len;

        /* Fill buffer if needed */
        while (sh_audio->a_out_buffer_len < write_len) {
          int buflen = sh_audio->a_out_buffer_len;

          /* Decode audio */
          int min = write_len - buflen;
          int max = sh_audio->a_out_buffer_size - buflen;
          int ret = decode_audio(sh_audio, (unsigned char*)&sh_audio->a_out_buffer[buflen], min, max);

          if (ret <= 0) { // EOF?
            if (ds_audio->eof) {
              while(audio_out->get_delay() > .04);

              sh_audio->a_out_buffer_len = 0;
              write_total = 0;
              endplay = 1;

              printf("Audio output truncated at end.\n");
            }
            break;
          }
          sh_audio->a_out_buffer_len += ret;
        }  // while

        if (write_len > sh_audio->a_out_buffer_len) {
            write_len = sh_audio->a_out_buffer_len;
        }
        if (!write_len) break;

        /* play audio */
        write_len = write_audio(audio_out, sh_audio->a_out_buffer, write_len, 0);

        if (write_len > 0) {
            sh_audio->a_out_buffer_len -= write_len;
            memmove(sh_audio->a_out_buffer,
                    &sh_audio->a_out_buffer[write_len],
                    sh_audio->a_out_buffer_len);

            state = PSTATE_PLAYING;
            audio_delay = get_delay_audio(audio_out);
            audio_clock += (double)write_len / audio_out->m_bps;
        }

      } // while

      if(paused) {
          pause_audio(audio_out);
          state = PSTATE_PAUSED;

          while(paused && !endplay) {
              usecSleep(0); /* idle */
          }

          if(!endplay) resume_audio(audio_out);
      }

    } // outside while

    state = PSTATE_STOPPED;

    return WINF_SUCCEEDED;
}

/**
 * Terminate the context for audio processing.
 * @return status code
 */
int Openwsp::endAudioPrcoessContext() {
    /* uninit */
    if(sh_audio){
        uninit_codec_sh_audio(sh_audio);
        sh_audio = 0;
    }

    if(demuxer){
        stream=demuxer->stream;
        free_demuxer(demuxer);
        demuxer = 0;
    }

    if(stream) {
        free_stream(stream);
        stream = 0;
    }

    if(audio_out) {
        /* Sync volume */
        //ctl_get_volume(&vol_buff);

        free_audio(audio_out, 0);
        audio_out = 0;
    }
    return WINF_SUCCEEDED;
}

/**
 * Dispatch & processing the UI events during idle.
 * @return status code
 */
int Openwsp::idle() {
    return gui.DoMessages();
}

int Openwsp::test(void *p, int i) {
    std::cout<<i<<std::endl;
    return 0;
}

/**
 * Run the main running loop.
 *
 * Here responding all the events/messages/etc. from GUI
 * process, and this function will return a value when the main window
 * has been closed.
 * @returns exit code of program
 */
int Openwsp::RunMainLoop() {
    int rc;

    mainThread.PostEvent(event.create(&Openwsp::test, 1001));

    mainThread.wait(&rc);
    AssertRC(rc);

    ioThread.wait(&rc);
    AssertRC(rc);
    return WINF_SUCCEEDED;
}

/**
 * Quit the openwsp thoroughly.
 */
void Openwsp::QuitApplication() {
}

} // namespace openwsp


////////////////////////////////////////////////////////////////////////////////

static void checkError(int rc) {
    if (WS_FAILURE(rc)) {
        const ErrorInfo *err = GetErrorMsg(rc);
        std::cout<<"Error tracing: ("<<err->code<<") "<<err->msgDefine<<": "<<
                err->msgFull<<std::endl;
    }
}

/**
 * Program main entry
 *
 * @param   argc        The count of command-line arguments.
 * @param   argv        Vector list of command-line arguments.
 * @returns Exiting status.
 */
int main(int argc, char *argv[]) {
    int rc = WINF_SUCCEEDED;
    std::cout<<appBanner<<std::endl;

    try {
        // create the main application instance
        g_mainapp = new openwsp::Openwsp(argc, argv);
        if (WS_SUCCESS(rc = g_mainapp->init())) {

            const char *url = "http://http.hz.qingting.fm/386.mp3"; //; //"D:/1.wav"
            //"http://api2.qingting.fm/v6/media/channellives/142486"
#if 0
            openwsp::stream_t *s;
            int filetype;
            char buff[BUFFER_SIZE];

            int len;
            s = openwsp::open_stream(url, NULL, &filetype);
            if (s) {
                while (1) {
                    len = openwsp::stream_read(s, buff, BUFFER_SIZE);
                    if (len > 0) {
                        std::cout<<"recv:"<<buff<<std::endl;
                    } else {
                        break;
                    }
                }
            } else
                std::cout<<"failed."<<std::endl;

            std::cout<<"closed"<<std::endl;
#endif

#if 0
            rc = g_mainapp->createAudioProcessContext(url);
            checkError(rc);

            if (WS_SUCCESS(rc)) {
                g_mainapp->runAudioProcess();
                std::cout<<"audio end"<<std::endl;
                g_mainapp->endAudioPrcoessContext();
            }
#endif

            /*
             Run the ui running loop.
             it will not return until the program has been closed.
             */
            rc = g_mainapp->RunMainLoop();
            checkError(rc);
            rc = g_mainapp->uninit();
        }

        checkError(rc);
        return -1;

    } catch(std::exception& e) {
        std::cout<<"Exception tracing:"<<e.what()<<std::endl;
        exit(0);
    }
}
