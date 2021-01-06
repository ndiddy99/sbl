/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   横ラインスクロール
 *
 */


#include "smp_bios.h"
#include "smp_sccg.h"
#include "smp_lnsc.h"


/* 
 *    外部参照ワーク
 */
extern SclSysreg Scl_s_reg;
extern SclWinscl Scl_w_reg;
extern SclNorscl Scl_n_reg;





/*
 *    関数プロトタイプ
 */
void SMLN_LnscMode(void);
void SMLN_cgset(void);
void SMLN_LineScrollAct(LNSCACT *ix);

void SMLN_hline_init(LNSCACT *ix);
void SMLN_hline_main(LNSCACT *ix);

void SMLN_vcell_init(LNSCACT *ix);
void SMLN_vcell_main(LNSCACT *ix);

void SMLN_hzoom_init(LNSCACT *ix);
void SMLN_hzoom_main(LNSCACT *ix);

void SMLN_hvscl_init(LNSCACT *ix);
void SMLN_hvscl_main(LNSCACT *ix);

void SMLN_mixed_init(LNSCACT *ix);
void SMLN_mixed_main(LNSCACT *ix);


/*
 *    メッセージデータ
 */

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    const Sint16 SMLN_hlinescl_pat[] = {
            12,
            ('H' - ' ') , 0, 0x0008, 0x0008,     0,  0,
            (' ' - ' ') , 0, 0x0008, 0x0008,     8,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    16,  0,
            ('I' - ' ') , 0, 0x0008, 0x0008,    24,  0,
            ('N' - ' ') , 0, 0x0008, 0x0008,    32,  0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    40,  0,
            ('S' - ' ') , 0, 0x0008, 0x0008,    48,  0,
            ('C' - ' ') , 0, 0x0008, 0x0008,    56,  0,
            ('R' - ' ') , 0, 0x0008, 0x0008,    64,  0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    72,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    80,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    88,  0,
        };

    const Sint16 SMLN_vcelscl_pat[] = {
            12,
            ('V' - ' ') , 0, 0x0008, 0x0008,     0,  0,
            (' ' - ' ') , 0, 0x0008, 0x0008,     8,  0,
            ('C' - ' ') , 0, 0x0008, 0x0008,    16,  0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    24,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    32,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    40,  0,
            ('S' - ' ') , 0, 0x0008, 0x0008,    48,  0,
            ('C' - ' ') , 0, 0x0008, 0x0008,    56,  0,
            ('R' - ' ') , 0, 0x0008, 0x0008,    64,  0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    72,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    80,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    88,  0,
        };

    const Sint16 SMLN_hvlinescl_pat[] = {
            13,
            ('H' - ' ') , 0, 0x0008, 0x0008,     0,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,     8,  0,
            ('I' - ' ') , 0, 0x0008, 0x0008,    16,  0,
            ('N' - ' ') , 0, 0x0008, 0x0008,    24,  0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    32,  0,
            (' ' - ' ') , 0, 0x0008, 0x0008,    40,  0,
            ('V' - ' ') , 0, 0x0008, 0x0008,    48,  0,
            ('S' - ' ') , 0, 0x0008, 0x0008,    56,  0,
            ('C' - ' ') , 0, 0x0008, 0x0008,    64,  0,
            ('R' - ' ') , 0, 0x0008, 0x0008,    72,  0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    80,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    88,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,    96,  0,
        };

    const Sint16 SMLN_hlzoomscl_pat[] = {
            10,
            ('H' - ' ') , 0, 0x0008, 0x0008,     0,  0,
            ('L' - ' ') , 0, 0x0008, 0x0008,     8,  0,
            ('I' - ' ') , 0, 0x0008, 0x0008,    16,  0,
            ('N' - ' ') , 0, 0x0008, 0x0008,    24,  0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    32,  0,
            (' ' - ' ') , 0, 0x0008, 0x0008,    40,  0,
            ('Z' - ' ') , 0, 0x0008, 0x0008,    48,  0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    56,  0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    64,  0,
            ('M' - ' ') , 0, 0x0008, 0x0008,    72,  0,
        };

    const Sint16 SMLN_mixed_pat[] = {
            5,
            ('M' - ' ') , 0, 0x0008, 0x0008,     0,  0,
            ('I' - ' ') , 0, 0x0008, 0x0008,     8,  0,
            ('X' - ' ') , 0, 0x0008, 0x0008,    16,  0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    24,  0,
            ('D' - ' ') , 0, 0x0008, 0x0008,    32,  0,
        };


