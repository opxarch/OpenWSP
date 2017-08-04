/** @file
 * Util - filesystem.
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
#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/list.h>
#include <openwsp/string.h>
#include <openwsp/filesystem.h>

#if OS(WIN32)
# include <Windows.h>
#endif

/**
 * Enumerate all the files on filesystem by its suffix
 *
 * @param suffix        The path and suffix string.
 * @param files         Where to store the list of filenames, type is at
 *                      openwsp::List<char *>. When it becomes unused,
 *                      we should release all the memory to which pointers
 *                      in the list points to avoid leaking.
 * @param count         Optional, Where to store the count number.
 * @return status code.
 */
int enumFiles(const char *suffix, openwsp::WSList<char *> *files, int *count) {
    WS_ASSERT(suffix && files);
    int files_count = 0;
#if OS(WIN32)
    int rc = WERR_FAILED;
    char *buff;
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(suffix, &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // is it a file?
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                // link it to the list
                rc = openwsp::stringDuplicate(fd.cFileName, 0, &buff);
                if (WS_SUCCESS(rc)) {
                    files->Pushfront(buff);
                    files_count++;
                } else
                    return rc; // break unexpectedly
            }
        } while (::FindNextFile(hFind, &fd));

        ::FindClose(hFind);
    }
#else // OS(WIN32)
#error port me!
#endif
    if (count) {
        *count = files_count;
    }
    return WINF_SUCCEEDED;
}

/**
 * Join the path and filename string.
 *
 * @param path          Pointer to path string.
 * @param name          Pointer to name string.
 * @param out           Where to store the output.
 * @param maxlen        The maximum length of output buffer.
 * @param unix          Using unix style.
 * @return status code.
 */
int joinFilename(const char *path, const char *name, char *out, size_t maxlen, bool unix) {
    size_t len0 = strlen(path), len1 = strlen(name);
    if ( (len0 + len1 + 2) > maxlen)
        return WERR_BUFFER_OVERFLOW;
    memcpy(out, path, len0);
    out += len0;
    out[0] = unix ? '/' : '\\';
    out++;
    memcpy(out, name, len1);
    out += len1;
    out[0] = '\0';
    return WINF_SUCCEEDED;
}
