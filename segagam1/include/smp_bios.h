/*	Copyright(c) 1994 SEGA			*/
/*
 * グローバル変数を一つにまとめるためのファイル
 *
 *  
 */
#include "sega_xpt.h"
#include "sega_def.h"
#include "sega_mth.h"
#define _SPR2_
#define _SPR3_
#include "sega_spr.h" 
#include "sega_scl.h"
#include "sega_dma.h"
#include "sega_gfs.h"
#include "sega_int.h"

#include "smp_def.h"
#include "smp_ass.h"
#include "smp_main.h"
#include "smp_int.h"
#include "smp_task.h"
#include "smp_vdp1.h"
#include "smp_vdp2.h"
#include "smp_pad.h"
#include "smp_misc.h"
#include "smp_cdfs.h"
#include "smp_snd.h"
#include "smp_asc.h"
