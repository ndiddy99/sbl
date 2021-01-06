/*	Copyright(c) 1994 SEGA			*/
/*
 *     
 *     NBG0ägëÂèkè¨ÉeÉXÉg
 *
 */
 
#ifndef __SMP_NBGZ_H
#define __SMP_NBGZ_H


/*
 *    SMNB_MODE SCROLL VRAM ASSIGN
 */
#define SMNB_NBG_PTGEN (SCR_VRAMA0 + 0x00000) /* NBG PTGEN                  */

#define SMNB_NBG0_PTNAMA (SCR_VRAMA1 + 0x10000)  /* NBG0 PTNAM              */




SMTA_DefActWk(SMNBACT,
	      SPRITE sprite;
              Sint16 wcnt[4];
              Sint16 wreg[4];
);


/* FUNCTION PROTOTYPE */
extern void SMNB_MainMode(void);

/* GLOBAL HENSUU */

#endif
 
