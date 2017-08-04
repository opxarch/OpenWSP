/*
 *  OpenWSP (an opensource web stream player)
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

#ifndef OPENWSP_H_
#define OPENWSP_H_

#include <openwsp/webservice.h>
#include "gui/gui.h"
#include "threads.h"
#include "eventloop.h"
#include "eventfactory.h"

namespace openwsp {

/*
 playback status
 */
enum PSTATE {
    PSTATE_PLAYING = 0,
    PSTATE_PAUSED,
    PSTATE_STOPPED
};

class IAudioOutput;

/***************************************************
  *****  OpenWSP application object            *****
  ***************************************************/

class Openwsp {
public:
    Openwsp(int &argc, char **argv);
    ~Openwsp();

    int init();
    int uninit();

    int RunMainLoop();
    void QuitApplication();

public:
    int idle();
    int createAudioProcessContext(const char *url);
    int runAudioProcess();
    int endAudioPrcoessContext();
    int test(void *p, int i);

private:
    int initAudioChain();

private:
    struct demuxer_s *demuxer;
    struct stream_s *stream;

    struct sh_audio_s *sh_audio;
    struct demux_stream_s *ds_audio;
    IAudioOutput *audio_out;

    int     file_format;

    /* status */
    int     state;
    int     audio_delay;
    double  audio_clock;
    double  audio_clock_length;

    EventFactory<Openwsp> event;

protected:
    Webservice      websrv;
    wsGUI           gui;
    IOThread        ioThread;
    MainThread      mainThread;
public:
    EventLoop       events;
};

} // namespace openwsp

#endif //!defined(OPENWSP_H_)
