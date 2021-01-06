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

#ifdef MODEL_M
#define PAD_UP		0x0001
#define PAD_DOWN	0x0002
#define PAD_LEFT	0x0004
#define PAD_RIGHT	0x0008
#define PAD_B			0x0010
#define PAD_C			0x0020
#define PAD_A			0x0040
#define PAD_START	0x0080
#define PAD_Z			0x0100
#define PAD_Y			0x0200
#define PAD_X		0x0400
#define PAD_MODE	0x0800
#define	PAD_R	    0x0800
#define PAD_L     0x1000
#elif MODEL_S
/* direct mode */
#define PAD_DOWN	0x0001
#define PAD_UP  	0x0002
#define PAD_LEFT	0x0004
#define PAD_RIGHT	0x0008
#define PAD_C			0x0010
#define PAD_B			0x0020
#define PAD_A			0x0040
#define PAD_START	0x0080
#define PAD_Z			0x0100
#define PAD_Y			0x0200
#define PAD_X			0x0400
#define PAD_MODE	0x0800
#define	PAD_R	    0x0800
#define PAD_L     0x1000
#endif


/* ---- FUNCTION PROTOTYPE ---- */
void SMPA_SwInit(void);
void SMPA_SwRead(void);


extern Sint16 SMPA_pad_id1;         /* 1P PAD ID            */
extern Sint16 SMPA_pad_data1;       /* 1P PAD DATA          */
extern Sint16 SMPA_pad_edge1;       /* 1P PAD EDGE          */

extern Sint16 SMPA_pad_id2;         /* 2P PAD ID            */
extern Sint16 SMPA_pad_data2;       /* 2P PAD DATA          */
extern Sint16 SMPA_pad_edge2;       /* 2P PAD EDGE          */

extern Sint16 SMPA_pad_repdata;	    /* 1P Repeat PAD DATA   */
extern Sint8  SMPA_pad_repcnt1;	    /* Repeat Counter1	    */

#endif



