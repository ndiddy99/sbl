
/*	Copyright(c) 1994 SEGA			*/
/*
 *     
 *     ラインカラーを使ったサンプル
 *
 */

#include "smp_bios.h"
#include "smp_sccg.h"
#include "smp_lclr.h"


#define SMPR_logocharNo 0x40


#define DrawPolMax    0x200   /* 最大ポリゴン数           */
#define ObjSurfMax     16     /* １オブジェクト最大面数   */
#define ObjVertMax     16     /* １オブジェクト最大頂点数 */

/* ３Dライブラリワーク定義 */
SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax )



/* 箱の面データ */
#define HAKO_DRAW	0x4000 |00 | COLOR_5 | ECDSPD_DISABLE | COMPO_REP
#define HAKO_COLOR 	0x41    /* テクスチャキャラクタ No */


/* プレーヤーの面データ */
#define PL_DRAW1 	00 | COLOR_0 | ECDSPD_DISABLE | COMPO_REP
#define PL_COLOR1 	RGB16_COLOR(10,31,0)




/* 箱 テクスチャテーブル */
const SprTexture YUKATEXT[] = {
    {
	0x41,			/* キャラクタNo */
	COLOR_5,		/* カラーモード */
        0,
	32,			/* キャラクタ幅 */
	32,			/* キャラクタ高さ */
	&YUKA_cg[0],         	/* キャラクタポインタ */
	NULL			/* ルックアップテーブルポインタ */
      },
    {
	0xffff,			/* キャラクタ番号 */
	COLOR_5,		/* カラーモード */
        0,
	0,			/* キャラクタ幅 */
	0,			/* キャラクタ高さ */
	NULL,			/* キャラクタポインタ */
	NULL			/* ルックアップテーブルポインタ */
    }

};

/* 箱 頂点座標テーブル */
const MthXyz HakoVertPoint[] = {
	{MTH_FIXED(-0.1), MTH_FIXED(-0.1), MTH_FIXED(-0.1)}, /* 0 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.1), MTH_FIXED(-0.1)}, /* 1 */
	{MTH_FIXED(0.1),  MTH_FIXED(0.1),  MTH_FIXED(-0.1)}, /* 2 */ 
	{MTH_FIXED(-0.1), MTH_FIXED(0.1),  MTH_FIXED(-0.1)}, /* 3 */

	{MTH_FIXED(-0.1), MTH_FIXED(-0.1), MTH_FIXED(0.1)},  /* 4 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.1), MTH_FIXED(0.1)},  /* 5 */
	{MTH_FIXED(0.1),  MTH_FIXED(0.1),  MTH_FIXED(0.1)},  /* 6 */ 
	{MTH_FIXED(-0.1), MTH_FIXED(0.1),  MTH_FIXED(0.1)},  /* 7 */

};

/* 箱 面定義テーブル */
const SprSurface HakoSurface[] = {
	{	0,1,2,3,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,5,6,7,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,0,3,7,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	1,5,6,2,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,5,1,0,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	7,6,2,3,	/* 面を構成する頂点番号 */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},

};

/* 箱 面の法線ベクトルテーブル */
const MthXyz HakoMenHosen[6] = {
	{MTH_FIXED(0),   MTH_FIXED(0),  MTH_FIXED(-1)},
	{MTH_FIXED(0),   MTH_FIXED(0),  MTH_FIXED(1)},
	{MTH_FIXED(-1),  MTH_FIXED(0),  MTH_FIXED(0)},
	{MTH_FIXED(1),   MTH_FIXED(0),  MTH_FIXED(0)},
	{MTH_FIXED(0),   MTH_FIXED(-1), MTH_FIXED(0)},
	{MTH_FIXED(0),   MTH_FIXED(1),  MTH_FIXED(0)},
};


/* 箱 オブジェクト定義 */
const SprObject3D Hako_Obj = {
        1,                         /* オブジェクト番号                   */
	DISP_POLYGON,              /* 表示フラグ                         */
	8,		           /* 頂点数                             */
	6,			   /* 面数                               */
	(MthXyz *)&HakoVertPoint,   /* 頂点座標テーブル                   */
	NULL,			   /* 頂点の法線ベクトルテーブル         */
	(SprSurface *)&HakoSurface, /* 面定義テーブル                     */
	(MthXyz *)&HakoMenHosen,    /* 面の法線ベクトルテーブル           */
        NULL,                      /* 面の代表頂点座標テーブル           */
	NULL,                      /* シェーディングインデックステーブル */
        MTH_FIXED(1.0),
	NULL		 	   /* 次オブジェクト                     */
};




