/** @file
 * Openwsp file stream wrapper.
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
#include <openwsp/err.h>
#include <openwsp/filestream.h>

namespace openwsp {

Filestream::Filestream() :
    fd (NULL),
    lasterr (STREAM_ERR_INVALID) {
}

Filestream::~Filestream() {
}

/**
 * Open a file stream.
 *
 * @returns status code.
 * @param filename  Path name of file to open.
 * @param mode      Open mode. followed the fopen() standard.
 *                  Syntax: <a|r|w>[+][b|t].
 */
int Filestream::Open(const char *filename, const char *mode) {
    if (fd)
        return WERR_FILESTREAM_HAS_BEEN_OPENED;

    fd = fopen(filename, mode);

    if (!fd || ferror(fd))
        lasterr = STREAM_ERR_OPEN;
    if (fd)
        return WINF_SUCCEEDED; else
        return WERR_FAILED; //@todo map the errcode to rc.
}

/**
 * Close the opened file.
 * @returns status code
 */

int Filestream::Close() {
    int nerr = 1;
    if (fd)
        nerr = fclose(fd);
    if (nerr) {
        fd = NULL;
        lasterr = STREAM_ERR_CLOSE;
        return WERR_FAILED;
    } else
        return WINF_SUCCEEDED; //@todo map the errcode to rc.
}

/**
 * Read a count of bytes to the buffer specified.
 *
 * @returns The number of bytes read, if success.
 * @returns 0 if failed.
 * @param buffer            Pointer to the target buffer.
 * @param size              The size of elements.
 * @param count             The count of elements.So the valid size
 *                          should be size * count in bytes.
 */
file_off Filestream::Read(void *buffer, file_off size, file_off count) {
    file_off len = 0;
    if (fd) {
        //@todo fixme later.
        len = static_cast<file_off>(
                fread(buffer,
                        static_cast<size_t>(size),
                        static_cast<size_t>(count), fd) );

        if (!len)
            lasterr = STREAM_ERR_READ;
    }

    return len;
}

/**
 * Write a count of bytes that is from the buffer specified.
 *
 * @returns The number of bytes written, if success.
 * @returns 0 if failed.
 * @param buffer            Pointer to the source buffer.
 * @param size              The size of elements.
 * @param count             The count of elements.So the valid size
 *                          should be size * count in bytes.
 */
file_off Filestream::Write(const void *buffer, file_off size, file_off count) {
    file_off len = 0;
    if (fd) {
        //@todo fixme later.
        len = static_cast<file_off>(
                fwrite(buffer,
                        static_cast<size_t>(size),
                        static_cast<size_t>(count), fd) );
        if (!len)
            lasterr = STREAM_ERR_WRITE;
    }

    return len;
}

/**
 * Set reading/writing offset position.
 *
 * @return status code.
 * @param pos    The new position.
 * @param mode   Mode of seeking.
 */
int Filestream::Seek(file_off pos, StreamSeekMode mode) {
    int rc = WERR_FAILED;
    static const int mapMode[] = {
        SEEK_SET, // STREAM_SEEK_SET
        SEEK_CUR, // STREAM_SEEK_CUR
        SEEK_END  // STREAM_SEEK_END
    };

    if (fd) {
        rc = fseek(fd, static_cast<long>(pos), mapMode[mode])<0 ? WERR_FAILED:WINF_SUCCEEDED;
        if (ferror(fd))
            lasterr = STREAM_ERR_SEEK;
    }
    return rc;
}

/**
 * Tell the current position.
 * @return the result.
 */
file_off Filestream::Tell() {
    file_off offset = 0;
    if (fd) {
        offset = static_cast<file_off>(ftell(fd));
        if (ferror(fd))
            lasterr = STREAM_ERR_TELL;
    }
    return offset;
}

/**
 * Get the size of file.
 * @return the result.
 */
file_off Filestream::GetSize() {
    file_off len = 0;

    if (fd) {
        //@todo fixme later.
        size_t lastPos = (size_t)ftell(fd);
        fseek(fd, 0, SEEK_END);
        len = (file_off)ftell(fd);
        fseek(fd, lastPos, SEEK_SET);

        if (ferror(fd))
            lasterr = STREAM_ERR_GETSIZE;
    }

    return len;
}

/**
 * Flush the stream.
 * @return status code.
 */
int Filestream::Flush() {
    int rc = WERR_FAILED;
    if (fd) {
        rc = fflush(fd)<0 ? WERR_FAILED:WINF_SUCCEEDED;
        if (ferror(fd))
            lasterr = STREAM_ERR_FLUSH;
    }
    return rc;
}

/**
 * Get the last error code.
 * @return the result
 */
StreamError Filestream::GetError() const {
    return lasterr;
}

}; // namespace openwsp
