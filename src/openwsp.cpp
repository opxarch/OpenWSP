/** @file
 * OpenWSP - the application main logic.
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

#include <iostream>
#include <assert.h>
#include <log.h>

#include "openwsp.h"

#include <openwsp/err.h>
#include <openwsp/version.h>
#include <openwsp/misc.h>
#include <openwsp/timer.h>

#include "config.h"
#include "log.h"
#include "eventfactory.h"
#include "eventloop.h"

#include "gui/gui.h"

namespace openwsp {


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/

Openwsp *openwsp_instance_ = 0;


/*******************************************************************************
*   the Manager Object of the program                                                   *
*******************************************************************************/

Openwsp::Openwsp(int &argc, char **argv)
    : event (this),
      gui (new gui::wsGUI),
      ioThread (new ThreadIO),
      mainThread (new ThreadMain),
      audioThread (new ThreadAudio),
      events (new EventLoop)
{
}

Openwsp::~Openwsp() {
    delete gui;
    delete ioThread;
    delete mainThread;
    delete audioThread;
    delete events;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Do the logical initialization.
 * @return status code.
 */
int Openwsp::init() {
    int rc = WERR_FAILED;
    
    /*
     GUI
     */
    rc = gui->init(); UPDATE_RC(rc);

    /*
     Threads
     */
    rc = mainThread->init(this); UPDATE_RC(rc);
    rc = ioThread->init(this); UPDATE_RC(rc);
    rc = audioThread->init(this); UPDATE_RC(rc);

    rc = events->init(mainThread, ioThread, audioThread); UPDATE_RC(rc);

    return WINF_SUCCEEDED;
}

/**
 * uninitiate the application.
 * @return status code.
 */
int Openwsp::uninit() {
    int rc;
    rc = events->uninit();
    AssertRC(rc);
    rc = ioThread->uninit();
    AssertRC(rc);
    rc = mainThread->uninit();
    AssertRC(rc);
    rc = audioThread->uninit();
    AssertRC(rc);
    rc = gui->uninit();
    AssertRC(rc);
    return WINF_SUCCEEDED;
}


/**
 * Dispatch & processing the UI events during idle.
 * @return status code
 */
int Openwsp::idle() {
    return gui->DoMessages();
}


/**
 * Run the main running loop.
 *
 * Here responding all the events/messages/etc. from GUI
 * process, and this function will return a value when the main window
 * has been closed.
 * @returns exit code of program
 */
int Openwsp::RunMainLoop() {
    int rc;
    mainThread->wait(&rc);
    AssertRC(rc);

    ioThread->cancel();

    ioThread->wait(&rc);
    AssertRC(rc);

    audioThread->cancel();

    audioThread->wait(&rc);
    AssertRC(rc);
    return WINF_SUCCEEDED;
}

/**
 * Quit the openwsp thoroughly.
 */
void Openwsp::QuitApplication() {
}

#if ENABLE(ASSERTIONS)

/*
 * Only used by assertions.
 */
int Openwsp::validThread(ThreadType type, const Locator &loc) {

    ThreadId_t tid = WSThread::currentId();
    ThreadId_t exp = threadLoop().threadId(type);

    if (tid != exp) {
#if ENABLE(ASSERTIONS)
        ws_log(log::ERR) << "Invalid thread context: @" << loc.GetFilename() << " :"
                        << loc.GetFunctionName() << " line:"
                        << loc.GetLine() << endlog;
#endif
        WS_ASSERT_LOG(0, ("%s:%s:%d\n", loc.GetFilename(), loc.GetFunctionName(), loc.GetLine()));
        return 0;
    }
    return 1;
}

#endif // ENABLE(ASSERTIONS)


} // namespace openwsp

