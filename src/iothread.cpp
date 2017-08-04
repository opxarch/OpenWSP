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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <openwsp/thread.h>
#include <openwsp/err.h>

#include "openwsp.h"
#include "threads.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

IOThread::IOThread() {

}

IOThread::~IOThread() {

}

////////////////////////////////////////////////////////////////////////////////

int IOThread::init(Openwsp *app) {
    int rc;
    rc = create(app);
    return rc;
}

int IOThread::uninit() {
    return WINF_SUCCEEDED;
}

/**
 * Entry of I/O thread context.
 * @param opaque pointer data.
 * @return status code.
 */
int IOThread::run(void *opaque) {
    return WINF_SUCCEEDED;
}

} // namespace openwsp
