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

#ifndef EVENTPACK_H_
#define EVENTPACK_H_

#include <new>
#include "eventpump.h"

namespace openwsp {

//using openwsp::WSEvent; //fixme?

class ThreadBase;

/***************************************************
  *****             Tuple object               *****
  ***************************************************/

struct Tuple0 {
    explicit Tuple0() {
    }
};

template <class A>
struct Tuple1 {
  Tuple1() {}
  explicit Tuple1(A a) : a(a) {}

  A a;
};

template <class A, class B>
struct Tuple2 {
 public:
  Tuple2() {}
  Tuple2(A a,
         B b)
      : a(a), b(b) {
  }

  A a;
  B b;
};

template <class ObjT, class Method>
inline int CallMethod(ObjT* obj,
                      Method method,
                      ThreadBase *thread,
                      const Tuple0& arg) {
  return (obj->*method)(thread);
}

template <class ObjT, class Method, class A>
inline int CallMethod(ObjT* obj,
                      Method method,
                      ThreadBase *thread,
                      const Tuple1<A>& arg) {
  return (obj->*method)(thread, arg.a);
}

template <class ObjT, class Method, class A, class B>
inline int CallMethod(ObjT* obj,
                      Method method,
                      ThreadBase *thread,
                      const Tuple2<A,B>& arg) {
  return (obj->*method)(thread, arg.a, arg.b);
}

/***************************************************
  *****         Event factory object           *****
  ***************************************************/

template<class T>
class EventFactory {
public:
    explicit EventFactory(T* object)
        : m_ptr(object) {
    }

    /**
     * Create a WSEvent package that runs the method specified.
     * @param method Pointer to the target method.
     * @param arg0 Optional, Argument #1
     * @param arg1 Optional, Argument #2
     */
   template <class Method>
    inline WSEvent* bind(Method method) {
        return new (std::nothrow) RunnableMethod<Method, Tuple0>(
                m_ptr, method, Tuple0());
    }

   template <class Method, class A>
    inline WSEvent* bind(Method method, const A& a) {
        return new (std::nothrow) RunnableMethod<Method, Tuple1<A> >(
            m_ptr, method, Tuple1<A>(a));
    }

   template <class Method, class A, class B>
    inline WSEvent* bind(Method method, const A& a, const B& b) {
        return new (std::nothrow) RunnableMethod<Method, Tuple2<A, B> >(
            m_ptr, method, Tuple2<A,B>(a, b));
    }

protected:
   template <class Method, class Params>
    class RunnableMethod : public WSEvent {
    public:
        RunnableMethod(T* obj, Method meth, const Params& params)
            : m_obj(obj),
              m_meth(meth),
              m_params(params) {
        }

        virtual int run(ThreadBase *thread) {
            return CallMethod(m_obj, m_meth, thread, m_params);
        }

    private:
        T* m_obj;
        Method m_meth;
        Params m_params;
    };

private:
    T* m_ptr;
};

} // namespace openwsp

#endif //!defined(EVENTDISPATCH_H_)
