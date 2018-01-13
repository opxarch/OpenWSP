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
#include <openwsp/misc.h>
#include <openwsp/tracker.h>
#include "threads.h"
#include "eventloop.h"
#include "eventfactory.h"
#include "gui/gui.h"
#include "config.h"

namespace openwsp {

inline class Openwsp &app();

/**
 * Assert that we are in the thread context for which we expected.
 */
#if ENABLE(ASSERTIONS)
# define AssertThread(type) { app().validThread(type, MakeLocator()); } while(0)
#else
# define AssertThread(type) (void)0
#endif


extern class Openwsp *openwsp_instance_;

/***************************************************
  *****  OpenWSP application object            *****
  ***************************************************/

class Openwsp {
public:
    Openwsp();
    ~Openwsp();

    int init(int &argc, char **argv);
    int uninit();

    int RunMainLoop();
    void QuitApplication();

public:

#if ENABLE(ASSERTIONS)
    int validThread(ThreadType type, const Locator &loc);
#endif

    inline EventLoop &threadLoop() {
        return *m_eventloop;
    }

    /////////////////////////////////////////////////////////////////////
    // for I/O Thread context only
    ////////////////////////////////////////////////////////////////////
    inline ThreadIO &threadIO() {
        AssertThread(THREAD_IO);
        return *m_ioThread;
    }

    /////////////////////////////////////////////////////////////////////
    // for Main Thread context only
    ////////////////////////////////////////////////////////////////////
    inline ThreadMain &threadMain() {
        AssertThread(THREAD_MAIN);
        return *m_mainThread;
    }

    /////////////////////////////////////////////////////////////////////
    // for Audio Thread context only
    ////////////////////////////////////////////////////////////////////
    inline ThreadAudio &threadAudio() {
        AssertThread(THREAD_AUDIO);
        return *m_audioThread;
    }

    /////////////////////////////////////////////////////////////////////
    // for Core Thread context only
    ////////////////////////////////////////////////////////////////////
    WSConfig &config();

    /**
     * Get the instance (create a new one if needed).
     * @return pointer to the instance.
     */
    static inline Openwsp &instance() {
        if (!openwsp_instance_) {
            openwsp_instance_ = new Openwsp();
        }
        return *openwsp_instance_;
    }
public:
    int idle();

private:
    static Openwsp *openwsp_instance_;
    EventFactory<Openwsp> event;
    ThreadId_t            m_coreThread;
    bool                  m_unstart;

protected:
    WSConfig              *m_config;
    class gui::wsGUI      *m_gui;
    class ThreadIO        *m_ioThread;
    class ThreadMain      *m_mainThread;
    class ThreadAudio     *m_audioThread;
    class EventLoop       *m_eventloop;
};

inline Openwsp &app() {
    return Openwsp::instance();
}

inline WSConfig &config() {
    return app().config();
}

} // namespace openwsp

#endif //!defined(OPENWSP_H_)
