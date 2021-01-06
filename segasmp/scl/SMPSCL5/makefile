 #----------------------------------------------------------------------------
 #  demo makefile
 #  Copyright(c) 1994 SEGA
 #----------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = del


## Grab any EXTRA_FLAGS we might need
include ../../../segalib/global/global.mk

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -I../../../segalib/include

ASFLAGS = $(EXTRA_ASFLAGS) 

LDFLAGS =

GNU_LIBS = -Lc:/gnu/lib -lm -lgcc -lc -lgcc

LDIR = -L../../../segalib/lib

SOJ_LIBS = -lsat -lper -lsat


OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	smpscl5.o \
        asciifon.o \
        textur.o \
        ../../v_blank/v_blank.o \
        ../../v_blank/set_vb.o \


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst


bin: smpscl5.bin

smpscl5.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(LDIR) $(SOJ_LIBS) $(GNU_LIBS) -Map smpscl5.bin
	
smpscl5.bin: smpscl5.sre
	sretobin smpscl5.sre $@

cof: smpscl5.cof

smpscl5.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(LDIR) $(SOJ_LIBS) $(GNU_LIBS) -Map smpscl5.bin

clean:
	del *.o

