/*----------------------------------------------------------------------------
 *  mth_rand.c -- MTH ライブラリ 乱数生成処理 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  このライブラリはシミュレーション用乱数生成処理モジュールで、
 *  以下のルーチンを含む。
 *
 *  MTH_InitialRand           -  乱数生成の初期化
 *  MTH_GetRand               -  乱数値を返す
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
 * 　　(1) Uint32   initVal       - <i> 乱数発生の初期パラメータ値
 *
 * DESCRIPTION
 *
 *	MTH_GetRand で返す乱数の演算用初期パラメータを設定する。本初期化
 *      ルーチンを呼び出さないと演算用初期パラメータ値は 0 となっている。
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
 *	呼び出し毎に乱数値を返す。
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
