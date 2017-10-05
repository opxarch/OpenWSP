/** @file
 * Webservice - the webservice management.
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
#include <new>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <openwsp/javacore.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/filesystem.h>
#include <openwsp/crc.h>
#include <openwsp/apifile.h>
#include <openwsp/timer.h>
#include "stream/tcp.h"
#include "stream/stream.h"

#include <openwsp/misc.h>

#include <openwsp/webservice.h>

namespace openwsp {


/*******************************************************************************
*   Internal classes                                                           *
*******************************************************************************/

class moduleNode {
public:
    moduleNode(const char *name,
        const char *author,
        int major,
        int minor,
        int revise,
        const char *date,
        moduleNode *prev,
        moduleNode *next)
    {
        m_name = name ? strdup(name) : 0;
        m_author = author ? strdup(author) : 0;
        m_major = major;
        m_minor = minor;
        m_revise = revise;
        m_date = date ? strdup(date) : 0;
        m_code = 0;
        m_prev = prev;
        m_next = next;
    }

    ~moduleNode() {
        if (m_name) free(m_name);
        if (m_author) free(m_author);
        if (m_date) free(m_date);
        if (m_code) free(m_code);
    }

    int dupCode(const char *code) {
        m_code = strdup(code);
        return !m_code ?
                WERR_ALLOC_MEMORY
              : WINF_SUCCEEDED;
    }

public:
    char *m_name;
    char *m_author;
    int m_major;
    int m_minor;
    int m_revise;
    char *m_date;
    char *m_code;

    moduleNode *m_prev;
    moduleNode *m_next;
};

/*******************************************************************************
*   Internal functions                                                         *
*******************************************************************************/

static void ws_stream_constructor(jscContext *p);
static void ws_stream_recycle(jscContext *p, void *mem);
static void ws_stream_err(jscContext *p);
static void ws_stream_seek(jscContext *p);
static void ws_stream_read(jscContext *p);
static void ws_stream_tell(jscContext *p);
static void ws_util_constructor(jscContext *p);
static void ws_util_get_response_time(jscContext *p);
static void ws_debug_constructor(jscContext *p);
static void ws_debug_print(jscContext *p);
static void ws_debug_error(jscContext *p);
void api_register(jscContext *p);

#define DEBUG_WEBSERVICE 1

////////////////////////////////////////////////////////////////////////////////

Webservice::Webservice() {
    js = 0;
    stream = 0;
    module_registry = 0;
    module_loaded = false;
}

Webservice::~Webservice() {
}

/**
 * Do the logical initialization.
 * @return status code.
 */
int Webservice::init() {
    int rc = WERR_FAILED;

    /*
     initiate the javascript core
     */
    js = new (std::nothrow) Javacore();
    if (!js)
        return WERR_ALLOC_MEMORY;

    rc = js->init();
    if (WS_FAILURE(rc))
        return rc;

    js->setContext(this);

    //
    // stream
    //
    rc = js->prepareObject();
    if (WS_SUCCESS(rc)) {
        js->objectRegisterFunc("stream.err", ws_stream_err, 0);
        js->objectRegisterFunc("stream.seek", ws_stream_seek, 1);
        js->objectRegisterFunc("stream.read", ws_stream_read, 0);
        js->objectRegisterFunc("stream.tell", ws_stream_tell, 0);
        js->createObject("stream");
    } else
        return rc;

    //
    // Uitl
    //
    rc = js->prepareObject();
    if (WS_SUCCESS(rc)) {
        js->objectRegisterFunc("util.getResponseTime", ws_util_get_response_time, 1);
        js->createObject("util");
    } else
        return rc;

    //
    // Debug
    //
    rc = js->prepareObject();
    if (WS_SUCCESS(rc)) {
        js->objectRegisterFunc("debug.print", ws_debug_print, 1);
        js->objectRegisterFunc("debug.error", ws_debug_error, 1);
        js->createObject("debug");
    } else
        return rc;

    //
    // registering
    //
    js->registerFunc("register", api_register, 6);

    //
    // Global object
    //
    rc = js->prepareObject();
    if (WS_SUCCESS(rc)) {
        js->objectSetConstructor("stream", "stream", ws_stream_constructor, 1);
        js->objectSetConstructor("util", "util", ws_util_constructor, 0);
        js->objectSetConstructor("debug", "debug", ws_debug_constructor, 0);
        js->createGlobalObject("OpenWSP");
    } else
        return rc;

    /*
     initialize the root of registry.
     */
    module_registry = (void*) new (std::nothrow) moduleNode(
            0,0,0,0,0,0,0,0
            );
    if (!module_registry)
        return WERR_ALLOC_MEMORY;

    /*
     enumerate all the modules
     */
    rc = this->ResolveModules();
    if (WS_FAILURE(rc))
        return rc;

    return WINF_SUCCEEDED;
}

