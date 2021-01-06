/*-----------------------------------------------------------------------------
 *  FILE: per_def.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      SMPC�n�[�h�֘A�w�b�_�t�@�C��
 *
 *  DESCRIPTION:
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_DEF_H
#define PER_DEF_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* smpc conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* ���W�X�^�A�h���X **********************************************************/
#ifndef _DEB
#define REG_PDRA    ((Uint8 *)0x20100075)       /* PDRA                      */
#define REG_DDRA    ((Uint8 *)0x20100079)       /* DDRA                      */
#define REG_PDRB    ((Uint8 *)0x20100077)       /* PDRB                      */
#define REG_DDRB    ((Uint8 *)0x2010007b)       /* DDRB                      */
#define REG_IOSEL   ((Uint8 *)0x2010007d)       /* IOSELB,IOSELA             */
#define REG_EXLE    ((Uint8 *)0x2010007f)       /* EXLEB,EXLEA               */
#else
#define REG_PDRA    ((Uint8 *)0x06030075)       /* PDRA                      */
#define REG_DDRA    ((Uint8 *)0x06030079)       /* DDRA                      */
#define REG_PDRB    ((Uint8 *)0x06030077)       /* PDRB                      */
#define REG_DDRB    ((Uint8 *)0x0603007b)       /* DDRB                      */
#define REG_IOSEL   ((Uint8 *)0x0603007d)       /* IOSELB,IOSELA             */
#define REG_EXLE    ((Uint8 *)0x0603007f)       /* EXLEB,EXLEA               */
#endif /* _DEB */

/* �r�b�g�ʒu ****************************************************************/
/***** IOSEL */
#define B_IOSELB    (1 << 1)                    /* IOSELB                    */
#define B_IOSELA    (1 << 0)                    /* IOSELA                    */
/***** EXLE */
#define B_EXLEB     (1 << 1)                    /* EXLEB                     */
#define B_EXLEA     (1 << 0)                    /* EXLEA                     */
/***** �X�e�[�^�X���W�X�^ */
#define B_SR_PDE    (1 << 5)                    /* �c���PC�f�[�^            */
#define B_SR_RESB   (1 << 4)                    /* �z�b�g���Z�b�g�{�^�����  */

/* �萔 **********************************************************************/
/***** �C���g�o�b�N�֘A */
/********* IREG0 */
#define SM_GET_ON   (0x1)                       /* SM�f�[�^��Ԃ�            */
#define SM_GET_OFF  (0x0)                       /* SM�f�[�^��Ԃ��Ȃ�        */
/********* IREG0 */
/************* �|�[�g1���[�h */
#define P1MD_15     (0x0 << 4)                  /* 15�o�C�g���[�h            */
#define P1MD_255    (0x1 << 4)                  /* 255�o�C�g���[�h           */
#define P1MD_0      (0x3 << 4)                  /* 0�o�C�g���[�h             */
/************* �|�[�g2���[�h */
#define P2MD_15     (0x0 << 6)                  /* 15�o�C�g���[�h            */
#define P2MD_255    (0x1 << 6)                  /* 255�o�C�g���[�h           */
#define P2MD_0      (0x3 << 6)                  /* 0�o�C�g���[�h             */
/************* �y���t�F�����f�[�^�C�l�[�u�� */
#define PEN_RET     (0x1 << 3)                  /* PC�f�[�^��Ԃ�            */
#define PEN_NRET    (0x0 << 3)                  /* PC�f�[�^��Ԃ��Ȃ�        */
/************* �擾���ԍœK�� */
#define OPE_ON      (0x0 << 1)                  /* �œK�����s��              */
#define OPE_OFF     (0x1 << 1)                  /* �œK�����s��Ȃ�          */
/******** �X�e�[�^�X���W�X�^ */
#define SR_PDE_ON   (0x1 << 5)                  /* �c��y���t�F��������
#define SR_PDE_OFF  (0x0 << 5)                  /* �c��y���t�F�����Ȃ�

/*
 * TYPEDEFS
 */
/* �f�o�C�X�f�[�^�^ */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
#endif  /* ifndef PER_DEF_H */
