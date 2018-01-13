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

#ifndef CONFIG_H_
#define CONFIG_H_

#define CFG_ELEMS_COUNT 255
#define CFG_MAX_BUFF 8192
#define CFG_FLOAT_PRECISION 1000000L

#include <openwsp/string.h>

namespace openwsp {

class Filestream;

/*
 * Config element type
 */
typedef enum configType {
    CFG_TYPE_INVALID = 0,
    CFG_TYPE_INTEGER,
    CFG_TYPE_FLOAT,
    CFG_TYPE_BOOLEAN,
    CFG_TYPE_STRING
} configType;

/*
 * Config element descriptor
 */
class configElement {
public:
    /*
     constructor for integer type
     */
    configElement(const char *name, int defval, int *intout, const char *comment = 0) {
        m_name = name;
        m_comment = comment;
        m_type = CFG_TYPE_INTEGER;
        m_default.integer = defval;
        m_val.pinteger = intout;
        m_pbuffer = 0;
        valid = false;
        saved = false;
    }
    /*
     constructor for float type
     */
    configElement(const char *name, float defval, float *fltout, const char *comment = 0) {
        m_name = name;
        m_comment = comment;
        m_type = CFG_TYPE_FLOAT;
        m_default.floatv = defval;
        m_val.pfloat = fltout;
        m_pbuffer = 0;
        valid = false;
        saved = false;
    }
    /*
     constructor for boolen type
     */
    configElement(const char *name, bool defval, bool *boolout, const char *comment = 0) {
        m_name = name;
        m_comment = comment;
        m_type = CFG_TYPE_BOOLEAN;
        m_default.boolen = defval;
        m_val.pboolean = boolout;
        m_pbuffer = 0;
        valid = false;
        saved = false;
    }
    /*
     constructor for string type
     */
    configElement(const char *name, const char *defval, const char **strout, const char *comment = 0) {
        m_name = name;
        m_comment = comment;
        m_type = CFG_TYPE_STRING;
        m_default.string = stringDuplicate(defval);
        m_val.ppstring = strout;
        m_pbuffer = 0;
        valid = false;
        saved = false;
    }

    /*
     Release the memory of value
     */
    void eraseValue() {
        if (m_type == CFG_TYPE_STRING) {
            delete [] m_default.string;
            delete [] m_pbuffer;

            m_default.string = 0;
            m_pbuffer = 0;
        }
    }

    ~configElement() {
        eraseValue();
    }

public:
    bool valid;
    bool saved;
    const char *m_name;
    const char *m_comment;
    configType  m_type;

    /** variant values */
    union {
        int         integer;
        float       floatv;
        bool        boolen;
        const char *string;
    } m_default;

    union {
        int         *pinteger;
        float       *pfloat;
        bool        *pboolean;
        const char  **ppstring;
    } m_val;

    const char  *m_pbuffer;
};


/***************************************************
  *****  Global configuration object           *****
  ***************************************************/

class WSConfig {
public:
    WSConfig();
    ~WSConfig();

    int load();
    int store();

    /* ................................................ */

    int insert(configElement *elem);

public:
    /* static items */
    /* ................................................ */
    static const char *configFilename;
    static const char *configBackupFilename;

    /* dynamic items */
    /* ................................................ */
#ifdef CONFIG_EXAMPLE
    int mainWindowX;
    int mainWindowY;
    int mainWindowHeight;
    int mainWindowWidth;
    int streamCacheSize;
    int streamCacheMinPercent;
    int streamCacheSeekMinPercent;
#endif

    unsigned m_configElementNum;

protected:
    /* ................................................ */

    configElement *configElements[CFG_ELEMS_COUNT];

    /* ................................................ */
    int parseConfig(Filestream &stream);
    int writeConfig(const char *origin, unsigned filelen, Filestream &streamNew);
    unsigned skipLine(const char *buff, unsigned offset, unsigned maxlen);
    unsigned skipSpaces(const char *buff, unsigned offset, unsigned maxlen);
    int isValueTokenEnd(const char *buff, unsigned pos, unsigned maxlen);
    int reserveDefaultValues();
    int findConfigElement(const char *ptr, configElement **elem);
    int writeToStream(Filestream &stream, const char *buff, unsigned len);
    int writeDefaultValues(Filestream &stream);
    int getElementInValueString(configElement *elem, char *buff, int maxlen, int *outlen);
    int setElementOutValue(configElement *elem, const char *str, unsigned len);
};

#define CONFIG_VAR(typ, varname)    typ varname
#define CONFIG_GET(var)             (var)
#define CONFIG_SET(var, val)        var = val


}; // namespace openwsp

#endif //!defined(CONFIG_H_)
