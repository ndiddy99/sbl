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

GLOBAL_DEFS = -D_SH -DMODEL_S

OPTIMIZE_FLAGS = -finline-functions -fomit-frame-pointer

DEBUG_FLAGS = -g

CFLAGS = -m2 -g $(GLOBAL_DEFS)

INCLUDES = -I../../../segalib/include

ASFLAGS =

LDFLAGS =

GNU_LIBS = -Lc:/gnu/lib -lm -lgcc -lc -lgcc

SOJ_LIBS = -L../../../segalib/lib -lsat -lper -lsat

OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	smpdbg10.o \
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


bin: smpdbg1.bin


smpdbg1.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map smpdbg1.map

smpdbg1.bin: smpdbg1.sre
	sretobin smpdbg1.sre $@

cof: smpdbg1.cof


smpdbg1.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map smpdbg1.map

clean:
	del *.o

