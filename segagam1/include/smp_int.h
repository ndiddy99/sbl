/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * V_BLANK���荞�ݏ���
 *
 *
 */

#ifndef __SMP_INT_H
#define __SMP_INT_H


/* �֐��v���g�^�C�v */
extern void SMIN_VblankIn(void);
extern void SMIN_VblankOut(void);
extern void SMIN_SpriteEnd(void);


/* �O���[�o���ϐ��@�錾 */
GLOBAL Uint32 SMIN_vintcnt;        /* V-BLANK IN �J�E���^ */
GLOBAL Uint8 SMIN_vintflag;        /* Vint Flag           */
GLOBAL Uint32 SMIN_vintdwcnt;
GLOBAL void (*SMIN_VintHook1)(void);

#endif  /* endif __SMP_INT_H */



