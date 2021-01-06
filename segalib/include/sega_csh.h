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
/*	レジスタ定義															*/
/****************************************************************************/
#define CSH_CCR			((Uint16 *)0xfffffe92)	/*	ｷｬｯｼｭｺﾝﾄﾛｰﾙﾚｼﾞｽﾀｱﾄﾞﾚｽ	*/

/****************************************************************************/
/*	定数定義																*/
/****************************************************************************/
#define CSH_4WAY			((Uint16)0x0000)	/*	4ｳｪｲｾｯﾄｱｿｼｴｲﾃｨﾌﾞﾓｰﾄﾞ	*/
#define CSH_2WAY			((Uint16)0x0008)	/*	2ｳｪｲｾｯﾄｱｿｼｴｲﾃｨﾌﾞﾓｰﾄﾞ	*/
#define CSH_DISABLE			((Uint16)0x0000)	/*	ｷｬｯｼｭﾃﾞｨｾｰﾌﾞﾙﾓｰﾄﾞ		*/
#define CSH_ENABLE			((Uint16)0x0001)	/*	ｷｬｯｼｭｲﾈｰﾌﾞﾙﾓｰﾄﾞ			*/
#define CSH_CODE_ENABLE		((Uint16)0x0000)	/*	ｺｰﾄﾞｲﾈｰﾌﾞﾙﾓｰﾄﾞ			*/
#define CSH_CODE_DISABLE	((Uint16)0x0002)	/*	ｺｰﾄﾞﾃﾞｨｾｰﾌﾞﾙﾓｰﾄﾞ		*/
#define CSH_DATA_ENABLE		((Uint16)0x0000)	/*	ﾃﾞｰﾀｲﾈｰﾌﾞﾙﾓｰﾄﾞ			*/
#define CSH_DATA_DISABLE	((Uint16)0x0004)	/*	ﾃﾞｰﾀﾃﾞｨｾｰﾌﾞﾙﾓｰﾄﾞ		*/

/****************************************************************************/
/*	低水準関数宣言（マクロ）												*/
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
/*	高水準関数宣言															*/
/****************************************************************************/
void CSH_Init(Uint16);
void CSH_AllClr(void);
void CSH_Purge(void *, Uint32);
