/** @file
 * Openwsp - URL processing.
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include <new>

#include <openwsp/types.h>
#include <openwsp/assert.h>
#include <openwsp/err.h>

#include "url.h"

#include "../log.h"

namespace openwsp {

/*******************************************************************************
*   Macro definitions                                                          *
*******************************************************************************/
#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#endif


////////////////////////////////////////////////////////////////////////////////


URLi::URLi() {
    m_password =
    m_username = NULL;
    m_port = 0;
    m_file =
    m_hostname =
    m_protocol =
    m_url = NULL;
    m_inited = false;
}

URLi::~URLi() {
    if (m_inited)
        uninit();
}

////////////////////////////////////////////////////////////////////////////////


int URLi::init(const char* url) {
    int pos1, pos2,v6addr = 0;
    char *escfilename=NULL;
    char *ptr1=NULL, *ptr2=NULL, *ptr3=NULL, *ptr4=NULL;
    int jumpSize = 3;
    if (url==NULL) return WERR_FAILED;
    // reset if needed
    if (m_inited)
        uninit();
    if (strlen(url) > (SIZE_MAX / 3 - 1)) {
        ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
        goto err_out;
    }
    escfilename = (char*)malloc(strlen(url)*3+1);
    if (!escfilename ) {
        ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
        goto err_out;
    }

    escapeString(escfilename,url);

    // Copy the url in the URL container
    m_url = strdup(escfilename);
    if( m_url==NULL ) {
        ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
        goto err_out;
    }
    ws_log(log::M_OPEN,log::VERBOSE) << "Filename for url is now " << escfilename << endlog;

    // extract the protocol
    ptr1 = strstr(escfilename, "://");
    if( ptr1==NULL ) {
            // Check for a special case: "sip:" (without "//"):
            if (strstr(escfilename, "sip:") == escfilename) {
                ptr1 = (char *)&url[3]; // points to ':'
            jumpSize = 1;
        } else {
                ws_log(log::M_NETWORK,log::VERBOSE) << "Not an URL!" << endlog;
            goto err_out;
        }
    }
    pos1 = ptr1-escfilename;
    m_protocol = (char*)malloc(pos1+1);
    if( m_protocol==NULL ) {
        ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
        goto err_out;
    }
    strncpy(m_protocol, escfilename, pos1);
    m_protocol[pos1] = '\0';

    // jump the "://"
    ptr1 += jumpSize;
    pos1 += jumpSize;

    // check if a username:password is given
    ptr2 = strstr(ptr1, "@");
    ptr3 = strstr(ptr1, "/");
    if( ptr3!=NULL && ptr3<ptr2 ) {
        // it isn't really a username but rather a part of the path
        ptr2 = NULL;
    }
    if( ptr2!=NULL ) {
        // We got something, at least a username...
        int len = ptr2-ptr1;
        m_username = (char*)malloc(len+1);
        if( m_username==NULL ) {
            ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
            goto err_out;
        }
        strncpy(m_username, ptr1, len);
        m_username[len] = '\0';

        ptr3 = strstr(ptr1, ":");
        if( ptr3!=NULL && ptr3<ptr2 ) {
            // We also have a password
            int len2 = ptr2-ptr3-1;
            m_username[ptr3-ptr1]='\0';
            m_password = (char*)malloc(len2+1);
            if( m_password==NULL ) {
                ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
                goto err_out;
            }
            strncpy( m_password, ptr3+1, len2);
            m_password[len2]='\0';
        }
        ptr1 = ptr2+1;
        pos1 = ptr1-escfilename;
    }

    // before looking for a port number check if we have an IPv6 type numeric address
    // in IPv6 URL the numeric address should be inside square braces.
    ptr2 = strstr(ptr1, "[");
    ptr3 = strstr(ptr1, "]");
    ptr4 = strstr(ptr1, "/");
    if( ptr2!=NULL && ptr3!=NULL && ptr2 < ptr3 && (!ptr4 || ptr4 > ptr3)) {
        // we have an IPv6 numeric address
        ptr1++;
        pos1++;
        ptr2 = ptr3;
        v6addr = 1;
    } else {
        ptr2 = ptr1;

    }

    // look if the port is given
    ptr2 = strstr(ptr2, ":");
    // If the : is after the first / it isn't the port
    ptr3 = strstr(ptr1, "/");
    if(ptr3 && ptr3 - ptr2 < 0) ptr2 = NULL;
    if( ptr2==NULL ) {
        // No port is given
        // Look if a path is given
        if( ptr3==NULL ) {
            // No path/filename
            // So we have an URL like http://www.hostname.com
            pos2 = strlen(escfilename);
        } else {
            // We have an URL like http://www.hostname.com/file.txt
                        pos2 = ptr3-escfilename;
        }
    } else {
        // We have an URL beginning like http://www.hostname.com:1212
        // Get the port number
        m_port = atoi(ptr2+1);
        pos2 = ptr2-escfilename;
    }
    if( v6addr ) pos2--;
    // copy the hostname in the URL container
    m_hostname = (char*)malloc(pos2-pos1+1);
    if( m_hostname==NULL ) {
        ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
        goto err_out;
    }
    strncpy(m_hostname, ptr1, pos2-pos1);
    m_hostname[pos2-pos1] = '\0';

    // Look if a path is given
    ptr2 = strstr(ptr1, "/");
    if( ptr2!=NULL ) {
        // A path/filename is given
        // check if it's not a trailing '/'
        if( strlen(ptr2)>1 ) {
            // copy the path/filename in the URL container
            m_file = strdup(ptr2);
            if( m_file==NULL ) {
                ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
                goto err_out;
            }
        }
    }
    // Check if a filename was given or set, else set it with '/'
    if( m_file==NULL ) {
        m_file = (char*)malloc(2);
        if( m_file==NULL ) {
            ws_log(log::M_NETWORK,log::FATAL) << MSGTR_MemAllocFailed << endlog;
            goto err_out;
        }
        strcpy(m_file, "/");
    }

    free(escfilename);
    return WINF_SUCCEEDED;
err_out:
    if (escfilename) free(escfilename);
    uninit();
    return WERR_FAILED;
}

