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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/thread.h>

#include "threads.h"
#include "eventfactory.h"
#include "eventloop.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

EventLoop::EventLoop()
    : m_mainThread(0),
      m_ioThread(0)
{
}

EventLoop::~EventLoop() {

}

/**
 * Do the logical initialization.
 * @return status code.
 */
int EventLoop::init(ThreadBase *main, ThreadBase *io) {
    m_mainThread = main;
    m_ioThread = io;
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
 * Dispatch the events to the target thread.
 * @param type Specify the target thread.
 * @param event Pointer to a Initialized WSEventReq object.
 * @return status code.
 */
int EventLoop::PostEvent(ThreadType type, WSEvent *event) {
    WS_ASSERT(m_mainThread && m_ioThread);

    ThreadBase *target;

    /*
     * Resolve the target thread
     */
    switch(type) {
    case THREAD_IO:
        target = m_ioThread;
        break;
    case THREAD_MAIN:
        target = m_mainThread;
        break;
    default:
        return WERR_INVALID_TARGET;
    }

    /*
     * Push the event
     */
    return target->PostEvent(static_cast<WSEvent*>(event));
}

} // namespace openwsp
