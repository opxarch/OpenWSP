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

#ifndef WEBSERVICE_SERVICEBASE_H_
#define WEBSERVICE_SERVICEBASE_H_

namespace openwsp {

class ServiceBase {
public:
    virtual ~ServiceBase() {}
    virtual int init() = 0;
    virtual int uninit() = 0;

    virtual const char *getname() = 0;
    virtual const char *getshortname() = 0;

    virtual int connect() = 0;
    virtual int process() = 0;
    virtual int disconnect() = 0;
};

} // namespace openwsp

#endif //!defined(WEBSERVICE_SERVICEBASE_H_)
