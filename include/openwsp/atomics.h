/** @file
 * Util - Atomic operation
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

#ifndef OPENWSP_ATOMICS_H_
#define OPENWSP_ATOMICS_H_

#include <openwsp/misc.h>
#include <openwsp/types.h>

BEGIN_C_DECLS

/**
 * Compare And Swap (CAS) for various arches.
 */
#if ARCH(PPC)
static inline int compareAndSwap (uint32_t volatile *p)
{
    int ret;
    __asm__ __volatile__ (
                          "      lwarx %0,0,%1\n"
                          "      xor. %0,%3,%0\n"
                          "      bne $+12\n"
                          "      stwcx. %2,0,%1\n"
                          "      bne- $-16\n"
                          : "=&r" (ret)
                          : "r" (p), "r" (1), "r" (0)
                          : "cr0", "memory");
    return ret;
}
#elif ARCH(X86)
static inline int compareAndSwap (uint32_t volatile *p)
{
    long int readval = 0;

    __asm__ __volatile__ ("lock; cmpxchgl %2, %0"
                          : "+m" (*p), "+a" (readval)
                          : "r" (1)
                          : "cc");
    return readval;
}
#elif ARCH(AMD64)
static inline int compareAndSwap (uint32_t volatile *p)
{
    long int readval = 0;

    __asm__ __volatile__ ("lock; cmpxchgl %2, %0"
                          : "+m" (*p), "+a" (readval)
                          : "r" (1)
                          : "cc");
    return readval;
}
#elif ARCH(S390)
static inline int compareAndSwap (uint32_t volatile *p)
{
    int ret;

    __asm__ __volatile__ ("0: cs    %0,%1,0(%2)\n"
              "   jl    0b"
              : "=&d" (ret)
              : "r" (1), "a" (p), "0" (*p)
              : "cc", "memory" );
    return ret;
}
#elif ARCH(ALPHA)
static inline int compareAndSwap (uint32_t volatile *p)
{
    int ret;
    unsigned long one;

    __asm__ __volatile__ ("0:   mov 1,%2\n"
              " ldl_l %0,%1\n"
              " stl_c %2,%1\n"
              " beq %2,1f\n"
              ".subsection 2\n"
              "1:   br 0b\n"
              ".previous"
              : "=r" (ret), "=m" (*p), "=r" (one)
              : "m" (*p));
    return ret;
}
#elif ARCH(SPARC)
static inline int compareAndSwap (uint32_t volatile *p)
{
    int ret;

    __asm__ __volatile__("ldstub    [%1], %0"
                 : "=r" (ret)
                 : "r" (p)
                 : "memory");

    return (ret ? 1 : 0);
}
#elif ARCH(ARM)
static inline int compareAndSwap (int *spinlock)
{
    register unsigned int ret;
    __asm__ __volatile__("swp %0, %1, [%2]"
                         : "=r"(ret)
                         : "0"(1), "r"(spinlock));

    return ret;
}
#elif ARCH(MC68000)
static inline int compareAndSwap (uint32_t volatile *p)
{
    char ret;
    __asm__ __volatile__("tas %1; sne %0"
                         : "=r" (ret)
                         : "m" (p)
                         : "cc","memory");
    return ret;
}

#elif ARCH(IA64)

#include <ia64intrin.h>

static inline int compareAndSwap (uint32_t volatile *p)
{
    return __sync_lock_test_and_set (p, 1);
}
#elif ARCH(MIPS)
static inline int compareAndSwap (uint32_t volatile *p)
{
    int ret;

    __asm__ __volatile__ (
    "   .set push       \n"
    "   .set noat       \n"
    "   .set mips2      \n"
    "1: li  $1, 1       \n"
    "   ll  %0, %1      \n"
    "   sc  $1, %1      \n"
    "   beqz    $1, 1b      \n"
    "   .set pop        "
    : "=r" (ret), "+R" (*p)
    :
    : "memory");

    return ret;
}
#else
#error unimplemented CPU support
#endif

END_C_DECLS

/** @} */

#endif //!defined(OPENWSP_ATOMICS_H_)
