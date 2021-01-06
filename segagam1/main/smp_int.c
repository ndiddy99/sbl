/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * V_BLANK ���荞�ݏ���
 *
 */

#include "smp_bios.h"


/*
 *    �O���Q��
 */
extern void SCL_VblankStart(void);   /* ���C�u�����W�� VBLANKIN �֐�  */
extern void SCL_VblankEnd(void);     /* ���C�u�����W�� VBLANKOUT�֐�  */


Uint32 SMIN_vintcnt;        /* V-BLANK IN �J�E���^ */
Uint8 SMIN_vintflag;        /* Vint Flag           */
Uint32 SMIN_vintdwcnt;
void (*SMIN_VintHook1)(void);


/* ========================================================================= */
/*
 *  V_BLANK IN ���荞�ݏ���
 */
void SMIN_VblankIn(void)
{
    SCL_VblankStart();
    SMPA_SwRead(); 
    SMIN_vintcnt++;
    if (SMIN_vintdwcnt != 0) 
        SMIN_vintdwcnt--;

    if (SMIN_VintHook1)
        (*SMIN_VintHook1)();
}



/* ========================================================================= */
/*
 *    V_BLANK OUT ���荞�ݏ���
 */
void SMIN_VblankOut(void)
{
    SCL_VblankEnd();

}




/* ========================================================================= */
/*
 *    SPRITE �`��I�����荞��
 */
void SMIN_SpriteEnd(void)
{

}
