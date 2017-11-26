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
#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/asm.h>
#include <openwsp/atomics.h>

#include <openwsp/assert.h>

#include <openwsp/thread.h>

#if HAVE(PTHREADS)
# include <pthread.h>
# include <semaphore.h>
#endif


////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

/*******************************************************************************
    Spinlock object
*******************************************************************************/

WSSpinLock::WSSpinLock()
    : count(0)
{
#if HAVE(PTHREADS)
    pthread_spin_init(static_cast<pthread_spinlock_t*>(&spin), 0);
#endif
}

void WSSpinLock::acquire() {
#if HAVE(PTHREADS)
    pthread_spin_lock(static_cast<pthread_spinlock_t*>(&spin));
#else
    while (compareAndSwap(&count, 1, 0)) {
#if ARCH(X86) || ARCH(AMD64)
        asmNopPause();
#endif
        //DebugBreakPoint();
    }
#endif
}

void WSSpinLock::release() {
#if HAVE(PTHREADS)
    pthread_spin_unlock(static_cast<pthread_spinlock_t*>(&spin));
#else
    if (!compareAndSwap(&count, 0,1)) {
        WS_ASSERT_LOG(0, ("Spinlock %p was not locked!\n", this));
    }
#endif
}

/*
 * Test whether we can acquire the lock,
 * and attain the lock if allowed.
 * @return 0 Upon successful completion.
 */
int WSSpinLock::testAcquire() {
#if HAVE(PTHREADS)
    return pthread_spin_trylock(static_cast<pthread_spinlock_t*>(&spin));
#else
    return !compareAndSwap(&count, 1,0) ? WINF_SUCCEEDED: WERR_FAILED;
#endif
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

WSMutex::~WSMutex() {
#if HAVE(PTHREADS)
    pthread_mutex_destroy(static_cast<pthread_mutex_t*>(&mutex));
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

/***************************************************
  *****           Semaphore object             *****
  ***************************************************/

WSSemaphore::WSSemaphore() {
#if HAVE(PTHREADS)
    sem_init(static_cast<sem_t *>(&semaphore), 0, 0);
#else
#error port me!
#endif
}

WSSemaphore::~WSSemaphore() {
#if HAVE(PTHREADS)
    sem_destroy(static_cast<sem_t *>(&semaphore));
#endif
}

void WSSemaphore::signal() {
#if HAVE(PTHREADS)
    sem_post(static_cast<sem_t *>(&semaphore));
#endif
}

void WSSemaphore::wait() {
#if HAVE(PTHREADS)
    sem_wait(static_cast<sem_t *>(&semaphore));
#endif
}

/*******************************************************************************
    Thread object
*******************************************************************************/

WSThread::WSThread() {
    m_opaque = 0;
    m_thread = 0;
    m_canceled = false;
}

WSThread::~WSThread() {
}

/**
 * Inner, callback from thread context.
 * @param ptr Pointer to the WSThread object.
 * @return status code.
 */
inline int __threadContext(void *ptr) {
    WSThread *thread = static_cast<WSThread*>(ptr);
    /* wait for the sync signal */
    thread->m_semaphore.wait();
    return thread->run(thread->m_opaque);
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
 * Sync the thread( start to call the run() ).
 */
void WSThread::sync() {
    m_semaphore.signal();
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

/**
 * Get the id of this thread.
 * @return thread id.
 */
ThreadId_t WSThread::self() {
    return (reinterpret_cast<ThreadId_t>(m_thread));
}

/**
 * Get the id of CURRENT thread context.
 * @return thread id.
 */
ThreadId_t WSThread::currentId() {
#if HAVE(PTHREADS)
    pthread_t tid = pthread_self();
#endif
    return (static_cast<ThreadId_t>(tid));
}

} //namespace openwsp
