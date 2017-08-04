/** @file
 * OpenWSP - global configuration management.
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
#include "config.h"
int CFG_stream_cache_size = 320;
int CFG_stream_cache_min_percent = 20;
int CFG_stream_cache_seek_min_percent = 50;

int init_app_config(void) {
    CFG_stream_cache_size = 320;
    CFG_stream_cache_min_percent = 20;
    CFG_stream_cache_seek_min_percent = 50;
    return WINF_SUCCEEDED;
}
