/** @file
 * OpenWSP - A Distributed Configuration management.
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
#include <openwsp/filestream.h>
#include <openwsp/string.h>
#include <openwsp/assert.h>

#include <new>

#include "log.h"
#include "config.h"

namespace openwsp {

/*******************************************************************************
*   Static constants                                                           *
*******************************************************************************/
const char *WSConfig::configFilename = "openwsp.conf";


WSConfig::WSConfig() :
#ifdef CONFIG_EXAMPLE
    mainWindowX(0),
    mainWindowY(0),
    mainWindowHeight(0),
    mainWindowWidth(0),
    streamCacheSize(0),
    streamCacheMinPercent(0),
    streamCacheSeekMinPercent(0)
#endif
    m_configElementNum(0)
{
    for (unsigned n = 0; n < CFG_ELEMS_COUNT; n++) {
        configElements[n] = 0;
    }
#if CONFIG_EXAMPLE
    int n = 0;
    
    /*
     * built-in configuration elements
     */
    configElements[n++] = new configElement("mainWindowX", 50, &mainWindowX);
    configElements[n++] = new configElement("mainWindowY", 50, &mainWindowY);
    configElements[n++] = new configElement("mainWindowHeight", 400, &mainWindowHeight);
    configElements[n++] = new configElement("mainWindowWidth", 800, &mainWindowWidth);
    configElements[n++] = new configElement("streamCacheSize", 320, &streamCacheSize);
    configElements[n++] = new configElement("streamCacheMinPercent", 20, &streamCacheMinPercent);
    configElements[n++] = new configElement("streamCacheSeekMinPercent", 50, &streamCacheSeekMinPercent);

    m_configElementNum = n;
#endif
}

WSConfig::~WSConfig() {
    for (unsigned n = 0; n < m_configElementNum; n++) {
        delete configElements[n];
    }
}

/**
 * Do the logical initialization.
 *
 * This will Read the config files and parse the parameters
 * in them. Them storage the parameters in RAM for accessing later.
 * @return Status code.
 */
int WSConfig::load() {
    int rc;
    Filestream fs;

    /*
     * load configurations from file
     */
    rc = fs.open(configFilename, "rb");
    if (WS_SUCCESS(rc)) {
        rc = parseConfig(fs);
        fs.close();
    }

    /*
     * if failed, load default parameters
     */
    if (WS_FAILURE(rc)) {
        rc = reserveDefaultValues();
    }

    return rc;
}

/**
 * Save all the settings.
 * It is to be modify the original config file.
 * @return status code
 */
int WSConfig::store() {
    int rc;
    Filestream streamRd;
    Filestream streamWr;

    /*
     * Open the configuration file to write
     */
    rc = streamRd.open(configFilename, "rb");

    if (WS_SUCCESS(rc)) {
        const char *origin = NULL;
        file_off len = 0;
        file_off filelen = streamRd.getSize();

        /*
         * Allocate a heap memory as buffer and then
         * Read the original file to store in the buffer
         */
        if (filelen) {
            origin = new (std::nothrow) char[filelen];
            if (!origin)
                return WERR_ALLOC_MEMORY;

            char *pos = const_cast<char*>(origin);
            for (;;) {
                len = streamRd.read(pos, filelen, 1);
                if (len <= 0) break;
                pos += len;
            }

            streamRd.close();

            rc = streamWr.open(configFilename, "wb");
            if (WS_SUCCESS(rc)) {
                rc = writeConfig(origin, filelen, streamWr);
                streamWr.close();
            }
            delete [] origin;

        } else {
            /* there is nothing we can read */
            rc = WERR_FAILED;
        }
    }

    /*
     * Append the default items if the config file is not
     * existing.
     */
    if (WS_FAILURE(rc)) {
        rc = streamWr.open(configFilename, "wb");
        if (WS_SUCCESS(rc)) {
            rc = writeDefaultValues(streamWr);
            streamWr.close();
        }
    }

    return rc;
}

/**
 * Register a configElements.
 * @param elem Pointer to the source elements. It will be deleted
 *             automatically when WSConfig is under destruction.
 * @return status code.
 */