Sint16 * const SMLN_mode_pattbl[] = {
            SMLN_hlinescl_pat,
            SMLN_vcelscl_pat,
            SMLN_hlzoomscl_pat,
            SMLN_hvlinescl_pat,
            SMLN_mixed_pat,
         };


/*==========================================================================*/

/*------------------------------------------------------
 *    横ラインスクロールメインルーチン
 *-----------------------------------------------------*/
void SMLN_LnscMode(void)
{

    enum {
          INIT,
          LOOP,
          NEXT
    };

    /* VRAMアクセスサイクルパターンテーブル */
    const Uint16 LNSC_CYC[] = { 0xc044, 0xffff,  
                                0xffff, 0xffff, 
                                0xffff, 0xffff, 
                                0xffff, 0xffff };

    Uint16 i, x;
    Uint16 *wrptr16;
    SclWinscl *winreg;

    while (SMMA_MainMode == LNSC_MODE) {
        switch (SMMA_MainLevel) {

            case INIT:    

                SMV2_TvOff();
                SMMA_initall();

                /* NBG1表示offとVRAMアクセスサイクルパターンセット */
                SCL_Open(SCL_NBG1);
                SCL_InitConfigTb(&SMV2_nscrlreg1); 
                SMV2_nscrlreg1.dispenbl = OFF;                  
                SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
                SCL_SetCycleTable(LNSC_CYC);  
                SCL_Close();

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                /* プライオリティの設定 */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);

                /* ラインスクロールパラメータ初期化 */
                SCL_Open(SCL_NBG0); 
                SCL_InitLineParamTb(&SMV2_lparam);
                SCL_Close();        

                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SMLN_cgset();                     /* CG DATA SET */

                /* アクションセット */
                SMTA_MakeAction(SMLN_LineScrollAct);


                SMV2_TvOn();
                SMMA_MainLevel++;
                break;

            case LOOP:    
                SMV1_SprCmdStart();

                /* アクション実行 */
                SMTA_ActionLoop();

                SMV1_SprCmdEnd();

                /* V-BLANKインタラプト待ち*/
                SMMA_intwait();     
                if (SMPA_pad_edge1 & PAD_START)
                    SMMA_MainLevel++;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
        }
    }

}



/*==========================================================================*/

/*---------------------------
 *    CG SET
 *--------------------------*/
