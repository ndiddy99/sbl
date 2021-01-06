#--------------------------------------------------------------------------
# File Name: Makefile   File Title: Makefile for SOJ Saturn Program Libs
# Copyright (c) 1994 Sega of America, Inc. All Rights Reserved.
#
# Author(s): rcs reh
# Date: 10-18-94
# Description:	Makefile to create Saturn system libraries
#		Assumes GNUmake and DOS environment
#
#--------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = rm

## Grab any EXTRA_FLAGS we might need
include ../../../segalib/global/global.mk
## Get Pattern Rules
include ../../../segalib/global/global.pat

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -I../../../segalib/include -Igfs

ASFLAGS =

LDFLAGS = $(EXTRA_LDFLAGS)

LDIR = lib

LLDIR = -Llib

GNU_LIBS = -lgcc -lc -lm

SYS_LIBS = -lsat -lcdc -lper -lsat

## Object Files

FONT_OBJS = FNT_COD0.o \
	    FNT_COD1.o \
	    FNT_DAT0.o \
	    FNT_DAT1.o \
	    FNT_DAT2.o

font: smp_font.a


smp_font.a: $(FONT_OBJS)
	   $(AR) rv $@ $?

clean:
	del *.a
	del *.o

#----------------------------------------------------------------------
# File Name: Makefile
# Copyright (c) 1995 Sega of America, Inc. All Rights Reserved.
# End of File
#
#----------------------------------------------------------------------