int WSConfig::insert(configElement *elem) {
    WS_ASSERT(m_configElementNum + 1 < CFG_ELEMS_COUNT);

    configElements[m_configElementNum++] = elem;

    return WINF_SUCCEEDED;
}

/*
 * status of state machine
 */
enum parserStatus {
    SM_INITED = 0,
    SM_BEGIN_FIELD,
    SM_GOT_FIELD,
    SM_BEGIN_VALUE,
    SM_GOT_VALUE
};

/**
 * Inner, parse the config file
 * @param stream            Reference of filestream
 * @return Status code
 */
int WSConfig::parseConfig(Filestream &stream) {
    int rc = WINF_SUCCEEDED;
    const char *buff = NULL;
    file_off len = 0;
    file_off filelen = stream.getSize();

    if (filelen) {
        if (!(buff = new (std::nothrow) char[filelen])) {
            return WERR_ALLOC_MEMORY;
        }

        /*
         * Read the file to store in buffer
         */
        char *pos = const_cast<char*>(buff);
        for (;;) {
            len = stream.read(pos, filelen, 1);
            if (!len) break;
            pos += len;
        }

        /*
         * Reset all the valid bits in configElement.
         * it's used to resolve whether or not a element is set a value.
         */
        for (unsigned i = 0; i < m_configElementNum; i++) {
            configElements[i]->valid = false;
        }

        parserStatus    state;
        configElement   *elem;
        unsigned        valueBegin = 0;
        unsigned        valueEnd = 0;

        /*
         * Parse the tokens
         */
        state = SM_INITED;

        for (unsigned i = 0; i < filelen; i++) {
            switch (buff[i]) {
                case '#':
                    /*
                     * We consider a terminated line as a comment.
                     */
                    i = skipLine(buff, i, filelen) - 1;
                    break;
                case '.':
                    state = SM_BEGIN_FIELD;
                    break;
                case '=':
                    if (state == SM_GOT_FIELD) {
                        /*
                         * Skip the leading blanks before realy value.
                         */
                        valueBegin = skipSpaces(buff, i + 1, filelen);
                        i = valueBegin - 1;
                        state = SM_BEGIN_VALUE;
                        break;
                    } else {
                        // ignored
                        //rc = WERR_CFG_SYNTAX; // syntax error
                        break;
                    }

                /*
                 * State Machine for the parser
                 */
                default:
                    switch (state) {
                        /*
                         * To parse the field
                         */
                        case SM_BEGIN_FIELD: {
                            if (WS_FAILURE(findConfigElement(&buff[i], &elem)))
                                { elem = NULL; } // no matched
                            // then read the value and set it.
                            state = SM_GOT_FIELD;
                            break;
                        }
                        /*
                         * Having got the field
                         */
                        case SM_GOT_FIELD:
                            break;
                        /*
                         * To parse the value
                         */
                        case SM_BEGIN_VALUE: {
                            switch (isValueTokenEnd(buff, i, filelen)) {
                            case WINF_SKIP:
                                valueEnd = i;
                                continue;
                            case WINF_SUCCEEDED:
                                if (elem) {
                                setElementOutValue(elem, &buff[valueBegin], valueEnd-valueBegin+1);
                                }
                                state = SM_GOT_VALUE;
                            }
                            break;
                        }
                        /*
                         * Having got the value
                         */
                        case SM_GOT_VALUE:
                            break;
                        /*
                         * Root state
                         */
                        case SM_INITED:
                        default:
                            //error = true;
                            break;
                    }
            }

            if (WS_FAILURE(rc)) break; // for
        }
        delete [] buff;

    } else {
        /* there is nothing to read */
        rc = WINF_SUCCEEDED;
    }

    /*
     * Check for the default settings which are not specified by
     * the original
     */
    if (WS_SUCCESS(rc)) {
        rc = reserveDefaultValues();
    }

    return rc;
}

/**
 * Inner, reserve the default values
 */
