/*	Copyright(c) 1994 SEGA			*/
/****************************************************************************/
/*  サンプルプログラム                                                      */
/*  スクロールサンプル                                                      */
/*  回転スクロール                                                          */
/*  filename "smp_scl3.c"													*/
/*  author   Y.K                                                            */
/*  created  1994-05-11                                                     */
/****************************************************************************/

#include "smp_bios.h"

#include "smp_sccg.h"


SMTA_DefActWk(SMSCACT, );						/*	アクションワーク作成	*/

static void SMSC_rbg0Act(SMSCACT *ix);
static void SMSC_cgset(void);

/****************************************************************************/
/*	回転スクロールエントリーポイント										*/
/****************************************************************************/
void SMSC_RotateScl(void)
{
	enum {
		SCL_INIT,
		SCL_MAIN,
		SCL_EXIT
	};

	SclVramConfig tp;
	SclConfig scfg;
	Uint16 BackCol;
        Uint32 RotateTbAddr, i;
        Uint16 *adrs;

	while(SMMA_MainMode == SMSC_ROTSCLMODE) {
		switch(SMMA_MainLevel) {
			case SCL_INIT:
				SMMA_initall();
/*******************************************
 *	Priority Initialization            *
 *	Scroll Parameters Initialization   *
 *******************************************/
			    SCL_Vdp2Init();
				SCL_VblInit();
                SMV2_TvOff();	
				SCL_SetFrameInterval(1);
			    SCL_SetDisplayMode(SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A);
				SCL_SetPriority(SCL_SP0 | SCL_SP1 | SCL_SP2 | SCL_SP3
				 | SCL_SP4 | SCL_SP5 | SCL_SP6 | SCL_SP7, 0);
			    SCL_SetColRamMode(SCL_CRM15_2048);

			    SMSC_cgset();					/*	ＣＧセット				*/

/*******************************************
 *	バック画面の色を黒に設定           *
 *******************************************/
			    BackCol = 0x0000;
			    SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

/*******************************************
 *	ＶＲＡＭの使用方法の設定              *
 *******************************************/
			    SCL_InitVramConfigTb(&tp);
				tp.vramModeA = ON;
				tp.vramModeB = ON;
		        tp.vramA0 = SCL_RBG0_CHAR;
		        tp.vramA1 = SCL_RBG0_K;
		        tp.vramB0 = SCL_RBG0_PN;
		        tp.vramB1 = SCL_NON;
			    SCL_SetVramConfig(&tp);

/*******************************************
 *	回転パラメータテーブルの初期化
 *******************************************/
			    RotateTbAddr = SCL_VDP2_VRAM_A1 + 0x10000;
			    SCL_InitRotateTable(RotateTbAddr, 2, SCL_RBG0, SCL_NON);

/*******************************************
 *	Scroll Configure Set               *
 *******************************************/
			    SCL_InitConfigTb(&scfg);
				scfg.dispenbl      = ON;
			    scfg.charsize      = SCL_CHAR_SIZE_1X1;
			    scfg.pnamesize     = SCL_PN1WORD;
			    scfg.platesize     = SCL_PL_SIZE_1X1;
				scfg.coltype       = SCL_COL_TYPE_256;
				scfg.datatype      = SCL_CELL;
			    for(i = 0; i < 16; i++) {
			        scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
			    }
				SCL_SetConfig(SCL_RBG0, &scfg);

/*******************************************
 *	VRAM Access Pattern Set            *
 *******************************************/
	
			    SCL_SetPriority(SCL_RBG0,7);

/*********************************************
 *	これをやらないと絵が出ない
 **********************************************/
				set_imask(0);

/***********************************************
 *	回転スクロールの座標、スケールなどの設定
 ***********************************************/
			    SCL_Open(SCL_RBG_TB_A);
				SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
				SCL_Scale(FIXED(1.0), FIXED(1.0));
			    SCL_Close();
				
/***********************************************
 *	アクションの設定
 **************************************************/
				SMTA_MakeAction(SMSC_rbg0Act);

				SMMA_MainLevel++;
				break;
			case SCL_MAIN:
				SMMA_intwait();					/*	Ｖ－ＢＬＡＮＫを待つ	*/
				SMTA_ActionLoop();				/*	アクションループ実行	*/
				break;

			case SCL_EXIT:
				SMMA_MainMode = SMSC_SELECTMODE;
				SMMA_MainLevel = 0;
				break;
		}
	}		
}

