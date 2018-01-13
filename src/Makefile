## @file
# The makefile for openwsp
#

#
#  OpenWSP (an opensource web stream player)
#  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
#  please contact with <diyer175@hotmail.com> if you have any problems.
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

SUB_DEPTH = ./..

include $(SUB_DEPTH)/autoconf.mk

#########################################################################
# Targets.

OBJS =  util/err.cpp.o                      \
        util/assert.cpp.o                   \
        util/crc32.cpp.o					\
        util/thread.cpp.o					\
        util/filesystem.cpp.o				\
        util/string.cpp.o					\
        util/sharedmem.cpp.o				\
        util/timer.cpp.o					\
        util/glob.cpp.o						\
        util/tracker.cpp.o					\
        javacore/javacore.cpp.o				\
		javacore/jsarray.c.o				\
		javacore/jsboolean.c.o				\
		javacore/jsbuiltin.c.o				\
		javacore/jscompile.c.o				\
		javacore/jsdate.c.o					\
		javacore/jsdtoa.c.o					\
		javacore/jsdump.c.o					\
		javacore/jserror.c.o				\
		javacore/jsfunction.c.o				\
		javacore/jsgc.c.o					\
		javacore/jsintern.c.o				\
		javacore/jslex.c.o					\
		javacore/jsmath.c.o					\
		javacore/jsnumber.c.o				\
		javacore/jsobject.c.o				\
		javacore/json.c.o					\
		javacore/jsparse.c.o				\
		javacore/jsproperty.c.o				\
		javacore/jsregexp.c.o				\
		javacore/jsrun.c.o					\
		javacore/jsstate.c.o				\
		javacore/jsstring.c.o				\
		javacore/jsvalue.c.o				\
		javacore/regexp.c.o					\
		javacore/utf.c.o					\
		javacore/utftype.c.o				\
		stream/tcp.cpp.o					\
		stream/url.cpp.o					\
		stream/network.cpp.o				\
		stream/cookies.cpp.o				\
		stream/stream_http.cpp.o			\
		stream/stream_file.cpp.o			\
		stream/stream.cpp.o					\
		stream/cache.cpp.o					\
		stream/filestream.cpp.o				\
		webservice/webservice.cpp.o			\
		audiosys/audiosys_null.cpp.o		\
		audiosys/audiosys_sdl.cpp.o			\
		audiosys/audiosys_dsound.cpp.o		\
		audiosys/audiosystem.cpp.o			\
		audiosys/audioformat.cpp.o			\
		demuxer/demux_aac.cpp.o				\
		demuxer/demux_audio.cpp.o			\
		demuxer/demux_demuxers.cpp.o		\
		demuxer/demuxer.cpp.o				\
		demuxer/formatprint.cpp.o			\
		demuxer/mf.cpp.o					\
		demuxer/mp3header.cpp.o				\
		demuxer/extension.cpp.o				\
		codec/audcodec_ffmpeg.cpp.o			\
		codec/audcodec_pcm.cpp.o			\
		codec/audcodec.cpp.o				\
		codec/aud.cpp.o						\
		gui/gui.cpp.o						\
		log.cpp.o							\
		config.cpp.o						\
		eventloop.cpp.o						\
		eventfactory.cpp.o					\
		eventpump.cpp.o						\
		gc.cpp.o							\
		threadmain.cpp.o					\
		threadio.cpp.o						\
		threadaudio.cpp.o					\
        openwsp.cpp.o						\
        main.cpp.o							\
        

OBJS += $(LIB_DIR)/avcodec.$(.LIB) \
		$(LIB_DIR)/avutil.$(.LIB) \
		$(LIB_DIR)/swscale.$(.LIB)
OBJS += $(LIB_DIR)/gui.$(.LIB) \
		$(LIB_DIR)/guilib.$(.LIB)

LIBS += sdl
LIBS += iconv

ifeq ($(CONFIG_TARGET_OS),win32)
LIBS += gdi32
endif

ifeq ($(HAVE_PTHREADS),y)
LIBS += pthread
endif
ifeq ($(HAVE_W32THREADS),y)
LIBS += kernel32
endif

.PHONY: all clean subs openwsp subs_clean

all: subs openwsp

clean: subs_clean generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/rules.generic.mk


#########################################################################
# Main rules.

INCS += codec/libavcodec \
		codec/libavutil \
		codec/libswscale

openwsp: openwsp.$(.EXEC)

#
# openwsp console
#
openwsp.$(.EXEC): $(OBJS)

#
# Generate the status code descriptors.
#
util/err.cpp.o: $(ROOT_DIR)/$(OUT_DIR)/errors-generated.h

$(ROOT_DIR)/$(OUT_DIR)/errors-generated.h: \
		util/errs.sed \
		$(ROOT_DIR)/include/openwsp/err.h
        
	$(call run-command,$(SED) -f $< $(filter %.h,$^) > $@,"  SED      $@")

#
# Sub items
#
subs:
	$(MAKE) all -C ./webservice
#	$(MAKE) all -C ./codec/libavutil
#	$(MAKE) all -C ./codec/libavcodec
#	$(MAKE) all -C ./codec/libswscale
ifeq ($(CONFIG_GUI),wspv1)
	$(MAKE) all -C ./gui/wspv1
endif
subs_clean:
	$(MAKE) clean -C ./webservice
#	$(MAKE) clean -C ./codec/libavutil
#	$(MAKE) clean -C ./codec/libavcodec
#	$(MAKE) clean -C ./codec/libswscale
ifeq ($(CONFIG_GUI),wspv1)
	$(MAKE) clean -C ./gui/wspv1
endif