int WSConfig::reserveDefaultValues() {
    for (unsigned i = 0; i < m_configElementNum; i++) {
    if (!configElements[i]->valid) {
        switch(configElements[i]->m_type) {
        // integer
        case CFG_TYPE_INTEGER:
            *(configElements[i]->m_val.pinteger) = configElements[i]->m_default.integer;
            configElements[i]->valid = true;
            break;
        // float
        case CFG_TYPE_FLOAT:
            *(configElements[i]->m_val.pfloat) = configElements[i]->m_default.floatv;
            configElements[i]->valid = true;
            break;
        // boolean
        case CFG_TYPE_BOOLEAN:
            *(configElements[i]->m_val.pboolean) = configElements[i]->m_default.boolen;
            configElements[i]->valid = true;
            break;
        // string
        case CFG_TYPE_STRING:
            configElements[i]->m_pbuffer = configElements[i]->m_default.string;
            *(configElements[i]->m_val.ppstring) = configElements[i]->m_default.string;
            configElements[i]->valid = true;
            break;
        default:
            WS_ASSERT(0);
            return WERR_FAILED;
        }
    }
    }
    return WINF_SUCCEEDED;
}

/**
 * Inner, Skip a line and return the next position.
 * @returns the next position absolute.
 * @param str           Pointer to the base of buffer.
 * @param offset        Offset from the base of buffer, specifying where to start.
 * @param maxlen        The length of buffer,count in bytes.
 */
unsigned WSConfig::skipLine(const char *buff, unsigned offset, unsigned maxlen) {
    for (unsigned n = offset; n < maxlen; n++) {
        if (buff[n] == '\r')
            continue;
        else if (buff[n] == '\n' ||
                 buff[n] == '\0') {
            return n;
        }
    }
    return maxlen-1;
}

/**
 * Inner, Skip spaces and return the next position.
 * @returns the next position absolute.
 * @param str           Pointer to the base of buffer.
 * @param offset        Offset from the base of buffer, specifying where to start.
 * @param maxlen        The length of buffer,count in bytes.
 */
unsigned WSConfig::skipSpaces(const char *buff, unsigned offset, unsigned maxlen) {
    for (unsigned n = offset; n < maxlen; n++) {
        if (buff[n] == ' ')
            continue; else {
            return n;
        }
    }
    return maxlen-1;
}

/**
 * Inner, Skip a value token and return the next position.
 * @returns S_SKIP if meet an meaningless character.
 * @returns S_OK if the token is terminated.
 * @param str           Pointer to the base of buffer.
 * @param pos           Specify where to start the judgment.
 * @param maxlen        The length of buffer,count in bytes.
 */
int WSConfig::isValueTokenEnd(const char *buff, unsigned pos, unsigned maxlen) {
    register unsigned nextpos = pos + 1;
    if ((buff[pos] == '\n' ||
         buff[pos] == '\0') ||
        ((nextpos < maxlen) &&
         (buff[nextpos] == '#')) ) { // terminated
        return WINF_SUCCEEDED;
    } else {
        return WINF_SKIP;
    }
}

/**
 * Inner, match the config element
 * @param ptr           The start position in buffer.
 * @param elem          Where to store the element data.
 * @return Status code
 */
int WSConfig::findConfigElement(const char *ptr, configElement **elem) {
    WS_ASSERT(ptr && elem);
    int n;
    configElement *e;

    // Resolve the config element
    for (n = 0; this->configElements[n]; n++) {
        e = this->configElements[n];
        if (!compareStringNocase(
                ptr,
                e->m_name,
                strlen(e->m_name))) {
            *elem = e;
            return WINF_SUCCEEDED;
        }
    }
    *elem = NULL;
    return WERR_FAILED;
}

/**
 * Inner, write the config file.
 * Note: It will CLOSE the streamOld before writing new one.
 *
 * @param origin            Pointer to the original buffer.
 * @param
 * @param streamNew         Reference of new file stream.
 * @return Status code
 */
