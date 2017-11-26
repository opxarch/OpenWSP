/** @file
 * Javacore - Wrapper of javascript engine
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
#include <openwsp/types.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>

#include <string.h>
#include "jsi.h"

#include "openwsp/javacore.h"


////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

Javacore::Javacore() :
    jsc(0),
    objcount(0),
    codebuff(0),
    uctx(0) {

}

Javacore::~Javacore() {

}

////////////////////////////////////////////////////////////////////////////////

/**
 * Do the logical initialization.
 * @return status code.
 */
int Javacore::init() {
    /*
     Create the javascript interpreter
     */
    jsc = js_newstate((js_Alloc)NULL, NULL, JS_STRICT);
    if (!jsc)
        return WERR_ALLOC_MEMORY;

    /* hidden parameter */
    jsc->opaque = this;
    return WINF_SUCCEEDED;
}

/**
 * uninitiate the class logically.
 * @return status code.
 */
int Javacore::uninit() {
    /*
     Release the resource of javascript interpreter
     */
    if (jsc) {
        js_freestate(jsc);
        return WINF_SUCCEEDED;
    }

    return WERR_FAILED;
}

/**
 * Run javascript by giving string code.
 * VERY IMPORTANT: don't call it among two or more threads
 * as this object is non-reentrant.
 *
 * @return status code
 * @param   str             Pointer to the string, which must be
 *                          terminated by '\0' (null) char, otherwise
 *                          may caused a buffer overflowing fault.
 */
int Javacore::exec(const char *str) {
    codebuff = str;
    int jsr = js_dostring(jsc, str);
    codebuff = 0;

    return jsr ? WERR_FAILED : WINF_SUCCEEDED;
}

/**
 * Clean up all the registries of javascript context.
 * VERY IMPORTANT: don't operate it among two or more threads
 * as this object is non-reentrant.
 * @return status code.
 */
int Javacore::cleanUp() {
    js_gc(jsc, 0);
    return WINF_SUCCEEDED;
}

/**
 * Get the code buffer during the script is
 * running.
 *
 * VERY IMPORTANT: don't call it among two or more threads
 * as this object is non-reentrant.
 *
 * NOTE: Any time we are not in context of script, this
 * function will return WERR_FAILED.
 *
 * @param out Where to store the pointer of buffer.
 * @return status code.
 */
int Javacore::getCodeBuffer(const char **out) {
    if (!codebuff) {
        *out = 0;
        return WERR_FAILED;
    } else {
        *out = codebuff;
        return WINF_SUCCEEDED;
    }
}

/**
 * Set the user context pointer.
 * @param p User defined pointer
 */
void Javacore::setContext(void *p) {
    uctx = p;
}

/**
 * Get the user context pointer.
 * @return user defined pointer
 */
void *Javacore::getContext() {
    return uctx;
}

//
// Javascript Object operation
//

int Javacore::prepareObject() {
    js_newobject(jsc);
    objcount++;
    return WINF_SUCCEEDED;
}

void Javacore::objectSetTag(const char *objname, const char *name, void *tag) {
    js_getregistry(jsc, objname);
    js_newuserdata(jsc, name, tag, NULL);
}

/**
 * NOTE: all the functions that are registered should
 * push a return value by calling members in jsContext.
 * call pushundefined(p) even though the C function returns nothing.
 * if not, the virtual stack will be broken.
 */
void Javacore::objectRegisterFunc(const char *name, jscCallback func, unsigned length) {
    const char *realname = strchr(name, '.');
    realname = realname ? realname + 1 : name;
    js_newcfunction(jsc, (js_CFunction)func, name, length);
    js_defproperty(jsc, -2, realname, JS_DONTENUM);
}

void Javacore::objectSetConstructor(const char *objname, const char *name, jscCallback func, unsigned length) {
    const char *realname = strchr(name, '.');
    realname = realname ? realname + 1 : name;
    js_getregistry(jsc, objname);
    js_newcconstructor(jsc, (js_CFunction)func, (js_CFunction)func, name, length);
    js_defproperty(jsc, -2, realname, JS_DONTENUM);
}

int Javacore::createObject(const char *name) {
    js_setregistry(jsc, name);
    return WINF_SUCCEEDED;
}

int Javacore::createGlobalObject(const char *name) {
    js_setglobal(jsc, name);
    return WINF_SUCCEEDED;
}

void Javacore::registerFunc(const char *name, jscCallback func, unsigned length) {
    js_newcfunction(jsc, (js_CFunction)func, name, length);
    js_setglobal(jsc, name);
}

jscContext *Javacore::context() {
    return (jscContext*)jsc;
}

jscContext *Javacore::getGlobal(const char *name) {
    js_getglobal(jsc, name);
    return (jscContext*)jsc;
}

jscContext *Javacore::getProperty(int idx, const char *name) {
    js_getproperty(jsc, idx, name);
    return (jscContext*)jsc;
}

jscContext *Javacore::getIndex(int idx, unsigned int i) {
    js_getindex(jsc, idx, i);
    return (jscContext*)jsc;
}

unsigned int Javacore::getLength(int idx) {
    return js_getlength(jsc, idx);
}

//
// Javascript context
//

Javacore &jscContext::getJsc(jscContext *p) {
    return *((Javacore*) ((js_State*)p)->opaque);
}

void *jscContext::getTag(jscContext *p, const char *name) {
    return js_touserdata((js_State*)p, 0, name);
}
void jscContext::setTag(jscContext *p, const char *objname, const char *name, void *tag) {
    setTag(p, objname, name, tag, NULL/*recycle*/);
}
void jscContext::setTag(jscContext *p, const char *objname, const char *name, void *tag, jscRecycle recycle) {
    js_getregistry((js_State*)p, objname);
    js_newuserdata((js_State*)p, name, tag, (js_Finalize)recycle);
}

bool jscContext::isundefined(jscContext *p, int idx) {
    return js_isundefined((js_State*)p, idx) ? true : false;
}
const char *jscContext::tostring(jscContext *p, int idx) {
    return js_tostring((js_State*)p, idx);
}
double jscContext::tonumber(jscContext *p, int idx) {
    return js_tonumber((js_State*)p, idx);
}
int jscContext::tointeger(jscContext *p, int idx) {
    return js_tointeger((js_State*)p, idx);
}
bool jscContext::toboolean(jscContext *p, int idx) {
    return js_toboolean((js_State*)p, idx);
}

void jscContext::pushundefined(jscContext *p) {
    js_pushundefined((js_State*)p);
}
void jscContext::pushnull(jscContext *p) {
    js_pushnull((js_State*)p);
}
void jscContext::pushboolean(jscContext *p, bool v) {
    js_pushboolean((js_State*)p, v ? 1 : 0);
}
void jscContext::pushnumber(jscContext *p, double v) {
    js_pushnumber((js_State*)p, v);
}
void jscContext::pushstring(jscContext *p, const char *v) {
    js_pushstring((js_State*)p, v);
}
void jscContext::pop(jscContext *p, int idx) {
    js_pop((js_State*)p, idx);
}

void jscContext::throwError(jscContext *p, const char *msg) {
    js_newerror((js_State *)p, msg);
    js_throw((js_State *)p);
}

} // namespace openwsp
