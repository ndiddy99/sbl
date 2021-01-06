/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   MODE SELECT
 *
 */


#ifndef __SMP_SLCT_H
#define __SMP_SLCT_H

typedef struct menu_info {
	Uint8 *msgadd;
	Uint8 execmode;
	} MENU_INFO;

typedef struct slct_info {
        struct menu_info *menuptr;         /* SELECT MENU ptr          */
        Sint16 wreg[4];                    /* etc work      0x08 bytes */
        Sint16 wcnt[4];                    /* etc work      0x08 bytes */
        Uint8 mode;                        /* mode number         1 2  */
        } SLCT_INFO;

SMTA_DefActWk(MDSELACT,                    /* ACTWK SYSTEM  0x10 bytes */
        struct menu_info *menuptr;         /* SELECT MENU ptr          */
        Sint16 wreg[4];                    /* etc work      0x08 bytes */
        Sint16 wcnt[4];                    /* etc work      0x08 bytes */
        SLCT_INFO sel;
        SPRITE sprite;                     /* SPRITE SYSTEM 0x50 bytes */
);


/* FUNCTION PROTOTYPE */
void SMSL_ModeSel(void);
void SMSL_SlctTblSet(MDSELACT *ix);
void SMSL_SlctAct(MDSELACT *ix);


/* GLOBAL */

#endif

