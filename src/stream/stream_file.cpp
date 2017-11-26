
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>

#include "../log.h"
#include "stream.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

static int openf(struct stream_s *stream, const char *filename, int *file_name);
stream_info_t stream_info_file =
{
	"The local file support",
	"File",
	{ "file", "", NULL },
	openf
};



static int fill_buffer(stream_t *stream, char* buffer, int max_len){
    int ret = read(stream->fd,buffer,max_len);

    return (ret <= 0) ? -1 : ret;
}

static int seek(stream_t *stream, off_t newpos) {
    stream->pos = newpos;

    if(lseek(stream->fd,stream->pos,SEEK_SET)<0) {

        stream->eof=1;
        return 0;
    }
    return 1;
}

static int seek_forward(stream_t *s,off_t newpos) {
    if(newpos<s->pos){
        ws_log(log::M_STREAM,log::INFO) << "Cannot seek backward in linear streams!" << endlog;
        return 0;
    }
    while(s->pos<newpos){
        int len=s->fill_buffer(s,(char*)s->buffer,STREAM_BUFFER_SIZE);
        if(len<=0){ s->eof=1; s->buf_pos=s->buf_len=0; break; } // EOF
        s->buf_pos=0;
        s->buf_len=len;
        s->pos+=len;
    }
    return 1;
}

static int control(stream_t *stream, int cmd, void *arg) {
    switch(cmd) {
        case STREAM_CTRL_GET_SIZE: {
            off_t size;

            size = lseek(stream->fd, 0, SEEK_END);
            lseek(stream->fd, stream->pos, SEEK_SET);
            if(size != (off_t)-1) {
                *((off_t*)arg) = size;
                return 1;
            }
        }
    }
    return STREAM_UNSUPPORTED;
}

static void closef(struct stream_s *stream)
{
    if(stream->fd > 0) {
        close(stream->fd);
    }
}

static int openf(struct stream_s *stream, const char *filename, int *file_name) {
    int fd;
    off_t len;

    fd = open(filename, O_RDONLY | O_BINARY);
    if(fd<0) {
        ws_log::write(log::M_OPEN,log::ERR, MSGTR_FileNotFound,filename);
        return STREAM_ERROR;
    }

    len=lseek(fd,0,SEEK_END); lseek(fd,0,SEEK_SET);

    if(len == -1) {
        stream->seek = seek_forward;
        stream->type = STREAMTYPE_STREAM; /* Must be move to STREAMTYPE_FILE */
        stream->flags |= STREAM_SEEK_FW;
    } else if(len >= 0) {
        stream->seek = seek;
        stream->end_pos = len;
        stream->type = STREAMTYPE_FILE;
    }
    stream->fd = fd;

    stream->fill_buffer = fill_buffer;
    stream->control = control;
    stream->close = closef;

    return STREAM_OK;
}

} // namespace openwsp
