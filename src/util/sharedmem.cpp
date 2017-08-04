/** @file
 * openwsp shared memory management
 */

/*
 *  OpenWSP (an opensource webstream codec)
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

#include <openwsp/misc.h>
#include <openwsp/assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !OS(WIN32)
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#if HAVE(SYS_MMAN_H)
#include <sys/mman.h>
#elif defined(__BEOS__)
#include <mman.h>
#endif
#include <sys/socket.h>
#include <fcntl.h>
#endif //!OS(WIN32)

#include "../log.h"

#ifdef AIX
#include <sys/select.h>
#endif

#ifdef HAVE_SHM
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#if defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
#define MAP_ANON MAP_ANONYMOUS
#endif

#include "../../include/openwsp/sharedmem.h"

#if !OS(WIN32)
static int shmem_type = 0;
#endif

/**
 * Allocate a shared memory
 * @param size      Length of new memory.
 * @return succeeded:   pointer to the shared memory.
 * @return failed:      a null pointer.
 */
void* shmem_alloc(int size) {
#if !OS(WIN32)
    void* ret;
    static int devzero = -1;

    while(1){
        switch(shmem_type) {
            case 0:  /* ========= MAP_ANON|MAP_SHARED ========== */
#ifdef MAP_ANON
                    ret = mmap(0,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_SHARED,-1,0);
                    if(ret==MAP_FAILED) break; // failed

                    ws_dbg(MSGT_OSDEP, MSGL_DBG2, "shmem: %d bytes allocated using mmap anon (%p)\n",size,ret);
                    return ret;
#else
                    // system does not support MAP_ANON at all (e.g. solaris 2.5.1/2.6), just fail
                    ws_dbg(MSGT_OSDEP, MSGL_DBG3, "shmem: using mmap anon failed\n");
#endif
                break;

            case 1:  /* ========= MAP_SHARED + /dev/zero ========== */
                if (devzero == -1 && (devzero = open("/dev/zero", O_RDWR, 0)) == -1) break;
                ret = mmap(0,size,PROT_READ|PROT_WRITE,MAP_SHARED,devzero,0);
                if(ret==MAP_FAILED) break; // failed

                ws_dbg(MSGT_OSDEP, MSGL_DBG2, "shmem: %d bytes allocated using mmap /dev/zero (%p)\n",size,ret);

                return ret;

            case 2: { /* ========= shmget() ========== */
                #ifdef HAVE_SHM
                    struct shmid_ds shmemds;
                    int shmemid;
                    if ((shmemid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0600)) == -1) break;
                    if ((ret = shmat(shmemid, 0, 0)) == (void *)-1){
                      ws_msg(MSGT_OSDEP, MSGL_ERR, "shmem: shmat() failed: %s\n", strerror(errno));
                      shmctl (shmemid, IPC_RMID, &shmemds);
                      break;
                    }
                    if (shmctl(shmemid, IPC_RMID, &shmemds) == -1) {
                      ws_msg(MSGT_OSDEP, MSGL_ERR, "shmem: shmctl() failed: %s\n", strerror(errno));
                      if (shmdt(ret) == -1) perror ("shmdt()");
                      break;
                    }
                    ws_dbg(MSGT_OSDEP, MSGL_DBG2, "shmem: %d bytes allocated using SHM (%p)\n",size,ret);
                    return ret;
                #else
                    ws_msg(MSGT_OSDEP, MSGL_FATAL, "shmem: no SHM support was compiled in!\n");
                    return(NULL);
                #endif
            }
            default:
                ws_msg(MSGT_OSDEP, MSGL_FATAL,
                "FATAL: Cannot allocate %d bytes of shared memory :(\n",size);
                return NULL;
        }
        ++shmem_type;
    }
#else
    //!TODO unimplemented
    WS_ASSERT(0);
    return NULL;
#endif
}

/**
 * Release the shared memory
 * @param p         Pointer to the start of target memory.
 * @param size      Length of the target memory.
 */
void shmem_free(void* p,int size){
#if !OS(WIN32)
    switch(shmem_type){
        case 0:
        case 1:
            if(munmap(p,size)) {
                ws_msg(MSGT_OSDEP, MSGL_ERR, "munmap failed on %p %d bytes: %s\n",
                    p,size,strerror(errno));
            }
            break;

        case 2:
#ifdef HAVE_SHM
            if (shmdt(p) == -1)
            ws_msg(MSGT_OSDEP, MSGL_ERR, "shmfree: shmdt() failed: %s\n",
                strerror(errno));
#else
            ws_msg(MSGT_OSDEP, MSGL_ERR, "shmfree: no SHM support was compiled in!\n");
#endif
            break;
    }
#else
    //!TODO unimplemented
    WS_ASSERT(0);
#endif
}
