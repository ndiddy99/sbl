 #----------------------------------------------------------------------------
 #  SBL Sample Makefile 
 #----------------------------------------------------------------------------

CC = sh-elf-gcc
AS = sh-elf-as
LD = sh-elf-ld
AR = sh-elf-ar
OBJCOPY = sh-elf-objcopy
RM = rm

TARGET = smpscl1

GLOBAL_DEFS = -D_SH -DMODEL_S

OPTIMIZE_FLAGS = -finline-functions -fomit-frame-pointer

DEBUG_FLAGS = -g

CFLAGS = -m2 $(GLOBAL_DEFS)

INCLUDES = -I../../../segalib/include

ASFLAGS =

LDFLAGS = -T lnk_elf.x -e _entry -nostartfiles -Wl,-Map,$(TARGET).map

LDIR = ../../../segalib/lib

GNU_LIBS =

LIBS = $(LDIR)/libsat.a


OBJS =	../../entry/entry.o \
	../../entry/stack.o \
	../../v_blank/v_blank.o \
	../../v_blank/set_vb.o \
	$(TARGET).o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst



all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS)
	# $(LD) -o $@ -T lnk_elf.x $(OBJS) $(LDIR) $(LIBS) $(GNU_LIBS) -Map smpscl1.map
	$(CC) $(LDFLAGS) -o $(TARGET).elf -Xlinker -Map -Xlinker $(TARGET).map $(OBJS) $(LIBS)

clean:
	rm *.o

