/** @file
 * OpenWSP - I/O Thread.
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
#include <openwsp/webservice.h>

#include "config.h"
#include "openwsp.h"
#include "threads.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

ThreadIO::ThreadIO()
    : websrv(new Webservice),
      webmoduleName(0)
{
}
ThreadIO::~ThreadIO() {
    delete websrv;
}

////////////////////////////////////////////////////////////////////////////////

int ThreadIO::init(void *opaque) {
    /*
     Register configurations
     */
    config().insert(new configElement("webmoduleName", WEBMOULE_DEFAULT, &webmoduleName));

    int rc = this->WSThread::create(opaque);

    /*
     initiate the Web service
     */
    rc = websrv->init();
    return rc;
}

int ThreadIO::uninit() {
    int rc;
    rc = websrv->uninit();
    AssertRC(rc);
    return WINF_SUCCEEDED;
}


/////////////////////////////////////////////////////////////////////

// Begin event looping


/**
 * Entry of I/O thread context.
 * @param opaque pointer data.
 * @return status code.
 */
int ThreadIO::run(void *opaque) {
    int rc;
    WSEvent *event;

    for (;;) {
        /*
         Process the I/O events
         */
        rc = this->EventPump().pop(&event);
        if (WS_SUCCESS(rc)) {
            event->run(this);
            event->release(MakeLocator());
        }

        if (this->cancelRaised()) {
            return WINF_TERMINATED;
        }
    }

    return WINF_SUCCEEDED;
}

// End event looping

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// for I/O Thread context only
////////////////////////////////////////////////////////////////////

int ThreadIO::onConnectService() {
    AssertThread(THREAD_IO);

    int rc = websrv->LoadModule(webmoduleName);
    if (WS_SUCCESS(rc)) {
        rc = websrv->connect();
    }
    return rc;
}

int ThreadIO::onGetCatalogs(WSList<catalog *> *dst) {
    AssertThread(THREAD_IO);
    return websrv->catalogs(dst);
}

int ThreadIO::onGetChannels(int id, WSList<channel *> *dst) {
    AssertThread(THREAD_IO);
    return websrv->channels(id, dst);
}

int ThreadIO::onGetChannelInfo(int id, channel **out) {
    AssertThread(THREAD_IO);
    return websrv->queryChannelInfo(id, out);
}

int ThreadIO::onGetChannelURL(int id, channelURL **out) {
    AssertThread(THREAD_IO);
    return websrv->queryChannelURL(id, out);
}


} // namespace openwsp
