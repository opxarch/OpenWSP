/** @file
 * OpenWSP - garbage collection.
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

#include "gc.h"

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

void GCTarget::setPtr(void *ptr) {
    m_ptr = ptr;
}

bool GCTarget::noref() {
    WS_ASSERT( m_ref >=0 );
    return m_ref <= 0;
}

void GCTarget::release() {
    m_ref--;
    WS_ASSERT( m_ref >=0 );
}

////////////////////////////////////////////////////////////////////////////////

int GC::addRegistry(GCTarget *src) {
    return m_targets.Pushfront(src);
}

void GC::scan() {
    int rc;
    GCTarget *gc;

    for (m_targets.begin(); m_targets.end(); m_targets++)
    {
        rc = m_targets.get(gc);
        if (WS_SUCCESS(rc)) {
            /*
             * The object has no reference, so just do destruction
             * and delete it from the memory.
             */
            if (gc->noref()) {
                gc->deleteRel();
            }
        }
    }
}

} // namespace openwsp
