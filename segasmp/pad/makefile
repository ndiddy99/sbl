 #----------------------------------------------------------------------------
 #  SmpScl2.Mak -- スクロールサンプルプログラムＭａｋｅファイル
 #			（MS-C Ver 5.1添付 make.exe形式）
 #  Copyright(c) 1994 SEGA
 #  Written by K.M on 1994-07-29 Ver.1.00
 #  Updated by K.M on 1994-07-29 Ver.1.00
 #----------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = rm

## Grab any EXTRA_FLAGS we might need
include ../../segalib/global/global.mk

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -I../../segalib/include

ASFLAGS = $(EXTRA_ASFLAGS)

LDFLAGS = $(EXTRA_LDFLAGS)

LDIR = -L ../../segalib/lib

GNU_LIBS = -Lc:/gnu/lib -lgcc -lc -lm

LIBS = -lsega

OBJS =	smp_pad.o 


## Pick any pattern rules we might need
include ../../segalib/global/global.pat

all: $(OBJS)

clean:
	$(RM) $(OBJS)

