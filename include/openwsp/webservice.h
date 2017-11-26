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

#include <string>
#include <openwsp/list.h>

namespace openwsp {

class Javacore;
class jscContext;
class catalog;
class channel;
class channelURL;
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
    int channels(int id, WSList<channel *> *out);
    int queryChannelInfo(int id, channel **out);
    int queryChannelURL(int id, channelURL **out);

private:
    Javacore    *js;
    stream_s    *stream;
    void        *module_registry;

    bool         module_loaded;

private:
    int resolveModule_inner(const char *fn);
    int parserExecModule(void *buff, size_t len, char **coffset, int *clen);
    int execCode(const char *code);

    int channelInner(jscContext *p, channel **ch);

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
        const char *name, const char *desc, const char *catalog)
    {
        m_id = id;
        m_name = name ? name : "unknown";
        m_desc = desc ? desc : "unknown";
        m_catalog = catalog ? catalog : "unknown";
    }

    ~catalog() {
    }

public:
    int m_id;
    std::string m_name;
    std::string m_desc;
    std::string m_catalog;
};

/**
 * Channels data fields
 */
class channel {
public:
    channel(int id,
        const char *name, const char *desc,
        const char *thumb_small, const char *thumb_medium, const char *thumb_large
        )
    {
        m_id = id;
        m_name = name ? name : "unknown";
        m_desc = desc ? desc : "unknown";

        m_thumb_small = thumb_small ? thumb_small : "";
        m_thumb_medium = thumb_medium ? thumb_medium : "";
        m_thumb_large = thumb_large ? thumb_large : "";
    }

    ~channel() {
        m_mediainfo.RemoveAllPtr();
        m_properties.RemoveAllPtr();
    }

    /**
     * Media information fields
     */
    class mediainfo {
    public:
        mediainfo (int mid)
            :m_mid(mid)
        {}

        int m_mid;
    };

    /*
     * extra properties fields
     */
    class exproperty {
    public:
        exproperty(const char *name, const char *desc, const char *value) {
            m_name = name ? name : "(unknown)";
            m_desc = desc ? desc : "(unknown)";
            m_value = value ? value : "(unset)";
        }
    public:
        std::string m_name;
        std::string m_desc;
        std::string m_value;
    };

public:
    /*! the media info pushed to the list will be
     * deleted when this class is deconstructed.
     */
    int pushMediaInfo(const channel::mediainfo *mediainfo) {
        return m_mediainfo.Pushfront(mediainfo);
    }

    /*! the property pushed to the list will be
     * deleted when this class is deconstructed.
     */
    int pushProperty(const channel::exproperty *property) {
        return m_properties.Pushfront(property);
    }

public:
    int m_id;
    std::string m_name;
    std::string m_desc;
    std::string m_thumb_small;
    std::string m_thumb_medium;
    std::string m_thumb_large;
    WSList<const channel::mediainfo *> m_mediainfo;
    WSList<const channel::exproperty *> m_properties;
};


/**
 * Channels data fields
 */
class channelURL {
public:
    channelURL(int id, const char *url)
        : m_id(id),
          m_url(url ? url : "") {
    }

public:
    int m_id;
    std::string m_url;
};

} // namespace openwsp

#endif //!defined(OPENWSP_WEBSERVICE_H_)
