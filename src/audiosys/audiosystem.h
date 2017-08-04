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

#ifndef AUDIOSYSTEM_H_
#define AUDIOSYSTEM_H_

namespace openwsp {

/* set up audio OUTBURST. */
#define OUTBURST (512)

/***************************************************
  *****             Audio output               *****
  ***************************************************/

class IAudioOutput {
public:
    IAudioOutput();
    virtual ~IAudioOutput();

    virtual int init(int rate, int channels, int format, int flags)=0;
    virtual int uninit(int flags)=0;
    virtual int reset()=0;
    virtual int get_space(void)=0;
    virtual float get_delay(void)=0;
    virtual void pause(void)=0;
    virtual void resume(void)=0;
    virtual int control(int cmd, void* arg, ...)=0;
    virtual int write(void *data, int length, int flags)=0;

    virtual const char *getname() const;
    virtual const char *getshortname() const;
    virtual const char *getauthor() const;
    virtual const char *getcomment() const;

public:
    int m_samplerate;
    int m_channels;
    int m_format;
    int m_bps;
    int m_outburst;
    int m_buffersize;
    int m_pts;
};

/* returned value */
#define CONTROL_OK 1
#define CONTROL_TRUE 1
#define CONTROL_FALSE 0
#define CONTROL_UNKNOWN -1
#define CONTROL_ERROR -2
#define CONTROL_NA -3

/* control commands */
#define AOCONTROL_GET_VOLUME 4
#define AOCONTROL_SET_VOLUME 5

#define AOPLAY_FINAL_CHUNK 1

/* the volume information */
typedef struct ao_control_vol_s {
	int left;
	int right;
} ao_control_vol_t;

void list_audio_out(void);
IAudioOutput* init_audio_device(const char *name, int rate, int channels, int format, int flags);

int free_audio(IAudioOutput *func, int flags);
void resume_audio(IAudioOutput *func);
void pause_audio(IAudioOutput *func);
int get_delay_audio(IAudioOutput *func);
int control_audio(IAudioOutput *func, int cmd, void* arg, ...);
int write_audio(IAudioOutput *func, char *data, int length, int flags);

} // namespace openwsp

#endif //__AUDIO_OUT_H__
