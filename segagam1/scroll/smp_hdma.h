/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      SCU_DMA で VDP2 へ 送る サンプル
 *
 */

 
#ifndef __SMP_HDMA_H
#define __SMP_HDMA_H


/*
 *    SCUDMA SAMPLE 
 */
#define SMHD_NBG_PTGEN   (SCR_VRAMA0 + 0x00000)  /* NBG PTGEN               */
#define SMHD_NBG0_PTNAMA (SCR_VRAMA0 + 0x10000)  /* NBG0 PTNAM              */
#define SMHD_NBG0_PTNAMB (SCR_VRAMA0 + 0x12000)
#define SMHD_NBG0_PTNAMC (SCR_VRAMA0 + 0x14000)
#define SMHD_NBG0_PTNAMD (SCR_VRAMA0 + 0x16000)

#define SMHD_NBG0_HLSCLTBL (SCR_VRAMA0 + 0x18000) /* NBG0 HLINE SCROLL TABLE */
#define SMHD_NBG0_VCSCLTBL (SCR_VRAMA0 + 0x18A00) /* NBG0 VCELL SCROLL TABLE */

#define SMHD_LCLRTBLADD (SCR_VRAMA0 + 0x19000)    /* Linecolortable 256 word */
#define SMHD_BCLRTBLADD (SCR_VRAMA0 + 0x19200)    /* Backcolortable 256 word */

#define SMHD_LWND0TBLADD (SCR_VRAMA0 + 0x19800)   /* LineWindow #0 256*2word */
#define SMHD_LWND1TBLADD (SCR_VRAMA0 + 0x1A000)   /* LineWindow #1 256*2word */

#define SMHD_RBG0_RPAR  (SCR_VRAMA0 + 0x1B000)    /* RBG0 ROT PARAM 256bytes */

#define SMHD_RBG0_PTGEN (SCR_VRAMA1 + 0x00000)    /* RBG0 PTGEN              */
#define SMHD_RBG0_KEISU (SCR_VRAMB0 + 0x00000)    /* RBG0 係数table          */
#define SMHD_RBG0_PTNAM (SCR_VRAMB1 + 0x00000)    /* RBG0 PTNAM              */


/* アクションワークアサイン */
SMTA_DefActWk(SMHDMAINACT,
              Sint16 wcnt[4];
              Sint16 wreg[4];
);


/* FUNCTION PROTOTYPE */
extern void SMHD_MainMode(void);


/* WORK */
Uint8 SMHD_CellBuf[0x100];

#endif
 
