C_FILES   = stm_init.c stm_svr.c stm_trn.c stm_acc.c stm_loc.c
INC_FILES = sega_stm.h stm_loc.h stm_def.h
SRC_FILES = $(C_FILES) $(INC_FILES)
OBJ_FILES = stm_init.obj stm_svr.obj stm_trn.obj stm_acc.obj stm_loc.obj
C_OPTIONS = -sub=sega_stm.shc
OUT_LIB	  = sega_stm.lib

all: $(SRC_FILES) $(OBJ_FILES) $(OUT_LIB)

.SUFFIXES: .c .obj

.c.obj: 
	shc $(C_OPTIONS) $< -OB=$@

$(OUT_LIB): $(OBJ_FILES)
	rm sega_stm.lib
	lbr -sub=sega_stm.lbr

