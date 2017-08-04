/** @file
 * Util - Event pump (queue management) with thread friendly.
 */

/**
 * @todo: Lock-free queue
 * Currently we are using spinning lock.
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
#include <iostream>
#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/thread.h>

#include <openwsp/eventpump.h>

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

WSEvent::WSEvent()
    : prev(0),
      next(0)
{
}

WSEvent::~WSEvent() {

}

////////////////////////////////////////////////////////////////////////////////

WSEventPump::WSEventPump() {
    for (unsigned i=0; i<EVENT_PRI_MAX; i++)
        root[i] = 0;
}

WSEventPump::~WSEventPump() {

}

/**
 * Push a event object to the queue.
 * @param pri The priority of the event.
 * @param event Pointer to the target event object.
 * @return status code.
 */
int WSEventPump::push(WSEvent *event) {
    return push(EVENT_PRI_NORMAL, event);
}
int WSEventPump::push(EventPriority pri, WSEvent *event) {
    /*
     check the priority index
     */
    if ((pri < EVENT_PRI_NORMAL) || (pri >= EVENT_PRI_MAX)) {
        WS_ASSERT(0);
        return WERR_FAILED;
    }

    event->prev = 0;
    event->next = root[pri];

    /*
     Link it to the list within spinning mutex.
     */
    spin[pri].acquire();
    {
        if (root[pri])
            root[pri]->prev = event;
        root[pri] = event;
    }
    spin[pri].release();

    return WINF_SUCCEEDED;
}

/**
 * Pick a event in the queue.
 * @param out Where to store the pointer of the event.
 *            The event picked will be rejected from the queue,
 *            and should be deleted explicitly outside.
 * @return WINF_SUCCEEDED if a event was popped.
 * @return else if the queue is empty or an error raised.
 */
int WSEventPump::pop(WSEvent **out) {
    WSEvent *node;

    for (int pri=EVENT_PRI_MAX-1; pri>=0; pri--) {
        AutoSpinLock lock;
        {
            if (root[pri]) {
                node = root[pri];

                /* pop */
                root[pri] = node->next;

                /* ret */
                node->next = 0;
                node->prev = 0;
                *out = node;
                return WINF_SUCCEEDED;
            }
        }
    }

    *out = 0;
    return WERR_FAILED;
}

} //namespace openwsp
