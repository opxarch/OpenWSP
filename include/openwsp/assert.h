/** @file
 * openwsp assertions
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

#ifndef OPENWSP_ASSERT_H_
#define OPENWSP_ASSERT_H_

#include <openwsp/misc.h>
#include <assert.h>

BEGIN_C_DECLS

/**
 On assertion failure, panic the system or program.
 */
#if RING(0)
# if ENABLE(ASSERTIONS)
#   define WS_ASSERT_PANIC()   DebugBreakPoint()
# else
#   define WS_ASSERT_PANIC()   ((void)0)
# endif

# define DebugBreakPoint() ((void)0)

#elif RING(3)
# define WS_ASSERT_PANIC()     assert(0)
#endif

#if RING(3)

/**
 * Report a assertion failure.
 *
 * @param   file       Filename of source.
 * @param   line       The line failure located at
 * @param   func       Function name
 * @param   expr       Expression string
 */
void wpAssertFailure(const char* file, int line, const char* func, const char*expr);

/**
 * Report a assertion failure with message log.
 *
 * @param   format    Printf like format string.
 * @param   ...       Arguments to that string.
 */
void wpAssertFailureLog(const char* format,...);

#else

static void wpAssertFailure(const char* file, int line, const char* func, const char*expr) {
    UNUSED(file);
    UNUSED(line);
    UNUSED(func);
    UNUSED(expr);
    
}
static void wpAssertFailureLog(const char* format,...) {
    UNUSED(format);
}
#endif

#if ENABLE(ASSERTIONS)

/** @todo: implement the variable parameter by __VA_ARGS__ */

/** @def WP_ASSERT
 * ASSERT that an expression is true. If it's not emit the breakpoint.
 * @param   assertion  The Expression.
 */
#define WS_ASSERT(assertion) \
    (UNLIKELY(!(assertion)) ? \
        (wpAssertFailure(__FILE__, __LINE__, WS_CURRENT_FUNCTION, #assertion), \
         WS_ASSERT_PANIC()) : \
        (void)0)


/** @def WP_ASSERT_LOG
 * If the expression isn't true, will report the txt-message and emit the breakpoint.
 * @param   assertion  The Expression.
 * @param   msg        Message format,args,... (must be in brackets).
 *                     eg. WP_ASSERT_LOG(expr, ("msg format", arg1, arg2, ...) );
 */
#define WS_ASSERT_LOG(assertion, msg) \
    (UNLIKELY(!(assertion)) ? \
        (wpAssertFailure(__FILE__, __LINE__, WS_CURRENT_FUNCTION, #assertion), \
         wpAssertFailureLog msg , \
         WS_ASSERT_PANIC()) : \
        (void)0)


/** @def ASSERT_STATIC_INT
 * static_assert emulated.(non standard)
 * This differs from AssertCompile in that it accepts some more expressions
 * than what C++0x allows
 * @param   assertion    The expression.
 */
#define WS_ASSERT_STATIC_INT(assertion)  typedef int known[(assertion) ? 1 : -1]

/** @def ASSERT_STATIC
 * Asserts that a C++0x compile-time expression is true. If it's not break the
 * build.
 * @param   assertion    The Expression.
 */
#ifdef HAVE_STATIC_ASSERT
# define WS_ASSERT_STATIC(assertion) static_assert(!!(assertion), #assertion)
#else
# define WS_ASSERT_STATIC(assertion) WS_ASSERT_STATIC_INT(assertion)
#endif


#else

/*
 dummy
 */
#define WS_ASSERT(assertion) ((void)0)
#define WS_ASSERT_LOG(assertion, msg) ((void)0)
#define WS_ASSERT_STATIC(assertion) ((void)0)

#endif //ENABLE(ASSERTIONS)


END_C_DECLS


#endif //!defined(OPENWSP_ASSERT_H_)
