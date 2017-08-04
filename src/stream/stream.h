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

#ifndef STREAM_H_
#define STREAM_H_

#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>

#include "../log.h"
#include "network.h"
#include "cache.h"

namespace openwsp {

/* Open return code
 * This can't open the requested protocol (used by stream wich have a
 * protocol when they don't know the requested protocol) */
#define STREAM_UNSUPPORTED -1
#define STREAM_ERROR 0
#define STREAM_OK    1

/* atm it will always use mode == STREAM_READ
/// streams that use the new api should check the mode at open*/
#define STREAM_READ  0
#define STREAM_WRITE 1

#define STREAM_SEEK_BW  2
#define STREAM_SEEK_FW  4
#define STREAM_SEEK  (STREAM_SEEK_BW|STREAM_SEEK_FW)

#define STREAMTYPE_DUMMY -1    // for placeholders, when the actual reading is handled in the demuxer
#define STREAMTYPE_FILE 0      // read from seekable file
#define STREAMTYPE_STREAM 2    // same as FILE but no seeking (for net/stdin)
#define STREAMTYPE_MEMORY  4   // read data from memory area

#define STREAM_CTRL_RESET 0
#define STREAM_CTRL_GET_TIME_LENGTH 1
#define STREAM_CTRL_SEEK_TO_CHAPTER 2
#define STREAM_CTRL_GET_CURRENT_CHAPTER 3
#define STREAM_CTRL_GET_NUM_CHAPTERS 4
#define STREAM_CTRL_GET_CURRENT_TIME 5
#define STREAM_CTRL_SEEK_TO_TIME 6
#define STREAM_CTRL_GET_SIZE 7
#define STREAM_CTRL_GET_ASPECT_RATIO 8

#define MAX_STREAM_PROTOCOLS 32
#define STREAM_BUFFER_SIZE 2048//32768

struct stream_s;
typedef struct stream_info_s {
    const char *comment;
    const char *name;

    const char* protocols[MAX_STREAM_PROTOCOLS];
    int (*open)(struct stream_s *stream, const char *filename, int *file_format);
} stream_info_t;

typedef struct stream_s {
    //int (*open)(struct stream_s *stream, const char *filename, int *file_format);
    int (*fill_buffer)(struct stream_s *stream, char* buffer, int max_len);
    int (*seek)(struct stream_s *stream,off_t pos);
    int (*control)(struct stream_s *stream,int cmd,void* arg);
    void (*close)(struct stream_s *stream);

    int type;
    int flags;
    int mode;
    int fd;   /* file descriptor, see man open(2) */
    int sector_size; /* sector size (seek will be aligned on this size if non 0) */
    off_t buf_pos,buf_len;
    off_t pos,start_pos,end_pos;
    int eof;
    unsigned int cache_pid; /*<<<For cache by fork() */
    void* cache_thread; /*<<<For cache by multi-thread */
    void* cache_data;

    streaming_ctrl_t *streaming_ctrl;

    unsigned char buffer[STREAM_BUFFER_SIZE];
    //unsigned char *pbuffer;
} stream_t;

stream_t* new_stream(int fd, int type);
void free_stream(stream_t *stream);
stream_t* new_memory_stream(unsigned char* data,int len);

stream_t* open_stream(const char* filename,char** options, int* file_format);
stream_t* open_stream_full(const char* filename,char** options,int* file_format);
int stream_fill_buffer(stream_t *stream);
int stream_control(stream_t *stream, int cmd, void *arg);
int stream_seek_long(stream_t *stream, off_t pos);
void stream_reset(stream_t *stream);


inline static int stream_read_char(stream_t *stream) {
    return (stream->buf_pos<stream->buf_len)?stream->buffer[stream->buf_pos++]:
            (cache_stream_fill_buffer(stream)?stream->buffer[stream->buf_pos++]:-256);
    /*

    if(stream->buf_pos<s->buf_len) return stream->buffer[stream->buf_pos++];
    stream_fill_buffer(stream);
    if(stream->buf_pos<s->buf_len) return stream->buffer[stream->buf_pos++];
    return 0; // EOF

    */
}

inline static unsigned int stream_read_word(stream_t *stream) {
    int x,y;
    x=stream_read_char(stream);
    y=stream_read_char(stream);
    return (x<<8)|y;
}

inline static unsigned int stream_read_dword(stream_t *stream) {
    unsigned int y;
    y=stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    return y;
}

#define stream_read_fourcc stream_read_dword_le

inline static unsigned int stream_read_word_le(stream_t *stream) {
    int x,y;
    x=stream_read_char(stream);
    y=stream_read_char(stream);
    return (y<<8)|x;
}

inline static unsigned int stream_read_dword_le(stream_t *stream) {
    unsigned int y;
    y=stream_read_char(stream);
    y|=stream_read_char(stream)<<8;
    y|=stream_read_char(stream)<<16;
    y|=stream_read_char(stream)<<24;
    return y;
}

inline static uint64_t stream_read_qword(stream_t *stream) {
    uint64_t y;
    y = stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    return y;
}

inline static uint64_t stream_read_qword_le(stream_t *stream) {
    uint64_t y;
    y = stream_read_dword_le(stream);
    y|=(uint64_t)stream_read_dword_le(stream)<<32;
    return y;
}

inline static unsigned int stream_read_int24(stream_t *stream) {
    unsigned int y;
    y = stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    y=(y<<8)|stream_read_char(stream);
    return y;
}

inline static int stream_read(stream_t *stream,char* mem,int total) {
    int len = total;
    while(len > 0) {
        int x;
        x = stream->buf_len - stream->buf_pos;
        if(x==0) {
            if(!cache_stream_fill_buffer(stream)) return total-len; // EOF
            x = stream->buf_len - stream->buf_pos;
        }
        if(stream->buf_pos > stream->buf_len)
            ws_log(log::M_DEMUX, log::WARN) << "stream_read: WARNING! s->buf_pos > s->buf_len" << endlog;
        if(x>len)
            x = len;

        memcpy(mem,&stream->buffer[stream->buf_pos],x);
        stream->buf_pos+=x; mem+=x; len-=x;
    }
    return total;
}

inline static unsigned char* stream_read_line(stream_t *stream,unsigned char* mem, int max_len) {
    int len;
    unsigned char* _end,*ptr = mem;;
    do {
        len = stream->buf_len - stream->buf_pos;
        // try to fill the buffer
        if(len <= 0 &&
           (!cache_stream_fill_buffer(stream) ||
            (len = stream->buf_len - stream->buf_pos) <= 0)) break;

        _end = (unsigned char*) memchr((void*)(stream->buffer + stream->buf_pos),'\n',len);
        if(_end) len = _end - (stream->buffer + stream->buf_pos) + 1;

        if(len > 0 && max_len > 1) {
            int l = len > max_len-1 ? max_len-1 : len;
            memcpy(ptr,stream->buffer + stream->buf_pos,l);
            max_len -= l;
            ptr += l;
        }
        stream->buf_pos += len;
    } while(!_end);

    if(stream->eof && ptr == mem) return NULL;
    if(max_len > 0) ptr[0] = 0;

    return mem;
}

inline static int stream_seek(stream_t *stream,off_t pos) {
    ws_dbg(MSGT_DEMUX, MSGL_DBG3, "seek to 0x%qX\n",(long long)pos);

    if(pos < stream->pos) {
        off_t x=pos-(stream->pos - stream->buf_len);
        if(x>=0) {
            stream->buf_pos=x;
    //      putchar('*');fflush(stdout);
            return 1;
        }
    }

    return cache_stream_seek_long(stream,pos);
}

inline static off_t stream_tell(stream_t *stream) {
    return stream->pos + stream->buf_pos - stream->buf_len;
}

inline static int stream_skip(stream_t *stream,off_t len) {
    if( (len<0 && (stream->flags & STREAM_SEEK_BW)) || (len>2*STREAM_BUFFER_SIZE && (stream->flags & STREAM_SEEK_FW)) ) {
        /* negative or big skip! */
        return stream_seek(stream, stream_tell(stream) + len);
    }
    while(len>0) {
        int x = stream->buf_len - stream->buf_pos;
        if(x==0) {
            if(!cache_stream_fill_buffer(stream)) return 0; // EOF
            x = stream->buf_len - stream->buf_pos;
        }
        if(x>len) x=len;
        //memcpy(mem,&stream->buf[stream->buf_pos],x);
        stream->buf_pos+=x; len-=x;
    }
    return 1;
}

inline static int stream_eof(stream_t *stream) {
    return stream->eof;
}

} // namespace openwsp

#endif // !defined(STREAM_H_)
