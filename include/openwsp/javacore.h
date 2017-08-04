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

#ifndef OPENWSP_JAVACORE_H_
#define OPENWSP_JAVACORE_H_

namespace openwsp {

class jscContext;
typedef void (*jscCallback)(jscContext *context);
typedef void (*jscRecycle)(jscContext *context, void *mem);


/***************************************************
  *****         Javacore object                *****
  ***************************************************/

class Javacore {
public:
    Javacore();
    ~Javacore();

    int init();
    int uninit();

    int execCode(const char *str);
    int cleanUp();
    int getCodeBuffer(const char **out);
    void setContext(void *p);
    void *getContext();
    int prepareObject();
    void objectSetTag(const char *objname, const char *name, void *tag);
    void objectRegisterFunc(const char *name, jscCallback func, unsigned length);
    void objectSetConstructor(const char *objname, const char *name, jscCallback func, unsigned length);
    int createObject(const char *name);
    int createGlobalObject(const char *name);

    void registerFunc(const char *name, jscCallback func, unsigned length);

    jscContext *getGlobal(const char *name);
    jscContext *getProperty(int idx, const char *name);
    jscContext *getIndex(int idx, unsigned int i);
    unsigned int getLength(int idx);

    inline int objectCount() const { return objcount; }

private:
    struct js_State_s *jsc;    // Pointer to Javacore object
    int objcount;
    const char *codebuff;
    void *uctx;
};

/***************************************************
  *****        Javascript context object       *****
  ***************************************************/

/**
 * NOTE: in some older compiler, you can call the following
 * static member functions even by NULL pointer.
 * eg.
 *
 * ((JscContext*)0)->setTag(p, name);
 *
 */

class jscContext {
public:
    static Javacore &getJsc(jscContext *p);
    static void *getTag(jscContext *p, const char *name);
    static void setTag(jscContext *p, const char *objname, const char *name, void *tag);
    static void setTag(jscContext *p, const char *objname, const char *name, void *tag, jscRecycle recycle);
    static bool isundefined(jscContext *p, int idx);
    static const char *tostring(jscContext *p, int idx);
    static double tonumber(jscContext *p, int idx);
    static int tointeger(jscContext *p, int idx);
    static bool toboolean(jscContext *p, int idx);
    static void pushundefined(jscContext *p);
    static void pushnull(jscContext *p);
    static void pushboolean(jscContext *p, bool v);
    static void pushnumber(jscContext *p, double v);
    static void pushstring(jscContext *p, const char *v);
    static void pop(jscContext *p, int idx);
    static void throwError(jscContext *p, const char *msg);

};

} // namespace openwsp

#endif //!defined(OPENWSP_JAVACORE_H_)
