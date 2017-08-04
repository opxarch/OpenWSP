## @file
# The makefile for swscale
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

include $(SUB_DEPTH)/autoconf.mk


#########################################################################
# Targets.

OBJS = rgb2rgb.c.o swscale.c.o

OBJS-$(CONFIG_GPL)         +=  yuv2rgb.c.o
OBJS-$(HAVE_ALTIVEC)       +=  yuv2rgb_altivec.c.o

OBJS-$(ARCH_BFIN)          +=  swscale_bfin.c.o \
                               yuv2rgb_bfin.c.o \

ASM_OBJS-$(ARCH_BFIN)      += internal_bfin.s.o

CFLAGS += --std=gnu99
INCS += ../ ../libavutil
DEFS += HAVE_AV_CONFIG_H

.PHONY: all clean swscale

all: swscale

clean: generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/rules.generic.mk

#########################################################################
# Main rules.

swscale: swscale.$(.LIB)

swscale.$(.LIB): $(OBJS) $(OBJS-y) $(OBJS-y) $(ALTIVEC-OBJS-y) $(ASM_OBJS-y)