/* 箱 クラスタ定義 */
const SprCluster Hako_Cluster = {
        1,                          /* クラスタ番号  */
	ROT_SEQ_XYZ,		    /* 回転順序 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＸ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＹ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＺ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＸ座標 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＹ座標 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＺ座標 */
	&Hako_Obj,		    /* ３Ｄオブジェクト */
	NULL,			    /* 次クラスタ */
	NULL     		    /* 子クラスタ */
};





/* プレーヤー 頂点座標テーブル */
const MthXyz PlVertPoint[] = {
	{MTH_FIXED(0),    MTH_FIXED(0.02),  MTH_FIXED(0.5)},    /* 0 */
	{MTH_FIXED(-0.1), MTH_FIXED(-0.02), MTH_FIXED(0.5)},    /* 1 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.02), MTH_FIXED(0.5)},    /* 2 */
	{MTH_FIXED(0),    MTH_FIXED(0),     MTH_FIXED(0.54)}, /* 3 */
};

/* プレーヤー 面定義テーブル */
const SprSurface PlSurface[] = {
	{	0,1,2,0,	/* 面を構成する頂点番号 */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	0,3,1,0,	/* 面を構成する頂点番号 */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	0,3,2,0,	/* 面を構成する頂点番号 */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	3,1,2,3,	/* 面を構成する頂点番号 */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},

};

/* プレーヤー 面の法線ベクトルテーブル */
const MthXyz PlMenHosen[4] = {
	{MTH_FIXED(0.0),  MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(-1.0), MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(1.0),  MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(0.0),  MTH_FIXED(-1.0), MTH_FIXED(-1.0)},
};


/* プレーヤー オブジェクト定義 */
const SprObject3D PL_Obj = {
        1,                          /* オブジェクト番号                   */
	DISP_POLYLINE,              /* 表示フラグ                         */
	4,			    /* 頂点数                             */
	4,			    /* 面数                               */
	(MthXyz *)&PlVertPoint,     /* 頂点座標テーブル                   */
	NULL,			    /* 頂点の法線ベクトルテーブル         */
	(SprSurface *)&PlSurface,   /* 面定義テーブル                     */
	(MthXyz *)&PlMenHosen,      /* 面の法線ベクトルテーブル           */
        NULL,                       /* 面の代表頂点座標テーブル           */
	NULL,                       /* シェーディングインデックステーブル */
        MTH_FIXED(1.0),
	NULL		 	    /* 次オブジェクト                     */
};

/* プレーヤー クラスタ定義 */
const SprCluster Pl_Cluster = {
        1,                          /* クラスタ番号  */
	ROT_SEQ_XYZ,		    /* 回転順序 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＸ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＹ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系におけるＺ回転 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＸ座標 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＹ座標 */
	MTH_FIXED(0.0),             /* ワールド座標系における原点のＺ座標 */
	&PL_Obj,		    /* ３Ｄオブジェクト */
	NULL,			    /* 次クラスタ */
	NULL     		    /* 子クラスタ */
};



/* 
 *    外部参照ワーク
 */
extern SclSysreg Scl_s_reg;
extern SclNorscl Scl_n_reg;


/*
 *    関数プロトタイプ
 */
void SMLC_cgset(void);
void SMLC_HakoAct(G3DACT *ix);
void SMLC_PlayerAct(G3DACT *ix);
void SMLC_PutObject(PLYOBJ *obj);



/*
 *  ラインカラーを使ったサンプル
 */
void SMLC_LclrMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k;
    Uint32 times;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    XyInt reg_xy;

    while (SMMA_MainMode == LCLR_MODE) {
        SMV1_SprCmdStart();
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SPR_3Initial(&work3D);
                SCL_SetPriority(SCL_SP0, 6);
                SCL_SetPriority(SCL_NBG0, 5);
                SCL_SetPriority(SCL_NBG1, 0);
                SCL_SetPriority(SCL_RBG0, 0);


                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_2048);
                
                /* スプライトデータタイプの設定 */
                SCL_SetSpriteMode(SCL_TYPE7, SCL_MIX, SCL_SP_WINDOW);

                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* CGデータの展開 */
                SMLC_cgset();

                /* プレーヤーアクションのセット */
                SMTA_MakeAction(SMLC_PlayerAct);

                SMV2_TvOn();               
                SMMA_MainLevel++;
            case MAIN:
                /* バックカラーセット */
                poke_w(BCLRTBLADD, ((31 << 10) + (31 << 5) + 31));    

                /* ローカル座標（スプライト原点）のセット */
                reg_xy.x = WIDTH_H / 2;
                reg_xy.y = WIDTH_V / 2;
                SPR_2LocalCoord(SPR_2MOST_FAR+3, &reg_xy);

                /* 解像度セット */
                SPR_3SetPixelCount(320, 224);

                /* クリップレベルの設定 */
                SPR_3SetClipLevel(3, MTH_FIXED(-20.0), MTH_FIXED(-0.1));

                /* アクション実行 */
                SMTA_ActionLoop();

                /* ポリゴン描画   */
                SPR_3Flush();

                /* スタート押されたら抜ける */
                if (SMPA_pad_edge1 & PAD_START)
                    SMMA_MainLevel++;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    }

}

	    
/*
 *   SPRITE CG SET
 */