void SMLN_cgset(void)
{
    const Uint16 nbg1_map[] = {
        0x1082,0x1084,0x1086,0x1088,
        0x108a,0x108c,0x108e,0x1090,
        0x1092,0x1094,0x1096,0x1098,
        0x109a,0x109c,0x109e,0x10a0,
    };

    Uint16 *dst16, *src16, *temp16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;

    /* アスキーフォントのセット */
    SMV1_SetFont4sp(0,1,0);
    SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);

    /* アスキーフォントカラーのセット */
    poke_w(COLOR_RAM + 0, 0x0000);
    poke_w(COLOR_RAM + 2, 0x739c);
    poke_w(COLOR_RAM + 0x20, 0x0000);
    poke_w(COLOR_RAM + 0x22, 0x739c);

    /* スクロール用CGの転送 */
    SMMI_Ldir8((Uint8 *)scra_infosc.cgadd, 
               (Uint8 *)(NBG_PTGEN+0x40*0x40), 
               (Uint32)scra_infosc.cgsize); 


    /* スクロール用カラーの転送 */
    SMMI_Ldir16((Uint16 *)scra_infosc.clradd, 
                (Uint16 *)(COLOR_RAM+0x200), 
                  (Sint32)scra_infosc.clrcnt); 

    /* NBG0表示画面クリア */
    SMV2_FilMem16((Uint16 *)NBG0_PTNAMA, 
                  (0x40 * 0x40 / sizeof (Uint16)),
                   0x0000);

    /* スクロールマップのセット */
    dst16 = (Uint16 *)NBG0_PTNAMA;
    for (v = 0; v < (64 / 4); v++) {
         temp16 = dst16;
         for (h = 0; h < (64 / 4); h++) {
              SMV2_BoxWrite16(temp16, nbg1_map, 4, 4);
              temp16 += 0x4;
         }
         dst16 +=  (0x40 * 4);  /* 次のラインへ */
    }


}





/*==========================================================================*/

/*
 *    ラインスクロールコントール
 */
void SMLN_LineScrollAct(LNSCACT *ix)
{


    enum {
        INIT,
        MODE_INIT,
        MODE_MAIN,
        NEXT 
    };

    /* 各デモの初期化ルーチンアドレステーブル */
    void (* const SMLN_Linescl_initfunc[])(LNSCACT *ix) = {
        SMLN_hline_init,
        SMLN_vcell_init,
        SMLN_hzoom_init,
        SMLN_hvscl_init,
        SMLN_mixed_init,
    };

    /* 各デモの実行ルーチンアドレステーブル   */
    void (* const SMLN_Linescl_execfunc[])(LNSCACT *ix) = {
        SMLN_hline_main,
        SMLN_vcell_main,
        SMLN_hzoom_main,
        SMLN_hvscl_main,
        SMLN_mixed_main,
    };


    Uint16 swdata, i;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_sdd;
            ix->sprite.patbase = SMLN_mode_pattbl;
            ix->sprite.xposi = 0x8 * 0x10000;
            ix->sprite.yposi = 0x8 * 0x10000;
            ix->sprite.patno = 0;
            ix->sprite.color = 0;
          
            ix->exeno = 0;
            ix->mode++;
        case MODE_INIT:
            /* ラインスクロール初期化           */
            SCL_Open(SCL_NBG0); 
            SCL_InitLineParamTb(&SMV2_lparam);
            SCL_Close();        

            /* 各モードでの初期化ルーチンを呼ぶ */
            (SMLN_Linescl_initfunc[ix->exeno])(ix);

            /* 各モードをスプライトで画面表示   */
            ix->sprite.patno = ix->exeno;

            ix->mode++;
        case MODE_MAIN:
            /* 各モードでの実行ルーチンを呼ぶ   */
            (SMLN_Linescl_execfunc[ix->exeno])(ix);

            if (SMPA_pad_edge1 & PAD_C) {
                ix->exeno++;
                if (ix->exeno >= sizeof (SMLN_Linescl_execfunc) / 
                                 (sizeof (SMLN_Linescl_execfunc[0])))
                    ix->exeno = 0;
                ix->mode = MODE_INIT;
            }
            break;
    }	    

    /* ラインスクロールパラメータセット */
    SCL_Open(SCL_NBG0); 
    SMV2_lparam.h_enbl = ON;        /* h方向スクロールする  */
    SMV2_lparam.v_enbl = ON;        /* v方向スクロールする  */
    SMV2_lparam.delta_enbl = ON;    /* ラインズームする     */
    SMV2_lparam.cell_enbl = ON;     /* 縦セルスクロールする */
    SMV2_lparam.interval = 0;       /* 1ライン間隔          */
    SMV2_lparam.line_addr = NBG0_HLSCLTBL; /* H-LINESCRL TABLE VRAM ADD SET */
    SMV2_lparam.cell_addr = NBG0_VCSCLTBL; /* V-CELLSCRL TABLE VRAM ADD SET */
    SCL_SetLineParam(&SMV2_lparam);
    SCL_Close();        

    /* スプライト表示 */
    SMV1_PutSprite(&ix->sprite);
}




