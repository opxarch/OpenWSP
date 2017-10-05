/** @file
 * Openwsp - audio system.
 */

/*
 *  OpenWSP (an opensource webstream codec)
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
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <openwsp/misc.h>
#include <openwsp/err.h>
#include "audiosystem.h"

namespace openwsp {

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/

#if USES(SDL_AUDIO)
extern IAudioOutput *audio_out_sdl;
#endif
extern IAudioOutput *audio_out_dsound;
extern IAudioOutput *audio_out_null;

IAudioOutput* audio_out_drivers[] = {
#if USES(SDL_AUDIO)
    audio_out_sdl,
#endif
    audio_out_dsound,
    audio_out_null,
    NULL
};


////////////////////////////////////////////////////////////////////////////////


IAudioOutput::IAudioOutput() {
    m_pts = 0;
    m_buffersize = -1;
    m_outburst =
    m_bps =
    m_format =
    m_channels =
    m_samplerate = 0;
}

IAudioOutput::~IAudioOutput() {
}

/*
 * void implements
 */
const char *IAudioOutput::getname() const {
    return "NULL";
}
const char *IAudioOutput::getshortname() const {
    return "NULL";
}
const char *IAudioOutput::getauthor() const {
    return "NULL";
}
const char *IAudioOutput::getcomment() const {
    return "NULL";
}



////////////////////////////////////////////////////////////////////////////////

void list_audio_outs(void) {
    int i = 0;
    while (audio_out_drivers[i]) {
        IAudioOutput *ia = audio_out_drivers[i++];
        printf("device: short_name[%s] name(%s)\n", ia->getshortname(), ia->getname());
    }
}

IAudioOutput* init_audio_device(const char *name, int rate, int channels, int format, int flags) {
    int rc;
    int i = 0;
    const char *buf = 0;
	for(i=0;audio_out_drivers[i];i++){
	    IAudioOutput* audio_out = audio_out_drivers[i];

	    buf = audio_out->getshortname();
	    if (!strncmp(buf, name, strlen(buf))) {
            /* name matches, try it */
	        rc = audio_out->init(rate,channels,format,flags);
            if (WS_SUCCESS(rc)) {
                return audio_out; /* succeeded! */
            } else
                printf("failed to init sdl\n");
	    }
	}

    /* now try the rest... */
    for(i=0;audio_out_drivers[i];i++) {
        IAudioOutput* audio_out=audio_out_drivers[i];

        rc = audio_out->init(rate,channels,format,flags);
        if (WS_SUCCESS(rc)) {
            return audio_out; /* succeeded! */
        }
    }
    return NULL;
}

int free_audio(IAudioOutput *func, int flags) {
    return func->uninit(flags);
}
void resume_audio(IAudioOutput *func) {
    func->resume();
}
void pause_audio(IAudioOutput *func) {
    func->pause();
}
int get_delay_audio(IAudioOutput *func) {
    return func->get_delay();
}
int control_audio(IAudioOutput *func, int cmd, void* arg, ...) {
    return func->control(cmd, arg);
}
int write_audio(IAudioOutput *func, char *data, int length, int flags) {
    return func->write(data, length, flags);
}

} // namespace openwsp
