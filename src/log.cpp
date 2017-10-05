/** @file
 * OpenWSP - text log management.
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
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>

#include <openwsp/misc.h>
#include <openwsp/tracker.h>

#ifdef USE_ICONV
#include <iconv.h>
#include <errno.h>
extern char* get_term_charset(void);
#endif

#include "log.h"

namespace openwsp {

/*******************************************************************************
*   Macro definitions                                                           *
*******************************************************************************/
/** maximum message length of ws_msg */
#define MSGSIZE_MAX 3072


/*******************************************************************************
*   Typedefs and structures                                                           *
*******************************************************************************/
struct module_descor {
    const char *filename;
    const char *modname;
};

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
static bool log_inited = false;
static int log_levels[log::M_MAX]; // verbose level of this module. inited to -2
static int log_level_all = log::STATUS;
static int verbose = 0;
#ifdef USE_ICONV
static char *ws_msg_charset = NULL;
static char *old_charset = NULL;
static iconv_t msgiconv;
#endif

static module_descor mod_text[log::M_MAX]= {
    {0,                 "DEFAULT"},
    {"util/*",          "UTIL"},
    {"webservice/*",    "WEBSRV"},
    {"javacore/*",      "JAVASC"},
    {"gui/*",           "GUI"},
    {"dsp/*",           "DSP"},
    {"audiosys/*",      "AUDIOSYS"},
    {"demuxer/*",       "DEMUXER"},
    {"demuxer/*",       "DS"},
    {"demuxer/demux_*", "DEMUX"},
    {"demuxer/*",       "HEADER"},
    {"codec/aud*",      "DECAUDIO"},
    {"stream/*",        "OPEN"},
    {"stream/*",        "SEEK"},
    {"stream/*",        "WIN32"},
    {"stream/*",        "STREAM"},
    {"stream/*",        "CACHE"},
    {"util/*",          "OSDEP"},
    {"stream/*",        "NETWORK"},
    {0,                 "IDENTIFY"}
};


/*******************************************************************************
*   Internal Functions                                                         *
*******************************************************************************/
static void log_init();


////////////////////////////////////////////////////////////////////////////////

/* default constructors */
ws_log::ws_log() {
    constructor(log::M_DEFAULT, log::INFO);
}
ws_log::ws_log(log::module module) {
    constructor(m_module, log::INFO);
}
ws_log::ws_log(log::level level) {
    constructor(log::M_DEFAULT, level);
}
ws_log::ws_log(log::module module, log::level level) {
    constructor(module, level);
}

/**
 * Inner, used by log constructor function.
 * @param mod       Module id. See enum module in namespace log.
 * @param lev       Message level. See enum level.
 */
void ws_log::constructor(log::module module, log::level level) {
    m_module = module;
    m_level = level;
    m_newline = false;

    /*
     Resolve the output stream.
     */
    m_stream = (m_level) <= log::WARN ? stderr : stdout;

    loghead(m_module, m_level);
}


/*
 Output for different source types
 */

#define LOG_FMT(fmt, val) do { ws_log::writeEx(m_module, m_level, false, fmt, val); return (*this); } while(0)

const ws_log& ws_log::operator<<(char value) const {
    LOG_FMT("%c", value);
}
const ws_log& ws_log::operator<<(unsigned char value) const {
    LOG_FMT("%u", (unsigned)value);
}
const ws_log& ws_log::operator<<(short value) const {
    LOG_FMT("%d", (int)value);
}
const ws_log& ws_log::operator<<(unsigned short value) const {
    LOG_FMT("%u", (unsigned)value);
}
const ws_log& ws_log::operator<<(int value) const {
    LOG_FMT("%d", value);
}
const ws_log& ws_log::operator<<(unsigned int value) const {
    LOG_FMT("%u", value);
}
const ws_log& ws_log::operator<<(long value) const {
    LOG_FMT("%ld", value);
}
const ws_log& ws_log::operator<<(unsigned long value) const {
    LOG_FMT("%lu", value);
}
const ws_log& ws_log::operator<<(float value) const {
    LOG_FMT("%f", value);
}
const ws_log& ws_log::operator<<(double value) const {
    LOG_FMT("%lf", value);
}
const ws_log& ws_log::operator<<(long long value) const {
    LOG_FMT("%lld", value);
}
const ws_log& ws_log::operator<<(unsigned long long value) const {
    LOG_FMT("%llu", value);
}