/**
 * uninitiate the class logically.
 * @return status code.
 */
int Webservice::uninit() {
    int rc = WERR_FAILED;

    if (js) {
        js->uninit();
        delete js;
        rc = WINF_SUCCEEDED;
    }

    moduleNode *node;
    moduleNode *prev;

    /*
     Delete all the modules loaded.
     */
    for (node = ((moduleNode*)module_registry);
            node;
            prev = node,
            node = node->m_next,
            delete prev) {
        /*NOP*/
    }

    return rc;
}

/**
 * Inner, parser and execute the module data
 * @param buff      Pointer to the buffer area. Be careful that
 *                  parsing will REVISE or REWRITE the data.
 * @param len       Length of buffer.
 * @param coffset   Where to store the offset of code page. It was
 *                  only a reference to the buff memory, so BE SURE
 *                  that the memory specified by buff is valid.
 * @param clen      Where to store then length of code page.
 * @return status code.
 */
int Webservice::parserExecModule(void *buff, size_t len, char **coffset, int *clen) {
    uint32_t codelen;
    char *codepage;
    apifile_t *api = (apifile_t *)buff;

    /*
     Validate the magic number
     */
    if (api->magic != NUMBER_MAGIC_APIFILE)
        return WERR_INVALID_FILE;

#if DEBUG_WEBSERVICE > 1
    std::cout
        <<"Module header dump:"<<std::endl
        <<" checksum = "<< api->codepage_checksum <<std::endl
        <<" codepage_offset = "<< api->codepage_offset <<std::endl
        <<" codepage_len = "<< api->codepage_len <<std::endl;
#endif

    /*
     Gain the code page
     */
    codepage = ((char*)buff) + api->codepage_offset;
    codelen = api->codepage_len;

    /*
     Checksum the code page
     */
    uint32_t checksum = crc32(codepage, codelen);
    if (checksum != api->codepage_checksum)
        return WERR_CHECKSUM_DATA;

    /*
     Execute the code
     */
    for (unsigned i = 0; i < codelen; i++)
        codepage[i] -= ASCII_BASE_OFFSET;
    *coffset = codepage;
    if (clen)
        *clen = codelen;
    return WINF_SUCCEEDED;
}

/**
 * Inner, execute the code of extensive modules.
 * @param fn filename of module.
 * @return status code
 */
int Webservice::resolveModule_inner(const char *fn) {
    int rc;
    int filetype;
    stream_t *stream;
    size_t len = 0;
    size_t total = 0;
    size_t bufsize = 0;
    size_t d = 0;
    char chunk[256];
    char *buff = 0;
    char *codepage;
    int codelen;

    stream = open_stream(fn, 0, &filetype);
    if (!stream)
        return WERR_OPEN_STREAM;

    /*
     Read and processing the module
     */
    for (;;) {
        len = stream_read(stream, chunk, sizeof(chunk));
        if (len<=0) {
            /* Parser the module data. */
            rc = parserExecModule(buff, total, &codepage, &codelen);
            if (WS_FAILURE(rc)) {
                free(buff);
                return rc;
            }

            /* Execute the code page */
            rc = execCode(codepage);
            free(buff);
            return WS_FAILURE(rc) ? WERR_RUN_CODE : rc;
        }
        /* reallocate buffer */
        total += len;
        if (bufsize < total) {
            bufsize = total + 256; // a block
            char *p = buff;
            buff = (char*)realloc(buff, bufsize);
            if (!buff) {
                if (p) free(p);
                return WERR_ALLOC_MEMORY;
            }
            memset(buff + d, 0, bufsize - d);
        }

        memcpy(buff + d, chunk, len);
        d += len;
    } // for
    return WERR_FAILED;
}

