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

namespace openwsp {

/**
 * Event priority
 */
enum EventPriority {
    EVENT_PRI_NORMAL = 0,
    EVENT_PRI_1,
    EVENT_PRI_2,
    EVENT_PRI_MAX
};

/***************************************************
  *****           Event object                 *****
  ***************************************************/

class WSEvent {
public:
    WSEvent();
    virtual ~WSEvent();

    virtual int run(void *ptr)=0;

public:
    WSEvent *prev;
    WSEvent *next;
};


/***************************************************
  *****           Pump object                 *****
  ***************************************************/

class WSEventPump {
public:
    WSEventPump();
    ~WSEventPump();

    int push(WSEvent *event);
    int push(EventPriority pri, WSEvent *event);
    int pop(WSEvent **out);

private:
    WSEvent     *root[EVENT_PRI_MAX];
    WSSpinLock  spin[EVENT_PRI_MAX];
};


} // namespace openwsp

#endif //!defined(OPENWSP_EVENTPUMP_H_)
