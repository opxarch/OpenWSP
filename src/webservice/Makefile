## @file
# The makefile for apis
#

#
#  OpenWSP (an opensource web stream codec)
#  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
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

SUB_DEPTH = ./../..

#########################################################################
# Main rules.

.PHONY: all clean apicmp apis

all: apicmp apis

#########################################################################
# Building apicmp

CONFIG_BUILDING_HOST := y

OBJS := ./apicmp/apicmp.c.o ./../util/crc32.cpp.o

include $(SUB_DEPTH)/rules.generic.mk

apicmp: apicmp.$(.EXEC)

apicmp.$(.EXEC): $(OBJS)

# apicmp tool
APICMP := $(ROOT_DIR)/$(OUT_DIR)/apicmp.$(.EXEC)


#########################################################################
# Building API scripts

APIS_OBJ := QingtingFM.api

apis: apicmp $(ROOT_DIR)/$(OUT_APIS_DIR) $(APIS_OBJ)

%.api: %.js
	$(call run-command,$(APICMP) $< $(ROOT_DIR)/$(OUT_APIS_DIR)/$@,"  APICMP    $@")

$(ROOT_DIR)/$(OUT_APIS_DIR):
	$(MKDIR) $@
    
clean: generic_clean
	$(RM) -f $(ROOT_DIR)/$(OUT_APIS_DIR)/*.api
    