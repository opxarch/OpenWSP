/** @file
 * Openwsp - Memory operation and management
 */

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

#ifndef OPENWSP_MEM_H_
#define OPENWSP_MEM_H_

#include <openwsp/misc.h>
#include <openwsp/types.h>

namespace openwsp {

/** @def MEMTAG_DEF
 * The default mem tag used for tarcking.
 */
#ifndef MEMTAG_DEF
#  define MEMTAG_DEF   (__FILE__)
#endif

/** @def MEM_ALIGNMENT
 * The alignment of the memory blocks.
 */
#if OS(OS2)
# define MEM_ALIGNMENT    4
#else
# define MEM_ALIGNMENT    8
#endif

/** @def ALIGN_TYPE
 * alignment macro.
 * @param   u           Value to align.
 * @param   alignment  The alignment. Power of two!
 * @param   type        Integer type to use while aligning.
 */
#define ALIGN_TYPE(u, alignment, type) (((type)(u) + ((alignment) - 1)) & ~(type)((alignment) - 1))

/** @def ALIGN_32
 * alignment macro for a 32-bit value.
 * @param   u32         Value to align.
 * @param   alignment  The alignment. Power of two!
 */
#define ALIGN_32(u32, alignment) ALIGN_TYPE(u32, alignment, uint32_t)

/** @def ALIGN_64
 * alignment macro for a 64-bit value.
 * @param   u64         Value to align.
 * @param   alignment  The alignment. Power of two!
 */
#define ALIGN_64(u64, alignment) ALIGN_TYPE(u64, alignment, uint64_t)

/** @def ALIGN_SIZE
 * alignment macro for size_t.
 * @param   size       Value to align.
 * @param   alignment  The alignment. Power of two!
 */
#define ALIGN_SIZE(size, alignment) ALIGN_TYPE(size, alignment, size_t)

/** @def ALIGN_PTR_CAST
 * alignment macro for pointers with type cast.
 * @param   p           Pointer to align.
 * @param   alignment   The alignment. Power of two.
 * @param   type        Type of pointer. 
 */
#define ALIGN_PTR_CAST(p, alignment, type) ((type)ALIGN_TYPE(p, alignment, uintptr_t))

/** @def ALIGN_PTR
 * alignment macro for pointers.
 * @param   p          Pointer to align.
 * @param   alignment  The alignment. Power of two!
 */
#define ALIGN_PTR(p, alignment) ALIGN_PTR_CAST(p, alignment, void *)

} // namespace openwsp

#endif //!defined(OPENWSP_MEM_H_)
