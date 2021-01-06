/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * SATURN PAD CONTROLL
 *
 */

 
#ifndef __SMP_PAD_H
#define __SMP_PAD_H


/*
 *  パッドデータのビットアサイン
 *  11   10    9     8       7     6 5 4 3     2    1    0
 *  MODE,TRG_X,TRG_Y,TRG_Z , START,A,C,B,RIGHT,LEFT,DOWN,UP
 */
#define	PAD_UP		0x0001
#define	PAD_DOWN	0x0002
#define	PAD_LEFT	0x0004
#define	PAD_RIGHT	0x0008
#define	PAD_B		0x0010
#define	PAD_C		0x0020
#define	PAD_A		0x0040
#define	PAD_START	0x0080
#define	PAD_Z		0x0100
#define	PAD_Y		0x0200
#define	PAD_X		0x0400
#define	PAD_R	        0x0800
#define PAD_L           0x1000

#define SMPA_REPCNT1   0x10         /* repeat count            */


/* 
 *   パッド読みとり時のウエイトマクロ
 */
#define SMPA_padwait() \
  set_imask(get_imask());\
  set_imask(get_imask());\
  set_imask(get_imask());\
  set_imask(get_imask());\
  set_imask(get_imask());\
  set_imask(get_imask());\
  set_imask(get_imask());


/* ---- FUNCTION PROTOTYPE ---- */
extern void SMPA_SwInit(void);
extern void SMPA_SwRead(void);

/* ---- GLOBAL WORK ---- */
GLOBAL Sint16 SMPA_pad_id1;         /* 1P PAD ID            */
GLOBAL Sint16 SMPA_pad_data1;       /* 1P PAD DATA          */
GLOBAL Sint16 SMPA_pad_edge1;       /* 1P PAD EDGE          */

GLOBAL Sint16 SMPA_pad_id2;         /* 2P PAD ID            */
GLOBAL Sint16 SMPA_pad_data2;       /* 2P PAD DATA          */
GLOBAL Sint16 SMPA_pad_edge2;       /* 2P PAD EDGE          */

GLOBAL Sint16 SMPA_pad_repdata;	    /* 1P Repeat PAD DATA   */
GLOBAL Sint8  SMPA_pad_repcnt1;	    /* Repeat Counter1	    */



#endif



