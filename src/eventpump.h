/** @file
 * Openwsp - Event Pump (queue management).
 */

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

#ifndef OPENWSP_EVENTPUMP_H_
#define OPENWSP_EVENTPUMP_H_

#include <openwsp/thread.h>
#include <openwsp/autoptr.h>

namespace openwsp {

class ThreadBase;

/**
 * Event priority
 */
enum EventPriority {
    EVENT_PRI_NORMAL = 0,
    EVENT_PRI_1,
    EVENT_PRI_2,
    EVENT_PRI_MAX // reserved
};

typedef char EventSignal_t;

/***************************************************
  *****           Event object                 *****
  ***************************************************/

class WSEvent : public RefCounted<WSEvent> {
public:
    WSEvent();
    virtual ~WSEvent();

    virtual int run(ThreadBase *ptr)=0;

    inline void cancel() {
        m_canceled_sig = 1;
    }

public:
    WSEvent *prev;
    WSEvent *next;

protected:
    inline bool cancelSignal() {
        return m_canceled_sig;
    }

    inline const volatile EventSignal_t *cancelSignalPtr() {
        return &m_canceled_sig;
    }

private:
    volatile EventSignal_t m_canceled_sig;
};


/***************************************************
  *****           Pump object                 *****
  ***************************************************/

class WSEventPump {
public:
    WSEventPump();
    ~WSEventPump();

    int cancelEvents();
    int cancelEvents(WSEvent *resp);
    int cancelEvents(EventPriority pri, WSEvent *resp);

    int push(WSEvent *event);
    int push(EventPriority pri, WSEvent *event);
    int pop(WSEvent **out);

private:
    WSEvent     *root[EVENT_PRI_MAX];
    WSSpinLock   spin[EVENT_PRI_MAX];
};


} // namespace openwsp

#endif //!defined(OPENWSP_EVENTPUMP_H_)
