/*----------------------------------------------------------------------------
 *  mth_dsp.c -- MTH ライブラリ ＤＳＰによる３Ｄ座標変換処理 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  このライブラリはＤＳＰによる３Ｄ座標変換処理モジュールで
 *  以下のルーチンを含む。
 *
 *  MTH_PolyDataTransInit     -  座標変換処理の初期化
 *  MTH_PolyDataTransExec     -  座標変換処理実行
 *  MTH_PolyDataTransCheck    -  座標変換処理完了チェック
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <string.h>
#include "sega_mth.h"
#include "sega_dsp.h"
#include "sega_csh.h"
#ifdef _SH
#include <machine.h>
#endif

static Uint32 dspProgram[] = {
#include "mth_dspp.cod"
};

static MthPolyTransParm iPolyTransParm;


/*************************************************************************
 *
 * NAME : MTH_PolyDataTransInit  -  Initialize Coord Transfer by DSP
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰの初期化と座標変換処理プログラムのロード。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 *
 *************************************************************************
 */
void    MTH_PolyDataTransInit(void)
{
    /** BEGIN ************************************************************/
    DSP_LoadProgram(0, dspProgram, 256);
}


/*************************************************************************
 *
 * NAME : MTH_PolyDataTransExec  -  Execute Coord Transfer by DSP
 *
 * PARAMETERS
 *
 *     (1) MthPolyTransParm *polyTransParm  - <i/o>  座標変換パラメータテーブル
 *
 * DESCRIPTION
 *
 *	３Ｄスプライト表示ライブラリで用いているポリゴンデータ列
 *      （３Ｄオブジェクト）について次の一連の処理をＤＳＰにて行う。
 *
 *      ・ポリゴン面の隠面判定と輝度計算
 *      ・頂点の視点座標系への変換処理
 *      ・グーロー表示用頂点の輝度計算（オプション）
 *      ・頂点のワールド座標系への変換処理（オプション）
 *
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    MTH_PolyDataTransExec(MthPolyTransParm *polyTransParm)
{
    MthPolyTransParm wPolyTransParm;
    Uint32 w;

    /** BEGIN ************************************************************/
#if 0
    if(polyTransParm->surfCount)
       CSH_Purge(polyTransParm->surfBright,
                 polyTransParm->surfCount*sizeof(Sint32));
    if(polyTransParm->transViewVertCount)
       CSH_Purge(polyTransParm->transViewVertAns,
                 polyTransParm->transViewVertCount*sizeof(MthXyz));
    if(polyTransParm->gourVertCount)
       CSH_Purge(polyTransParm->vertBright,
                 polyTransParm->gourVertCount*sizeof(Sint32));
    if(polyTransParm->transWorldVertAns)
       CSH_Purge(polyTransParm->transWorldVertAns,
                 polyTransParm->transWorldVertCount*sizeof(MthXyz));
#endif
    memcpy(&wPolyTransParm,polyTransParm,sizeof(MthPolyTransParm));

    w = ((Uint32)wPolyTransParm.viewLight        ) >> 2;
    wPolyTransParm.viewLight         = (MthViewLight*)w;
    w = ((Uint32)wPolyTransParm.surfPoint        ) >> 2;
    wPolyTransParm.surfPoint	     = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.surfNormal       ) >> 2;
    wPolyTransParm.surfNormal        = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.surfBright       ) >> 2;
    wPolyTransParm.surfBright        = (Sint32*)w;
    w = ((Uint32)wPolyTransParm.transViewVertSrc ) >> 2;
    wPolyTransParm.transViewVertSrc  = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.transViewVertAns ) >> 2;
    wPolyTransParm.transViewVertAns  = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.vertNormal       ) >> 2;
    wPolyTransParm.vertNormal        = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.vertBright       ) >> 2;
    wPolyTransParm.vertBright        = (Sint32*)w;
    w = ((Uint32)wPolyTransParm.transWorldVertSrc) >> 2;
    wPolyTransParm.transWorldVertSrc = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.transWorldVertAns) >> 2;
    wPolyTransParm.transWorldVertAns = (MthXyz*)w;
    DSP_WriteData(DSP_RAM_0 | 0, (Uint32*)&wPolyTransParm,
                                              sizeof(MthPolyTransParm)/4);
    DSP_Start(0);
}


/*************************************************************************
 *
 * NAME : MTH_PolyDataTransCheck  -  Check Coord Transfer Complete
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰによる座標変換処理が完了するまで待つ。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    MTH_PolyDataTransCheck(void)
{
    /** BEGIN ************************************************************/
    while(DSP_CheckEnd() == DSP_NOT_END); 
}

/*  end of file */
