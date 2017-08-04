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
};

/***************************************************
  *****           AutoSpinlock object          *****
  ***************************************************/

class AutoSpinLock {
public:
    AutoSpinLock() {
        lock.acquire();
    }
    ~AutoSpinLock() {
        lock.release();
    }

private:
    WSSpinLock lock;
};

/***************************************************
  *****           Mutex object                *****
  ***************************************************/

class WSMutex {
public:
    WSMutex();

    void acquire();
    void release();

    int testAcquire();

private:
    void *mutex;
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
    int wait(int *rc);

protected:
    void *m_opaque;

private:
    void *m_thread;

    friend int __threadContext(void *);
};


} // namespace openwsp

#endif //!defined(OPENWSP_THREAD_H_)