int URLi::uninit() {
    if(!m_inited) return WERR_INTERNAL_LOGIC;
    if(m_url) free(m_url);
    if(m_protocol) free(m_protocol);
    if(m_hostname) free(m_hostname);
    if(m_file) free(m_file);
    if(m_username) free(m_username);
    if(m_password) free(m_password);
    m_inited = false;
    return WINF_SUCCEEDED;
}


////////////////////////////////////////////////////////////////////////////////


int URLi::redirect(const char *redir) {
    int rc;
    if (!strchr(redir, '/') || *redir == '/') {
        char *tmp;
        char *newurl = (char*)malloc(strlen(m_url) + strlen(redir) + 1);
        strcpy(newurl, m_url);
        if (*redir == '/') {
            redir++;
            tmp = strstr(newurl, "://");
            if (tmp) tmp = strchr(tmp + 3, '/');
        } else
            tmp = strrchr(newurl, '/');
            if (tmp) tmp[1] = 0;
        strcat(newurl, redir);
        rc = init(newurl);
        free(newurl);
    } else {
        rc = init(redir);
    }
    return rc;
}


/* Replace escape sequences in an URL (or a part of an URL) */
/* works like strcpy(), but without return argument */
void URLi::unescapeString(char *outbuf, const char *inbuf)
{
	unsigned char c,c1,c2;
        int i,len=strlen(inbuf);
        for (i=0;i<len;i++){
		c = inbuf[i];
		if (c == '%' && i<len-2) { //must have 2 more chars
			c1 = toupper(inbuf[i+1]); // we need uppercase characters
			c2 = toupper(inbuf[i+2]);
			if (	((c1>='0' && c1<='9') || (c1>='A' && c1<='F')) &&
				((c2>='0' && c2<='9') || (c2>='A' && c2<='F')) ) {
				if (c1>='0' && c1<='9') c1-='0';
				else c1-='A'-10;
				if (c2>='0' && c2<='9') c2-='0';
				else c2-='A'-10;
				c = (c1<<4) + c2;
                                i=i+2; //only skip next 2 chars if valid esc
			}
		}
		*outbuf++ = c;
	}
        *outbuf++='\0'; //add nullterm to string
}

