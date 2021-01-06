#--------------------------------------------------------------------------
# File Name: Makefile   File Title: Makefile for SOJ Saturn Library Demo
# Copyright (c) 1995 Sega of America, Inc. All Rights Reserved.
#
# Author(s): reh
# Date: 1-29-95
# Description:	Makefile to create Saturn system library demo
#		Assumes GNUmake and DOS environment
#
#--------------------------------------------------------------------------

CC = gcc
AS = as
LD = ld
AR = ar
CPP = cpp
RM = del


## Grab any EXTRA_FLAGS we might need
include ../segalib/global/global.mk
## Get Pattern Rules
include ../segalib/global/global.pat


CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -I../segalib/include -Iinclude

ASFLAGS =

LDFLAGS = $(EXTRA_LDFLAGS)

LDIR = lib

## Saturn Library Demo Binary files
GAME_BINS = smp_krnl.bin \
	    logo.bin \
	    titl.bin \
	    scrl.bin \
            sprt.bin \
	    wind.bin \
            gm2d.bin \
	    d214.bin

LLDIR = -Llib

GNU_LIBS = -Lc:/gnu/lib -lgcc -lc -lm

SOJ_LIBS = -L../segalib/lib -lsat -lcdc -lper -lsat


## Object Files

SMP_KRNL_OBJS = main/entry.o \
                main/stack.o \
                main/smp_main.o \
                main/smp_int.o \
                main/smp_task.o \
                main/smp_vdp1.o \
                main/smp_vdp2.o \
                main/smp_pad.o \
                main/smp_misc.o \
                main/smp_cdfs.o \
                main/smp_snd.o \
                main/smp_dcmp.o \
                main/smp_asc.o \
                main/smp_slct.o \
                main/smp_slcg.o \
                main/smp_sqrt.o 

SMP_LOGO_OBJS = logo/logoent.o \
		logo/smp_logo.o \
                logo/smp_lgcg.o 

SMP_TITL_OBJS = title/titlent.o \
		title/smp_titl.o


SMP_SCRL_OBJS = scroll/scrlent.o \
		scroll/smp_scl0.o \
		scroll/smp_scl1.o \
		scroll/smp_scl2.o \
		scroll/smp_scl3.o \
		scroll/smp_lclr.o \
		scroll/smp_lnsc.o \
		scroll/smp_sccg.o \
		scroll/smp_ncg.o \
		scroll/smp_rkei.o


SMP_SPRT_OBJS = sprite/sprtent.o \
		sprite/smp_spr.o\
		sprite/smp_spr2.o


SMP_WIND_OBJS = window/windent.o\
		window/smp_wind.o \
                window/smp_wicg.o 




SMP_GM2D_OBJS = game2d/gament.o \
		game2d/smp_g2d.o \
                game2d/smp_g2d1.o \
                game2d/smp_g2cg.o


SMP_D214_OBJS = d214/d214ent.o \
		d214/smp_d214.o \
		d214/smp_dcg0.o \
		d214/smp_dcg1.o \
		d214/smp_dcg2.o \
		d214/smp_dcg3.o \
		d214/smp_dcg4.o \
		d214/smp_dcg5.o \
		d214/smp_dcg6.o \
		d214/smp_dswd.o 

## Call this to make all binaries
all: $(GAME_BINS)


main/kernal.cof : $(SMP_KRNL_OBJS)

krnl: smp_krnl.bin

smp_krnl.bin : $(SMP_KRNL_OBJS)
	$(LD) -o main/kernal.cof -T main/kernal.lnk $(SMP_KRNL_OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map main/kernal.map
	$(LD) -o main/smp_krnl.sre -T main/smp_krnl.lnk $(SMP_KRNL_OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map main/krnl.map
	sretobin main/smp_krnl.sre smp_krnl.bin


logo: logo.bin

logo.bin : $(SMP_LOGO_OBJS)
	$(LD) -R main/kernal.cof -o logo/logo.sre -T smp_overlay.lnk $(SMP_LOGO_OBJS) $(SOJ_LIBS) -Map logo.map
	sretobin logo/logo.sre logo.bin


titl: titl.bin

titl.bin : $(SMP_TITL_OBJS)
	$(LD) -R main/kernal.cof -o title/titl.sre -T smp_overlay.lnk $(SMP_TITL_OBJS) $(SOJ_LIBS) -Map titl.map
	sretobin title/titl.sre titl.bin


scrl: scrl.bin

scrl.bin : $(SMP_SCRL_OBJS)
	$(LD) -R main/kernal.cof -o scroll/scrl.sre -T smp_overlay.lnk $(SMP_SCRL_OBJS) $(SOJ_LIBS) -Map scrl.map
	sretobin scroll/scrl.sre scrl.bin


sprt: sprt.bin

sprt.bin : $(SMP_SPRT_OBJS)
	$(LD) -R main/kernal.cof -o sprite/sprt.sre -T smp_overlay.lnk $(SMP_SPRT_OBJS) $(SOJ_LIBS) -Map sprt.map
	sretobin sprite/sprt.sre sprt.bin


wind: wind.bin

wind.bin : $(SMP_WIND_OBJS)
	$(LD) -R main/kernal.cof -o window/wind.sre -T smp_overlay.lnk $(SMP_WIND_OBJS) $(SOJ_LIBS) -Map wind.map
	sretobin window/wind.sre wind.bin


gm2d: gm2d.bin

gm2d.bin : $(SMP_GM2D_OBJS)
	$(LD) -R main/kernal.cof -o game2d/gm2d.sre -T smp_overlay.lnk $(SMP_GM2D_OBJS) $(SOJ_LIBS) -Map gm2d.map
	sretobin game2d/gm2d.sre gm2d.bin


d214: d214.bin

d214.bin : $(SMP_D214_OBJS)
	$(LD) -R main/kernal.cof -o d214/d214.sre -T smp_overlay.lnk $(SMP_D214_OBJS) $(SOJ_LIBS) -Map d214.map
	sretobin d214/d214.sre d214.bin






clean:
	del *.bin
	del main\*.o
	del logo\*.o
	del title\*.o
	del scroll\*.o
	del sprite\*.o
	del window\*.o
	del game2d\*.o
	del d214\*.o

#----------------------------------------------------------------------
# File Name: Makefile
# Copyright (c) 1995 Sega of America, Inc. All Rights Reserved.
# End of File
#
#----------------------------------------------------------------------

