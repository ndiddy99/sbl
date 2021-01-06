 #----------------------------------------------------------------------------
 #  smptim1 Makefile 
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

INCLUDES = -I../../../segalib/include

ASFLAGS =

LDFLAGS =

LDIR = -L../../../segalib/lib

GNU_LIBS = -Lc:/gnu/lib -lgcc -lc -lm -lgcc

LIBS = $(LDIR) -lsat -lper -lsat

OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	smptim1.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst


bin: smptim1.bin


smptim1.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(LDIR) $(LIBS) $(GNU_LIBS) -Map smptim1.map

smptim1.bin: smptim1.sre
	sretobin smptim1.sre $@

cof: smptim1.cof

smptim1.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(LDIR) $(LIBS) $(GNU_LIBS) -Map smptim1.map

clean:
	del *.o

