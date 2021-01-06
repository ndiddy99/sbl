CC            = shc
CFLAGS        = -sub=gfs.shc

OBJS	      = gfs.obj \
		gfs_buf.obj \
		gfs_cdb.obj \
		gfs_cdc.obj \
		gfs_cdf.obj \
		gfs_dir.obj \
		gfs_trn.obj

DOBJS	      = gfsd.obj \
		gfsd_buf.obj \
		gfsd_cdb.obj \
		gfsd_cdc.obj \
		gfsd_cdf.obj \
		gfsd_dir.obj \
		gfsd_mmb.obj \
		gfsd_mmc.obj \
		gfsd_mmf.obj \
		gfsd_sfb.obj \
		gfsd_sff.obj \
		gfsd_trn.obj

.SUFFIXES: .c .obj .src
.c.obj:
	$(CC) $< $(CFLAGS)

all:	$(OBJS) $(DOBJS) sega_gfs.lib segadgfs.lib

sega_gfs.lib: $(OBJS) gfs.lbr
	-rm $@
	lbr -sub=gfs.lbr

segadgfs.lib: $(DOBJS) gfsd1.lbr gfsd2.lbr
	-rm $@
	lbr -sub=gfsd1.lbr
	lbr -sub=gfsd2.lbr

###
gfs.obj gfsd.obj: sega_gfs.h gfs_def.h gfs_mmf.h gfs_mmb.h gfs_cdf.h \
	gfs_cdb.h gfs_dir.h gfs_buf.h gfs_trn.h gfs_cdc.h gfs_sf.h
gfsd.obj:	gfs.c
	$(CC) gfs.c -OB=$@ -DEF=DEBUG_LIB $(CFLAGS)

gfs_buf.obj gfsd_buf.obj: sega_gfs.h gfs_def.h gfs_buf.h
gfsd_buf.obj:	gfs_buf.c
	$(CC) gfs_buf.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_cdb.obj gfsd_cdb.obj: sega_gfs.h gfs_def.h gfs_cdf.h gfs_cdb.h gfs_cdc.h \
	gfs_trn.h
gfsd_cdb.obj:	gfs_cdb.c
	$(CC) gfs_cdb.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_cdc.obj gfsd_cdc.obj: sega_gfs.h gfs_cdc.h gfs_def.h
gfsd_cdc.obj:	gfs_cdc.c
	$(CC) gfs_cdc.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_cdf.obj gfsd_cdf.obj: sega_gfs.h gfs_def.h gfs_dir.h gfs_cdf.h gfs_cdb.h \
	gfs_cdc.h
gfsd_cdf.obj:	gfs_cdf.c
	$(CC) gfs_cdf.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_dir.obj gfsd_dir.obj : sega_gfs.h gfs_mmc.h gfs_def.h gfs_dir.h gfs_cdc.h \
	gfs_scsi.h
gfsd_dir.obj:	gfs_dir.c
	$(CC) gfs_dir.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_mmb.obj gfsd_mmb.obj: sega_gfs.h gfs_def.h gfs_mmf.h gfs_mmc.h gfs_mmb.h \
	gfs_trn.h
gfsd_mmb.obj:	gfs_mmb.c
	$(CC) gfs_mmb.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_mmc.obj gfsd_mmc.obj: gfs_mmc.h
gfsd_mmc.obj:	gfs_mmc.c
	$(CC) gfs_mmc.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_mmf.obj gfsd_mmf.obj: sega_gfs.h gfs_def.h gfs_mmf.h gfs_mmb.h gfs_mmc.h
gfsd_mmf.obj:	gfs_mmf.c
	$(CC) gfs_mmf.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfs_trn.obj gfsd_trn.obj: sega_gfs.h gfs_def.h gfs_trn.h gfs_buf.h
gfsd_trn.obj:	gfs_trn.c
	$(CC) gfs_trn.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

###

gfsd_sfb.obj: sega_gfs.h gfs_def.h gfs_sf.h gfs_scsi.h gfs_trn.h
gfsd_sfb.obj:	gfs_sfb.c
	$(CC) gfs_sfb.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)

gfsd_sff.obj: sega_gfs.h gfs_def.h gfs_scsi.h gfs_sf.h
gfsd_sff.obj:	gfs_sff.c
	$(CC) gfs_sff.c -OB=$@ -DEF=DEBUG_LIB  $(CFLAGS)