void SMLC_cgset(void)
{
    const Uint16 nbg1_map[] = {
        0x1082,0x1084,0x1086,0x1088,
        0x108a,0x108c,0x108e,0x1090,
        0x1092,0x1094,0x1096,0x1098,
        0x109a,0x109c,0x109e,0x10a0,
    };

    Uint16 *dst16, *src16, *temp16, *wrptr16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;
    Uint32 direc;


    /* スプライト登録キャラクタ初期化 */
    SPR_2ClrAllChar();

    /* アスキーフォント、カラーセット */
    SMV1_SetFont4sp(0,1,0);
    SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
    poke_w(COLOR_RAM+0x20, 0x7000);
    poke_w(COLOR_RAM+0x22, 0x739c);

    /* テクスチャデータセット */
    SPR_3SetTexture(YUKATEXT);

    /* バックカラーセット */
    poke_w(BCLRTBLADD, ((31 << 10) + (31 << 5) + 31));    

    /* ラインカラーの設定 */
    wrptr16 = (Uint16 *)(COLOR_RAM + 0x7c0);
    for (i = 0; i < 0x20; i++)
         *wrptr16++ = (i << 10) + (i << 5) + i;

    wrptr16 = (Uint16 *)(LCLRTBLADD);
    for (i = 0; i < 0xe0; i++) {
         direc = (i * 0x80) / 0xe0;
         *wrptr16++ = 0x3e0 + (0x1f * SMMI_Sinset8(direc)) / 0x100;
    }

    SCL_Open(SCL_NBG0);
    Scl_n_reg.linecolmode = 0x80000000 | ((LCLRTBLADD & 0xfffff) / 2);
    SCL_Close();

    /* カラー演算モードの設定 */
    SCL_SET_CCMD(1);
    SCL_SET_SPCCEN(1);
    SCL_SET_LCCCEN(1);
    SCL_SET_EXCCEN(0);
    SCL_SET_SPLCEN(1);  /* ラインカラー */
    SCL_SET_SPCCCS(3);  /* スプライト色演算条件 */


}
    




/*
 *     箱
 */
void SMLC_HakoAct(G3DACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};


    Sint32 ld0, ld1, ld2;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    G3DACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->obj.clsptr = &Hako_Cluster;
            ix->obj.direc.x = MTH_FIXED(0);
            ix->obj.direc.y = MTH_FIXED(0.0); 
            ix->obj.direc.z = MTH_FIXED(0);
            ix->obj.speed.x = 0;
            ix->obj.speed.y = 0;
            ix->obj.speed.z = 0;
            ix->mode++;
        case MOVE:
            /* ix->msttbl には プレーヤーアクションのポインタが入っている */
            iy = ix->msttbl;
            ix->obj.speed.z = iy->obj.speed.z;

            ix->obj.posi.z -= ix->obj.speed.z;
            if (ix->obj.posi.z < MTH_FIXED(-3.0))
                SMTA_KillAction(ix);
            else
                SMLC_PutObject(&ix->obj);
    }
   
}



/*
 *    プレーヤーアクション
 */
