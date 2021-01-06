#--------------------------------------------------------------------------
# File Name: Makefile   File Title: Makefile for SOJ Saturn Program Libs
# Copyright (c) 1994 Sega of America, Inc. All Rights Reserved.
#
# Author(s): rcs reh
# Date: 10-18-94
# Description:	Makefile to create Saturn system libraries
#		Assumes GNUmake and DOS environment
#
#--------------------------------------------------------------------------

CC = sh-elf-gcc
AS = sh-elf-as
LD = sh-elf-ld
AR = sh-elf-ar
RM = rm

## Grab any EXTRA_FLAGS we might need
include global/global.mk
## Get Pattern Rules
include global/global.pat

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES = -Iinclude -I$(GCC_DIR)/include

ASFLAGS = $(EXTRA_AFLAGS)

LDFLAGS = $(EXTRA_LDFLAGS)

LDIR = lib

## Saturn Library files
SAT_LIBS = \
	   $(LDIR)/libcdc.a \
	   $(LDIR)/libcmp.a \
	   $(LDIR)/libcsh.a \
	   $(LDIR)/libdbg.a \
	   $(LDIR)/libdma.a \
	   $(LDIR)/libdsp.a \
	   $(LDIR)/libgfs.a \
	   $(LDIR)/libmem.a \
	   $(LDIR)/libmth.a \
	   $(LDIR)/libscl.a \
	   $(LDIR)/libspr.a \
	   $(LDIR)/libsnd.a \
	   $(LDIR)/libstm.a \
	   $(LDIR)/libsat.a

LLDIR = -Llib

GNU_LIBS = -lgcc -lc -lm -lgcc

## Object Files
CDC_OBJS = cdc/cdc_reg.o \
		cdc/cdc_sub.o \
		cdc/cdc_cmn.o \
		cdc/cdc_drv.o \
		cdc/cdc_scd.o \
		cdc/cdc_dev.o \
		cdc/cdc_sel.o \
		cdc/cdc_bif.o \
		cdc/cdc_bio.o \
		cdc/cdc_cfs.o \
		cdc/cdc_unk.o \
		cdc/cdc_mdc.o \
		cdc/cdc_mst.o \
		cdc/cdc_mwn.o \
		cdc/cdc_mfb.o \
		cdc/cdc_mbu.o \
		cdc/cdc_mls.o \
		cdc/cdc_msb.o

CMP_OBJS = cmp/cmplrun.o  \
	   cmp/cmplrunb.o \
	   cmp/cmplrund.o \
	   cmp/cmplrunw.o \
	   cmp/cmpltool.o

CSH_OBJS = csh/csh_main.o

DBG_OBJS = dbg/dbg_disp.o \
	   dbg/dbg_moni.o \
	   dbg/dbg_intr.o

DMA_OBJS = dma/dma_cpu0.o \
	   dma/dma_cpu1.o \
	   dma/dma_cpu2.o \
	   dma/dma_cpu3.o \
	   dma/dma_cpu4.o \
	   dma/dma_cpu5.o \
	   dma/dma_scu0.o \
	   dma/dma_scu1.o

DSP_OBJS = dsp/dsp_ctrl.o

GFS_OBJS = gfs/gfs.o \
	   gfs/gfs_buf.o \
	   gfs/gfs_cdb.o \
	   gfs/gfs_cdc.o \
	   gfs/gfs_cdf.o \
	   gfs/gfs_dir.o \
	   gfs/gfs_trn.o


MEM_OBJS = mem/mem_call.o \
	   mem/mem_free.o \
	   mem/mem_init.o \
	   mem/mem_mall.o \
	   mem/mem_real.o \
	   mem/mem_clr.o

MTH_OBJS = mth/mth_curv.o \
	   mth/mth_rand.o \
	   mth/mth_dsp.o  \
	   mth/mth_dsps.o \
	   mth/mth_fixd.o \
	   mth/mth_tri.o  \
	   mth/mth_mtrx.o \
	   mth/mth_ps2d.o \
	   mth/mth_norm.o
	   
PER_OBJS = per/per_prt.o