int WSConfig::writeConfig(const char *origin, unsigned filelen, Filestream &streamNew) {
    int rc = WINF_SUCCEEDED;

    /*
     * Reset all the saved bits in configElement.
     * it's used to resolve whether or not a element is stored to disk.
     */
    for (unsigned i = 0; i < m_configElementNum; i++) {
        configElements[i]->saved = false;
    }

    char            buff[CFG_MAX_BUFF];
    int             bufflen = 0;
    parserStatus    state;
    configElement   *elem;
    unsigned        commentBegin = 0;
    unsigned        commentEnd = 0;
    unsigned        fieldBegin = 0;
    unsigned        fieldEnd = 0;
    unsigned        valueBegin = 0;
    unsigned        valueEnd = 0;

    /*
     * Parse the tokens
     */
    state = SM_INITED;

    for (unsigned i = 0; i < filelen; i++) {
        switch (origin[i]) {
        case '#':
            /*
             * We consider a terminated line as a comment.
             */
            commentBegin = i;
            commentEnd = skipLine(origin, i, filelen) /* - 1 */;
            i = commentEnd - 1;
            // make comment transparent
            rc = writeToStream(
                    streamNew,
                    &origin[commentBegin],
                    commentEnd - commentBegin /* + 1 */);
            break;
        case '.':
            fieldBegin = i;
            state = SM_BEGIN_FIELD;
            break;
        case '=':
            if (state == SM_GOT_FIELD) {
                /*
                 * Skip the the leading blanks before value
                 */
                valueBegin = skipSpaces(origin, i + 1, filelen);
                fieldEnd = valueBegin /*- 1*/;
                // make field transparent
                rc = writeToStream(
                        streamNew,
                        &origin[fieldBegin],
                        fieldEnd - fieldBegin /*+ 1*/);

                // then parse the value
                i = valueBegin - 1;
                state = SM_BEGIN_VALUE;
                break;
            } else {
                // ignored
                break;
            }

        /*
         * State Machine for the writer
         */
        default:
            switch (state) {
            /*
             * To parse the field
             */
            case SM_BEGIN_FIELD: {
                if (WS_FAILURE(findConfigElement(&origin[i], &elem)))
                    elem = NULL; // no matched
                state = SM_GOT_FIELD;
                break;
            }

            /*
             * Having got the field
             */
            //
            case SM_GOT_FIELD:
                break;

            /*
             * To parse a value
             */
            case SM_BEGIN_VALUE: {
                switch (isValueTokenEnd(origin, i, filelen)) {
                case WINF_SKIP:
                    valueEnd = i;
                    continue;
                case WINF_SUCCEEDED:
                    if (elem) {
                        // make value replaced
                        rc = getElementInValueString(elem, buff, sizeof(buff), &bufflen);
                        if (WS_SUCCESS(rc)) {
                            rc = writeToStream(
                                    streamNew,
                                    buff,
                                    bufflen - 1/* skip 'null' */);
                        }
                    } else {
                        // keep the original value
                        rc = writeToStream(
                                streamNew,
                                &origin[valueBegin],
                                valueEnd - valueBegin + 1);
                    }
                    state = SM_GOT_VALUE;
                } break;
            }

            /*
             * Having got the value
             */
            case SM_GOT_VALUE:
                break;

            /*
             * Root state
             */
            case SM_INITED:
            default:
                // ignored
                break;
            }

            /*
             * Process the control characters
             */
            switch (origin[i]) {
            case '\r':
                rc = writeToStream(streamNew, "\r", 1);
                break;
            case '\n':
                rc = writeToStream(streamNew, "\n", 1);
                break;
            }
        }

        if (WS_FAILURE(rc)) break; // for
    }

    /*
     * Write the default values which are not specified in original
     * file.
     */
    if (WS_SUCCESS(rc)) {
        rc = writeDefaultValues(streamNew);
    }

    return rc;
}

/**
 * Inner, Write a count of bytes to stream.
 * @return status code.
 * @param stream        Reference of target stream.
 * @param buff          Pointer to the buffer.
 * @param len           How many bytes to write.
 */
int WSConfig::writeToStream(Filestream &stream, const char *buff, unsigned len) {
    file_off total = 0;
    file_off off = 0;
    for (;;) {
        off = stream.write(static_cast<const void*>(buff), 1, len);
        total += off;
        if (off < len) {
            buff += off;
            continue;
        } else
            break;
        if (total > len)
            return WERR_WRITE_FILE; // buffer overriding
    }
    if (!total)
        return WERR_WRITE_FILE; else
        return WINF_SUCCEEDED;
}

/**
 * Inner, write the default values, which are not specified in original
 * config file.
 * @reutrn status code.
 */
