 #----------------------------------------------------------------------------
 #  SmpSpr2 Makefile 
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

GNU_LIBS = -Lc:/gnu/lib -lgcc -lc -lm

LIBS = $(LDIR) -lsat -lper -lsat

OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	smpspr20.o

VB_OBJS = ../../v_blank/v_blank.o \
	  ../../v_blank/set_vb.o


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst


bin: smpspr2.bin


smpspr2.sre: $(OBJS)
	$(LD) -o $@ -T lnk_sre.x $(OBJS) $(VB_OBJS) $(LDIR) $(LIBS) $(GNU_LIBS) -Map smpspr2.map

smpspr2.bin: smpspr2.sre
	sretobin smpspr2.sre $@

cof: smpspr2.cof

smpspr2.cof: $(OBJS)
	$(LD) -o $@ -T lnk_cof.x $(OBJS) $(VB_OBJS) $(LDIR) $(LIBS) $(GNU_LIBS) -Map smpspr2.map

clean:
	del *.o

