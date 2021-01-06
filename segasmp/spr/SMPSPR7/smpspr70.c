/*----------------------------------------------------------------------------
 *  smpspr70.c -- スプライトサンプル 7
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  スプライト３Ｄテスト表示。
 *  オブジェクト間接続ポリゴンオブジェクトの使用例。
 *
 *  [キーﾊﾟｯﾄﾞ操作]
 *      A        : 画面＝３５２ｘ２４０
 *                 グーローシェーディング表示
 *      B        : 画面＝７０４ｘ４８０
 *                 ８ビットモードによるフラットシェーディング表示
 *      C        : 画面＝３５２ｘ２４０ カラー＝ＲＧＢ
 *                 テクスチャ＋グーローシェーディング表示
 *      X        : モデルのＸ軸回転
 *      Y        : モデルのＹ軸回転
 *      Z        : モデルのＺ軸回転
 *      LEFT     : モデルのＸ軸マイナス移動
 *      RIGHT    : モデルのＸ軸プラス移動
 *      UP       : モデルのＺ軸マイナス移動
 *      DOWN     : モデルのＺ軸プラス移動
 *
 *----------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>

/* #define SPR_3USE_DOUBLE_BUF */
/* #define SPR_3NEED_WORLD_VERT */
/* #define DEBUG */

#define		SYSTEM_CLOCK   28

#define		_SPR3_
#include        "sega_spr.h"

#include	<machine.h>
#include        "sega_scl.h"
#include        "sega_sys.h"
#include	"../../v_blank/v_blank.h"

#ifdef DEBUG
#include        "sega_dbg.h"
#endif

/* スプライトシステムワークエリア定義 */
#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       16
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax,
                                                  CharMax, DrawPrtyMax)

#define DrawPolMax    300
#define ObjSurfMax     32
#define ObjVertMax     32
SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)

/* ３Ｄモデルの定義 */
#include	"model.dat"

#define   MTH_FIXED_MIN        0x80000000
#define   MTH_FIXED_MAX        0x7fffffff
#define   CLIP_MIN_Z       MTH_FIXED_MIN
#define   CLIP_MAX_Z       MTH_FIXED(0.0) /* -0.005 */
#define   ZSORT_MIN_Z      MTH_FIXED(-10.0)
#define   ZSORT_MAX_Z      MTH_FIXED(0)

			      /*    X            Y             Z      */
static MthXyz oLightAngle = {MTH_FIXED(60.0), MTH_FIXED(90.0), MTH_FIXED(0.0)};
static MthXyz oVeiwPoint  = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(2.0)};
static MthXyz oVeiwAngle  = {MTH_FIXED(0.0),  MTH_FIXED(180.0),MTH_FIXED(0.0)};
static MthXyz oModelPoint = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(0.0)};
static MthXyz oModelAngle = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(0.0)};

static MthXyz	lightAngle;
static MthXyz	viewPoint;
static MthXyz	viewAngle;
static MthXyz	modelPoint;
static MthXyz	modelAngle;
static char     mc;

static int dispXsize = 320, dispYsize = 240;

static Fixed32 getSin(Fixed32 n);
static void  changeObject(int kind);
static void initCluster(SprCluster *cluster);
static void moveCluster(SprCluster *cluster);

