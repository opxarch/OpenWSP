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

#ifndef OPENWSP_FILESTREAM_H_
#define OPENWSP_FILESTREAM_H_

#include <stdio.h>
#include <openwsp/types.h>

namespace openwsp {

/**
 * File stream error code
 */
typedef enum StreamError {
    STREAM_ERR_INVALID = 0,
    STREAM_ERR_NO,
    STREAM_ERR_OPEN,
    STREAM_ERR_CLOSE,
    STREAM_ERR_READ,
    STREAM_ERR_WRITE,
    STREAM_ERR_SEEK,
    STREAM_ERR_TELL,
    STREAM_ERR_GETSIZE,
    STREAM_ERR_FLUSH
} StreamError;

/**
 File stream Seek mode
 */
typedef enum StreamSeekMode {
    STREAM_SEEK_SET, /**< Seek from the beginning of the file */
    STREAM_SEEK_CUR, /**< Seek from current position */
    STREAM_SEEK_END  /**< Seek from the end of the file */
} StreamSeekMode;


/***************************************************
  *****  File stream object                    *****
  ***************************************************/

class Filestream {
public:
    Filestream();
    ~Filestream();

    int Open(const char *filename, const char *mode);
    int Close();

    /* in/out */
    /* ................................................ */
    file_off Read(void *buffer, file_off size, file_off count);
    file_off Write(const void *buffer, file_off size, file_off count);
    int Seek(file_off pos, StreamSeekMode mode);
    file_off Tell();
    file_off GetSize();
    int Flush();

    /* error handle */
    /* ................................................ */
    StreamError GetError() const;

private:
    FILE *fd;
    StreamError lasterr;
};

}; // namespace openwsp

#endif //!defined(OPENWSP_FILESTREAM_H_)