void SMLC_PlayerAct(G3DACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};

    Uint16 swdata, swdatax, xlp, ylp;
    Sint32 ld0, ld1, ld2;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    G3DACT *iy;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case INIT:
            ix->obj.clsptr = &Pl_Cluster;
            ix->obj.posi.x = MTH_FIXED(0.5);
            ix->obj.posi.y = MTH_FIXED(0.25);
            ix->obj.posi.z = MTH_FIXED(-3); 
            ix->obj.direc.x = MTH_FIXED(0);
            ix->obj.direc.y = MTH_FIXED(0.1);
            ix->obj.direc.z = MTH_FIXED(0.0);
            ix->obj.speed.x = MTH_FIXED(0.0);
            ix->obj.speed.y = MTH_FIXED(0.0);
            ix->obj.speed.z = MTH_FIXED(0.0);
            ix->obj.acc.x = MTH_FIXED(0.0);
            ix->obj.acc.y = MTH_FIXED(0.0);
            ix->obj.acc.z = MTH_FIXED(0.0);
            ix->lreg[0] = MTH_FIXED(0.0);
            ix->lreg[1] = MTH_FIXED(0.0);
            ix->lreg[2] = MTH_FIXED(0.0);
            ix->lreg[3] = 0;
        
            ix->mode++;

        case MOVE:

            if (swdata & PAD_C) {
                ix->obj.speed.z += MTH_FIXED(0.01);
                if (ix->obj.speed.z > MTH_FIXED(0.1))
                    ix->obj.speed.z = MTH_FIXED(0.1);
            } else {
                ix->obj.speed.z -= MTH_FIXED(0.01);
                if (ix->obj.speed.z < MTH_FIXED(0.03))
                    ix->obj.speed.z = MTH_FIXED(0.03);
            }


            switch (swdata & (PAD_LEFT + PAD_RIGHT)) {
                case PAD_LEFT:
                    ix->obj.speed.x = MTH_FIXED(0.02);
                    ix->obj.direc.z -= MTH_FIXED(2.0);
                    if (ix->obj.direc.z < MTH_FIXED(-10.0))
                        ix->obj.direc.z = MTH_FIXED(-10.0);
                    break;

                case PAD_RIGHT:
                    ix->obj.speed.x = MTH_FIXED(-0.02);
                    ix->obj.direc.z += MTH_FIXED(2.0);
                    if (ix->obj.direc.z > MTH_FIXED(10.0))
                        ix->obj.direc.z = MTH_FIXED(10.0);
                    break;

                case 0:
                    ix->obj.speed.x = MTH_FIXED(0.0);

                    if (ix->obj.direc.z > MTH_FIXED(0)) {
                        ix->obj.direc.z -= MTH_FIXED(1.0);
                        if (ix->obj.direc.z < MTH_FIXED(0))
                            ix->obj.direc.z = MTH_FIXED(0);
                    }

                    if (ix->obj.direc.z < MTH_FIXED(0)) {
                        ix->obj.direc.z += MTH_FIXED(1.0);
                        if (ix->obj.direc.z > MTH_FIXED(0))
                            ix->obj.direc.z = MTH_FIXED(0);
                    }
                    break;
            }
         

           
            ix->obj.posi.x += ix->obj.speed.x;
            ix->obj.posi.y += ix->obj.speed.y;

            if (ix->obj.posi.x < MTH_FIXED(0))
                ix->obj.posi.x = MTH_FIXED(0);

            if (ix->obj.posi.x > MTH_FIXED(1.0))
                ix->obj.posi.x = MTH_FIXED(1.0);


            /* 視点の設定 */
            ix->lreg[0] = ix->obj.direc.x;
            ix->lreg[1] = ix->obj.direc.y;
            ix->lreg[2] = MTH_FIXED(0.0) - ix->obj.direc.z;
            SPR_3SetView(1, &ix->obj.posi, 
                            (MthXyz *)&ix->lreg[0], ROT_SEQ_XYZ,0);

            /* 光源の設定 */
            SPR_3SetLight(1, &ix->obj.direc);

            /* lreg[3]がある値を越えると箱をセットする */
            ix->lreg[3] += ix->obj.speed.z;
            if (ix->lreg[3] > MTH_FIXED(0.5)) {
                ix->lreg[3] = MTH_FIXED(0.0);
                for (xlp = 0; xlp < 3; xlp++) {
                    for (ylp = 0; ylp < 2; ylp++) {
                        iy = SMTA_MakeAction(SMLC_HakoAct);
                        iy->obj.posi.x = MTH_FIXED(0.5) * xlp;
                        iy->obj.posi.y = MTH_FIXED(0.5) * ylp;
                        iy->obj.posi.z = MTH_FIXED(0.5) * 2;
                        iy->msttbl = ix;
                    }
                }
            }

            break;
    }
   
}




/*
 * ポリゴン表示関数
 */
void SMLC_PutObject(PLYOBJ *obj)
{
    SPR_3MoveCluster(obj->clsptr, 1, &obj->direc, &obj->posi);
    SPR_3DrawModel(obj->clsptr);
}








