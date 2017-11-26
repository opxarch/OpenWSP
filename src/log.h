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

#ifndef OPENWSP_LOG_H_
#define OPENWSP_LOG_H_

#include <iostream>
#if 0
# include <typeinfo>
#endif

#include <openwsp/misc.h>
#include <openwsp/tracker.h>

#include "localize/msgdata.h"



namespace openwsp {

/*
 * verbosity elevel:
 *
 * Only messages level MSGL_FATAL-MSGL_STATUS should be translated,
 * messages level MSGL_V and above should not be translated.
 * */

namespace log {

enum level {
    FATAL = -5,  // will exit/abort
    ERR = -4,    // continues
    WARN = -3,   // only warning
    HINT = -2,   // short help message
    INFO = -1,   // -quiet
    STATUS = 0,  // v=0
    VERBOSE, // v=1
    DBG2,    // v=2
    DBG3,    // v=3
    DBG4,    // v=4
    DBG5,    // v=5
    FIXME    // V=6
};

/*
 * code/module:
 */
enum module {
    M_DEFAULT = 0,  // common player stuff errors
    M_UTIL,
    M_WEBSERVICE,
    M_JAVACORE,
    M_GUI,
    M_DSP,

    M_AUDIOSYS,     // audiosys

    M_DEMUXER,    // demuxer (general stuff)
    M_DS,         // demux stream (add/read packet etc)
    M_DEMUX,      // fileformat-specific stuff
    M_HEADER,     // fileformat-specific header

    M_DECAUDIO,  // av decoder

    M_OPEN,    // open stream
    M_SEEK,    // seeking code
    M_WIN32,   // win32 dll stuff

    M_STREAM,  // stream
    M_CACHE,   // cache sub-system

    M_OSDEP,   // OS-dependent parts

    M_NETWORK,

    M_IDENTIFY, // -identify output
    M_MAX
};

} // namespace log


enum endlog_p {
    endlog_0
};


/***************************************************
  *****           Logging object               *****
  ***************************************************/

class ws_log {
public:
    ws_log();
    ws_log(log::level level);
    ws_log(log::module module);
    ws_log(log::module module, log::level level);
    ~ws_log();

#if 0
#define LOG_FMT(fmt, val) do { write(m_module, m_level, fmt, val); } while(0)

   template<class RHT>
    const ws_log& operator<<(RHT value) const {
       if (typeid(RHT) == typeid(char))
           LOG_FMT("%c", value);
       else if (typeid(RHT) == typeid(unsigned char))
           LOG_FMT("%u", (unsigned)value);
       else if (typeid(RHT) == typeid(short))
           LOG_FMT("%d", (int)value);
       else if (typeid(RHT) == typeid(unsigned short))
           LOG_FMT("%u", (unsigned)value);
       else if (typeid(RHT) == typeid(int))
           LOG_FMT("%d", value);
       else if (typeid(RHT) == typeid(unsigned int))
           LOG_FMT("%u", value);
       else if (typeid(RHT) == typeid(long))
           LOG_FMT("%ld", value);
       else if (typeid(RHT) == typeid(unsigned long))
           LOG_FMT("%lu", value);
       else if (typeid(RHT) == typeid(float))
           LOG_FMT("%f", value);
       else if (typeid(RHT) == typeid(double))
           LOG_FMT("%lf", value);
       else if (typeid(RHT) == typeid(long long))
           LOG_FMT("%lld", value);
       else if (typeid(RHT) == typeid(unsigned long long))
           LOG_FMT("%llu", value);
       else {
           write(m_module, m_level,"(unknown)");
       }

       return (*this);
   }
#endif

    const ws_log& operator<<(char value) const;
    const ws_log& operator<<(unsigned char value) const;
    const ws_log& operator<<(short value) const;
    const ws_log& operator<<(unsigned short value) const;
    const ws_log& operator<<(int value) const;
    const ws_log& operator<<(unsigned int value) const;
    const ws_log& operator<<(long value) const;
    const ws_log& operator<<(unsigned long value) const;
    const ws_log& operator<<(float value) const;
    const ws_log& operator<<(double value) const;
    const ws_log& operator<<(long long value) const;
    const ws_log& operator<<(unsigned long long value) const;
    const ws_log& operator<<(void* value) const;
    const ws_log& operator<<(const void* value) const;
    const ws_log& operator<<(char value[]) const;
    const ws_log& operator<<(const char value[]) const;

    const ws_log& operator<<(const ws_log& (*__pf)(const ws_log&, endlog_p)) const;

    static bool testLevel(log::module mod, log::level lev);

    static void loghead(log::module mod, log::level lev);
    static void write(log::module mod, log::level lev, const char *format, ...);
    static void writeEx(log::module mod, log::level lev, bool head, const char *format, ...);

    void flush() const;
    void endlog() const;

    log::module getModule(Locator &lc);

private:
    void constructor(log::module module, log::level level);

    static void writeInner(log::module mod, log::level lev, bool head, const char *format, void *args);

private:
    log::module m_module;
    log::level m_level;
    FILE *m_stream;
    bool m_newline;
};


/**
 * End a log record.
 * LOG()<<endlog;
 */
inline const ws_log& endlog(const ws_log& lobj, endlog_p p) {
#if 0
    lobj.operator <<("\n");
    lobj.flush();
#endif
    lobj.endlog();
    return lobj;
}

/**@def ws_logx
 * Simplified ws_log.
 * Read the module infomation Automatically
 */
#define ws_logx(lev) ws_log(ws_log::getModule(MakeLocator()), lev)


#if COMPILER(GCC)
//__attribute__ ((format (printf, 3, 4)));
#   ifdef MP_DEBUG
#      define ws_dbg(mod,lev, args... ) LOG::write(mod, lev, ## args )
#   else
#      define ws_dbg(mod,lev, args... ) /* only useful for developers */
#   endif
#else // not GNU C

#   ifdef MP_DEBUG
#      define ws_dbg(mod,lev, ... ) LOG::write(mod, lev, __VA_ARGS__)
#   else
#      define ws_dbg(mod,lev, ... ) /* only useful for developers */
#   endif
#endif

} // namespace openwsp


#endif //!defined(OPENWSP_LOG_H_)
