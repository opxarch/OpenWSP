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

#ifndef STREAM_NETWORK_H_
#define STREAM_NETWORK_H_

#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>

#include <openwsp/misc.h>
#if !HAVE(WINSOCK2)
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include "url.h"
#include "stream_http.h"
#include "stream.h"

namespace openwsp {

#define BUFFER_SIZE		2048

typedef struct {
	const char *mime_type;
	int demuxer_type;
} mime_struct_t;

typedef enum {
	streaming_stopped_e,
	streaming_playing_e
} streaming_status;

typedef struct streaming_control {
	URLi *url;
	streaming_status status;
	int buffering;	// boolean
	unsigned int prebuffer_size;
	char *buffer;
	unsigned int buffer_size;
	unsigned int buffer_pos;
	unsigned int bandwidth;	// The downstream available
	int (*streaming_read)( int fd, char *m_buffer, int m_buffer_size, struct streaming_control *stream_ctrl );
	int (*streaming_seek)( int fd, off_t pos, struct streaming_control *stream_ctrl );
	void *data;
} streaming_ctrl_t;

//int streaming_start( stream_t *stream, int *demuxer_type, URLi *url );
streaming_ctrl_t *streaming_ctrl_new(void);
int streaming_bufferize( streaming_ctrl_t *streaming_ctrl, char *buffer, int size);

int nop_streaming_read( int fd, char *buffer, int size, streaming_ctrl_t *stream_ctrl );
int nop_streaming_seek( int fd, off_t pos, streaming_ctrl_t *stream_ctrl );
void streaming_ctrl_free( streaming_ctrl_t *streaming_ctrl );

int http_send_request(URLi *url, off_t pos);
HTTP_header_t *http_read_response(int fd);

int http_authenticate(HTTP_header_t *http_hdr, URLi *url, int *auth_retry);
URLi* check4proxies(URLi *url);

} // namespace openwsp

#endif //!defined(STREAM_NETWORK_H_)
