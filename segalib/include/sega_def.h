/*
 * �V�X�e�����́F�\�t�g�E�F�A���C�u����
 *
 * ���W���[���L�����́Fsega_def.h
 *
 * ���W���[�����́F�}�V���ˑ����ʃw�b�_�t�@�C��
 *
 * �ړI�F���C�u�������ʂ̃w�b�_�t�@�C���ŁAsega_xpt.h ��蒊�ۓx�̒Ⴂ���̂�
 *       �}�V���Ɉˑ�������̂��`����B
 *
 * AUTHOR�FVer1.00  H.E 1994-03-22
 *
 * DATE�F1994-03-22
 */


#ifndef SEGA_DEF_H
#define SEGA_DEF_H

#include "sega_xpt.h"

/*
 * MACROS DECLARATIONS
 */
#define  RGB16_COLOR(r,g,b)  (Rgb16)(((b)<<10) + ((g)<<5) + (r) + 0x8000)
                                        /* Make 16 bit RGB Color             */
#define  RGB32_COLOR(r,g,b)  (Rgb32)(((b)<<16) + ((g)<<8) + (r) + 0x80000000)
                                        /* Make 32 bit RGB Color (FullColor) */

/*
 * DATA TYPES DECLARATIONS
 */
typedef  Uint16	     Rgb16;		/* 16 bit RGB Color                  */
typedef  Uint32	     Rgb32;		/* 32 bit RGB Color (Full Color)     */

typedef struct  XyInt {                 /* Screen 2 Dimension Point Data     */
	 Sint16      x;
	 Sint16      y;
} XyInt;

#endif  /* SEGA_DEF_H */