const ws_log& ws_log::operator<<(void* value) const {
    LOG_FMT("0x%p", value);
}
const ws_log& ws_log::operator<<(const void *value) const {
    LOG_FMT("0x%p", value);
}

const ws_log& ws_log::operator<<(char value[]) const {
    LOG_FMT("%s", value ? value : "(null)");
}
const ws_log& ws_log::operator<<(const char value[]) const {
    LOG_FMT("%s", value ? value : "(null)");
}

const ws_log& ws_log::operator<<(const ws_log& (*__pf)(const ws_log&, endlog_p)) const {
    return __pf(*this, endlog_0);
}


/**
 * initiate the global data of logger.
 */
static void log_init() {
    int i;
    char *env = getenv("OPENWSP_VERBOSE");
    if (env)
        verbose = atoi(env);
    // debug switch
    log_level_all = log::VERBOSE;
    for(i=0;i<log::M_MAX;i++) log_levels[i] = -2;
    log_levels[log::M_IDENTIFY] = -1; // no -identify output by default
#if USES(ICONV)
    ws_msg_charset = getenv("OPENWSP_VERBOSE");
    if (!ws_msg_charset)
      ws_msg_charset = get_term_charset();
#endif
}


/* static */
bool ws_log::testLevel(log::module mod, log::level lev) {
    if (!log_inited)
        log_init(); // lazy init
    return lev <= (log_levels[mod] == -2 ? log_level_all + verbose : log_levels[mod]);
}

/* static */
void ws_log::loghead(log::module mod, log::level lev) {
    FILE *stream= (lev) <= log::WARN ? stderr : stdout;
    fprintf(stream, "[%9s] ", mod_text[mod].modname);
}

/**
 * Output the message with log head.
 * @param mod       Module id. See enum module in namespace log.
 * @param lev       Message level. See enum level.
 * @param head      Whether write the log head.
 * @param format    Format string, the same as the c lib.
 * @param ...       Variable parameters.
 */
/* static */
void ws_log::write(log::module mod, log::level lev, const char *format, ... ){
    va_list va;
    va_start(va, format);
    writeInner(mod, lev, true, format, static_cast<void*>(&va));
    va_end(va);
}

/**
 * Output the message
 * @param mod       Module id. See enum module in namespace log.
 * @param lev       Message level. See enum level.
 * @param head      Whether write the log head.
 * @param format    Format string, the same as the c lib.
 * @param ...       Variable parameters.
 */
/* static */
void ws_log::writeEx(log::module mod, log::level lev, bool head, const char *format, ... ){
    va_list va;
    va_start(va, format);
    writeInner(mod, lev, head, format, static_cast<void*>(&va));
    va_end(va);
}

/**
 * Inner, Output the message
 * @param mod       Module id. See enum module in namespace log.
 * @param lev       Message level. See enum level.
 * @param head      Whether write the log head.
 * @param format    Format string, the same as the c lib.
 * @param arg       Package of variable parameters.
 */
