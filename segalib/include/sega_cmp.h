/*******************************************************************
*
*                �@�@�@ �T�^�[���p�f�[�^���k
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: �L�����C�u�����^���p�җp�w�b�_
*   File   : SEGA_CMP.H
*   Date   : 1994-01-18
*   Author : Y.T
*
*******************************************************************/
#ifndef SEGA_CMP2_H
#ifndef SEGA_CMP_H
#define SEGA_CMP_H

/* ��{�I�ȃf�[�^�`�Ȃǂ��` */
#include "sega_xpt.h"


/*******************************************************************
      �萔�}�N��
*******************************************************************/

/* �L�����C�u�����֐��̖߂�l�B�i�S�Ẵ��C�u�����֐��ɋ��ʁj 	*/
#define CMP_DEC_OK 			(0L)		/* ����I�� 			*/
#define CMP_DEC_STOP 		(1L)		/* �L���������f 		*/
#define CMP_DEC_ERR 		(-1L)		/* �ُ�I�� 			*/
										/* �s���w�b�_			*/
#define CMP_DEC_ERR_H_ALGO 	(-2L)		/* �@���Ή��A���S���Y��	*/
#define CMP_DEC_ERR_H_UNIT 	(-3L)		/* �@���Ή������P��		*/


/*******************************************************************
      �v���g�^�C�v�錾
*******************************************************************/

/*    ���������O�X�L��        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlen(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlen();
#endif

/*    ���������O�X�L���^�c�v�n�q�c        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenDword(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenDword();
#endif

/*    ���������O�X�L���^�v�n�q�c�P��        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenWord(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenWord();
#endif

/*    ���������O�X�L���^�a�x�s�d�P��        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenByte(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenByte();
#endif

#endif	/* SEGA_CMP_H */
#endif	/* SEGA_CMP2_H */
