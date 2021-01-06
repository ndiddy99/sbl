/*----------------------------------------------------------------------------
 *  mth_dsps.c -- MTH ライブラリ ＤＳＰによる３Ｄ座標変換処理 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  このライブラリはシミュレーション用ＤＳＰによる３Ｄ座標変換処理モジュールで
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
#include "sega_mth.h"
#ifdef _SH
#include <machine.h>
#endif

static MthMatrix *coordMatrix;
static MthXyz    *lightVector;

static void surfaceBright(Uint16 entryCount, MthXyz *surfacePoint,
			  MthXyz *surfaceNormal, Sint32 *surfaceBright);
static void coordTrans(Uint16 entryCount, MthXyz *src, MthXyz *ans);
static void vertBright(Uint16 entryCount, MthXyz *vertNormal,
					  Sint32 *pointBright);



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
    /** BEGIN ************************************************************/
    coordMatrix = &polyTransParm->viewLight->viewMatrix;
    lightVector = &polyTransParm->viewLight->lightVector;

    /* 面法線テーブルから面輝度テーブルを得る */
    surfaceBright(polyTransParm->surfCount,
		  polyTransParm->surfPoint,
		  polyTransParm->surfNormal,
		  polyTransParm->surfBright);

    /* 頂点データの視点座標変換後テーブルを得る */
    coordTrans(polyTransParm->transViewVertCount,
	       polyTransParm->transViewVertSrc,
	       polyTransParm->transViewVertAns);

    /* グーロー用頂点輝度テーブルを得る  */
    if(polyTransParm->gourVertCount)
	vertBright(polyTransParm->gourVertCount,
		   polyTransParm->vertNormal,
		   polyTransParm->vertBright);

    /* 頂点データのワールド座標変換後テーブルを得る */
    if(polyTransParm->transWorldVertCount) {
	coordMatrix = &polyTransParm->viewLight->worldMatrix;
	coordTrans(polyTransParm->transWorldVertCount,
		   polyTransParm->transWorldVertSrc,
		   polyTransParm->transWorldVertAns);
    }
}


static void surfaceBright(Uint16 entryCount, MthXyz *surfacePoint,
			  MthXyz *surfaceNormal, Sint32 *surfaceBright)
{
	Sint32	i,j,color;
	MthXyz  viewVect, tSurfPoint, tSurfNormal, *wSurfPoint;

	wSurfPoint = surfacePoint;
	for(i=0;i<entryCount;i++)
	{
	    MTH_CoordTrans(coordMatrix, wSurfPoint, &tSurfPoint);
	    viewVect.x = - tSurfPoint.x;
	    viewVect.y = - tSurfPoint.y;
	    viewVect.z = - tSurfPoint.z;
	    MTH_NormalTrans(coordMatrix, surfaceNormal++, &tSurfNormal);
	    color = MTH_ComputeBright(&tSurfNormal,lightVector);
	    if (MTH_Product((Fixed32*)&viewVect,
			    (Fixed32*)&tSurfNormal) <= 0)
		color |= 0xffff0000;
	    *surfaceBright++ = color;
	    wSurfPoint++;
	}
}


static void coordTrans(Uint16 entryCount, MthXyz *src, MthXyz *ans)
{
	Sint32	i;

	for(i=0;i<entryCount;i++)
	    MTH_CoordTrans(coordMatrix, src++, ans++);
}


static void vertBright(Uint16 entryCount, MthXyz *vertNormal,
					  Sint32 *pointBright)
{
	Uint16	i;
	MthXyz  ans;
	Sint16  color;

	for(i=0;i<entryCount;i++)
	{
	     MTH_NormalTrans(coordMatrix, vertNormal++, &ans);
	     color = MTH_ComputeBright(&ans,lightVector);
	     *pointBright++ = color;
	}
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
}


/*  end of file */
