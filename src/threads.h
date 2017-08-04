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

#include <openwsp/thread.h>
#include <openwsp/eventpump.h>

namespace openwsp {

class Openwsp;

/**
 * Base class of event-processing thread
 */
class ThreadBase {
public:
    ThreadBase() {}
    ~ThreadBase() {}

    /**
     * Push a event to the queue.
     * @param pri The priority of target event.
     * @param event Pointer to the target event.
     */
    int PostEvent(WSEvent *event) {
        return eventPump.push(event);
    }

protected:
    WSEventPump eventPump;
};

/***************************************************
  *****           I/O Thread object            *****
  ***************************************************/

class IOThread : public WSThread, public ThreadBase {
public:
    IOThread();
    ~IOThread();

    int init(Openwsp *app);
    int uninit();

    virtual int run(void *opaque);
};

/***************************************************
  *****           Main Thread object            *****
  ***************************************************/

class MainThread : public WSThread, public ThreadBase {
public:
    MainThread();
    ~MainThread();

    int init(Openwsp *app);
    int uninit();

    virtual int run(void *opaque);
};


} // namespace openwsp

#endif //!defined(THREADS_H_)