/* static */
void ws_log::writeInner(log::module mod, log::level lev, bool head, const char *format, void *args) {
    va_list *va = (va_list*)args;
    char tmp[MSGSIZE_MAX];

    if (!log_inited)
        log_init(); // lazy-init

    if (!testLevel(mod, lev)) return; // do not display
    vsnprintf(tmp, MSGSIZE_MAX, format, *va);
    tmp[MSGSIZE_MAX-2] = '\n';
    tmp[MSGSIZE_MAX-1] = 0;

#if defined(USE_ICONV) && defined(MSG_CHARSET)
    if (ws_msg_charset && strcasecmp(ws_msg_charset, "noconv")) {
      char tmp2[MSGSIZE_MAX];
      size_t inlen = strlen(tmp), outlen = MSGSIZE_MAX;
      char *in = tmp, *out = tmp2;
      if (!old_charset || strcmp(old_charset, ws_msg_charset)) {
        if (old_charset) {
          free(old_charset);
          iconv_close(msgiconv);
        }
        msgiconv = iconv_open(ws_msg_charset, MSG_CHARSET);
        old_charset = strdup(ws_msg_charset);
      }
      if (msgiconv == (iconv_t)(-1)) {
        fprintf(stderr,"iconv: conversion from %s to %s unsupported\n"
               ,MSG_CHARSET,ws_msg_charset);
      }else{
      memset(tmp2, 0, MSGSIZE_MAX);
      while (iconv(msgiconv, &in, &inlen, &out, &outlen) == -1) {
        if (!inlen || !outlen)
          break;
        *out++ = *in++;
        outlen--; inlen--;
      }
      strncpy(tmp, tmp2, MSGSIZE_MAX);
      tmp[MSGSIZE_MAX-1] = 0;
      tmp[MSGSIZE_MAX-2] = '\n';
      }
    }
#endif

    FILE *stream= (lev) <= log::WARN ? stderr : stdout;

#ifdef MSG_USE_COLORS
/* that's only a silly color test */
#ifdef MP_ANNOY_ME
    { int c;
      static int flag=1;
      if(flag)
      for(c=0;c<24;c++)
          printf("\033[%d;3%dm***  COLOR TEST %d  ***\n",(c>7),c&7,c);
      flag=0;
    }
#endif
    {   unsigned char v_colors[10]={9,1,3,15,7,2,2,8,8,8};

        int c=v_colors[lev];
        int c2=(mod+1)%15+1;
        static int header=1;
        if(header){
            fprintf(stream, "\033[%d;3%dm%9s\033[0;37m: ",c2>>3,c2&7, mod_text[mod]);
        }
        fprintf(stream, "\033[%d;3%dm",c>>3,c&7);
        header=    tmp[strlen(tmp)-1] == '\n'
                 ||tmp[strlen(tmp)-1] == '\r';
    }
#endif

    // log head
    if (head)
        loghead(mod, lev);

    //
    //!FIXME a mess. Simplify the following later.
    //

    size_t ubound = strlen(tmp);

    // find the first '\n'
    bool fn = false;
    const char *f = tmp;
    while(*f == ' ') f++;
    fn = (*f == '\n');

    // find the last '\n'
    const char *ln;
    f = tmp + ubound - 1;
#if 0
    while(*f == ' ') f--;
#endif
    ln = (*f == '\n' ? f : 0);

    // split
    unsigned offset = 0;
    unsigned lead = 0;
    unsigned len = 0;
    for (unsigned i = 0; i<ubound+1; i++) {
        if (tmp[i] != '\n' &&
            tmp[i]) {
            len++;
        } else {
            if ((tmp[lead] == '\n') &&
                    (lead || !fn) && (tmp+lead != ln))
                loghead(mod, lev);

            fprintf(stream, "%.*s", ++len, &tmp[offset]);
            lead = i;
            offset = i+1;
            len = 0;
        }
    }
    fflush(stream);
}

/**
 * Flush the log buffer.
 */
void ws_log::flush() const {
    fflush(m_stream);
}

/**
 * End a log record.
 */
void ws_log::endlog() const {
    fwrite("\n", 1,2, m_stream);
    fflush(m_stream);
}

/**
 * Get the module ID by locator.
 * @return module id.
 */
log::module ws_log::getModule(Locator &lc) {
    return log::M_DEFAULT; //!todo
}

} // namespace openwsp
