/** @file
 * OpenWSP - Event dispatcher.
 */

/*
 *  OpenWSP (an opensource web stream player)
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

#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/autoptr.h>
#include <openwsp/thread.h>

#include "threads.h"
#include "eventfactory.h"
#include "eventloop.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

EventLoop::EventLoop()
    : m_mainThread(0),
      m_ioThread(0),
      m_audioThread(0),
      m_tidMain(0),
      m_tidIo(0),
      m_tidAudio(0)
{
}

EventLoop::~EventLoop() {

}

/**
 * Do the logical initialization.
 * @return status code.
 */
int EventLoop::init(ThreadBase *main, ThreadBase *io, ThreadBase *audio) {
    WS_ASSERT(main && io && audio);

    m_mainThread = main;
    m_ioThread = io;
    m_audioThread = audio;

    m_tidMain = main->self();
    m_tidIo = io->self();
    m_tidAudio = audio->self();

    main->sync();
    io->sync();
    audio->sync();

    return WINF_SUCCEEDED;
}

/**
 * uninitiate the application.
 * @return status code.
 */
int EventLoop::uninit() {
    return WINF_SUCCEEDED;
}

/**
 * Get the pointer to ThreadBase by ThreadType.
 * @param out Where to store the pointer.
 * @return status code.
 */
int EventLoop::threadBasePtr(ThreadType type, ThreadBase **out) {
    switch(type) {
    case THREAD_IO:
        *out = m_ioThread;
        break;
    case THREAD_MAIN:
        *out = m_mainThread;
        break;
    case THREAD_AUDIO:
        *out = m_audioThread;
        break;
    default:
        *out = 0;
        return WERR_INVALID_TARGET;
    }
    return WINF_SUCCEEDED;
}

/**
 * Get the thread ID by ThreadType.
 * @return 0 if failed.
 * @return else equal tid.
 */
ThreadId_t EventLoop::threadId(ThreadType type) {
    ThreadId_t exp;

    switch (type) {
        case THREAD_IO:    exp = m_tidIo; break;
        case THREAD_MAIN:  exp = m_tidMain; break;
        case THREAD_AUDIO: exp = m_tidAudio; break;
        default:           exp = 0;
    }
    return exp;
}

/**
 * Dispatch the events to the target thread.
 * @param type Specify the target thread.
 * @param event Pointer to a Initialized WSEvent object.
 * @param rc Optionally, where to store the return code.
 * @return reference to the auto pointer.
 */
AutoPtr<WSEvent> EventLoop::PostEvent(ThreadType type, WSEvent *event, int *rc) {
    WS_ASSERT(m_mainThread && m_ioThread);

    if (!event) {
        if (rc) (*rc) = WERR_ALLOC_MEMORY;
        return AutoPtr<WSEvent>(0);
    }

    AutoPtrPass<WSEvent> ptr = AutoPtr<WSEvent>(event, MakeLocator());

    ThreadBase *target = 0;
    int ret = threadBasePtr(type, &target);

    if (WS_SUCCESS(ret)) {
        /*
         * Push the event
         */
        int ret = target->PostEvent(event);
        if (rc) (*rc) = ret;

        if (WS_SUCCESS(ret)) {
            return ptr; // done.
        }
    }
    if (rc) *rc = ret;

    //event->release(); //!fixme

    return AutoPtr<WSEvent>(0/*RefCounted<WSEvent>*/);
}

} // namespace openwsp
