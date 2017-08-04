/** @file
 * openwsp basic file type
 */

/*
 *  OpenWSP (an opensource web stream player)
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
#ifndef OPENWSP_FILETYPE_H_
#define OPENWSP_FILETYPE_H_

//
// demuxers type
//
#define DEMUXER_TYPE_UNKNOWN 0
#define DEMUXER_TYPE_MPEG_ES 1
#define DEMUXER_TYPE_MPEG_PS 2
#define DEMUXER_TYPE_AVI 3
#define DEMUXER_TYPE_AVI_NI 4
#define DEMUXER_TYPE_AVI_NINI 5
#define DEMUXER_TYPE_ASF 6
#define DEMUXER_TYPE_MOV 7
#define DEMUXER_TYPE_VIVO 8
#define DEMUXER_TYPE_TV 9
#define DEMUXER_TYPE_FLI 10
#define DEMUXER_TYPE_REAL 11
#define DEMUXER_TYPE_Y4M 12
#define DEMUXER_TYPE_NUV 13
#define DEMUXER_TYPE_FILM 14
#define DEMUXER_TYPE_ROQ 15
#define DEMUXER_TYPE_MF 16
#define DEMUXER_TYPE_AUDIO 17
#define DEMUXER_TYPE_OGG 18
#define DEMUXER_TYPE_RAWAUDIO 20
#define DEMUXER_TYPE_RTP 21
#define DEMUXER_TYPE_RAWDV 22
#define DEMUXER_TYPE_PVA 23
#define DEMUXER_TYPE_SMJPEG 24
#define DEMUXER_TYPE_XMMS 25
#define DEMUXER_TYPE_RAWVIDEO 26
#define DEMUXER_TYPE_MPEG4_ES 27
#define DEMUXER_TYPE_GIF 28
#define DEMUXER_TYPE_MPEG_TS 29
#define DEMUXER_TYPE_H264_ES 30
#define DEMUXER_TYPE_MATROSKA 31
#define DEMUXER_TYPE_REALAUDIO 32
#define DEMUXER_TYPE_MPEG_TY 33
#define DEMUXER_TYPE_LMLM4 34
#define DEMUXER_TYPE_LAVF 35
#define DEMUXER_TYPE_NSV 36
#define DEMUXER_TYPE_VQF 37
#define DEMUXER_TYPE_AVS 38
#define DEMUXER_TYPE_AAC 39
#define DEMUXER_TYPE_MPC 40
#define DEMUXER_TYPE_MPEG_PES 41
#define DEMUXER_TYPE_MPEG_GXF 42
#define DEMUXER_TYPE_NUT 43
#define DEMUXER_TYPE_LAVF_PREFERRED 44

//
// This should always match the higest demuxer type number.
// Unless you want to disallow users to force the demuxer to some types
//
#define DEMUXER_TYPE_MIN 0
#define DEMUXER_TYPE_MAX 44

#define DEMUXER_TYPE_DEMUXERS (1<<16)
// A virtual demuxer type for the network code
#define DEMUXER_TYPE_PLAYLIST (2<<16)

#endif //!defined(OPENWSP_FILETYPE_H_)