/**
 * Resolve all the extensive modules.
 * @return status code.
 */
int Webservice::ResolveModules() {
    int rc;
    char *          name;
    char            fullname[MAX_PATH_LEN];
    WSList<char *>  fls;

    /*
     ensure that we won't destroy the context of
     current module that has been loaded before.
     */
    if (module_loaded)
        return WERR_FAILED;

    /*
     enumerate all the api files
     */
    rc = enumFiles(WEBAPIS_PATH "/*" APIFILE_SUFFIX, &fls, 0);
    if (WS_SUCCESS(rc)) {

      for (fls.begin();
              fls.end();
              fls++)
      {
        rc = fls.get(name);
        if (WS_SUCCESS(rc))
        {
          std::cout<<"modules:"<<name<<std::endl;
          rc = joinFilename(WEBAPIS_PATH, name, fullname, sizeof(fullname), true);
          if (WS_SUCCESS(rc))
          {
            std::cout<<"path:"<<fullname<<" ... ";
            /*
             execute the module
             */
            rc = resolveModule_inner(fullname);
            if (WS_FAILURE(rc)) {
                std::cout<<"[ERR:"
                        <<GetErrorMsg(rc)->msgDefine
                        <<"]"
                        <<std::endl;
            } else
                std::cout<<"[DONE]"<<std::endl;
            continue;
          }
          return rc;
        }
      }
      fls.RemoveAllPtr();
    }
    return WINF_SUCCEEDED;
}

/**
 * Inner, execute the module code.
 * @param code Pointer to the code buffer.
 * @return status code
 */
int Webservice::execCode(const char *code) {
    int rc;

    js->cleanUp();
    rc = js->execCode(code);
    js->cleanUp();

    return rc;
}

/**
 * Load a module into memory context.
 * @param name The name of target module.
 * @return status code.
 */
int Webservice::LoadModule(const char *name) {
    int rc;
    bool found = false;
    moduleNode *node;
    /*
     Match the target
     */
    for (node = ((moduleNode*)module_registry);
            node;
            node = node->m_next) {
        if (node->m_name && !strcmp(node->m_name, name)) {
            found = true;
            break;
        }
    }

    /*
     Load the module found into memory
     */
    if (found && node->m_code) {
        js->cleanUp();
        rc = js->execCode(node->m_code);
        UPDATE_RC(rc);

        /* environment */
        rc = js->execCode("var __rc;");
        UPDATE_RC(rc);

        module_loaded = true;
        std::cout << "loaded module:" << node->m_name << std::endl;

        return WINF_SUCCEEDED;
    }

    return WERR_NO_MATCHED;
}

/**
 * Call connect() function in module script.
 * @return status code.
 */
int Webservice::connect() {
    bool jrc;
    int rc;

    if (!module_loaded)
        return WERR_FAILED;

    rc = js->execCode("__rc = wspAPI.connect();");
    if (WS_SUCCESS(rc)) {
        jscContext *ctx = js->getGlobal("__rc");
        jrc = ctx->toboolean(ctx, -1);
        ctx->pop(ctx, 1);

        return jrc ? WINF_SUCCEEDED : WERR_FAILED;
    }
    return WERR_RUN_CODE;
}

/**
 * Call catalogs() function in module script.
 * This will new some catalog objects, and you should delete
 * them explicitly.
 * @param out Pointer to a list to store the result.
 * @return status code.
 */
