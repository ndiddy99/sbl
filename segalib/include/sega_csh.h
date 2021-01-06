/*------------------------------------------------------------------------------
 *  FILE:
 *      $Author: $Y.K
 *      $Date: $1994-05-10
 *      $Locker: $
 *      $Revision: $0.90
 *      $Source: $
 *      $State: $
 *
 *      Copyright (C) by SEGA Enterprises Ltd. 1994. All rights reserved.
 *
 *  PURPOSE:
 *		Cache Simulation program header file
 *		Cache Register Data Structure Definition File
 *
 *  DESCRIPTION:
 *
 *
 *  AUTHOR(S):
 *		Y.K
 *
 *  MOD HISTORY:
 *
 *
 *------------------------------------------------------------------------------*/

/****************************************************************************/
/*	���W�X�^��`															*/
/****************************************************************************/
#define CSH_CCR			((Uint16 *)0xfffffe92)	/*	��������۰�ڼ޽����ڽ	*/

/****************************************************************************/
/*	�萔��`																*/
/****************************************************************************/
#define CSH_4WAY			((Uint16)0x0000)	/*	4�����ı����è��Ӱ��	*/
#define CSH_2WAY			((Uint16)0x0008)	/*	2�����ı����è��Ӱ��	*/
#define CSH_DISABLE			((Uint16)0x0000)	/*	������ި�����Ӱ��		*/
#define CSH_ENABLE			((Uint16)0x0001)	/*	������Ȱ���Ӱ��			*/
#define CSH_CODE_ENABLE		((Uint16)0x0000)	/*	���޲Ȱ���Ӱ��			*/
#define CSH_CODE_DISABLE	((Uint16)0x0002)	/*	�����ި�����Ӱ��		*/
#define CSH_DATA_ENABLE		((Uint16)0x0000)	/*	�ް��Ȱ���Ӱ��			*/
#define CSH_DATA_DISABLE	((Uint16)0x0004)	/*	�ް��ި�����Ӱ��		*/

/****************************************************************************/
/*	�ᐅ���֐��錾�i�}�N���j												*/
/****************************************************************************/
#define CSH_GET_CCR()			((Uint16)(*(Uint8 *)CSH_CCR))

#define CSH_SET_CCR(sw)			((Uint16)(*(Uint8 *)CSH_CCR = (Uint8)(sw)))

#define CSH_SET_ENABLE(sw)		((Uint16)(*(Uint8 *)CSH_CCR \
= *(Uint8 *)CSH_CCR & 0xfe | (Uint8)(sw)))

#define CSH_SET_CODE_FILL(sw)	((Uint16)(*(Uint8 *)CSH_CCR \
= *(Uint8 *)CSH_CCR & 0xfd | (Uint8)(sw)))

#define CSH_SET_DATA_FILL(sw)	((Uint16)(*(Uint8 *)CSH_CCR \
= *(Uint8 *)CSH_CCR & 0xfb | (Uint8)(sw)))

#define CSH_SET_WAY_MODE(sw)	((Uint16)(*(Uint8 *)CSH_CCR \
= *(Uint8 *)CSH_CCR & 0xf7 | (Uint8)(sw)))

#define CSH_SET_ACS_WAY(way)	((Uint16)(*(Uint8 *)CSH_CCR \
= *(Uint8 *)CSH_CCR & 0x3f | (Uint8)((way) << 6)))

/****************************************************************************/
/*	�������֐��錾															*/
/****************************************************************************/
void CSH_Init(Uint16);
void CSH_AllClr(void);
void CSH_Purge(void *, Uint32);
