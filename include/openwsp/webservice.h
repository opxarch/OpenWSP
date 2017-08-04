/*
 *  OpenWSP (an opensource web stream player)
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

#ifndef OPENWSP_WEBSERVICE_H_
#define OPENWSP_WEBSERVICE_H_

#include <string.h>
#include <openwsp/list.h>

namespace openwsp {

class Javacore;
class jscContext;
class catalog;
struct stream_s;

/***************************************************
  *****       Webservice object                *****
  ***************************************************/

class Webservice {
public:
    Webservice();
    ~Webservice();

    int init();
    int uninit();
    
    int ResolveModules();
    int LoadModule(const char *name);
    
    int connect();
    int catalogs(WSList<catalog *> *out);

private:
    Javacore    *js;
    stream_s    *stream;
    void        *module_registry;

    bool         module_loaded;

private:
    int resolveModule_inner(const char *fn);
    int parserExecModule(void *buff, size_t len, char **coffset, int *clen);
    int execCode(const char *code);

    friend void api_register(jscContext *);
#if 0
    friend void ws_stream_open(jscContext *);
    friend void ws_sream_close(jscContext *);
    friend void ws_stream_read(jscContext *);
#endif
};

/**
 * Catalog data fields
 */
class catalog {
public:
    catalog(int id,
        const char *name) {
        m_id = id;
        m_name = name ? strdup(name) : 0;
    }

    ~catalog() {
        if (m_name)
            free(m_name);
    }

public:
    int m_id;
    char *m_name;
};

} // namespace openwsp

#endif //!defined(OPENWSP_WEBSERVICE_H_)