main()
{
int     i, n;
XyInt	xy;
MthXyz  mAngle, mPoint;
Uint16 *colAddr;

#if(SYSTEM_CLOCK == 28)
        SYS_CHGSYSCK(1);              /* システムクロックを２８Ｍに切替え  */
#endif
	mPoint.x  = MTH_FIXED(0.0);
	mPoint.y  = MTH_FIXED(0.0);
	mPoint.z  = MTH_FIXED(0.0);

	set_imask(0); 			/* 割り込み可にします */
        SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                        SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
        SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
        SCL_SetColRamMode(SCL_CRM15_2048);
        SetVblank();

#ifdef DEBUG
        DBG_Initial(&PadData1,RGB16_COLOR(31,31,31),0);
        DBG_DisplayOn();
	DBG_SetCursol(0,0);
#endif

	SPR_2Initial(&work2D);		/* ２Ｄスプライト表示初期化 */
        SPR_2FrameChgIntr(-1); 

        SCL_DisplayFrame();
        SCL_DisplayFrame();

        colAddr = (Uint16*)SCL_AllocColRam(SCL_SPR,48,OFF);
        SCL_SetColRam(SCL_SPR,0,48,colorData);

	SPR_3Initial(&work3D);		/* ３Ｄスプライト表示初期化 */
	SPR_3SetTexture((SprTexture *)&Texture);
					/* ３Ｄ用テクスチャデータのセット */

	changeObject(0);

	SPR_3SetLight(1,&oLightAngle);
	SPR_3SetView(1,&oVeiwPoint,&oVeiwAngle,ROT_SEQ_XYZ,0);
	SPR_3MoveCluster(&RootCluster,1,&oModelAngle,&oModelPoint);
	SPR_3SetZSortMinMax(0,ZSORT_MIN_Z, ZSORT_MAX_Z);
	SPR_3SetClipLevel (3,  CLIP_MIN_Z,  CLIP_MAX_Z);
 
	SPR_3CallAllCluster(&RootCluster, &initCluster);

	for(;;) {
	    mAngle.x = MTH_FIXED(0.0);
	    mAngle.y = MTH_FIXED(0.0);
	    mAngle.z = MTH_FIXED(0.0);
            switch(PadData1) {
	        case PAD_A:
                     changeObject(0);
                     break;
		case PAD_B:
                     changeObject(1);
                     break;
		case PAD_C:
                     changeObject(2);
                     break;
	        case PAD_X:
	             mAngle.x = MTH_FIXED(1.0);
		     break;
	        case PAD_Y:
	             mAngle.y = MTH_FIXED(1.0);
                     break;
	        case PAD_Z:
	             mAngle.z = MTH_FIXED(1.0);
                     break;
	        case PAD_L:
	             mPoint.x -= MTH_FIXED(0.05);
                     break;
	        case PAD_R:
	             mPoint.x += MTH_FIXED(0.05);
                     break;
	        case PAD_U:
	             mPoint.z -= MTH_FIXED(0.05);
                     break;
	        case PAD_D:
	             mPoint.z += MTH_FIXED(0.05);
                     break;
            }

	    SPR_2OpenCommand(SPR_2DRAW_PRTY_ON);
	              	          /* スプライトコマンド書き込みのオープン */
	    xy.x = dispXsize-1; xy.y = dispYsize-1;
	    SPR_2SysClip(0, &xy);	/* システムクリップエリアコマンド */

	    xy.x = dispXsize/2; xy.y = dispYsize/2;
	    SPR_2LocalCoord(0, &xy);

	    SPR_3SetLight(0,&lightAngle);
	    SPR_3SetView(0,&viewPoint,&viewAngle,ROT_SEQ_XYZ,0);

	    SPR_3MoveCluster(&RootCluster, 0,&mAngle,0);
	    SPR_3MoveCluster(&RootCluster, 1,0,&mPoint);
	    SPR_3DrawModel(&RootCluster);	/* ３Ｄモデルの登録 */

	    SPR_3Flush();		/* ３Ｄスプライトコマンドセット */

	    SPR_2CloseCommand();     /* スプライトコマンド書き込みのクローズ */

	    SCL_DisplayFrame();    /* Ｖ－ＢＬＡＮＫ割り込みを待ち、       */
				     /* スプライト表示とスクロール動作を行う */
    }
}


static void initCluster(SprCluster *cluster)
{
   ClustData *clustData;
   MthXyz     angle, move;

   clustData = (ClustData*)cluster->context;
   if(clustData) {
       angle.x = MTH_Mul(MTH_Sin(clustData->rot.x),MTH_FIXED(20));
       angle.y = MTH_FIXED(0);
       angle.z = MTH_Mul(MTH_Sin(clustData->rot.z),MTH_FIXED(20));
       SPR_3MoveCluster(cluster, 1, &angle, 0);
       move.x = MTH_FIXED(0);
       move.y = clustData->posY;
       move.z = MTH_FIXED(0);
       SPR_3MoveCluster(cluster, 1, 0, &move);
       cluster->transStart = &moveCluster;
   }
}


static void moveCluster(SprCluster *cluster)
{
   ClustData *clustData;
   MthXyz     angle, move;

   clustData = (ClustData*)cluster->context;
   if(clustData->rot.x > MTH_FIXED(180)) clustData->rot.x -= MTH_FIXED(360);
   angle.x = getSin(clustData->rot.x);
   clustData->rot.x += MTH_FIXED(1);
   angle.y = MTH_FIXED(0);
   if(clustData->rot.z > MTH_FIXED(180)) clustData->rot.z -= MTH_FIXED(360);
   angle.z = getSin(clustData->rot.z);
   clustData->rot.z += MTH_FIXED(1);
   SPR_3MoveCluster(cluster, 0, &angle, 0);
   clustData->posY += clustData->movY;
   if((clustData->posY <= MTH_FIXED(HEIGHT_MIN/5.0)) ||
      (clustData->posY >= MTH_FIXED(HEIGHT_MAX/5.0)))
       clustData->movY *= -1;
   move.x = MTH_FIXED(0);
   move.y = clustData->posY;
   move.z = MTH_FIXED(0);
   SPR_3MoveCluster(cluster, 1, 0, &move);
}


