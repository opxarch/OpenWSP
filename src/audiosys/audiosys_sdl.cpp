/** @file
 * Openwsp - audiosystem - SDL.
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDL/SDL.h>

#include <openwsp/err.h>
#include <openwsp/misc.h>
#include <openwsp/audioformat.h>
#include <openwsp/timer.h>
#include <openwsp/fastmem.h>
#include "../log.h"
#include "audiosystem.h"

namespace openwsp {


/*******************************************************************************
*   Macro definitions                                                          *
*******************************************************************************/

// turn this on if you want to use the slower SDL_MixAudio
#undef USE_SDL_INTERNAL_MIXER

// Samplesize used by the SDLlib AudioSpec struct
#if defined(WIN32) || defined(SYS_AMIGAOS4)
#define SAMPLESIZE 2048
#else
#define SAMPLESIZE 1024
#endif

#define CHUNK_SIZE 4096
#define NUM_CHUNKS 8
// This type of ring buffer may never fill up completely, at least
// one byte must always be unused.
// For performance reasons (alignment etc.) one whole chunk always stays
// empty, not only one byte.
#define BUFFSIZE ((NUM_CHUNKS + 1) * CHUNK_SIZE)


/*******************************************************************************
*   Internal functions                                                         *
*******************************************************************************/

static void outputaudio(void *unused, Uint8 *stream, int len);


////////////////////////////////////////////////////////////////////////////////


static
class SDLImpl : public IAudioOutput {
public:
    const char *getname() const {
        return "SDLlib audio output";
    }
    const char *getshortname() const {
        return "sdl";
    }
    const char *getauthor() const {
        return "OpenWSP";
    }
    const char *getcomment() const {
        return "none";
    }

    // to set/get/query special features/parameters
    int control(int cmd, void *arg, ...){
    #ifdef USE_SDL_INTERNAL_MIXER
        switch (cmd) {
            case AOCONTROL_GET_VOLUME:
            {
                ao_control_vol_t* vol = (ao_control_vol_t*)arg;
                vol->left = vol->right = volume * 100 / SDL_MIX_MAXVOLUME;
                return CONTROL_OK;
            }
            case AOCONTROL_SET_VOLUME:
            {
                int diff;
                ao_control_vol_t* vol = (ao_control_vol_t*)arg;
                diff = (vol->left+vol->right) / 2;
                volume = diff * SDL_MIX_MAXVOLUME / 100;
                return CONTROL_OK;
            }
        }
    #endif
        return CONTROL_UNKNOWN;
    }

