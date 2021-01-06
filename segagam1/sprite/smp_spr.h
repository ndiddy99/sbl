/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   SPRITE SAMPLE HEADER FILE
 *
 */

/* INCLUDE FILE */

#ifndef __SMP_SPR_H
#define __SMP_SPR_H

/* TEISUU DEF */

/* íËêîêÈåæ */
SMTA_DefActWk(SPACT,
         Sint16 wreg[4];
         Sint16 wcnt[4];
	 void *chlnk;
         SPRITE sprite;
);

/* FUNCTION PROTOTYPE */
void SMSP_DemoMode(void);

/* GLOBAL */
Uint8  SMSP_SelectLevel;
/*---------------------------------------------------------------------------*/

#endif
