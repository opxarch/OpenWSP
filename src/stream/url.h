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

#ifndef STREAM_URL_H_
#define STREAM_URL_H_

namespace openwsp {

//#define URL_DEBUG

/***************************************************
  *****             URL descriptor             *****
  ***************************************************/

class URLi{
public:
    URLi();
    ~URLi();

    int init(const char* url);
    int uninit();

    static URLi* newinstance(const char* url);
    static void  deleteinstance(URLi* url);

public:
    int redirect(const char *redir);

    void unescapeString(char *outbuf, const char *inbuf);
    void escapeString(char *outbuf, const char *inbuf);

public:
    bool m_inited;
	char *m_url;
	char *m_protocol;
	char *m_hostname;
	char *m_file;
	unsigned int m_port;
	char *m_username;
	char *m_password;
};


#ifdef URL_DEBUG
void url_debug(const URLi* url);
#endif /* URL_DEBUG */

} // namespace openwsp

#endif // !defined(STREAM_URL_H_)
