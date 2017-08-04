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

#ifndef OPENWSP_TIMER_H_
#define OPENWSP_TIMER_H_

#include <openwsp/misc.h>

BEGIN_C_DECLS

const char *getTimerName();
unsigned int getTimer(void);
unsigned int getTimerMS(void);
int usecSleep(int usec_delay);

END_C_DECLS

#endif //!defined(OPENWSP_TIMER_H_)
