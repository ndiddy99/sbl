/*----------------------------------------------------------------------------
 *  mth_rand.c -- MTH ���C�u���� ������������ ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  ���̃��C�u�����̓V�~�����[�V�����p���������������W���[���ŁA
 *  �ȉ��̃��[�`�����܂ށB
 *
 *  MTH_InitialRand           -  ���������̏�����
 *  MTH_GetRand               -  �����l��Ԃ�
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_mth.h"
#ifdef _SH
#include <machine.h>
#endif

static Uint32 randVal = 0;


/*************************************************************************
 *
 * NAME : MTH_InitialRand  -  Initialize Random Value
 *
 * PARAMETERS
 *
 * �@�@(1) Uint32   initVal       - <i> ���������̏����p�����[�^�l
 *
 * DESCRIPTION
 *
 *	MTH_GetRand �ŕԂ������̉��Z�p�����p�����[�^��ݒ肷��B�{������
 *      ���[�`�����Ăяo���Ȃ��Ɖ��Z�p�����p�����[�^�l�� 0 �ƂȂ��Ă���B
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    MTH_InitialRand(Uint32 initVal)
{
	/** BEGIN ************************************************************/
	randVal = initVal;
}


/*************************************************************************
 *
 * NAME : MTH_GetRand  -  Return Random Value
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	�Ăяo�����ɗ����l��Ԃ��B
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 *
 *************************************************************************
 */
Uint32  MTH_GetRand(void)
{
    /** BEGIN ************************************************************/
    /* randVal = randVal + randVal + randVal + randVal + randVal + 0x3573; */
    randVal = randVal * 5 + 0x3573;
    return  randVal;
}

/*  end of file */
