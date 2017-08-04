/** @file
 * Util - Assembler misc
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

#ifndef OPENWSP_ASM_H_
#define OPENWSP_ASM_H_

#include <openwsp/misc.h>
#include <openwsp/types.h>

/*
 reuses intrinsics
 */
#if COMPILER(MSC) && USES(INTRIN)
# include <intrin.h>
# pragma intrinsic(_enable)
# pragma intrinsic(_disable)
# pragma intrinsic(__rdtsc)
#endif


BEGIN_C_DECLS


/**
 * Pause instructions
 *
 * supported x86 & amd64. it helps hyperthreaded CPUs detecting
 * a waiting loop.
 */
inline void asmNopPause(void) {
#if ARCH(AMD64) || ARCH(X86)
#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
    __asm__ __volatile__(".byte 0xf3,0x90\n\t");
#else
    __asm {
        _emit 0f3h
        _emit 090h
    }
#endif

#else
    /* dummy */
#endif
}


////////////////////////////////////////////////////////////////////////////////

/**
 amd64/x86 only
 */
#if ARCH(AMD64) || ARCH(X86)


/**
 * Tests if a bit in a bitmap is set.
 *
 * @returns true if the bit is set.
 * @returns false if the bit is clear.
 *
 * @param   pvBitmap    Pointer to the bitmap.
 * @param   iBit        The bit to test.
 *
 * @remarks The 32-bit aligning of pvBitmap is not a strict requirement.
 *          However, doing so will yield better performance as well as avoiding
 *          traps accessing the last bits in the bitmap.
 */
inline bool asmBitTest(const volatile void *pvBitmap, int32_t iBit) {
    union { bool f; uint32_t u32; uint8_t u8; } rc;
#if USES(INTRIN)
    rc.u32 = _bittest((long *)pvBitmap, iBit);
#elif COMPILER_SUP(INLINE_ASM_GNU_STYLE)

    __asm__ __volatile__("btl %2, %1\n\t"
                         "setc %b0\n\t"
                         "andl $1, %0\n\t"
                         : "=q" (rc.u32)
                         : "m" (*(const volatile long *)pvBitmap),
                           "Ir" (iBit)
                         : "memory");
#else
    __asm
    {
        mov   edx, [iBit]
# if ARCH(AMD64)
        mov   rax, [pvBitmap]
        bt    [rax], edx
# else
        mov   eax, [pvBitmap]
        bt    [eax], edx
# endif
        setc  al
        and   eax, 1
        mov   [rc.u32], eax
    }
# endif
    return rc.f;
}

/**
 * Get the [RE]FLAGS register.
 * @returns [RE]FLAGS.
 */
inline uint_reg asmGetFlags(void) {
    uint_reg uFlags;
#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
# if ARCH(AMD64)
    __asm__ __volatile__("pushfq\n\t"
                         "popq  %0\n\t"
                         : "=r" (uFlags));
# else
    __asm__ __volatile__("pushfl\n\t"
                         "popl  %0\n\t"
                         : "=r" (uFlags));
# endif

#else
    __asm
    {
# if ARCH(AMD64)
        pushfq
        pop  [uFlags]
# else
        pushfd
        pop  [uFlags]
# endif
    }
#endif
    return uFlags;
}


/**
 * Set the [RE]FLAGS register.
 * @param   uFlags      The new [RE]FLAGS value.
 */
inline void asmSetFlags(uint_reg uFlags) {
#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
# if ARCH(AMD64)
    __asm__ __volatile__("pushq %0\n\t"
                         "popfq\n\t"
                         : : "g" (uFlags));
# else
    __asm__ __volatile__("pushl %0\n\t"
                         "popfl\n\t"
                         : : "g" (uFlags));
# endif

#else

    __asm
    {
# if ARCH(AMD64)
        push    [uFlags]
        popfq
# else
        push    [uFlags]
        popfd
# endif
    }
#endif
}

/**
 * Ask whether the interrupts were enabled
 *
 * @returns true / false.
 */
inline bool asmIntAreEnabled(void) {
    uint_reg uFlags = asmGetFlags();
    return uFlags & 0x200 /* X86_EFL_IF */ ? true : false;
}

/**
 * Enable interrupts (EFLAGS.IF->1).
 */
inline void asmEnableInt(void) {
#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
    __asm("sti\n");
#elif USES(INTRIN)
    _enable();
#else
    __asm sti
#endif
}

/**
 * Disable interrupts (EFLAGS.IF->0).
 */
inline void asmDisableInt(void) {
#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
    __asm("cli\n");
#elif USES(INTRIN)
    _disable();
#else
    __asm cli
#endif
}


/**
 * Read the value of the CPU timestamp counter register.
 * @returns TSC.
 */
inline uint64_t asmReadTSC(void) {
    UINT64U u;

#if COMPILER_SUP(INLINE_ASM_GNU_STYLE)
    __asm__ __volatile__("rdtsc\n\t" : "=a" (u.s.lo), "=d" (u.s.hi));
#else

#if USES(INTRIN)
    u.u = __rdtsc();
#else
    __asm {
        rdtsc
        mov     [u.s.lo], eax
        mov     [u.s.hi], edx
    }
#endif

#endif
    return u.u;
}

#endif //ARCH(AMD64) || ARCH(X86)


END_C_DECLS

/** @} */

#endif //!defined(OPENWSP_ASM_H_)
