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

#ifndef EVENTDISPATCH_H_
#define EVENTDISPATCH_H_

#include <openwsp/thread.h>

#include <openwsp/autoptr.h>

namespace openwsp {

class ThreadBase;
class WSEvent;

/**
 * Thread Type
 */
enum ThreadType {
    THREAD_UNKNOWN = 0,
    THREAD_MAIN,
    THREAD_IO,
    THREAD_AUDIO
};


/***************************************************
  *****         Event request object           *****
  ***************************************************/

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    int init(ThreadBase *main, ThreadBase *io, ThreadBase *audio);
    int uninit();

    int threadBasePtr(ThreadType type, ThreadBase **out);
    ThreadId_t threadId(ThreadType type);

    AutoPtr<WSEvent> PostEvent(ThreadType type, WSEvent *event, int *rc = 0);

private:
    ThreadBase *m_mainThread;
    ThreadBase *m_ioThread;
    ThreadBase *m_audioThread;
    ThreadId_t  m_tidMain;
    ThreadId_t  m_tidIo;
    ThreadId_t  m_tidAudio;
};


} // namespace openwsp

#endif //!defined(EVENTDISPATCH_H_)
