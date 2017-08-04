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

#ifndef STREAM_TCP_H_
#define STREAM_TCP_H_

#include <openwsp/misc.h>

BEGIN_C_DECLS

namespace openwsp {

int connectServer (const char *host, int port, int verb);
int socketRecv(int fd, char *buffer, size_t size, int flags);
int socketSend (int socket, const char *buffer, size_t size, int flags);
int socketClose(int socket);

#define TCP_ERROR_TIMEOUT -3     /* connection timeout */
#define TCP_ERROR_FATAL   -2     /* unable to resolve name */
#define TCP_ERROR_PORT    -1     /* unable to connect to a particular port */

} // namespace openwsp

END_C_DECLS

#endif //!defined(STREAM_TCP_H_)