int WSConfig::writeDefaultValues(Filestream &stream) {
    int rc = WINF_SUCCEEDED;
    int bufflen;
    char buff[CFG_MAX_BUFF];
    configElement *elem;
    bool first = true;

    for (unsigned i = 0; i < m_configElementNum; i++) {
        elem = configElements[i];
        if (!elem->saved) {
            rc = getElementInValueString(elem, buff, sizeof(buff), &bufflen);
            if (WS_SUCCESS(rc)) {
                // add a newline at the end of file
                if (first) {
                    rc = writeToStream(stream, "\n", 1); UPDATE_RC(rc);
                    first = false;
                }
                rc = writeToStream(stream, ".", 1);           UPDATE_RC(rc);
                rc = writeToStream(stream, elem->m_name, strlen(elem->m_name)); UPDATE_RC(rc);
                rc = writeToStream(stream, " = ", 3);           UPDATE_RC(rc);
                rc = writeToStream(stream, buff, bufflen - 1);  UPDATE_RC(rc);
                if (elem->m_comment) {
                rc = writeToStream(stream, " # ", 3);           UPDATE_RC(rc);
                rc = writeToStream(stream, elem->m_comment, 3); UPDATE_RC(rc);
                }
                rc = writeToStream(stream, "\n", 1);            UPDATE_RC(rc);
            } else
                break;
        }
    }
    return rc;
}

/**
 * Inner, get the value of variable in configElement.
 *
 * @return status code
 * @param elem          Pointer to the configElement instance.
 * @param buff          Pointer to the string buffer.
 * @param maxlen        Length of buffer.
 * @param outlen        Optional, where to store the length of output.
 */
int WSConfig::getElementInValueString(configElement *elem, char *buff, int maxlen, int *outlen) {
    int rc;
    WS_ASSERT(elem && buff && maxlen);
    switch (elem->m_type) {
    // convert integer
    case CFG_TYPE_INTEGER:
        rc = integerToString(*(elem->m_val.pinteger), 10, buff, maxlen, outlen);
        break;
    // convert float
    case CFG_TYPE_FLOAT:
        rc = floatToString(*(elem->m_val.pfloat), 10, CFG_FLOAT_PRECISION, buff, maxlen, outlen);
        break;
    // convert boolean
    case CFG_TYPE_BOOLEAN:
        rc = booleanToString(*(elem->m_val.pboolean), buff, maxlen, outlen);
        break;
    // convert string
    case CFG_TYPE_STRING: {
        std::size_t len = strlen(elem->m_pbuffer) + 1;
        memcpyLimited(buff, elem->m_pbuffer, len, maxlen);
        if (outlen)
            *outlen = len;
        rc = WINF_SUCCEEDED;
    }
        break;
    // invalid
    default:
        WS_ASSERT(0);
        rc = WERR_FAILED;
    }
    if (WS_SUCCESS(rc)) {
        elem->saved = true;
        return WINF_SUCCEEDED;
    } else
        return rc;
}

/**
 * Inner, set the value to variable specified in configElement.
 *
 * @return status code
 * @param elem          Pointer to the configElement instance.
 * @param str           Pointer to the string buffer.
 * @param len           Length of string.
 */
int WSConfig::setElementOutValue(configElement *elem, const char *str, unsigned len) {
    int rc;
    WS_ASSERT(elem && str);
    if (!len)
        return WERR_FAILED;

    switch (elem->m_type) {
    // convert integer
    case CFG_TYPE_INTEGER:
        rc = stringToInteger(str, len, elem->m_val.pinteger);
        break;
    // convert float
    case CFG_TYPE_FLOAT:
        rc = stringToFloat(str, len, elem->m_val.pfloat);
        break;
    // convert boolean
    case CFG_TYPE_BOOLEAN:
        rc = stringToBoolean(str, len, elem->m_val.pboolean);
        break;
    // convert string
    case CFG_TYPE_STRING:
        rc = stringDuplicateEx(str, len, const_cast<char **>(&(elem->m_pbuffer)));
        *(elem->m_val.ppstring) = elem->m_pbuffer;
        break;
    // invalid
    default:
        WS_ASSERT(0);
        rc = WERR_FAILED;
    }

    if (WS_SUCCESS(rc)) {
        elem->valid = true;
        return WINF_SUCCEEDED;
    } else {
        elem->valid = false;
        return WERR_FAILED;
    }
}

} // namespace openwsp