/*
 *    Hラインスクロールデモ初期化
 */
void SMLN_hline_init(LNSCACT *ix)
{
    /* 角度クリア */
    ix->hlwk[0] = 0;

    /* 揺れ幅セット */ 
    ix->hlwk[1] = 0x2000;
            
    /* 初期スクロール量 */
    ix->hlwk[2] = 0;

}


/*
 *    Hラインスクロールデモメイン
 */
void SMLN_hline_main(LNSCACT *ix)
{
    Uint16 swdata, i;
    Sint16 direc;
    Sint32 hldata;
    
    /* 角度増加 */
    ix->hlwk[0]++;

    /* スクロール量更新 */
    ix->hlwk[2]++;

    /* パッドによる操作で揺れ幅をコントロール */
    swdata = SMPA_pad_data1;

    if (swdata & PAD_LEFT) {
        ix->hlwk[1] -= 0x100;
        if (ix->hlwk[1] < -0x7000)
            ix->hlwk[1] = -0x7000;
    }

    if (swdata & PAD_RIGHT) {
        ix->hlwk[1] += 0x100;
        if (ix->hlwk[1] > 0x7000)
            ix->hlwk[1] = 0x7000;
    }

    for (i = 0; i < 0xe0; i++) {
         direc = ix->hlwk[0] + i;
         hldata = SMMI_Sinset8(direc);
         hldata *= ix->hlwk[1];
         hldata += (ix->hlwk[2] * 0x10000);
         SMV2_lparam.line_tbl[i].h = hldata;
    }
}



/*
 *    縦セルスクロールデモ初期化
 */
void SMLN_vcell_init(LNSCACT *ix)
{
    /* 角度クリア */
    ix->vcwk[0] = 0;

    /* 揺れ幅セット */ 
    ix->vcwk[1] = 0x2000;
}

/*
 *    縦セルスクロールデモメイン
 */
void SMLN_vcell_main(LNSCACT *ix)
{
    Uint16 swdata, i;
    Sint16 direc;
    Sint32 vcdata;

    /* 角度増加 */
    ix->vcwk[0]++;

    /* パッドによる操作で揺れ幅をコントロール */
    swdata = SMPA_pad_data1;
    if (swdata & PAD_UP) {
        ix->vcwk[1] -= 0x100;
        if (ix->vcwk[1] < -0x5000)
            ix->vcwk[1] = -0x5000;
    }

    if (swdata & PAD_DOWN) {
        ix->vcwk[1] += 0x100;
        if (ix->vcwk[1] > 0x5000)
            ix->vcwk[1] = 0x5000;
    }

    for (i = 0; i < 0x40; i++) {
         direc = ix->vcwk[0] + (i * 4);                 
         vcdata = SMMI_Sinset8(direc);
         vcdata *= ix->vcwk[1];
         SMV2_lparam.cell_tbl[i] = vcdata;
    }
}


/*
 *     hラインズームデモ初期化
 */
void SMLN_hzoom_init(LNSCACT *ix)
{
    /* 角度クリア */
    ix->hzwk[0] = 0;

    /* 揺れ幅セット */ 
    ix->hzwk[1] = 0x0;
  
    /* 初期スクロール量 */
    ix->hzwk[2] = 0;
}

/*
 *     hラインズームデモメイン
 */
void SMLN_hzoom_main(LNSCACT *ix)
{
    Uint16 i;
    Sint16 direc;
    Sint32 hzdata;

    /* 角度増加 */
    ix->hzwk[0]++;
    ix->hzwk[2]++;

    for (i = 0; i < 0xe0; i++) {
         direc = ix->hzwk[0] + i;
         hzdata = 0xa000 + SMMI_Sinset8(direc) * 0x60;
         SMV2_lparam.line_tbl[i].dh = hzdata;
    }
}