SCL_OBJS = scl/scl_func.o \
	   scl/scl_fu01.o \
	   scl/scl_fu02.o \
	   scl/scl_fu03.o \
	   scl/scl_fu04.o \
	   scl/scl_fu05.o \
	   scl/scl_pri.o  \
	   scl/scl_ro00.o \
	   scl/scl_ro01.o \
	   scl/scl_ro02.o \
	   scl/scl_ro03.o \
	   scl/scl_ro04.o \
	   scl/scl_ro05.o \
	   scl/scl_ro10.o \
	   scl/scl_win0.o \
	   scl/scl_win1.o \
	   scl/scl_win2.o \
	   scl/scl_win3.o \
	   scl/scl_mosa.o \
	   scl/scl_lncl.o \
	   scl/scl_back.o \
	   scl/scl_vblv.o \
	   scl/scl_pr01.o \
	   scl/scl_pr02.o \
	   scl/scl_pr03.o \
	   scl/scl_pr04.o \
	   scl/scl_pr05.o \
	   scl/scl_pr06.o \
	   scl/scl_pr07.o \
	   scl/scl_pr08.o \
	   scl/scl_pr09.o \
	   scl/scl_pr10.o \
	   scl/scl_pr11.o \
	   scl/scl_pr12.o \
	   scl/scl_pr13.o \
	   scl/scl_pr14.o


SND_OBJS = snd/snd_main.o


SPR_OBJS = spr/spr_1c.o \
	   spr/spr_2a.o \
	   spr/spr_2c.o \
	   spr/spr_3a.o \
	   spr/spr_3c.o

STM_OBJS = stm/stm_init.o \
	   stm/stm_svr.o  \
	   stm/stm_trn.o  \
	   stm/stm_acc.o  \
	   stm/stm_loc.o


## Call this to make all libraries
all: $(SAT_LIBS)

cdc: $(LDIR)/libcdc.a
cmp: $(LDIR)/libcmp.a
csh: $(LDIR)/libcsh.a
dbg: $(LDIR)/libdbg.a
dma: $(LDIR)/libdma.a
dsp: $(LDIR)/libdsp.a
gfs: $(LDIR)/libgfs.a
mem: $(LDIR)/libmem.a
mth: $(LDIR)/libmth.a
per: $(LDIR)/libper.a
scl: $(LDIR)/libscl.a
spr: $(LDIR)/libspr.a
snd: $(LDIR)/libsnd.a
stm: $(LDIR)/libstm.a
sat: $(LDIR)/libsat.a

$(LDIR)/libcdc.a: $(CDC_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libcmp.a: $(CMP_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libcsh.a: $(CSH_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libdbg.a: $(DBG_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libdma.a: $(DMA_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libdsp.a: $(DSP_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libgfs.a: $(GFS_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libmem.a: $(MEM_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libmth.a: $(MTH_OBJS)
	$(AR) rv $@ $?
	
$(LDIR)/libper.a: $(PER_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libscl.a: $(SCL_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libspr.a: $(SPR_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libsnd.a: $(SND_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libstm.a: $(STM_OBJS)
	$(AR) rv $@ $?

$(LDIR)/libsat.a: \
		  $(CDC_OBJS) \
		  $(CMP_OBJS) \
		  $(CSH_OBJS) \
		  $(DBG_OBJS) \
		  $(DMA_OBJS) \
		  $(DSP_OBJS) \
		  $(GFS_OBJS) \
		  $(MEM_OBJS) \
		  $(MTH_OBJS) \
		  $(PER_OBJS) \
		  $(SPR_OBJS) \
		  $(SCL_OBJS) \
		  $(SND_OBJS) \
		  $(STM_OBJS) \

	$(AR) rv $@ $?

cleanlibs:
	$(RM) $(SAT_LIBS)

clean:
	$(RM) lib/*.a
	$(RM) cdc/*.o
	$(RM) cmp/*.o
	$(RM) csh/*.o
	$(RM) dbg/*.o
	$(RM) dma/*.o
	$(RM) dsp/*.o
	$(RM) gfs/*.o
	$(RM) mem/*.o
	$(RM) mth/*.o
	$(RM) per/*.o
	$(RM) scl/*.o
	$(RM) spr/*.o
	$(RM) snd/*.o
	$(RM) stm/*.o


#----------------------------------------------------------------------
# File Name: Makefile
# Copyright (c) 1994 Sega of America, Inc. All Rights Reserved.
# End of File
#
#----------------------------------------------------------------------

