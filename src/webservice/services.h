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

#ifndef WEBSERVICE_SERVICES_H_
#define WEBSERVICE_SERVICES_H_

#include "serviceBase.h"

namespace openwsp {

class ServiceNtpImpl : public ServiceBase {
public:
    int init();
    int uninit();

    const char *getname();
    const char *getshortname();

    int connect();
    int process();
    int disconnect();

private:
    int m_socket;
};

} // namespace openwsp

#endif //!defined(WEBSERVICE_SERVICES_H_)
