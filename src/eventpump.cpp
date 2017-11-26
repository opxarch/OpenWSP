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
#include <openwsp/autoptr.h>
#include <openwsp/thread.h>

#include "eventpump.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

WSEvent::WSEvent()
    : prev(0),
      next(0),
      m_canceled_sig(0)
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
 * Cancel all the events waiting and processing in the queue,
 * owning EVENT_PRI_NORMAL priority, without a respond.
 * @return status code.
 */
int WSEventPump::cancelEvents() {
    return cancelEvents(EVENT_PRI_NORMAL, 0);
}

/**
 * Cancel all the events, waiting and processing in the queue,
 * owning EVENT_PRI_NORMAL priority.
 * @param resp    Optionally, when the targets are all finished, send
 *                a response back.
 * @return status code.
 */
int WSEventPump::cancelEvents(WSEvent *resp) {
    return cancelEvents(EVENT_PRI_NORMAL, resp);
}

/**
 * Cancel all the events, owning the expected priority, waiting
 * and processing in the queue.
 * @param pri     The expected priority. if there is a event whose
 *                priority is different (!=), the event will be ignored.
 * @param resp    Optionally, when the targets are all finished, send
 *                a response back.
 * @return status code.
 */
int WSEventPump::cancelEvents(EventPriority pri, WSEvent *resp) {
    /*
     check the priority index
     */
    if ((pri < EVENT_PRI_NORMAL) || (pri >= EVENT_PRI_MAX)) {
        WS_ASSERT(0);
        return WERR_FAILED;
    }

    WSEvent *node;

    /* SPIN */ {
        AutoSpinLock lock(spin[pri]);

        /*
         * first of all, insert the respond event at the
         * back of queue so that it will be called after
         * events in queue are all finished.
         */
        resp->ref(MakeLocator());
        resp->prev = 0;
        resp->next = root[pri];

        if (root[pri])
            root[pri]->prev = resp;
        root[pri] = resp;

        /*
         * travel the queue and cancel all the events except
         * the respond event.
         */
        node = root[pri]->next;

        while(node) {
            node->cancel();
            node = node->next;
        }
    }

    return WINF_SUCCEEDED;
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

    event->ref(MakeLocator());
    event->prev = 0;

    /*
     Link it to the list within spinning mutex.
     */
    spin[pri].acquire();
    {
        event->next = root[pri];

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

    /*
     * This loop ensures the privilege order of which the event is
     * picked.
     */
    for (int pri=EVENT_PRI_MAX-1; pri>=0; pri--) {

        /* SPIN */ {
            AutoSpinLock lock(spin[pri]);

            if (root[pri]) {
                node = root[pri];

                /* pop */
                root[pri] = node->next;

                /* ret */
                node->next = 0;
                node->prev = 0;
                node->release(MakeLocator());
                *out = node;
                return WINF_SUCCEEDED;
            }
        }
    }

    *out = 0;
    return WERR_FAILED;
}

} //namespace openwsp
