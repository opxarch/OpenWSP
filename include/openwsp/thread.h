/** @file
 * Openwsp - Thread management.
 */

/*
 *  OpenWSP (an opensource web stream player)
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

#ifndef OPENWSP_THREAD_H_
#define OPENWSP_THREAD_H_

#include <openwsp/types.h>

namespace openwsp {

/***************************************************
  *****           Spinlock object                *****
  ***************************************************/

class WSSpinLock {
public:
    WSSpinLock();

    void acquire();
    void release();

    int testAcquire();

private:
    uint32_t volatile count;
    void *spin;
};

/***************************************************
  *****           AutoSpinlock object          *****
  ***************************************************/

class AutoSpinLock {
public:
    AutoSpinLock(WSSpinLock &lock) :
        m_lock(lock) {
        m_lock.acquire();
    }

    ~AutoSpinLock() {
        m_lock.release();
    }

private:
    WSSpinLock &m_lock;
};

/***************************************************
  *****           Mutex object                *****
  ***************************************************/

class WSMutex {
public:
    WSMutex();
    ~WSMutex();

    void acquire();
    void release();

    int testAcquire();

private:
    void *mutex;
};

typedef uint64_t ThreadId_t;

/***************************************************
  *****           Semaphore object             *****
  ***************************************************/

class WSSemaphore {
public:
    WSSemaphore();
    ~WSSemaphore();

    void signal();
    void wait();

private:
    void *semaphore;
};

/***************************************************
  *****           Thread object                *****
  ***************************************************/

class WSThread {
public:
    WSThread();
    virtual ~WSThread();

    virtual int run(void *opaque)=0;

    int create(void *opaque);
    void sync();
    int wait(int *rc);
    ThreadId_t self();
    static ThreadId_t currentId();

    inline void cancel() {
        m_canceled = true;
    }

    inline bool cancelRaised() {
        return m_canceled;
    }

protected:
    void *m_opaque;

private:
    void *m_thread;
    volatile bool m_canceled;
    WSSemaphore m_semaphore;

    friend int __threadContext(void *);
};


} // namespace openwsp

#endif //!defined(OPENWSP_THREAD_H_)