int Webservice::catalogs(WSList<catalog *> *out) {
    int rc;

    if (!module_loaded)
        return WERR_FAILED;

    rc = js->execCode(
            "__rc = wspAPI.catalogs();\n"
            ""
            );
    if (WS_SUCCESS(rc)) {
        jscContext *p = js->getGlobal("__rc");

        unsigned length = js->getLength(-1);

        int id;
        const char *name;

        for (unsigned i = 0; i < length; i++)
        {
            js->getIndex(-1, i);

            js->getProperty(-1, "name");
            name = p->tostring(p, -1);
            p->pop(p, 1);
            js->getProperty(-1, "id");
            id = p->tointeger(p, -1);

            p->pop(p, 2);

            catalog *c = new (std::nothrow) catalog(id, name);
            if (!c) {
                p->pop(p, 1);
                return WERR_ALLOC_MEMORY;
            }

            rc = out->Pushfront(c);
            if (WS_FAILURE(rc)) {
                p->pop(p, 1);
                return rc;
            }
        }

        p->pop(p, 1);

        return WINF_SUCCEEDED;
    }
    return WERR_RUN_CODE;
}

////////////////////////////////////////////////////////////////////////////////

//
// Stream
//

/**
 * Wrapper, Open a stream. (used in javascript)
 * @param filename The filename to open.
 * @return true operation succeeded.
 * @return false operation failed.
 */
static void ws_stream_constructor(jscContext *p) {
    int filetype;
    const char *fn;
    stream_t *stream;

    /* validate the parameter */
    if (p->isundefined(p, 1)) {
        return;
    }

    fn = p->tostring(p, 1);

    stream = open_stream(fn, 0, &filetype);
    p->setTag(p, "stream", "pstream", stream, ws_stream_recycle);
}

/**
 * Wrapper, Recycle the stream object. (used in javascript)
 */
static void ws_stream_recycle(jscContext *p, void *mem) {
    stream_t *s = (stream_t*)mem;
    if (s) {
        free_stream(s);
    }
}

/**
 * Wrapper, check if there is some errors in stream. (used in javascript)
 * @return true if error(s).
 * @return false if no.
 */
static void ws_stream_err(jscContext *p) {
    stream_t *stream = (stream_t*)p->getTag(p, "pstream");
    if (!stream)
        p->pushboolean(p, 1); // RETURN true
    else
        p->pushboolean(p, 0); // RETURN false
}

/**
 * Wrapper, Read all the STRING from the stream. (used in javascript)
 * @return string that has been read.
 * @return (undefined) if operation failed.
 */
static void ws_stream_read(jscContext *p) {
    size_t len = 0;
    size_t bufsize = 0;
    size_t total = 0;
    size_t d = 0;
    char chunk[256];
    char *buff = 0;

    stream_t *stream = (stream_t*)p->getTag(p, "pstream");
    if (!stream) {
        p->pushundefined(p); // RETURN (undefined)
        return;
    }

    /*
     * reading loop
     * NOTE: without the cache subsystem, some network
     * stream will be truncated, as a result, len != total.
     */
    for (;;) {
      /* get data from stream */
      len = stream_read(stream, chunk, sizeof(chunk));

      if (len<=0) {
          p->pushstring(p, buff); // RETURN res
          free(buff);
          break;
      }

      /* reallocate buffer */
      total += len;
      if (bufsize < total) {
          bufsize = total + 256; // a block
          char *f = buff;
          buff = (char*)realloc(buff, bufsize);
          if (!buff) {
              /* failed on memory allocating */
              if (f) free(f);
              p->pushundefined(p); // RETURN (undefined)
              break;
          }
          memset(buff + d, 0, bufsize - d);
      }

      // copy
      memcpy(buff + d, chunk, len);
      d += len;
    } // for
}

/**
 * Wrapper, seek the stream. (used in javascript)
 * @param pos Target position.
 * @return true if succeeded.
 * @return false if failed.
 */
static void ws_stream_seek(jscContext *p) {
    stream_t *stream = (stream_t*)p->getTag(p, "pstream");

    if (!stream ||
            p->isundefined(p, 1)) {
        p->pushboolean(p, 0); // RETURN false
        return;
    }

    int pos = p->tointeger(p, 1);
    if (stream_seek(stream, (off_t)pos))
        p->pushboolean(p, 1); // RETURN true
    else
        p->pushboolean(p, 0); // RETURN false
}

