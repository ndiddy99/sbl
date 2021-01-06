###########################################################
#
# File: global.mk
# Creator: Benjamin Lee
#
# This file is included by all make files ... to propogate
# global macros through
#
# DO NOT DETABIFY (unless your make doesn't care about tabs)
# ...
#
###########################################################

GLOBAL_DEFS = -D_SH -DMODEL_S

OPTIMIZE_FLAGS = -finline-functions -fomit-frame-pointer

DEBUG_FLAGS = -g

EXTRA_CFLAGS = -Wall -m2 -O1 $(GLOBAL_DEFS)

EXTRA_LDFLAGS =

EXTRA_ASFLAGS =

EXTRA_INCLUDES = -I../include

STD_INCLUDES = -I/gnu/sh/include

