/** @file
 * OpenWSP - Main Thread.
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

#include "audiosys/audiosystem.h"

#include "gc.h"
#include "openwsp.h"
#include "threads.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

ThreadMain::ThreadMain()
    : event(this),
      m_gc(new GC)
{
}
ThreadMain::~ThreadMain() {
    delete m_gc;
}

////////////////////////////////////////////////////////////////////////////////

int ThreadMain::init(void *opaque) {
    int rc;
    rc = this->WSThread::create(opaque);
    return rc;
}

int ThreadMain::uninit() {
    return WINF_SUCCEEDED;
}


/////////////////////////////////////////////////////////////////////

// Begin event looping

/**
 * Entry of I/O thread context.
 * @param opaque pointer data.
 * @return status code.
 */
int ThreadMain::run(void *opaque) {
    int rc;
    WSEvent *event;

    for (;;) {
        /*
         Process the main events
         */
        rc = this->EventPump().pop(&event);
        if (WS_SUCCESS(rc)) {
            event->run(this);
            event->release(MakeLocator());
        }

        /*
         Process the UI events
         */
        rc = app().idle();
        if (WS_FAILURE(rc) ||
                (rc == WINF_TERMINATED)) {
            return rc;
        }

        this->onScanGC();
    }

    return WINF_SUCCEEDED;
}

// End event looping

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// for Main Thread context only
////////////////////////////////////////////////////////////////////

int ThreadMain::onInsertGC(GCTarget *src) {
    AssertThread(THREAD_MAIN);
    return m_gc->addRegistry(src);
}

void ThreadMain::onScanGC() {
    AssertThread(THREAD_MAIN);
    m_gc->scan();
}

/////////////////////////////////////////////////////////////////////

int ThreadMain::onLoadMedia(const std::string &url) {
    app().threadLoop().PostEvent(
            THREAD_AUDIO,
            event.bind(&ThreadMain::eventRunAudioProcess, url));
    return 1;
}

// THREAD_AUDIO

int ThreadMain::eventRunAudioProcess(ThreadBase *thread, std::string url) {
    app().threadAudio().onLoadAudio(url.c_str());
    return 1;
}

} // namespace openwsp
