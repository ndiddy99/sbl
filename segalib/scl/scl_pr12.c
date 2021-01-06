/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr12.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	Scroll Operation function program
 *
 *  AUTHOR(S):
 *	K.M
 *		
 *  MOD HISTORY:
 *	Written by K.M on 1994-07-01 Ver.1.00
 *	Updated by K.M on 1994-07-01 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#include <sega_scl.h> 

/* �J���[�E�I�t�Z�b�g��ݒ肷�� */
void  SCL_SetColOffset(Uint32 OffsetReg, Uint32 Surfaces,
			Sint16 red, Sint16 green, Sint16 blue)
{
    /* �I�t�Z�b�g���W�X�^�ɒl��ݒ� */
    if(OffsetReg == SCL_OFFSET_A){
	SCL_SET_COARD(red);
	SCL_SET_COAGR(green);
	SCL_SET_COABL(blue);
    }else{/* OFFSET_B */
	SCL_SET_COBRD(red);
	SCL_SET_COBGR(green);
	SCL_SET_COBBL(blue);
    }
    /* �C�l�[�u���^�Z���N�g�ݒ� */
    if(Surfaces & SCL_SP0){
	SCL_SET_SPCOEN(1);
	SCL_SET_SPCOSL(OffsetReg);
    }
    if(Surfaces & SCL_NBG0){
	SCL_SET_N0COEN(1);
	SCL_SET_N0COSL(OffsetReg);
    }
    if(Surfaces & SCL_NBG1){
	SCL_SET_N1COEN(1);
	SCL_SET_N1COSL(OffsetReg);
    }
    if(Surfaces & SCL_NBG2){
	SCL_SET_N2COEN(1);
	SCL_SET_N2COSL(OffsetReg);
    }
    if(Surfaces & SCL_NBG3){
	SCL_SET_N3COEN(1);
	SCL_SET_N3COSL(OffsetReg);
    }
    if(Surfaces & SCL_RBG0){
	SCL_SET_R0COEN(1);
	SCL_SET_R0COSL(OffsetReg);
    }
}
