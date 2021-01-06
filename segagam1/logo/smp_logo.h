/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *     SEGA LOGO MODE
 *
 */
 
#ifndef __SMP_LOGO_H
#define __SMP_LOGO_H


/* SEGALOGO ACTION WORK DEFINE */
SMTA_DefActWk(LOGOACT,
         Sint16 wreg[4];
         Sint16 wcnt[4];
         SPRITE sprite;
);


/* FUNCTION PROTOTYPE */
extern void SMLO_SegaLogo(void);

#endif

 













