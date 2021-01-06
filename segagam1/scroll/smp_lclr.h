/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *     ���C���J���[���g�����T���v��
 *
 */
 
#ifndef __SMP_LCLR_H
#define __SMP_LCLR_H

#include "smp_def.h"




typedef struct PLYOBJ {
    MthXyz direc;           /* direc         12   0 */
    MthXyz posi;            /* posi          12  12 */
    MthXyz speed;           /* speed         24  12 */
    MthXyz acc;             /* acc           36  12 */
    SprCluster *clsptr;     /* cluster ptr   48   4 */
                            /* TOTAL       52 bytes */            
} PLYOBJ;



/*
 *     SPRP action work assign
 */
SMTA_DefActWk(G3DACT,      /* 0x10 bytes */
         Sint32 lreg[4];    /* 0x10 bytes */
         Sint32 lcnt[4];   /* 0x10 bytes */
         PLYOBJ obj;      /* 0x40 bytes */
);


/* 
 *     �֐��v���g�^�C�v
 */
extern void SMLC_LclrMode(void);




#endif

 













