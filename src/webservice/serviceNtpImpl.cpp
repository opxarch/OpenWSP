/** @file
 * Webservice - NTP (Network Time Protocol).
 */

/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
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
#include <openwsp/err.h>
#include <openwsp/types.h>
#include <openwsp/bswap.h>

#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/time.h>

#if !HAVE(WINSOCK2)
#include <sys/socket.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "stream/tcp.h"
#include "log.h"

#include "services.h"


/*******************************************************************************
*   Definitions and structures                                                 *
*******************************************************************************/

#define DEBUG 1

#define timeoffset_70s (0x83aa7e80U) // 3600s * 24h * (365d * 70y + 17d)

#define NTPSVR1  "132.163.4.102"        //USA
#define NTPSVR2  "132.163.135.132"      //USA
#define NTPSVR3  "192.53.103.103"       //Germany

#define NTPPORT  (123)

typedef unsigned long ulong32_t;
typedef long long long64_t;

/*
 * Stand NTP Pack
 */
struct NTPPACKET {
    uint8_t     li_vn_mode;
    uint8_t     stratum;
    uint8_t     poll;
    uint8_t     precision;
    ulong32_t   root_delay;
    ulong32_t   root_dispersion;
    int8_t      ref_id[4];
    ulong32_t   reftimestamphigh;
    ulong32_t   reftimestamplow;
    ulong32_t   oritimestamphigh;
    ulong32_t   oritimestamplow;
    ulong32_t   recvtimestamphigh;
    ulong32_t   recvtimestamplow;
    ulong32_t   trantimestamphigh;
    ulong32_t   trantimestamplow;
};

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

/* unused */
int ServiceNtpImpl::init() {
    return WINF_SUCCEEDED;
}
/* unused */
int ServiceNtpImpl::uninit() {
    return WINF_SUCCEEDED;
}

const char *ServiceNtpImpl::getname() {
    return "NTP (Network Time Protocol) Service.";
}
const char *ServiceNtpImpl::getshortname() {
    return "NTP Service";
}

int ServiceNtpImpl::connect() {
    /*
     * Connect to the NTP service.
     */
    m_socket = connectServer(NTPSVR1, NTPPORT, 1);
    if (m_socket < 0) {
        return WERR_CONNECT_TO_SERVICE;
    }
    return WINF_SUCCEEDED;
}

int ServiceNtpImpl::process() {
    int rc = WERR_FAILED;

    NTPPACKET ntppack, resp;
    long64_t   firstTimestamp, finalTimestamp;
    long64_t   difTime, delayTime;


    firstTimestamp = timeoffset_70s + time(NULL);

    /*
     * Fill in the package
     */
    std::memset(&ntppack, 0, sizeof(ntppack));

    ntppack.li_vn_mode = 0x1b; // 0|(3<<2)|(3<<5);
    ntppack.oritimestamphigh = htonl(firstTimestamp);

    /*
     * Send the request package
     */
    int len = socketSend(m_socket, static_cast<const char*>(&ntppack), sizeof(ntppack), 0);
    if (len) {
        /*
         * Receive the response
         */
        len = socketRecv(m_socket, static_cast<char*>(&resp), sizeof(resp), 0);
        if (len <= 0) {
            rc = WERR_RECV_FROM_SERVICE;
        } else {
            finalTimestamp = time(NULL) + timeoffset_70s;

            /*
             * Localize words
             */
            resp.root_delay  = ntohl(resp.root_delay);
            resp.root_dispersion    = ntohl(resp.root_dispersion);
            resp.reftimestamphigh   = ntohl(resp.reftimestamphigh);
            resp.reftimestamplow    = ntohl(resp.reftimestamplow);
            resp.oritimestamphigh   = ntohl(resp.oritimestamphigh);
            resp.oritimestamplow    = ntohl(resp.oritimestamplow);
            resp.recvtimestamphigh  = ntohl(resp.recvtimestamphigh);
            resp.recvtimestamplow   = ntohl(resp.recvtimestamplow);
            resp.trantimestamphigh  = ntohl(resp.trantimestamphigh);
            resp.trantimestamplow   = ntohl(resp.trantimestamplow);

            /*
             * Correct the time difference.
             * T(client to server) = ((T2-T1)+(T3-T4))/2
             */
            difTime = ((resp.recvtimestamphigh-firstTimestamp)+(resp.trantimestamphigh-finalTimestamp))>>1;
            delayTime = ((resp.recvtimestamphigh-firstTimestamp)-(resp.trantimestamphigh-finalTimestamp))>>1;

            timeval tv;
            tv.tv_sec = time(NULL) + difTime + delayTime;
            tv.tv_usec = 0;

#if DEBUG
            ws_log(log::M_WEBSERVICE, log::VERBOSE) << "NTP Service dump:" << endlog;
            ws_log(log::M_WEBSERVICE, log::VERBOSE) << "\t time(NULL) = " << time(NULL) << endlog;
            ws_log(log::M_WEBSERVICE, log::VERBOSE) << "\t difference time = " << difTime << endlog;
            ws_log(log::M_WEBSERVICE, log::VERBOSE) << "\t delay time = " << delayTime << endlog;
            ws_log(log::M_WEBSERVICE, log::VERBOSE) << "\t tv1.tv_sec = " << tv.tv_sec << endlog;
#endif
            rc = WINF_SUCCEEDED;
        }
    }
    return WINF_SUCCEEDED;
}

int ServiceNtpImpl::disconnect() {
    socketClose(m_socket);
    return WINF_SUCCEEDED;
}

} //namespace openwsp {
