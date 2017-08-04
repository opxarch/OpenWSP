/** @file
 * openwsp Error code Management.
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

#ifndef OPENWSP_ERR_H_
#define OPENWSP_ERR_H_

/* gen{{ */

/** Operation is terminated. */
#define WINF_TERMINATED (3)
/** Skip an operation. */
#define WINF_SKIP (2)
/** Operation was succeeded. */
#define WINF_SUCCEEDED (1)
/** Operation was failed. */
#define WERR_FAILED (0)
/** Failed to allocate the memory. */
#define WERR_ALLOC_MEMORY (-1)
/** Buffer overflow. */
#define WERR_BUFFER_OVERFLOW (-2)
/** Internal logical error. */
#define WERR_INTERNAL_LOGIC (-3)
/** A list In iterating mode. */
#define WERR_ITERATING (-4)
/** Type mismatch. */
#define WERR_TYPE_MISMATCH (-5)
/** File stream is already opened. */
#define WERR_FILESTREAM_HAS_BEEN_OPENED (-6)
/** Failed to enable stream cache. */
#define WERR_STREAM_ENABLE_CACHE (-7)
/** Failed to open stream. */
#define WERR_OPEN_STREAM (-8)
/** Failed to open demuxer. */
#define WERR_OPEN_DEMUXER (-9)
/** Failed to initiate the best audio codec. */
#define WERR_INIT_BEST_AUDIO_CODEC (-10)
/** Failed to initiate audio devices. */
#define WERR_INIT_AUDIO_DEVICE (-11)
/** File is invalid. */
#define WERR_INVALID_FILE (-12)
/** Failed on file checksum. */
#define WERR_CHECKSUM_DATA (-13)
/** Failed on running code. */
#define WERR_RUN_CODE (-14)
/** No item matched. */
#define WERR_NO_MATCHED (-15)
/** Thread exists. */
#define WERR_THREAD_EXISTS (-16)
/** The target is invalid. */
#define WERR_INVALID_TARGET (-17)
/** The object is a playlist */
#define WERR_PLAYLIST (-18)

/* }}gen */

#define WS_SUCCESS(rc) (rc>0)
#define WS_FAILURE(rc) (rc<1)

#define UPDATE_RC(rc) do {if WS_FAILURE(rc) return rc;} while(0)

#define AssertRC(rc) do {WS_ASSERT(WS_SUCCESS(rc)); } while(0)

/*
 Structs
 */
typedef struct ErrorInfo {
    const char *msgShort;  /**< Pointer to the short message string. */
    const char *msgFull;   /**< Pointer to the full message string. */
    const char *msgDefine; /**< Pointer to the define string. */
    int        code;       /**< Status code number. */
} ErrorInfo;


/*
 funcs
 */
const ErrorInfo* GetErrorMsg(int rc);


#endif //!defined(OPENWSP_ERR_H_)
