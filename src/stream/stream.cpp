/** @file
 * Openwsp - stream core.
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
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>

#include <openwsp/types.h>
#include <openwsp/misc.h>
#include "stream.h"

#include "../../include/openwsp/demuxtype.h"
#include "../log.h"

namespace openwsp {

extern stream_info_t stream_info_file;
extern stream_info_t stream_info_http1;
extern stream_info_t stream_info_http2;

stream_info_t* streams[] = {
    &stream_info_file,
    &stream_info_http1,
    &stream_info_http2,
    NULL
};

////////////////////////////////////////////////////////////////////////////////

stream_t* new_stream(int fd, int type)
{
    stream_t *stream = (stream_t*)malloc(sizeof(stream_t));

    if(stream==NULL) return NULL;
    memset(stream,0,sizeof(stream_t));

    stream->type = type;
    stream->fd = fd;
    stream->flags = 0;
    stream->sector_size = 0;
    stream->buf_pos=stream->buf_len = 0;
    stream->pos=stream->start_pos=stream->end_pos = 0;
    stream->eof = 0;

    stream_reset(stream);
    return stream;
}

void free_stream(stream_t *stream)
{
    stream->close(stream);
    if(stream->cache_pid) {
        cache_uninit(stream);
    }

    free(stream);
}

stream_t* new_memory_stream(unsigned char* data,int len)
{
    stream_t *stream;

    if(len < 0) return NULL;

    stream = (stream_t*)malloc(sizeof(stream_t)+len);
    memset(stream, 0, sizeof(stream_t));

    stream->fd=-1;
    stream->type=STREAMTYPE_MEMORY;
    stream->buf_pos=0; stream->buf_len=len;
    stream->start_pos=0; stream->end_pos=len;
    stream_reset(stream);
    stream->pos=len;
    memcpy(stream->buffer, data, len);

    return stream;
}

stream_t* open_stream(const char* filename,char** options, int* file_format)
{
    /* Check if playlist or unknown */
    if (*file_format != DEMUXER_TYPE_PLAYLIST){
        *file_format=DEMUXER_TYPE_UNKNOWN;
    }

    if(!filename) {
        ws_log(log::M_OPEN,log::ERR) << "NULL filename, report this bug" << endlog;
        return NULL;
    }

    /*============ Open STDIN or plain FILE ============*/

    return open_stream_full(filename,options,file_format);
}

stream_t* open_stream_full(const char* filename, char** options, int* file_format)
{
    int i = 0;
    int ip = 0;

    stream_t* stream;
    stream_info_t *sinfo;

    for(i = 0 ; streams[i] ; i++) {
        sinfo = streams[i];

        if(!sinfo->protocols) {
            ws_log(log::M_OPEN,log::WARN)
                << "Stream type " << sinfo->name << " has protocols == NULL, it's a bug"
                << endlog;
            continue;
        }
        int len = 0;
        for(ip = 0 ; sinfo->protocols[ip] ; ip++) {

            len = strlen(sinfo->protocols[ip]);
            if((len == 0 && !strstr(filename, "://")) ||
               ((strncmp(sinfo->protocols[ip], filename, len) == 0) &&
                (strncmp("://", filename + len, 3) == 0))) {

                /* matched it */
                int mode = STREAM_READ;
                stream = new_stream(-2,-2);
                stream->flags |= mode;

                if(sinfo->open(stream, filename, file_format) == STREAM_OK) {
                    /*if(stream->type <= -2)
                     * Warning streams need a type !!!! */
                    if((stream->flags & STREAM_SEEK) && !stream->seek)
                        stream->flags &= ~STREAM_SEEK;
                    if(stream->seek && !(stream->flags & STREAM_SEEK))
                        stream->flags |= STREAM_SEEK;
                    stream->mode = mode;

                    return stream; /* Succeed */
                }else{
                    //free_stream(stream);
                    free(stream);

                    ws_log::write(log::M_OPEN,log::ERR, MSGTR_FailedToOpen,filename);
                    return NULL;
                }
                break;
            }
        }
    }

    ws_log(log::M_OPEN,log::ERR) << "No stream found to handle url: " << filename << endlog;
    return NULL;
}

