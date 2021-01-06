 #----------------------------------------------------------------------------
 #  SmpScl3 Makefile  
 #----------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = rm


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
	smpscl3.o \
        scldata.o \
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


bin: smpscl3.bin

smpscl3.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(LDIR) $(SOJ_LIBS) $(GNU_LIBS) -Map smpscl3.map
	
smpscl3.bin: smpscl3.sre
	sretobin smpscl3.sre $@

cof: smpscl3.cof

smpscl3.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(LDIR) $(SOJ_LIBS) $(GNU_LIBS) -Map smpscl3.map

clean:
	$(RM) $(OBJS)

