 #----------------------------------------------------------------------------
 #  Smpbup1 Makefile 
 #----------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = rm

GLOBAL_DEFS = -D_SH -DMODEL_S

OPTIMIZE_FLAGS = -finline-functions -fomit-frame-pointer

DEBUG_FLAGS = -g

CFLAGS = -m2 $(GLOBAL_DEFS)

INCLUDES = -I../../segalib/include

ASFLAGS =

LDFLAGS =

LDIR = -L../../segalib/lib

GNU_LIBS = -Lc:/gnu/lib -lc -lgcc -lm

LIBS = -L../../segalib/lib  -lper -lscl -dma -lmth -lspr

OBJS =	../entry/entry.o \
	../entry/stack.o \
	smpbup1.o

VB_OBJS = ../v_blank/v_blank.o \
	  ../v_blank/set_vb.o


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst




bin: smpbup1.bin

objects: $(OBJS)

smpbup1.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(VB_OBJS) $(LDIR) $(LIBS) font/smp_font.a $(GNU_LIBS) -Map smpbup1.map
	
smpbup1.bin: smpbup1.sre
	sretobin smpbup1.sre $@

cof: smpbup1.cof


smpbup1.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(VB_OBJS) $(LDIR) $(LIBS) font/smp_font.a $(GNU_LIBS) -Map smpbup1.map

clean:
	$(RM) $(OBJS)

