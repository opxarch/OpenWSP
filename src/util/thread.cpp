/** @file
 * Util - Thread system.
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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <pthread.h>

#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/asm.h>
#include <openwsp/atomics.h>

#include <openwsp/thread.h>

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

/*******************************************************************************
    Spinlock object
*******************************************************************************/

WSSpinLock::WSSpinLock()
    : count(0)
{
}

void WSSpinLock::acquire() {
    while (compareAndSwap(&count)) {
#if ARCH(X86) || ARCH(AMD64)
        asmNopPause();
#endif
    }
}

void WSSpinLock::release() {
    count = 0;
}

/*
 * Test whether we can acquire the lock,
 * and attain the lock if allowed.
 * @return status code.
 */
int WSSpinLock::testAcquire() {
    return !compareAndSwap(&count) ? WINF_SUCCEEDED: WERR_FAILED;
}

/*******************************************************************************
    Mutex object
*******************************************************************************/

WSMutex::WSMutex() {
#if HAVE(PTHREADS)
    pthread_mutex_init(static_cast<pthread_mutex_t*>(&mutex), 0);
#else
#error port me!
#endif
}

void WSMutex::acquire() {
#if HAVE(PTHREADS)
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(&mutex));
#endif
}

void WSMutex::release() {
#if HAVE(PTHREADS)
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(&mutex));
#endif
}

int WSMutex::testAcquire() {
    int rc = WERR_FAILED;
#if HAVE(PTHREADS)
    rc = !pthread_mutex_trylock(static_cast<pthread_mutex_t*>(&mutex)) ? WINF_SUCCEEDED: WERR_FAILED;
#endif
    return rc;
}

/*******************************************************************************
    Thread object
*******************************************************************************/

WSThread::WSThread() {
    m_opaque = 0;
    m_thread = 0;
}

WSThread::~WSThread() {
}

/**
 * Inner, callback from thread context.
 * @param ptr Pointer to the WSThread object.
 * @return status code.
 */
int __threadContext(void *ptr) {
    WSThread *th = (WSThread*)ptr;
    return th->run(th->m_opaque);
}

#if HAVE(PTHREADS)
static void *threadContext(void *ptr) {
    return (void*)__threadContext(ptr);
}
#endif

/**
 * Create the thread that will call run() virtual function.
 * @param ptr User defined pointer parameter.
 * @return status code
 */
int WSThread::create(void *ptr) {
    int rc = WERR_FAILED;

    if (m_thread)
        return WERR_THREAD_EXISTS;

    m_opaque = ptr;

#if HAVE(PTHREADS)
    rc = !pthread_create((pthread_t*)(uintptr_t*)(&m_thread), 0, threadContext, this)
            ? WINF_SUCCEEDED
            : WERR_FAILED;
#endif

    if (WS_FAILURE(rc))
        m_thread = 0;

    return rc;
}

/**
 * Wait for this thread.
 * @param rc Where to store the status code that returned from
 *           Thread.
 * @return status code.
 */
int WSThread::wait(int *prc) {
    int rc = WERR_FAILED;

#if HAVE(PTHREADS)
    void *res;
    rc = !pthread_join((pthread_t)m_thread, &res)
            ? WINF_SUCCEEDED
            : WERR_FAILED;
#endif

    if (WS_SUCCESS(rc))
        *prc = (int)res;

    return rc;
}


} //namespace openwsp