    // open & setup audio device
    // return: 1=success 0=fail
    int init(int rate,int channels,int format,int flags){

        /* SDL Audio Specifications */
        SDL_AudioSpec aspec, obtained;

        /* Allocate ring-buffer memory */
        buffer = (unsigned char *) malloc(BUFFSIZE);

        ws_log::write(log::M_AUDIOSYS,log::INFO, MSGTR_AS_SDL_INFO, rate, (channels > 1) ? "Stereo" : "Mono", af_fmt2str_short(format));

    #if 0
        if(ao_subdevice) {
            setenv("SDL_AUDIODRIVER", ao_subdevice, 1);
            ws_log::write(log::M_AUDIOSYS,log::INFO, MSGTR_AS_SDL_DriverInfo, ao_subdevice);
        }
    #endif

        m_channels=channels;
        m_samplerate=rate;
        m_format=format;

        m_bps=channels*rate;
        if(format != AF_FORMAT_U8 && format != AF_FORMAT_S8)
          m_bps*=2;

        /* The desired audio format (see SDL_AudioSpec) */
        switch(format) {
            case AF_FORMAT_U8:
            aspec.format = AUDIO_U8;
            break;
            case AF_FORMAT_S16_LE:
            aspec.format = AUDIO_S16LSB;
            break;
            case AF_FORMAT_S16_BE:
            aspec.format = AUDIO_S16MSB;
            break;
            case AF_FORMAT_S8:
            aspec.format = AUDIO_S8;
            break;
            case AF_FORMAT_U16_LE:
            aspec.format = AUDIO_U16LSB;
            break;
            case AF_FORMAT_U16_BE:
            aspec.format = AUDIO_U16MSB;
            break;
            default:
                aspec.format = AUDIO_S16LSB;
                m_format = AF_FORMAT_S16_LE;
                ws_log::write(log::M_AUDIOSYS,log::WARN, MSGTR_AS_SDL_UnsupportedAudioFmt, format);
        }

        /* The desired audio frequency in samples-per-second. */
        aspec.freq     = rate;

        /* Number of channels (mono/stereo) */
        aspec.channels = channels;

        /* The desired size of the audio buffer in samples. This number should be a power of two, and may be adjusted by the audio driver to a value more suitable for the hardware. Good values seem to range between 512 and 8192 inclusive, depending on the application and CPU speed. Smaller values yield faster response time, but can lead to underflow if the application is doing heavy processing and cannot fill the audio buffer in time. A stereo sample consists of both right and left channels in LR ordering. Note that the number of samples is directly related to time by the following formula: ms = (samples*1000)/freq */
        aspec.samples  = SAMPLESIZE;

        /* This should be set to a function that will be called when the audio device is ready for more data. It is passed a pointer to the audio buffer, and the length in bytes of the audio buffer. This function usually runs in a separate thread, and so you should protect data structures that it accesses by calling SDL_LockAudio and SDL_UnlockAudio in your code. The callback prototype is:
    void callback(void *userdata, Uint8 *stream, int len); userdata is the pointer stored in userdata field of the SDL_AudioSpec. stream is a pointer to the audio buffer you want to fill with information and len is the length of the audio buffer in bytes. */
        aspec.callback = outputaudio;

        /* This pointer is passed as the first parameter to the callback function. */
        aspec.userdata = this;

        /* initialize the SDL Audio system */
        if (SDL_Init (SDL_INIT_AUDIO/*|SDL_INIT_NOPARACHUTE*/)) {
            ws_log::write(log::M_AUDIOSYS,log::ERR, MSGTR_AS_SDL_CantInit, SDL_GetError());
            return 0;
        }

        /* Open the audio device and start playing sound! */
        if(SDL_OpenAudio(&aspec, &obtained) < 0) {
            ws_log::write(log::M_AUDIOSYS,log::ERR, MSGTR_AS_SDL_CantOpenAudio, SDL_GetError());
            return(0);
        }

        /* did we got what we wanted ? */
        m_channels=obtained.channels;
        m_samplerate=obtained.freq;

        switch(obtained.format) {
            case AUDIO_U8 :
            m_format = AF_FORMAT_U8;
            break;
            case AUDIO_S16LSB :
            m_format = AF_FORMAT_S16_LE;
            break;
            case AUDIO_S16MSB :
            m_format = AF_FORMAT_S16_BE;
            break;
            case AUDIO_S8 :
            m_format = AF_FORMAT_S8;
            break;
            case AUDIO_U16LSB :
            m_format = AF_FORMAT_U16_LE;
            break;
            case AUDIO_U16MSB :
            m_format = AF_FORMAT_U16_BE;
            break;
            default:
                ws_log::write(log::M_AUDIOSYS,log::WARN, MSGTR_AS_SDL_UnsupportedAudioFmt, obtained.format);
                return 0;
        }

        ws_log::write(log::M_AUDIOSYS,log::INFO, "SDL: buf size = %d\n",obtained.size);
        m_buffersize=obtained.size;
        m_outburst = CHUNK_SIZE;

        reset();
        /* unsilence audio, if callback is ready */
        SDL_PauseAudio(0);

        return WINF_SUCCEEDED;
    }

    // close audio device
    int uninit(int immed){
        ws_log::write(log::M_AUDIOSYS,log::VERBOSE, "SDL: Audio Subsystem shutting down!\n");
        if (!immed)
          usecSleep(get_delay() * 1000 * 1000);
        SDL_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return WINF_SUCCEEDED;
    }

    // stop playing and empty buffers (for seeking/pause)
    int reset(void){

        //printf("SDL: reset called!\n");

        SDL_PauseAudio(1);
        /* Reset ring-buffer state */
        read_pos = 0;
        write_pos = 0;
        SDL_PauseAudio(0);
        return WINF_SUCCEEDED;
    }