static void  changeObject(int kind)
{
    int i;
    
    switch(kind) {
      case 1:
        for(i=0; i<Object6.surfaceCount; i++) {
            Obj6Surface[i].drawMode = DRAW8 | ZSORT_MIN;
            Obj6Surface[i].color    = COLOR8;
        }
        Object6.dispFlag  = INBETWEEN_OBJECT | DISP_POLYGON | FLAT_SHADING;
        Object6.shdIdxTbl = shdIdxTbl8;
        SCL_Vdp2Init();
        SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                        SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
        SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,SCL_SP_WINDOW);
        SCL_SetColRamMode(SCL_CRM15_2048);
        SPR_2FrameChgIntr(0); 
#if(SYSTEM_CLOCK == 28)
        SCL_SetDisplayMode(SCL_DOUBLE_INTER,SCL_240LINE,SCL_HIRESO_B);
        SPR_2SetTvMode(SPR_TV_HIRESO,SPR_TV_704X240,ON);
        dispXsize = 704;
        dispYsize = 480;
#else
        SCL_SetDisplayMode(SCL_DOUBLE_INTER,SCL_240LINE,SCL_HIRESO_A);
        SPR_2SetTvMode(SPR_TV_HIRESO,SPR_TV_640X240,ON);
        dispXsize = 640;
        dispYsize = 480;
#endif
        SPR_2FrameEraseData(0x2121);
        SPR_3SetPixelCount(512, 512);
        break;
        
      case 0:
	for(i=0; i<Object6.surfaceCount; i++) {
	    Obj6Surface[i].drawMode = DRAW16 | ZSORT_MIN;
	    Obj6Surface[i].color    = COLOR16;
	}
	Object6.dispFlag  = INBETWEEN_OBJECT | DISP_POLYGON | SHADING_KIND;
	Object6.shdIdxTbl = NULL;
	SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
			SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
	SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
	SCL_SetColRamMode(SCL_CRM15_2048);
	SPR_2FrameChgIntr(0);
#if(SYSTEM_CLOCK == 28)
        SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_B);
        SPR_2SetTvMode(SPR_TV_NORMAL,SPR_TV_352X240,OFF);
        dispXsize = 352;
        dispYsize = 240;
#else
        SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
        SPR_2SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
        dispXsize = 320;
        dispYsize = 240;
#endif
        SPR_2FrameEraseData(RGB16_COLOR(0, 0,31));
        SPR_3SetPixelCount(256, 256);
        break;
        
      case 2:
	for(i=0; i<Object6.surfaceCount; i++) {
	    Obj6Surface[i].drawMode = DRAWTX | ZSORT_MIN;
	    Obj6Surface[i].color    = COLORTX;
	}
	Object6.dispFlag  = INBETWEEN_OBJECT | DISP_POLYGON | SHADING_KIND;
	Object6.shdIdxTbl = NULL;
	SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
			SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
	SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
	SCL_SetColRamMode(SCL_CRM15_2048);
	SPR_2FrameChgIntr(0);
#if(SYSTEM_CLOCK == 28)
        SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_B);
        SPR_2SetTvMode(SPR_TV_NORMAL,SPR_TV_352X240,OFF);
        dispXsize = 352;
        dispYsize = 240;
#else
        SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
        SPR_2SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
        dispXsize = 320;
        dispYsize = 240;
#endif
        SPR_2FrameEraseData(RGB16_COLOR(0, 31,0));
        SPR_3SetPixelCount(256, 256);
        break;
     }
}


static Fixed32 getSin(Fixed32 n)
{
    Fixed32 m, rn, rm;
    m = n + MTH_FIXED(1);
    if(m > MTH_FIXED(180)) m -= MTH_FIXED(360);
    rn = MTH_Mul(MTH_Sin(n),MTH_FIXED(20));
    rm = MTH_Mul(MTH_Sin(m),MTH_FIXED(20));
    return MTH_IntToFixed(MTH_FixedToInt(rm) - MTH_FixedToInt(rn));
}

/*  end of file */