/*=================== STREAMER =========================*/

int stream_fill_buffer(stream_t *stream)
{
    int len;

    if (/*s->fd == NULL ||*/ stream->eof) {
        stream->buf_pos = 0;
        stream->buf_len = 0;
        return 0;
    }

    switch(stream->type) {
        case STREAMTYPE_STREAM:
            if(stream->streaming_ctrl!=NULL) {
                len = stream->streaming_ctrl->streaming_read(stream->fd,(char*)stream->buffer,STREAM_BUFFER_SIZE, stream->streaming_ctrl);
                break;
            } else {
                len = read(stream->fd, stream->buffer,STREAM_BUFFER_SIZE);
                break;
            }
        default:
            if(stream->fill_buffer != NULL) {
                len = stream->fill_buffer(stream, (char*)stream->buffer,STREAM_BUFFER_SIZE);
            }else{
                len = 0;
            }
    }
    if(len<=0){
        stream->eof = 1;
        stream->buf_pos = 0;
        stream->buf_len = 0;
        return 0;
    }
    stream->buf_pos = 0;
    stream->buf_len = len;
    stream->pos+=len;

    return len;
}

int stream_control(stream_t *stream, int cmd, void *arg)
{
    if(!stream->control) return STREAM_UNSUPPORTED;

    return stream->control(stream, cmd, arg);
}

int stream_seek_long(stream_t *stream,off_t pos)
{
    int ret = 0;
    off_t newpos = 0;

    stream->buf_pos=stream->buf_len = 0;

    if(stream->sector_size)
        newpos = (pos/stream->sector_size)*stream->sector_size;
    else
        newpos = pos&(~((off_t)STREAM_BUFFER_SIZE-1));

    pos -= newpos;

    if(newpos==0 || newpos!=stream->pos) {
        switch(stream->type) {
            case STREAMTYPE_STREAM:
                /*s->pos=newpos; // real seek
                // Some streaming protocol allow to seek backward and forward
                // A function call that return -1 can tell that the protocol
                // doesn't support seeking. */

                if(stream->seek != NULL) { /* new stream seek is much cleaner than streaming_ctrl one */
                    if(!(ret=stream->seek(stream, newpos))) {
                        ws_log(log::M_STREAM,log::ERR) << "Seek failed" << endlog;
                        return 0;
                    }
                    break;
                }

                if(stream->streaming_ctrl != NULL && stream->streaming_ctrl->streaming_seek != NULL ) {
                    if(stream->streaming_ctrl->streaming_seek(stream->fd, pos, stream->streaming_ctrl) <0 ) {
                        ws_log(log::M_STREAM,log::INFO) << "Stream not seekable!" << endlog;
                        return 0;
                    }
                }
                break;
            default:
                /* This should at the beginning as soon as all streams are converted */
                if(stream->seek == NULL)
                    return 0;
                /* Now seek */
                if(!stream->seek(stream,newpos)) {
                    ws_log(log::M_STREAM,log::ERR) << "Seek failed" << endlog;
                    return 0;
                }
        }
    }
    while(stream_fill_buffer(stream) > 0 && pos >= 0) {
        if(pos<=stream->buf_len){
            stream->buf_pos=pos; // byte position in sector
            return 1;
        }
        pos -= stream->buf_len;
    }

    return 0;
}

void stream_reset(stream_t *stream)
{
    if(stream->eof){
        stream->pos=0; //ftell(f);
//    stream->buf_pos=stream->buf_len=0;
        stream->eof=0;
    }
    if(stream->control) stream->control(stream,STREAM_CTRL_RESET,NULL);
  //stream_seek(stream,0);
}

} // namespace openwsp