    // stop playing, keep buffers (for pause)
    void pause(void) {
        //printf("SDL: audio_pause called!\n");
        SDL_PauseAudio(1);

    }

    // resume playing, after audio_pause()
    void resume(void) {
        //printf("SDL: audio_resume called!\n");
        SDL_PauseAudio(0);
    }


    // return: how many bytes can be played without blocking
    int get_space(void) {
        return buf_free();
    }

    // plays 'len' bytes of 'data'
    // it should round it down to outburst*n
    // return: number of bytes played
    int write(void* data,int len,int flags){
        if (!(flags & AOPLAY_FINAL_CHUNK))
        len = (len/m_outburst)*m_outburst;
    #if 0
        int ret;

        /* Audio locking prohibits call of outputaudio */
        SDL_LockAudio();
        // copy audio stream into ring-buffer
        ret = write_buffer(data, len);
        SDL_UnlockAudio();

            return ret;
    #else
        return write_buffer((unsigned char*)data, len);
    #endif
    }

    // return: delay in seconds between first and last sample in buffer
    float get_delay(void){
        int buffered = BUFFSIZE - CHUNK_SIZE - buf_free(); // could be less
        return (float)(buffered + m_buffersize)/(float)m_bps;
    }

private:

    unsigned char *buffer;

    // may only be modified by SDL's playback thread or while it is stopped
    volatile int read_pos;
    // may only be modified by audio thread
    volatile int write_pos;
    #ifdef USE_SDL_INTERNAL_MIXER
    unsigned char volume=SDL_MIX_MAXVOLUME;
    #endif

private:

    /*
     * Ring buffer operation
     */

    // may only be called by audio thread
    // return value may change between immediately following two calls,
    // and the real number of free bytes might be larger!
    int buf_free(void) {
      int free = read_pos - write_pos - CHUNK_SIZE;
      if (free < 0) free += BUFFSIZE;
      return free;
    }

    // may only be called by SDL's playback thread
    // return value may change between immediately following two calls,
    // and the real number of buffered bytes might be larger!
    int buf_used(void) {
      int used = write_pos - read_pos;
      if (used < 0) used += BUFFSIZE;
      return used;
    }

    int write_buffer(unsigned char* data,int len){
      int first_len = BUFFSIZE - write_pos;
      int free = buf_free();
      if (len > free) len = free;
      if (first_len > len) first_len = len;
      // till end of buffer
      fast_memcpy (&buffer[write_pos], data, first_len);
      if (len > first_len) { // we have to wrap around
        // remaining part from beginning of buffer
        fast_memcpy (buffer, &data[first_len], len - first_len);
      }
      write_pos = (write_pos + len) % BUFFSIZE;
      return len;
    }

    int read_buffer(unsigned char* data,int len){
      int first_len = BUFFSIZE - read_pos;
      int buffered = buf_used();
      if (len > buffered) len = buffered;
      if (first_len > len) first_len = len;
      // till end of buffer
    #ifdef USE_SDL_INTERNAL_MIXER
      SDL_MixAudio (data, &buffer[read_pos], first_len, volume);
    #else
      fast_memcpy (data, &buffer[read_pos], first_len);
    #endif
      if (len > first_len) { // we have to wrap around
        // remaining part from beginning of buffer
    #ifdef USE_SDL_INTERNAL_MIXER
        SDL_MixAudio (&data[first_len], buffer, len - first_len, volume);
    #else
        fast_memcpy (&data[first_len], buffer, len - first_len);
    #endif
      }
      read_pos = (read_pos + len) % BUFFSIZE;
      return len;
    }

friend void outputaudio(void *, Uint8 *, int);

} audio_out_sdl_instance;

//
// extern
//
IAudioOutput *audio_out_sdl = static_cast<IAudioOutput*>(&audio_out_sdl_instance);


// SDL Callback function
static void outputaudio(void *unused, Uint8 *stream, int len) {
    SDLImpl *impl = static_cast<SDLImpl*>(unused);

    printf("fetch len=%d\n", len);
    // fetch data from ring queue
    impl->read_buffer((unsigned char*)stream, len);
}


} // namespace openwsp