static void SMSC_rbg0Act(SMSCACT *ix)
{
	enum {
		BGN0,
        BGN1,
        INIT,
		MAIN
	};

	static Fixed32 mx, my, mz, axy, az, ad;
    static Fixed32 sx, sy;
    static Uint32 cnt;

	switch(ix->level) {
        case BGN0:
            sx = FIXED(0.00);
            sy = FIXED(0.00);
            cnt = 0;
            SCL_Open(SCL_RBG_TB_A);
            SCL_Scale(sx, sy);
            SCL_Close();
            SMV2_TvOn();
            ix->level++;
            break;
        case BGN1:
            sx += FIXED(0.01);
            sy += FIXED(0.01);
            SCL_Open(SCL_RBG_TB_A);
            SCL_Scale(sx, sy);
            cnt++;
            if(cnt == 100) {
                ix->level++;
                SCL_Scale(FIXED(1),FIXED(1));
            }
            SCL_Close();
            break;
		case INIT:
			mx = FIXED(0);
			my = FIXED(0);
            mz = FIXED(0);
            axy = FIXED(0);
            az = FIXED(0);
            ad = FIXED(0);
            SCL_Open(SCL_RBG_TB_A);
            SCL_MoveTo(mx, my, mz);
            SCL_RotateTo(axy, az, ad, SCL_X_AXIS);
            SCL_Close();
			ix->level++;
			break;
		case MAIN:
                    if ((SMPA_pad_repdata & PAD_LEFT))
                        mx = mx + FIXED(0.1);
                    if ((SMPA_pad_repdata & PAD_RIGHT))
                        mx = mx - FIXED(0.1);
                    if ((SMPA_pad_repdata & PAD_UP))
                        my = my + FIXED(0.1);
                    if ((SMPA_pad_repdata & PAD_DOWN))
                        my = my - FIXED(0.1);
                    if ((SMPA_pad_repdata & PAD_L))
	                az = az + FIXED(1);
	            if ((SMPA_pad_repdata & PAD_R))
	                az = az - FIXED(1);
	            if ((SMPA_pad_repdata & PAD_A))
	                ad = ad - FIXED(1);
	            if ((SMPA_pad_repdata & PAD_C))
	                ad = ad + FIXED(1);
	            if ((SMPA_pad_repdata & PAD_Y))
	                axy = axy - FIXED(1);
	            if ((SMPA_pad_repdata & PAD_B))
	                axy = axy + FIXED(1);
		    mz = FIXED(0);
	            if ((SMPA_pad_repdata & PAD_X))
	                mz = FIXED(-1);
	            if ((SMPA_pad_repdata & PAD_Z))
	                mz = FIXED(1);
		    if((SMPA_pad_repdata & PAD_L) && 
                      (SMPA_pad_repdata & PAD_R)) {
		 	 mx = 0; my = 0; mz = 0; axy = 0; az = 0; ad = 0;
	                 SCL_Open(SCL_RBG_TB_A);
	                 SCL_MoveTo(0,0,0);
     	                 SCL_Close(); 
		    }
		    if ((SMPA_pad_repdata & PAD_START)) {
		        SMMA_MainLevel++;
	            }
			SCL_Open(SCL_RBG_TB_A);
                    SCL_RotateTo(axy, az, ad, SCL_X_AXIS);
			SCL_Move(mx, my, mz);
			SCL_Close();
			break;
	}
}

static void SMSC_cgset(void)
{
    const Uint16 nbg1_map[] = {
        0x0002, 0x0004, 0x0006, 0x0008,
        0x000a, 0x000c, 0x000e, 0x0010,
        0x0012, 0x0014, 0x0016, 0x0018, 
        0x001a, 0x001c, 0x001e, 0x0020
    };

    Uint16 *dst16, *src16, *temp16;
    Uint16 h, v, i, wk;
    Uint32 *dst32, *temp32, *dst32b;

    SMV1_SetFont4sp(0,1,0);
    SMV2_SetFont8((Uint8 *)SCL_VDP2_VRAM_A0, 1, 0);

    poke_w(COLOR_RAM + 0, 0x0000);
    poke_w(COLOR_RAM + 2, 0x739c);
    poke_w(COLOR_RAM + 0x20, 0x0000);
    poke_w(COLOR_RAM + 0x22, 0x739c);

    SMMI_Ldir8((Uint8 *)scra_infosc.cgadd,
     (Uint8 *)(SCL_VDP2_VRAM_A0),
     (Uint32)scra_infosc.cgsize);

    SMMI_Ldir16((Uint16 *)scra_infosc.clradd,
     (Uint16 *)(COLOR_RAM),
     (Uint32)scra_infosc.clrcnt);

    SMV2_FilMem16((Uint16 *)SCL_VDP2_VRAM_B0,
     (0x40 * 0x40 / sizeof(Uint16)),
     0x0000);

    dst16 = (Uint16 *)SCL_VDP2_VRAM_B0;
    for(v = 0; v < (64 / 4); v++) {
        temp16 = dst16;
        for(h = 0; h < (64 / 4); h++) {
            SMV2_BoxWrite16(temp16, nbg1_map, 4, 4);
            temp16 += 0x4;
        }
        dst16 += (0x40 * 4);
    }
}

