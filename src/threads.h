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

#ifndef THREADS_H_
#define THREADS_H_

#include <string>

#include <openwsp/thread.h>
#include <openwsp/autoptr.h>
#include <openwsp/list.h>

#include "eventfactory.h"
#include "eventpump.h"

namespace openwsp {

class Webservice;
class catalog;
class channel;
class channelURL;
class GC;
class GCTarget;

/**
 * Base class of event-processing thread
 */
class ThreadBase : public WSThread {
public:
    ThreadBase() {}
    ~ThreadBase() {}

    inline WSEventPump &EventPump() { return m_event_pump; }

    /**
     * Push a event to the queue.
     * @param pri The priority of target event.
     * @param event Pointer to the target event.
     */
    int PostEvent(WSEvent *event) {
        return m_event_pump.push(event);
    }

protected:
    WSEventPump m_event_pump;
};

/***************************************************
  *****           I/O Thread object            *****
  ***************************************************/

class ThreadIO : public ThreadBase {
public:
    ThreadIO();
    ~ThreadIO();

    int init(void *opaque);
    int uninit();

    virtual int run(void *opaque);

public:
    /////////////////////////////////////////////////////////////////////
    // for I/O Thread context only
    ////////////////////////////////////////////////////////////////////
    int onGetCatalogs(WSList<catalog *> *dst);
    int onGetChannels(int id, WSList<channel *> *dst);
    int onGetChannelInfo(int id, channel **out);
    int onGetChannelURL(int id, channelURL **out);

private:
    Webservice *websrv;
};

/*
 root status of audio thread
 */
enum ARS_e {
    ARS_IDLE = 0,
    ARS_LOADING,
    ARS_PROCESSING,
    ARS_STALLING,
    ARS_STALLED,
    ARS_RESUMING,
    ARS_TERMINATING
};

/***************************************************
  *****           Audio Thread object            *****
  ***************************************************/

class ThreadAudio : public ThreadBase {
public:
    ThreadAudio();
    ~ThreadAudio();

    int init(void *opaque);
    int uninit();

    virtual int run(void *opaque);

public:
    /////////////////////////////////////////////////////////////////////
    // for Audio Thread context only
    ////////////////////////////////////////////////////////////////////
    int onSetupAudioChain();
    int onCreateContext(const char *url);
    int onRunProcess(ThreadBase *thread);
    int onEndContext();

    int onLoadAudio(const char *file);

    int onGetVolume(int *left, int *right);
    int onSetVolume(int left, int right);

private:
    struct demuxer_s        *demuxer;
    struct stream_s         *stream;
    struct sh_audio_s       *sh_audio;
    struct demux_stream_s   *ds_audio;
    class IAudioOutput      *audio_out;

    int             file_format;

    /* status */
    ARS_e           state;
    int             audio_delay;
    double          audio_clock;
    double          audio_clock_length;
    std::string     filenameToLoad;
    int             m_write_len;
    int             m_write_total;
    int             volumeLeft;
    int             volumeRight;
};

/***************************************************
  *****           Main Thread object            *****
  ***************************************************/

class ThreadMain : public ThreadBase {
public:
    ThreadMain();
    ~ThreadMain();

    int init(void *opaque);
    int uninit();

    virtual int run(void *opaque);

public:
    /////////////////////////////////////////////////////////////////////
    // for Main Thread context only
    ////////////////////////////////////////////////////////////////////
    int     onInsertGC(GCTarget *src);
    void    onScanGC();
    int     onLoadMedia(const std::string &url);

private:
    int eventRunAudioProcess(ThreadBase *thread, std::string url);

private:
    EventFactory<ThreadMain> event;
    GC *m_gc;
};


} // namespace openwsp

#endif //!defined(THREADS_H_)