static void escape_string_part(char *outbuf, const char *inbuf)
{
	unsigned char c,c1,c2;
    int i,len=strlen(inbuf);

	for(i=0;i<len;i++) {
		c = inbuf[i];
                if ((c=='%') && i<len-2 ) { //need 2 more characters
                    c1=toupper(inbuf[i+1]); c2=toupper(inbuf[i+2]); // need uppercase chars
                   } else {
                    c1=129; c2=129; //not escape chars
                   }

		if(	(c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9') ||
			(c >= 0x7f)) {
			*outbuf++ = c;
                } else if ( c=='%' && ((c1 >= '0' && c1 <= '9') || (c1 >= 'A' && c1 <= 'F')) &&
                           ((c2 >= '0' && c2 <= '9') || (c2 >= 'A' && c2 <= 'F'))) {
                                                              // check if part of an escape sequence
                            *outbuf++=c;                      // already

                                                              // dont escape again
                            ws_log::write(log::M_NETWORK,log::ERR,MSGTR_DEMUX_URL_StringAlreadyEscaped,c,c1,c2);
                                                              // error as this should not happen against RFC 2396
                                                              // to escape a string twice
		} else {
			/* all others will be escaped */
			c1 = ((c & 0xf0) >> 4);
			c2 = (c & 0x0f);
			if (c1 < 10) c1+='0';
			else c1+='A'-10;
			if (c2 < 10) c2+='0';
			else c2+='A'-10;
			*outbuf++ = '%';
			*outbuf++ = c1;
			*outbuf++ = c2;
		}
	}
        *outbuf++='\0';
}

/* Replace specific characters in the URL string by an escape sequence */
/* works like strcpy(), but without return argument */
void URLi::escapeString(char *outbuf, const char *inbuf)
{
	unsigned char c=0;
        int i = 0,j,len = strlen(inbuf);
	char* tmp,*unesc = NULL, *in;

	// Look if we have an ip6 address, if so skip it there is
	// no need to escape anything in there.
	tmp = strstr(inbuf,"://[");
	if(tmp) {
		tmp = strchr(tmp+4,']');
		if(tmp && (tmp[1] == '/' || tmp[1] == ':' ||
			   tmp[1] == '\0')) {
			i = tmp+1-inbuf;
			strncpy(outbuf,inbuf,i);
			outbuf += i;
			tmp = NULL;
		}
	}

	while(i < len) {
		// look for the next char that must be kept
		for  (j=i;j<len;j++) {
			c = inbuf[j];
			if(c=='-' || c=='_' || c=='.' || c=='!' || c=='~' ||	/* mark characters */
			   c=='*' || c=='\'' || c=='(' || c==')' || 	 	/* do not touch escape character */
			   c==';' || c=='/' || c=='?' || c==':' || c=='@' || 	/* reserved characters */
			   c=='&' || c=='=' || c=='+' || c=='$' || c==',') 	/* see RFC 2396 */
				break;
		}
		// we are on a reserved char, write it out
		if(j == i) {
			*outbuf++ = c;
			i++;
			continue;
		}
		// we found one, take that part of the string
		if(j < len) {
			if(!tmp) tmp = (char*)malloc(len+1);
			strncpy(tmp,inbuf+i,j-i);
			tmp[j-i] = '\0';
			in = tmp;
		} else // take the rest of the string
			in = (char*)inbuf+i;

		if(!unesc) unesc = (char*)malloc(len+1);
		// unescape first to avoid escaping escape
		unescapeString(unesc,in);
		// then escape, including mark and other reserved chars
		// that can come from escape sequences
		escape_string_part(outbuf,unesc);
		outbuf += strlen(outbuf);
		i += strlen(in);
	}
	*outbuf = '\0';
	if(tmp) free(tmp);
	if(unesc) free(unesc);
}

URLi* URLi::newinstance(const char* url) {
    URLi *ins = new (std::nothrow) URLi;
    if (!ins) return 0;
    if (WS_FAILURE( ins->init(url) )) {
        delete ins;
        return 0;
    }
    return ins;
}

void URLi::deleteinstance(URLi* url) {
    if (url) delete url;
}

#ifdef __URL_DEBUG
void url_debug(const URLi *url)
{
	if( url==NULL ) {
		printf("URL pointer NULL\n");
		return;
	}
	if( url->m_url!=NULL ) {
		printf("url=%s\n", m_url->m_url );
	}
	if( url->m_protocol!=NULL ) {
		printf("protocol=%s\n", m_url->m_protocol );
	}
	if( url->m_hostname!=NULL ) {
		printf("hostname=%s\n", m_url->m_hostname );
	}
	printf("port=%d\n", m_url->m_port );
	if( url->m_file!=NULL ) {
		printf("file=%s\n", m_url->m_file );
	}
	if( url->m_username!=NULL ) {
		printf("username=%s\n", m_url->m_username );
	}
	if( url->m_password!=NULL ) {
		printf("password=%s\n", m_url->m_password );
	}
}
#endif //__URL_DEBUG

} // namespace openwsp
