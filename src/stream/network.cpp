/** @file
 * Openwsp - network processing.
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
#include <string.h>
#include <unistd.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <errno.h>
#include <ctype.h>

#include <openwsp/types.h>
#include <openwsp/misc.h>

#include "../log.h"
#include "tcp.h"
#include "stream.h"

#include "network.h"
#include "network_ext.h"
#include "tcp.h"
#include "stream_http.h"
#include "cookies.h"
#include "url.h"

#include "config.h"

namespace openwsp {

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
#include "public.h"
char *network_username=NULL;
char *network_password=NULL;
int   network_bandwidth=0;
int   network_cookies_enabled = 0;
char *network_useragent=NULL;

/* IPv6 options */
int   network_ipv4_only_proxy = 0;

#include <openwsp/demuxtype.h>
mime_struct_t mime_type_table[] = {
	// MP3 streaming, some MP3 streaming server answer with audio/mpeg
	{ "audio/mpeg", DEMUXER_TYPE_AUDIO },
	// MPEG streaming
	{ "video/mpeg", DEMUXER_TYPE_UNKNOWN },
	{ "video/x-mpeg", DEMUXER_TYPE_UNKNOWN },
	{ "video/x-mpeg2", DEMUXER_TYPE_UNKNOWN },
	// AVI ??? => video/x-msvideo
	{ "video/x-msvideo", DEMUXER_TYPE_AVI },
	// MOV => video/quicktime
	{ "video/quicktime", DEMUXER_TYPE_MOV },
	// ASF
    { "audio/x-ms-wax", DEMUXER_TYPE_ASF },
	{ "audio/x-ms-wma", DEMUXER_TYPE_ASF },
	{ "video/x-ms-asf", DEMUXER_TYPE_ASF },
	{ "video/x-ms-afs", DEMUXER_TYPE_ASF },
	{ "video/x-ms-wmv", DEMUXER_TYPE_ASF },
	{ "video/x-ms-wma", DEMUXER_TYPE_ASF },
	{ "application/x-mms-framed", DEMUXER_TYPE_ASF },
	{ "application/vnd.ms.wms-hdr.asfv1", DEMUXER_TYPE_ASF },
	{ "application/octet-stream", DEMUXER_TYPE_UNKNOWN },
	// Playlists
	{ "video/x-ms-wmx", DEMUXER_TYPE_PLAYLIST },
	{ "video/x-ms-wvx", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-scpls", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-mpegurl", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-pls", DEMUXER_TYPE_PLAYLIST },
	// Real Media
//	{ "audio/x-pn-realaudio", DEMUXER_TYPE_REAL },
	// OGG Streaming
	{ "application/x-ogg", DEMUXER_TYPE_OGG },
	// NullSoft Streaming Video
	{ "video/nsv", DEMUXER_TYPE_NSV},
	{ "misc/ultravox", DEMUXER_TYPE_NSV},
	{ NULL, DEMUXER_TYPE_UNKNOWN},
};


////////////////////////////////////////////////////////////////////////////////

streaming_ctrl_t * streaming_ctrl_new(void)
{
	streaming_ctrl_t *streaming_ctrl;
	streaming_ctrl = (streaming_ctrl_t*)malloc(sizeof(streaming_ctrl_t));
	if(streaming_ctrl == NULL) {
		ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
		return NULL;
	}
	memset(streaming_ctrl, 0, sizeof(streaming_ctrl_t) );
	return streaming_ctrl;
}

void streaming_ctrl_free( streaming_ctrl_t *streaming_ctrl )
{
	if( streaming_ctrl==NULL ) return;
	if( streaming_ctrl->url ) URLi::deleteinstance( streaming_ctrl->url );
	if( streaming_ctrl->buffer ) free( streaming_ctrl->buffer );
	if( streaming_ctrl->data ) free( streaming_ctrl->data );
	free( streaming_ctrl );
}

URLi* check4proxies( URLi *url )
{
	URLi *url_out = NULL;
	if( url==NULL ) return NULL;
	url_out = URLi::newinstance( url->m_url );
	if( !strcasecmp(url->m_protocol, "http_proxy") ) {
		ws_log(log::M_NETWORK,log::VERBOSE)
		        << "Using HTTP proxy: http://" << url->m_hostname << ":" << url->m_port
		        << endlog;
		return url_out;
	}

	// Check if the http_proxy environment variable is set.
	if( !strcasecmp(url->m_protocol, "http") ) {
		char *proxy;
		proxy = getenv("http_proxy");
		if( proxy!=NULL ) {
			// We got a proxy, build the URL to use it
			int len;
			char *new_url;
			URLi *tmp_url;
			URLi *proxy_url = URLi::newinstance( proxy );

			if(proxy_url==NULL) {
				ws_log(log::M_NETWORK,log::WARN)
					<< MSGTR_DEMUX_NW_InvalidProxySettingTryingWithout << endlog;
				return url_out;
			}

#ifdef HAVE_AF_INET6
			if (network_ipv4_only_proxy && (gethostbyname(m_url->m_hostname)==NULL)) {
				ws_log(MSGT_NETWORK,log::WARN,
					MSGTR_DEMUX_NW_CantResolvTryingWithoutProxy);
				url_free(proxy_url);
				return url_out;
			}
#endif

			ws_log(log::M_NETWORK,log::VERBOSE) << "Using HTTP proxy: " << proxy_url->m_url << endlog;
			len = strlen(proxy_url->m_hostname) + strlen(url->m_url) + 20;	// 20 = http_proxy:// + port
			new_url = (char*)malloc( len+1 );
			if(new_url == NULL) {
				ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
				URLi::deleteinstance(proxy_url);
				return url_out;
			}
			sprintf(new_url, "http_proxy://%s:%d/%s", proxy_url->m_hostname, proxy_url->m_port, url->m_url );
			tmp_url = URLi::newinstance( new_url );
			if(tmp_url == NULL) {
				free(new_url);
				URLi::deleteinstance(proxy_url);
				return url_out;
			}
			URLi::deleteinstance(url_out);
			url_out = tmp_url;
			free(new_url);
			URLi::deleteinstance(proxy_url);
		}
	}
	return url_out;
}

int http_send_request( URLi *url, off_t pos )
{
	HTTP_header_t *http_hdr;
	URLi *server_url;
	char str[256];
	int fd = -1;
	int ret;
	int proxy = 0;		/* Boolean*/

	http_hdr = http_new_header();

	if( !strcasecmp(url->m_protocol, "http_proxy") ) {
		proxy = 1;
		server_url = URLi::newinstance( (url->m_file)+1 );
		http_set_uri(http_hdr, server_url->m_url );
	} else {
		server_url = url;
		http_set_uri(http_hdr, server_url->m_file );
	}
	if (server_url->m_port && server_url->m_port != 80)
	    snprintf(str, 256, "Host: %s:%d", server_url->m_hostname, server_url->m_port );
	else
	    snprintf(str, 256, "Host: %s", server_url->m_hostname );
	http_set_field( http_hdr, str);
	if (network_useragent)
	{
	    snprintf(str, 256, "User-Agent: %s", network_useragent);
	    http_set_field(http_hdr, str);
	}
	else
	    http_set_field(http_hdr, "User-Agent: mpcore");

	http_set_field(http_hdr, "Icy-MetaData: 1");

	if(pos>0) {
        /* Extend http_send_request with possibility to do partial content retrieval */
	    snprintf(str, 256, "Range: bytes=%"PRId64"-", (int64_t)pos);
	    http_set_field(http_hdr, str);
	}

	if (network_cookies_enabled) cookies_set(http_hdr, server_url->m_hostname, server_url->m_url);

	http_set_field(http_hdr, "Connection: close");
	http_add_basic_authentication(http_hdr, url->m_username, url->m_password);
	if(http_build_request(http_hdr)==NULL ) {
		goto err_out;
	}

	if(proxy) {
		if(url->m_port==0 ) url->m_port = 8080;			// Default port for the proxy server
		fd = connectServer(url->m_hostname, url->m_port,1 );
		URLi::deleteinstance(server_url );
		server_url = NULL;
	} else {
		if(server_url->m_port==0 ) server_url->m_port = 80;	// Default port for the web server
		fd = connectServer(server_url->m_hostname, server_url->m_port,1 );
	}
	if(fd<0) {
	    ws_log(log::M_NETWORK,log::ERR) << "err: connectServer()" << endlog;
		goto err_out;
	}
	ws_log(log::M_NETWORK,log::DBG2) << "Request: [" << http_hdr->buffer << "]" << endlog;

	ret = socketSend(fd, http_hdr->buffer, http_hdr->buffer_size, 0);

	if(ret != (int)http_hdr->buffer_size ) {
		ws_log(log::M_NETWORK,log::ERR) << MSGTR_DEMUX_NW_ErrSendingHTTPRequest << endlog;
		goto err_out;
	}

	http_free( http_hdr );

	return fd;
err_out:
	if (fd > 0) socketClose(fd);
	http_free(http_hdr);
	if (proxy && server_url)
	    URLi::deleteinstance(server_url);
	return -1;
}

HTTP_header_t * http_read_response( int fd )
{
	HTTP_header_t *http_hdr;
	char response[BUFFER_SIZE];
	int i;

	http_hdr = http_new_header();
	if(http_hdr==NULL) {
		return NULL;
	}

	do {
		i = socketRecv(fd, response, BUFFER_SIZE, 0);
		if(i < 0) {
			ws_log(log::M_NETWORK,log::ERR) << MSGTR_DEMUX_NW_ReadFailed << endlog;
			http_free(http_hdr);
			return NULL;
		}
		if(i==0) {
			ws_log(log::M_NETWORK,log::ERR) << MSGTR_DEMUX_NW_Read0CouldBeEOF << endlog;
			http_free(http_hdr);
			return NULL;
		}
		http_response_append(http_hdr, response, i);
	} while( !http_is_header_entire(http_hdr) );
	http_response_parse(http_hdr);
	return http_hdr;
}

int http_authenticate(HTTP_header_t *http_hdr, URLi *url, int *auth_retry)
{
	char *aut;

	if(*auth_retry==1) {
		ws_log(log::M_NETWORK,log::ERR) << MSGTR_DEMUX_NW_AuthFailed << endlog;
		return -1;
	}
	if(*auth_retry>0) {
		if(url->m_username) {
			free(url->m_username);
			url->m_username = NULL;
		}
		if(url->m_password) {
			free(url->m_password);
			url->m_password = NULL;
		}
	}

	aut = http_get_field(http_hdr, "WWW-Authenticate");
	if(aut!=NULL) {
		char *aut_space;
		aut_space = strstr(aut, "realm=");
		if(aut_space!=NULL) aut_space += 6;
		ws_log::write(log::M_NETWORK,log::INFO,MSGTR_DEMUX_NW_AuthRequiredFor, aut_space);
	} else {
		ws_log(log::M_NETWORK,log::INFO) << MSGTR_DEMUX_NW_AuthRequired << endlog;
	}
	if(network_username) {
		url->m_username = strdup(network_username);
		if(url->m_username==NULL) {
			ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
			return -1;
		}
	} else {
		ws_log(log::M_NETWORK,log::ERR) << MSGTR_DEMUX_NW_AuthFailed << endlog;
		return -1;
	}
	if(network_password) {
		url->m_password = strdup(network_password);
		if(url->m_password==NULL) {
			ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
			return -1;
		}
	} else {
		ws_log(log::M_NETWORK,log::INFO) << MSGTR_DEMUX_NW_NoPasswdProvidedTryingBlank << endlog;
	}
	(*auth_retry)++;
	return 0;
}

int http_seek(stream_t *stream, off_t pos)
{
	HTTP_header_t *http_hdr = NULL;
	int fd;
	if(stream==NULL) return 0;

	if(stream->fd > 0) socketClose(stream->fd); /* need to reconnect to seek in http-stream*/
	fd = http_send_request(stream->streaming_ctrl->url, pos);
	if(fd < 0) return 0;

	http_hdr = http_read_response( fd );

	if(http_hdr==NULL) return 0;

	switch(http_hdr->status_code) {
		case 200:
		case 206: // OK
			ws_log(log::M_NETWORK,log::VERBOSE) << "Content-Type: [" << http_get_field(http_hdr, "Content-Type") <<"]" << endlog;
			ws_log(log::M_NETWORK,log::VERBOSE) << "Content-Length: [" << http_get_field(http_hdr, "Content-Length") << "]" << endlog;
			stream->end_pos = atoi(http_get_field(http_hdr, "Content-Length"));

			if(http_hdr->body_size>0) {
				if(streaming_bufferize(stream->streaming_ctrl, http_hdr->body, http_hdr->body_size)<0 ) {
					http_free(http_hdr);
					return -1;
				}
			}
			break;
		default:
			ws_log::write(log::M_NETWORK,log::ERR,MSGTR_DEMUX_NW_ErrServerReturned, http_hdr->status_code, http_hdr->reason_phrase );
			close(fd);
			fd = -1;
	}
	stream->fd = fd;

	if(http_hdr) {
		http_free(http_hdr);
		stream->streaming_ctrl->data = NULL;
	}

	stream->pos=pos;

	return 1;
}


int streaming_bufferize( streaming_ctrl_t *streaming_ctrl, char *buffer, int size)
{
	streaming_ctrl->buffer = (char*)malloc(size);
	if(streaming_ctrl->buffer==NULL) {
		ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
		return -1;
	}
	memcpy( streaming_ctrl->buffer, buffer, size );
	streaming_ctrl->buffer_size = size;

	return size;
}

int nop_streaming_read( int fd, char *buffer, int size, streaming_ctrl_t *stream_ctrl )
{
	int len=0;
//printf("nop_streaming_read\n");
	if( stream_ctrl->buffer_size!=0 ) {
		int buffer_len = stream_ctrl->buffer_size-stream_ctrl->buffer_pos;
//printf("%d bytes in buffer\n", stream_ctrl->buffer_size);
		len = (size<buffer_len)?size:buffer_len;
		memcpy( buffer, (stream_ctrl->buffer)+(stream_ctrl->buffer_pos), len );
		stream_ctrl->buffer_pos += len;
//printf("buffer_pos = %d\n", stream_ctrl->buffer_pos );
		if(stream_ctrl->buffer_pos>=stream_ctrl->buffer_size) {
			free(stream_ctrl->buffer);
			stream_ctrl->buffer = NULL;
			stream_ctrl->buffer_size = 0;
			stream_ctrl->buffer_pos = 0;
//printf("buffer cleaned\n");
		}
//printf("read %d bytes from buffer\n", len );
	}

	if(len<size) {
		int ret;
		ret = socketRecv(fd, buffer+len, size-len, 0);
		if(ret<0) {
			ws_log(log::M_NETWORK,log::ERR) << "nop_streaming_read error : " << strerror(errno) << endlog;
		}
		len += ret;
//printf("read %d bytes from network\n", len );
	}

	return len;
}

int nop_streaming_seek(int fd, off_t pos, streaming_ctrl_t *stream_ctrl)
{
	return -1;
	// To shut up gcc warning
	fd++;
	pos++;
	stream_ctrl=NULL;
}


void fixup_network_stream_cache(stream_t *stream)
{
    if(stream->streaming_ctrl->buffering) {
        if(CFG_stream_cache_size<0) {
            /* cache option not set, will use our computed value.
            // buffer in KBytes, *5 because the prefill is 20% of the buffer.*/
            CFG_stream_cache_size = (stream->streaming_ctrl->prebuffer_size/1024)*5;
            if( CFG_stream_cache_size<64 ) CFG_stream_cache_size = 64;	// 16KBytes min buffer
        }
        ws_log::write(log::M_NETWORK,log::INFO,MSGTR_DEMUX_NW_CacheSizeSetTo, CFG_stream_cache_size);
    }
}


int streaming_stop( stream_t *stream )
{
	stream->streaming_ctrl->status = streaming_stopped_e;
	return 0;
}

} // namespace openwsp
