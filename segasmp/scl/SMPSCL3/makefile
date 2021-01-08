 #----------------------------------------------------------------------------
 #  SBL Sample Makefile 
 #----------------------------------------------------------------------------

CC = sh-elf-gcc
AS = sh-elf-as
LD = sh-elf-ld
AR = sh-elf-ar
OBJCOPY = sh-elf-objcopy
RM = rm


## Grab any EXTRA_FLAGS we might need
include ../../../segalib/global/global.mk

TARGET = smpscl3

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -I../../../segalib/include

ASFLAGS = $(EXTRA_ASFLAGS)

LDFLAGS = -T lnk_elf.x -e _entry -nostartfiles -Wl,-Map,$(TARGET).map

LDIR = ../../../segalib/lib

LIBS = $(LDIR)/libsat.a


OBJS = \
	../../entry/entry.o \
	../../entry/stack.o \
	../../v_blank/v_blank.o \
	../../v_blank/set_vb.o \
	scldata.o \
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
	$(CC) $(LDFLAGS) -o $(TARGET).elf -Xlinker -Map -Xlinker $(TARGET).map $(OBJS) $(LIBS)

clean:
	$(RM) *.o