/*
 *    HラインVスクロールデモ初期化
 */
void SMLN_hvscl_init(LNSCACT *ix)
{
    ix->hlwk[0] = 0;
    ix->hlwk[1] = 0;
    ix->hlwk[2] = 0;

    ix->vswk[0] = 0;
    ix->vswk[1] = 0;
    ix->vswk[2] = 0;
}


/*
 *    HラインVスクロールデモメイン
 */
void SMLN_hvscl_main(LNSCACT *ix)
{
    enum {
        MODE1, MODE2, MODE3
    };

    Uint16 i;
    Sint16 direc,wdata0, wdata1;
    Sint32 hldata, vsdata;

    ix->vswk[0]++;

    /* 0x00~0x1fラインまでのスクロール量決定 */
    for (i = 0; i < 0x20; i++) {
         vsdata = (ix->vswk[0] + i) * 0x10000;
         SMV2_lparam.line_tbl[i].v = vsdata;
    }

    /* 0x20~0x3fラインまでのスクロール量決定 */
    for (i; i < 0x40; i++) {
         vsdata = -ix->vswk[0]+i;
         vsdata *= 0x10000;
         SMV2_lparam.line_tbl[i].v = vsdata;
    }      

    /* 0x40~0x7fラインまでのスクロール量決定 */
    wdata0 = i + (ix->vswk[0] & 0x3f);
    for (i; i < wdata0; i++) {
         vsdata = i * 0x10000;
         SMV2_lparam.line_tbl[i].v = vsdata;
    }
    for (i; i < 0x80; i++) {
         vsdata = wdata0 * 0x10000;
         SMV2_lparam.line_tbl[i].v = vsdata;
     }

    /* 0x80~0xdfラインまでのスクロール量決定 */
    wdata0 = ix->vswk[0] & 0x3f0;
    wdata1 = i * 0x100;
    for (i; i < 0xe0; i++) {
         vsdata = wdata1 * 0x100;
         wdata1 += wdata0;
         SMV2_lparam.line_tbl[i].v = vsdata;
    }

}







/*
 *    ラインスクロール応用例２ (初期化)
 */
void SMLN_mixed_init(LNSCACT *ix)
{
    Uint16 swdata, i;
    Sint16 hldirec, hzdirec;
    Sint32 hldata;
    Sint32 hzdata;

    SMLN_hline_init(ix);
    SMLN_hzoom_init(ix);
    SMLN_hvscl_init(ix);

    /* hラインのズームをあらかじめ計算しておく */
    for (i = 0; i < 0xe0; i++) {
         hzdirec = (i * 0x80) / 0xe0;
         hzdata = 0x4000 + 0xc0 * SMMI_Sinset8(hzdirec);
         SMV2_lparam.line_tbl[i].dh = hzdata;
    }


}

/*
 *    ラインスクロール応用例２ (メインルーチン)
 */
void SMLN_mixed_main(LNSCACT *ix)
{
    Uint16 swdata, i;
    Sint16 hldirec, hzdirec;
    Sint32 hldata, hzdata;
    Sint32 vsdata;
    SclLineTb *lineptr;

    /* スクロール量更新 */
    ix->hlwk[2]++;
    ix->vswk[0]++;

    lineptr = &SMV2_lparam.line_tbl[0];
    for (i = 0; i < 0xe0; i++) {
         hzdata = lineptr->dh;
         hldata = (WIDTH_H/2 * 0x10000) - (WIDTH_H/2 * hzdata);
         hldata += (ix->hlwk[2] * 0x10000);
         vsdata = (ix->vswk[0] + i) * 0x10000;
         lineptr->h = hldata;
         lineptr->v = vsdata;
	 lineptr++;
    }




    
}









