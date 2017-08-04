/** @file
 * Openwsp - cache system.
 * Note it runs in 2 processes (using fork()), but doesn't requires locking!!
 */

// TODO: seeking, data consistency checking

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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <openwsp/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <openwsp/misc.h>
#include <openwsp/timer.h>
#if !OS(WIN32)
#include <sys/wait.h>
#include <openwsp/sharedmem.h>
#else
#include <windows.h>
#endif

#include "../log.h"
#include "stream.h"

namespace openwsp {

/*******************************************************************************
*   Macro definitions                                                          *
*******************************************************************************/
#undef DEBUG_CACHE
#define READ_USLEEP_TIME 10000
#define FILL_USLEEP_TIME 50000
#define PREFILL_SLEEP_TIME 200

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
static int min_fill=0;
int cache_fill_status=0;

/*******************************************************************************
*   Static functions                                                           *
*******************************************************************************/
#if OS(WIN32)
static DWORD WINAPI ThreadProc(void* s);
#endif

/*******************************************************************************
*   Typedef and structures                                                     *
*******************************************************************************/

typedef struct {
  // constats:
  unsigned char *buffer;      // base pointer of the alllocated buffer memory
  int buffer_size; // size of the alllocated buffer memory
  int sector_size; // size of a single sector (2048/2324)
  int back_size;   // we should keep back_size amount of old bytes for backward seek
  int fill_limit;  // we should fill buffer only if space>=fill_limit
  int seek_limit;  // keep filling cache if distanse is less that seek limit
  // filler's pointers:
  int eof;
  off_t min_filepos; // buffer contain only a part of the file, from min-max pos
  off_t max_filepos;
  off_t offset;      // filepos <-> bufferpos  offset value (filepos of the buffer's first byte)
  // reader's pointers:
  off_t read_filepos;
  // commands/locking:
//  int seek_lock;   // 1 if we will seek/reset buffer, 2 if we are ready for cmd
//  int fifo_flag;  // 1 if we should use FIFO to notice cache about buffer reads.
  // callback
  stream_t* stream;
} cache_vars_t;


////////////////////////////////////////////////////////////////////////////////

void cache_stats(cache_vars_t* s){
  int newb=s->max_filepos-s->read_filepos; // new bytes in the buffer
  ws_log::write(log::M_CACHE,log::INFO, "0x%06X  [0x%06X]  0x%06X   ",(int)s->min_filepos,(int)s->read_filepos,(int)s->max_filepos);
  ws_log::write(log::M_CACHE,log::INFO, "%3d %%  (%3d%%)\n",100*newb/s->buffer_size,100*min_fill/s->buffer_size);
}

int cache_read(cache_vars_t* s,unsigned char* buf,int size){
  int total=0;
  while(size>0){
    int pos,newb,len;

#ifdef DEBUG_CACHE
  //printf("CACHE2_READ: 0x%X <= 0x%X <= 0x%X  \n",s->min_filepos,s->read_filepos,s->max_filepos);
#endif

    if(s->read_filepos>=s->max_filepos || s->read_filepos<s->min_filepos){
    // eof?
    if(s->eof) break;
    // waiting for buffer fill...
    usecSleep(READ_USLEEP_TIME); // 10ms
    continue; // try again...
    }

    newb=s->max_filepos-s->read_filepos; // new bytes in the buffer
    if(newb<min_fill) min_fill=newb; // statistics...

#ifdef DEBUG_CACHE
//    printf("*** newb: %d bytes ***\n",newb);
#endif

    pos=s->read_filepos - s->offset;
    if(pos<0) pos+=s->buffer_size; else
    if(pos>=s->buffer_size) pos-=s->buffer_size;

    if(newb>s->buffer_size-pos) newb=s->buffer_size-pos; // handle wrap...
    if(newb>size) newb=size;

    // check:
    if(s->read_filepos<s->min_filepos) {
        ws_log(log::M_CACHE,log::ERR)
            << "Ehh. s->read_filepos<s->min_filepos !!! Report bug..."
            << endlog;
    }

#ifdef DEBUG_CACHE
    // len=write(mem,newb)
    //printf("Buffer read: %d bytes\n",newb);
#endif
    memcpy(buf,&s->buffer[pos],newb);
    buf+=newb;
    len=newb;
    // ...

    s->read_filepos+=len;
    size-=len;
    total+=len;

  }
  cache_fill_status=(s->max_filepos-s->read_filepos)/(s->buffer_size / 100);
  return total;
}

int cache_fill(cache_vars_t* s){
  int back,back2,newb,space,len,pos;
  off_t read=s->read_filepos;

  if(read<s->min_filepos || read>s->max_filepos){
      // seek...
      ws_log::write(log::M_CACHE, log::DBG2, "Out of boundaries... seeking to 0x%"PRIX64"  \n",(int64_t)read);
      // streaming: drop cache contents only if seeking backward or too much fwd:
      if(s->stream->type!=STREAMTYPE_STREAM ||
          read<s->min_filepos || read>=s->max_filepos+s->seek_limit)
      {
        s->offset= // FIXME!?
        s->min_filepos=s->max_filepos=read; // drop cache content :(
        if(s->stream->eof) stream_reset(s->stream);
        stream_seek(s->stream,read);
        ws_log::write(log::M_CACHE,log::DBG2,"Seek done. new pos: 0x%"PRIX64"  \n",(int64_t)stream_tell(s->stream));
      }
  }

  // calc number of back-bytes:
  back=read - s->min_filepos;
  if(back<0) back=0; // strange...
  if(back>s->back_size) back=s->back_size;

  // calc number of new bytes:
  newb=s->max_filepos - read;
  if(newb<0) newb=0; // strange...

  // calc free buffer space:
  space=s->buffer_size - (newb+back);

  // calc bufferpos:
  pos=s->max_filepos - s->offset;
  if(pos>=s->buffer_size) pos-=s->buffer_size; // wrap-around

  if(space<s->fill_limit){
#ifdef DEBUG_CACHE
//    printf("Buffer is full (%d bytes free, limit: %d)\n",space,s->fill_limit);
#endif
    return 0; // no fill...
  }

#ifdef DEBUG_CACHE
//  printf("### read=0x%X  back=%d  newb=%d  space=%d  pos=%d\n",read,back,newb,space,pos);
#endif

  // reduce space if needed:
  if(space>s->buffer_size-pos) space=s->buffer_size-pos;

//  if(space>32768) space=32768; // limit one-time block size
  if(space>4*s->sector_size) space=4*s->sector_size;

//  if(s->seek_lock) return 0; // FIXME

#if 1
  // back+newb+space <= buffer_size
  back2=s->buffer_size-(space+newb); // max back size
  if(s->min_filepos<(read-back2)) s->min_filepos=read-back2;
#else
  s->min_filepos=read-back; // avoid seeking-back to temp area...
#endif

#ifdef DEBUG_CACHE
  //printf("Buffer fill: %d bytes of %d\n",space,s->buffer_size);
  //len=stream_fill_buffer(s->stream);
  //memcpy(&s->buffer[pos],s->stream->buffer,len); // avoid this extra copy!
#endif

  len = stream_read(s->stream, (char*)&s->buffer[pos], space);
  if(!len) s->eof=1;

  s->max_filepos+=len;
  if(pos+len>=s->buffer_size){
      // wrap...
      s->offset+=s->buffer_size;
  }

  return len;

}

cache_vars_t* cache_init(int size,int sector){
  int num;
#ifndef WIN32
  cache_vars_t* s=shmem_alloc(sizeof(cache_vars_t));
#else
  cache_vars_t* s=(cache_vars_t*)malloc(sizeof(cache_vars_t));
#endif
  if(s==NULL) return NULL;

  memset(s,0,sizeof(cache_vars_t));
  num=size/sector;
  if(num < 16){
     num = 16;
  }//32kb min_size
  s->buffer_size=num*sector;
  s->sector_size=sector;
#ifndef WIN32
  s->buffer=shmem_alloc(s->buffer_size);
#else
  s->buffer=(unsigned char*)malloc(s->buffer_size);
#endif

  if(s->buffer == NULL){
#ifndef WIN32
    shmem_free(s,sizeof(cache_vars_t));
#else
    free(s);
#endif
    return NULL;
  }

  s->fill_limit=8*sector;
  s->back_size=s->buffer_size/2;
  return s;
}

void cache_uninit(stream_t *s) {
  cache_vars_t* c = (cache_vars_t*)s->cache_data;
  if(!s->cache_pid) return;
#ifndef WIN32
  kill(s->cache_pid,SIGKILL);
  waitpid(s->cache_pid,NULL,0);
#else
  TerminateThread((HANDLE)s->cache_thread,0);
  free(c->stream);
#endif
  if(!c) return;
#ifndef WIN32
  shmem_free(c->buffer,c->buffer_size);
  shmem_free(s->cache_data,sizeof(cache_vars_t));
#else
  free(c->buffer);
  free(s->cache_data);
#endif
}

static void exit_sighandler(int x){
  // close stream
  exit(0);
}

int stream_enable_cache(stream_t *stream,int size,int min,int seek_limit) {
  int ss=/*(stream->type==STREAMTYPE_VCD)?VCD_SECTOR_DATA:*/STREAM_BUFFER_SIZE;
  cache_vars_t* s;

  if (stream->type==STREAMTYPE_STREAM && stream->fd < 0) {
    // The stream has no 'fd' behind it, so is non-cacheable
    ws_log(log::M_CACHE, log::STATUS)
        << "\rThis stream is non-cacheable"
        << endlog;
    return 1;
  }

  /*
   create a cache system for the stream
   */
  s=cache_init(size,ss);
  if(s == NULL) return 0;
  stream->cache_data=s;
  s->stream=stream; // callback
  s->seek_limit=seek_limit;


  //make sure that we won't wait from cache_fill
  //more data than it is alowed to fill
  if (s->seek_limit > s->buffer_size - s->fill_limit ){
     s->seek_limit = s->buffer_size - s->fill_limit;
  }
  if (min > s->buffer_size - s->fill_limit) {
     min = s->buffer_size - s->fill_limit;
  }

#if !OS(WIN32)
  if((stream->cache_pid=fork())) {
#else
  {
    /*
     create a native thread
     */
    DWORD threadId;
    stream_t* stream2=(stream_t*)malloc(sizeof(stream_t));
    memcpy(stream2,s->stream,sizeof(stream_t));
    s->stream=stream2;
    stream->cache_thread = CreateThread(NULL,0, ThreadProc,s,0,&threadId);
    stream->cache_pid = threadId;
#endif
    // wait until cache is filled at least prefill_init %
    ws_log::write(log::M_CACHE,log::VERBOSE, "CACHE_PRE_INIT: %"PRId64" [%"PRId64"] %"PRId64"  pre:%d  eof:%d  \n",
    (int64_t)s->min_filepos,(int64_t)s->read_filepos,(int64_t)s->max_filepos,min,s->eof);

    while(s->read_filepos<s->min_filepos || s->max_filepos-s->read_filepos<min) {
        ws_log::write(log::M_CACHE,log::STATUS, MSGTR_CacheFill,
            100.0*(float)(s->max_filepos-s->read_filepos)/(float)(s->buffer_size),
            (int64_t)s->max_filepos-s->read_filepos);
        if(s->eof) break; // file is smaller than prefill size
    }
    ws_log(log::M_CACHE, log::STATUS) << endlog;
    return 1; // parent exits
  }

#if !OS(WIN32)
  /*
   * cache mainloop in child process
   */
  signal(SIGTERM,exit_sighandler); // kill
  while(1){
    if(!cache_fill((cache_vars_t*)s)){
     usecSleep(FILL_USLEEP_TIME); // idle
    }
//   cache_stats(s->cache_data);
  }

#endif
  return 0; // child exits
}

#if OS(WIN32)
/*
 * cache thread mainloop for win
 */
static DWORD WINAPI ThreadProc(void*s) {
  signal(SIGTERM,exit_sighandler); // kill
  while(1){
    if(!cache_fill((cache_vars_t*)s)){
     usecSleep(FILL_USLEEP_TIME); // idle
    }
//   cache_stats(s->cache_data);
  }
}
#endif

int cache_stream_fill_buffer(stream_t *s) {
  int len;
  cache_vars_t *v;
  if(s->eof){ s->buf_pos=s->buf_len=0; return 0; }
  if(!s->cache_pid) return stream_fill_buffer(s);

#ifdef DEBUG_CACHE
  //cache_stats(s->cache_data);
#endif

  v = (cache_vars_t*)s->cache_data;

  if(s->pos!=v->read_filepos) {
      ws_log(log::M_CACHE,log::ERR)
          << "!!! read_filepos differs!!! report this bug..."
          << endlog;
  }

  len = cache_read(v, s->buffer, v->sector_size);

#ifdef DEBUG_CACHE
  //printf("cache_stream_fill_buffer->read -> %d\n",len);
#endif

  if(len<=0){ s->eof=1; s->buf_pos=s->buf_len=0; return 0; }
  s->buf_pos=0;
  s->buf_len=len;
  s->pos+=len;
#ifdef DEBUG_CACHE
  //printf("[%d]",len);fflush(stdout);
#endif
  return len;
}

int cache_stream_seek_long(stream_t *stream,off_t pos){
  cache_vars_t* s;
  off_t newpos;
  if(!stream->cache_pid) return stream_seek_long(stream,pos);

  s=(cache_vars_t*)stream->cache_data;
//  s->seek_lock=1;


  ws_log::write(log::M_CACHE,log::DBG2, "CACHE_SEEK: 0x%"PRIX64" <= 0x%"PRIX64" (0x%"PRIX64") <= 0x%"PRIX64"  \n",
          (int64_t)s->min_filepos, (int64_t)pos,
          (int64_t)s->read_filepos, (int64_t)s->max_filepos);

  newpos=pos/s->sector_size; newpos*=s->sector_size; // align
  stream->pos=s->read_filepos=newpos;
  s->eof=0; // !!!!!!!

  cache_stream_fill_buffer(stream);

  pos-=newpos;
  if(pos>=0 && pos<=stream->buf_len){
    stream->buf_pos=pos; // byte position in sector
    return 1;
  }

//  stream->buf_pos=stream->buf_len=0;
//  return 1;

  ws_log::write(log::M_CACHE,log::VERBOSE, "cache_stream_seek: WARNING! Can't seek to 0x%"PRIX64" !\n",(int64_t)(pos+newpos));
  return 0;
}

} // namespace openwsp
