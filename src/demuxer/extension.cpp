/** @file
 * Openwsp - file extension.
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

#include <openwsp/misc.h>
#include "../log.h"
#include "stream/stream.h"
#include "demuxer.h"

namespace openwsp {

////////////////////////////////////////////////////////////////////////////////


/*
 * An autodetection based on the extension is not a good idea, but we don't care ;-)
 */
static struct {
        const char *extension;
        int demuxer_type;
} extensions_table[] = {
//        { "mpeg", DEMUXER_TYPE_MPEG_PS },
//        { "mpg", DEMUXER_TYPE_MPEG_PS },
//        { "mpe", DEMUXER_TYPE_MPEG_PS },
        { "vob", DEMUXER_TYPE_MPEG_PS },
        { "m2v", DEMUXER_TYPE_MPEG_PS },
        { "avi", DEMUXER_TYPE_AVI },
        { "mp4", DEMUXER_TYPE_MOV },
        { "mov", DEMUXER_TYPE_MOV },
        { "qt", DEMUXER_TYPE_MOV },
        { "asx", DEMUXER_TYPE_ASF },
        { "asf", DEMUXER_TYPE_ASF },
        { "wmv", DEMUXER_TYPE_ASF },
        { "wma", DEMUXER_TYPE_ASF },
        { "viv", DEMUXER_TYPE_VIVO },
        { "vivo", DEMUXER_TYPE_VIVO },
        { "rm", DEMUXER_TYPE_REAL },
        { "rmvb", DEMUXER_TYPE_REAL },
        { "ra", DEMUXER_TYPE_REAL },
        { "y4m", DEMUXER_TYPE_Y4M },
        { "mp3", DEMUXER_TYPE_AUDIO },
        { "wav", DEMUXER_TYPE_AUDIO },
        { "flac", DEMUXER_TYPE_AUDIO },
        { "fla", DEMUXER_TYPE_AUDIO },
        { "ogg", DEMUXER_TYPE_OGG },
        { "ogm", DEMUXER_TYPE_OGG },
//        { "pls", DEMUXER_TYPE_PLAYLIST },
//        { "m3u", DEMUXER_TYPE_PLAYLIST },
        { "xm", DEMUXER_TYPE_XMMS },
        { "mod", DEMUXER_TYPE_XMMS },
        { "s3m", DEMUXER_TYPE_XMMS },
        { "it", DEMUXER_TYPE_XMMS },
        { "mid", DEMUXER_TYPE_XMMS },
        { "midi", DEMUXER_TYPE_XMMS },
        { "vqf", DEMUXER_TYPE_XMMS },
        { "nsv", DEMUXER_TYPE_NSV },
        { "nsa", DEMUXER_TYPE_NSV },
        { "mpc", DEMUXER_TYPE_MPC },
#ifdef USE_WIN32DLL
        { "avs", DEMUXER_TYPE_AVS },
#endif
	{ "302", DEMUXER_TYPE_LAVF },
        { "264", DEMUXER_TYPE_H264_ES },
        { "26l", DEMUXER_TYPE_H264_ES },
	{ "ac3", DEMUXER_TYPE_LAVF },
        { "ape", DEMUXER_TYPE_LAVF },
        { "apl", DEMUXER_TYPE_LAVF },
        { "mac", DEMUXER_TYPE_LAVF },

// At least the following are hacks against broken autodetection
// that should not be there

};

int demuxer_type_by_filename(char* filename){
  unsigned i;
  char* extension=strrchr(filename,'.');
  ws_log(log::M_OPEN, log::VERBOSE)
      << "Searching demuxer type for filename " << filename << " ext: " << extension
      << endlog;
  if(extension) {
    ++extension;
//    ws_log(MSGT_CPLAYER,MSGL_DBG2,"Extension: %s\n", extension );
    // Look for the extension in the extensions table
    for( i=0 ; i<(sizeof(extensions_table)/sizeof(extensions_table[0])) ; i++ ) {
      if( !strcasecmp(extension, extensions_table[i].extension) ) {
        ws_log(log::M_OPEN, log::VERBOSE) << "Trying demuxer " << extensions_table[i].demuxer_type << " based on filename extension" << endlog;
        return extensions_table[i].demuxer_type;
      }
    }
  }
  return DEMUXER_TYPE_UNKNOWN;
}

} // namespace openwsp
