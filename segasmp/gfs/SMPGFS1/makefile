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

SOJ_LIBS = -L../../../segalib/lib -lsat -lcdc -lper -lsat

OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	smpgfs1.o \
        ../../v_blank/v_blank.o \
        ../../v_blank/set_vb.o \
 	smpmath.o


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst


bin: smpgfs1.bin

smpgfs1.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map smpgfs1.map
	
smpgfs1.bin: smpgfs1.sre
	sretobin smpgfs1.sre $@

cof: smpgfs1.cof

smpgfs1.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map smpgfs1.map

clean:
	del *.o

