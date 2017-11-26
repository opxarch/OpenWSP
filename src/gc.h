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

#ifndef OPENWSP_GC_H_
#define OPENWSP_GC_H_

#include "openwsp/assert.h"
#include "openwsp/list.h"

namespace openwsp {

class GCTarget {
public:
    GCTarget() :
        m_ref(1),
        m_ptr(0)
    {
    }

    virtual ~GCTarget() {}
    virtual void setPtr(void *ptr);
    virtual bool noref();
    virtual void release();
    virtual void deleteRel() = 0;

protected:
    int m_ref;
    void *m_ptr;
};

/***************************************************
  *****        Garbage collection object       *****
  ***************************************************/

class GC {
public:
    int addRegistry(GCTarget *src);
    void scan();

private:
    WSList<GCTarget *> m_targets;
};

} // namespace openwsp

#endif //!defined(OPENWSP_GC_H_)