/**
 * Wrapper, tell the stream. (used in javascript)
 * @return Current position.
 */
static void ws_stream_tell(jscContext *p) {
    stream_t *stream = (stream_t*)p->getTag(p, "pstream");

    if (!stream) {
        p->pushnumber(p, 0); // RETURN 0
        return;
    }

    p->pushnumber(p, (double)stream_tell(stream)); // RETURN pos
}


////////////////////////////////////////////////////////////////////////////////


//
// Util
//

static void ws_util_constructor(jscContext *p) {
    p->setTag(p, "util", "util", 0);
}

/**
 * Network, Get the response time.
 * @param host IP or URL string of target host.
 * @param port Target connection port.
 * @return 0 if failed on connecting.
 * @return > 0 the time escaped (in us).
 */
static void ws_util_get_response_time(jscContext *p) {
    if (p->isundefined(p, 1)) {
        p->pushnumber(p, 0.0); // RETURN 0.0
        return;
    }

    const char *host = p->tostring(p, 1);
    int port = p->tointeger(p, 2);

    int fd;
    register unsigned int t0, t1;
    unsigned int dt;

    /* connect to the server and time the process */
    t0 = getTimer();
    fd = connectServer(host, port, 1);
    t1 = getTimer();

    if (!fd) {
        p->pushnumber(p, 0.0); // RETURN 0.0
        return;
    } else
        socketClose(fd);

    dt = t1 - t0;

    p->pushnumber(p, (double)dt); // RETURN dt
}


////////////////////////////////////////////////////////////////////////////////


//
// Debug
//

static void ws_debug_constructor(jscContext *p) {
    p->setTag(p, "debug", "debug", 0);
}

/**
 * Print a debugging message.
 * @param msg Message string.
 */
static void ws_debug_print(jscContext *p) {
    if (p->isundefined(p, 1)) {
        return;
    }

    const char *msg = p->tostring(p, 1);
    std::cout << "msg:" << msg << std::endl;
}

/**
 * Throw a error and terminate the script.
 * @param msg Message string.
 */
static void ws_debug_error(jscContext *p) {
    if (p->isundefined(p, 1)) {
        return;
    }

    const char *msg = p->tostring(p, 1);
    std::cout << "error:" << msg << std::endl;
    p->throwError(p, msg);
}


////////////////////////////////////////////////////////////////////////////////

//
// API register
//

void api_register(jscContext *p) {
    int rc;
    const char *name = p->tostring(p, 1);
    const char *author = p->tostring(p, 2);
    int major = p->tointeger(p, 3);
    int minor = p->tointeger(p, 4);
    int revise = p->tointeger(p, 5);
    const char *date = p->tostring(p, 6);

    Javacore &jsc = p->getJsc(p);
    Webservice *websrv = (Webservice*)(jsc.getContext());

#if DEBUG_WEBSERVICE
    std::cout<<"api_register():"<<std::endl
            <<" name = "<< name <<std::endl
            <<" author = "<< author <<std::endl
            <<" major = "<< major <<std::endl
            <<" minor = "<< minor <<std::endl
            <<" revise = "<< revise <<std::endl
            <<" date = "<< date <<std::endl;
#endif

    /*
     create a link-list node
     */
    moduleNode *prev = (moduleNode*)websrv->module_registry;
    moduleNode *node = new (std::nothrow) moduleNode(
                name,
                author,
                major,
                minor,
                revise,
                date,
                0 /*prev*/,
                prev /*next*/
            );
    if (!node) {
        p->throwError(p, "api_register(): allocating memory.");
        return;
    }

    const char *buff;
    rc = jsc.getCodeBuffer(&buff);

    /*
     link it to registry
     */
    if (WS_SUCCESS(rc)) {
        rc = node->dupCode(buff);

        if (WS_SUCCESS(rc)) {
            ((moduleNode*)websrv->module_registry)->m_prev = node;
            websrv->module_registry = node;
            return;
        }
    }

    p->throwError(p, "api_register(): create new module node in registry.");
}

} // namespace openwsp
