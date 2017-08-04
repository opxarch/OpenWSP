## @file
# The makefile for libavutil
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

SUB_DEPTH = ./../../..


#########################################################################
# Targets.

OBJS = adler32.c.o \
       aes.c.o \
       base64.c.o \
       crc.c.o \
       intfloat_readwrite.c.o \
       fifo.c.o \
       lls.c.o \
       log.c.o \
       lzo.c.o \
       mathematics.c.o \
       md5.c.o \
       mem.c.o \
       random.c.o \
       rational.c.o \
       string.c.o \
       tree.c.o

DEFS += HAVE_AV_CONFIG_H

.PHONY: all clean avutil

all: avutil

clean: generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/rules.generic.mk

#########################################################################
# Main rules.

avutil: avutil.$(.LIB)

avutil.$(.LIB): $(OBJS)

