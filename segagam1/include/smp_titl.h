/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *     TITLE MODE
 *
 */
 
#ifndef __SMP_TITLE_H
#define __SMP_TITLE_H

/* #include "smp_msg.h" */

/* SEGALOGO ACTION WORK DEFINE */
SMTA_DefActWk(TITLEACT,              /* ACTWK  SYSTEM 0x10 bytes */
         SPRITE sprite;              /* SPRITE SYSTEM 0x50 bytes */
         Sint16 wreg[4];             /* etc work      0x08 bytes */
         Sint16 wcnt[4];             /* etc work      0x08 bytes */
         Fixed32 lcnt[2];
);


/* FUNCTION PROTOTYPE */
extern void SMTI_Title(void);

#endif


