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

#ifndef OPENWSP_REFPTR_H_
#define OPENWSP_REFPTR_H_

#define DEBUG_AUTOPTR 1

#include <openwsp/atomics.h>
#include <openwsp/thread.h>
#include <openwsp/assert.h>
#include <openwsp/misc.h>

#include <openwsp/tracker.h>

#if DEBUG_AUTOPTR
# include "log.h"
#endif

namespace openwsp {


/***************************************************
  *****     Pointer Reference Base object      *****
  ***************************************************/

template <class T>
class RefCounted {
public:
    RefCounted() :
            m_ptr(static_cast<T *>(this)), // ensure this is a base class
            m_count(1)
        {
        }

    RefCounted(T *ptr) :
            m_ptr(ptr), m_count(1)
        {
        }

    inline void ref() {
        ref(Locator());
    }

    void ref(const Locator &locate) {
        register int cnt;
        /* SPIN */ {
            AutoSpinLock(this->m_spin);
            cnt = ++m_count;
        }

#if DEBUG_AUTOPTR
        ws_log(log::INFO) << "ref() :" << locate.GetFilename() << ":" << locate.GetLine() << ": ptr = " << m_ptr << " cnt = " << cnt << "\n";
#else
        UNUSED(cnt);
#endif
    }

    inline void release() {
        release(Locator());
    }

    void release(const Locator &locate) {
        register int cnt;

        /* SPIN */ {
            AutoSpinLock(this->m_spin);

            WS_ASSERT(m_count > 0); // this is NOT to keep its balance

            cnt = --m_count;
        }

#if DEBUG_AUTOPTR
        ws_log(log::INFO) << "release() :" << locate.GetFilename() << ":" << locate.GetLine() << ": ptr = " << m_ptr << " cnt = " << cnt << "\n";
#endif

        if (cnt == 0) {
            delete m_ptr;
        }
    }

private:
    T *m_ptr;
    int m_count;
    WSSpinLock m_spin; //!todo: Atomic Increasing/Decreasing
};


template <class T>
  class AutoPtr;

/***************************************************
  *****     Auto Pointer Reference Passed      *****
  ***************************************************/

template <class T>
  class AutoPtrPass {
public:
    AutoPtrPass(RefCounted<T> *obj) :
        m_obj(obj) {
    }

    AutoPtrPass(RefCounted<T> *obj, const Locator &locate) :
        m_locate(locate),
        m_obj(obj) {
    }

    AutoPtrPass(const AutoPtr<T> &src) {
        m_locate = src.m_locate;
        m_obj    = src.m_obj;
    }

    AutoPtrPass(const AutoPtrPass<T> &src) {
        m_locate = src.m_locate;
        m_obj    = src.m_obj;
    }

    /*********************************************************/

    const AutoPtrPass<T> &operator = (const AutoPtr<T> &src) {
        m_obj = src.m_obj;
        return (*this);
    }
public:
    Locator m_locate;
    RefCounted<T> *m_obj;
};

/***************************************************
  *****     Auto Pointer Reference object      *****
  ***************************************************/

template <class T>
  class AutoPtr {
public:
    AutoPtr(RefCounted<T> *obj)
        : m_obj(obj) {
        if (m_obj)
            m_obj->ref(m_locate);
    }

    AutoPtr(RefCounted<T> *obj, const Locator &locate)
        : m_locate(locate),
          m_obj(obj) {
        if (m_obj)
            m_obj->ref(m_locate);
    }

    AutoPtr(const AutoPtr &src)
        : m_locate(src.m_locate),
          m_obj(src.m_obj) {
        if (m_obj)
            m_obj->ref(m_locate);
    }

    AutoPtr(const AutoPtrPass<T> &pass)
        : m_locate(pass.m_locate),
          m_obj(pass.m_obj) {
        if (m_obj)
            m_obj->ref(m_locate);
    }

    ~AutoPtr() {
        if (m_obj)
            m_obj->release(m_locate);
    }

    /*********************************************************/

    const AutoPtr &operator = (const AutoPtr &src) {
        WS_ASSERT(0);
        copy(src.m_locate, src.m_obj);
        return (*this);
    }

private:
    void copy(const Locator &locate, RefCounted<T> *obj) {
        m_locate = locate;

        if (m_obj)
            m_obj->release(m_locate);
        if (obj)
            obj->ref(m_locate);

        m_obj = obj;
    }

public:
    Locator m_locate;
    RefCounted<T> *m_obj;
};


} // namespace openwsp

#endif //!defined(OPENWSP_REFPTR_H_)
