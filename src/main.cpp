/** @file
 * OpenWSP - the program entrance.
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
#include <exception>
#include <string.h>

#include "openwsp/err.h"
#include "openwsp/version.h"
#include "openwsp/assert.h"

#if 1
# include "stream/stream.h"
# include "audiosys/audiosystem.h"
# include "codec/audcodec.h"
# include "codec/aud.h"
#endif

#include "openwsp.h"

using namespace openwsp;

////////////////////////////////////////////////////////////////////////////////

static void checkError(int rc) {
    if (WS_FAILURE(rc)) {
        const ErrorInfo *err = GetErrorMsg(rc);
        std::cout<<"Error tracing: ("<<err->code<<") "<<err->msgDefine<<": "<<
                err->msgFull<<std::endl;
    }
}

/**
 * Program main entry
 *
 * @param   argc        The count of command-line arguments.
 * @param   argv        Vector list of command-line arguments.
 * @returns Exiting status.
 */
int main(int argc, char *argv[]) {
    int rc = WINF_SUCCEEDED;
    std::cout<<appBanner<<std::endl;

    try {
        // create the main application instance
        openwsp_instance_ = new Openwsp(argc, argv);
        if (WS_SUCCESS(rc = app().init())) {

            const char *url = "http://http.hz.qingting.fm/386.mp3"; //; //"D:/1.wav"
            //"http://api2.qingting.fm/v6/media/channellives/142486"
#if 0
            openwsp::stream_t *s;
            int filetype;
            char buff[BUFFER_SIZE];

            int len;
            s = openwsp::open_stream(url, NULL, &filetype);
            if (s) {
                while (1) {
                    len = openwsp::stream_read(s, buff, BUFFER_SIZE);
                    if (len > 0) {
                        std::cout<<"recv:"<<buff<<std::endl;
                    } else {
                        break;
                    }
                }
            } else
                std::cout<<"failed."<<std::endl;

            std::cout<<"closed"<<std::endl;
#endif

#if 0
            rc = app().createAudioProcessContext(url);
            checkError(rc);

            if (WS_SUCCESS(rc)) {
                app().runAudioProcess();
                std::cout<<"audio end"<<std::endl;
                app().endAudioPrcoessContext();
            }
#endif

            /*
             Run the ui running loop.
             it will not return until the program has been closed.
             */
            rc = app().RunMainLoop();
            checkError(rc);
            rc = app().uninit();
        }

        checkError(rc);
        return -1;

    } catch(std::exception& e) {
        std::cout<<"Exception tracing:"<<e.what()<<std::endl;
        exit(0);
    }
}
