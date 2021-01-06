
/*	Copyright(c) 1994 SEGA			*/
/*
 *     
 *     NBG0拡大縮小テスト
 *
 */

#include "smp_bios.h"

#include "smp_ncg.h"
#include "smp_nbgz.h"



/* 
 *    外部参照ワーク
 */
extern SclSysreg Scl_s_reg;
extern SclNorscl Scl_n_reg;


/*
 *    関数プロトタイプ
 */
void SMNB_cgset(void);
void SMNB_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize);

void SMNB_scrlcnt(SMNBACT *ix);


/* スクロールVRAMアクセスサイクルパターンテーブル */
const Uint16 SMNB_CYC[] = { 0xf044, 0xf44f, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff };


/*
 *  メインルーチン
 */
void SMNB_MainMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k;
    Uint16 swedge;
    Uint32 times;

    do {  /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
        swedge = SMPA_pad_edge1;
        SMV1_SprCmdStart();                /* VDP1 SPRITE START */
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Vdp2Init();
                SCL_SetFrameInterval(0xffff);   /* Intrpt Mode  */

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);


                SCL_InitVramConfigTb(&SMV2_sclvram);
                SMV2_sclvram.vramModeA = OFF;
                SMV2_sclvram.vramModeB = OFF; 
                SMV2_sclvram.vramA0 = SCL_NON;
                SMV2_sclvram.vramA1 = SCL_NON;
                SMV2_sclvram.vramB0 = SCL_NON;
                SMV2_sclvram.vramB1 = SCL_NON;
                SCL_SetVramConfig(&SMV2_sclvram);

                SCL_Open(SCL_NBG0);
                SCL_InitConfigTb(&SMV2_nscrlreg0);
                SMV2_nscrlreg0.dispenbl = ON;
                SMV2_nscrlreg0.charsize = SCL_CHAR_SIZE_1X1;
                SMV2_nscrlreg0.pnamesize = SCL_PN1WORD;
                SMV2_nscrlreg0.platesize = SCL_PL_SIZE_1X1;
                SMV2_nscrlreg0.bmpsize = SCL_BMP_SIZE_512X256;
                SMV2_nscrlreg0.coltype = SCL_COL_TYPE_256;
                SMV2_nscrlreg0.datatype = SCL_CELL;
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_nscrlreg0.mapover = SCL_OVER_0;
                SMV2_nscrlreg0.plate_addr[0] = SMNB_NBG0_PTNAMA;
                SMV2_nscrlreg0.plate_addr[1] = SMNB_NBG0_PTNAMA;
                SMV2_nscrlreg0.plate_addr[2] = SMNB_NBG0_PTNAMA;
                SMV2_nscrlreg0.plate_addr[3] = SMNB_NBG0_PTNAMA;
                SCL_SetConfig(SCL_NBG0, &SMV2_nscrlreg0);
                SCL_SetCycleTable(SMNB_CYC);
                SCL_Close();

                SCL_Open(SCL_NBG1);
                SCL_InitConfigTb(&SMV2_nscrlreg1);
                SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
                SCL_Close();

                Scl_n_reg.linecolmode = 0x00000000;
                Scl_n_reg.backcolmode = 0x00000000 |
                                         ((0x00000000 & 0xfffff) / 2);


                /* プライオリティの設定 */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);

                Scl_s_reg.dispenbl |= 0x0100; /* NBG0 透明 ON */

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* CGデータの展開 */
                SMNB_cgset(); 


                /* アクションのセット */
                SMTA_MakeAction(SMNB_scrlcnt);
                SMV2_TvOn();               
                SMMA_MainLevel++;

            case MAIN:
                SCL_Open(SCL_NBG0);
                SCL_Close();
                SMTA_ActionLoop();  /* アクション実行   */

                if (swedge & PAD_START) SMMA_MainLevel = NEXT;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    } while (SMMA_MainMode == SMNB_MODE);

}

	    
/*
 *   SPRITE CG SET
 */
void SMNB_cgset(void)
{
    
    Uint16 *dst16, *src16, *temp16, *wrptr16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;
    Uint32 direc;


    SMMI_Ldir8(SNC256_info.cgadd, (Uint8 *)SMNB_NBG_PTGEN+0x40,
                                                     SNC256_info.cgsize);

    SMNB_BoxWrite16((Uint16 *)SMNB_NBG0_PTNAMA, SNC256_info.mapadd, 
                                  SNC256_info.hsize, SNC256_info.vsize);

    SMMI_Ldir16(SNC256_info.clradd, (Uint16 *)COLOR_RAM, SNC256_info.clrcnt);

}


/* <<<< このモード専用箱型書き込み >>>> */
void SMNB_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    Uint16 rdata0, rdata1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
             rdata0 = (*mapdata) & 0x0fff;
             rdata1 = (*mapdata) & 0xf000;
             rdata0 = rdata1 + (rdata0 * 2);
 	     *dst1 = rdata0;
             dst1++;
             mapdata++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}



/*
 *     コントロールアクション
 */
void SMNB_scrlcnt(SMNBACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};

    Uint16 swdata;
    SclNorscl *n_reg_ptr;

    swdata = SMPA_pad_data1;
    n_reg_ptr = &Scl_n_reg;
    switch (ix->mode) {
        case INIT:
            n_reg_ptr->zoomenbl |= 0x01; /* 縮小設定 1/2 */ 
            n_reg_ptr->n0_move_x = 0x000000000;
            n_reg_ptr->n0_move_y = 0x000000000;
            n_reg_ptr->n0_delta_x = 0x10000;
            n_reg_ptr->n0_delta_y = 0x10000;
            ix->mode++;
            break;
        case MOVE:
            if (swdata & PAD_UP)
                n_reg_ptr->n0_move_y += 0x10000;
            if (swdata & PAD_DOWN)
                n_reg_ptr->n0_move_y -= 0x10000;

            if (swdata & PAD_LEFT)
                n_reg_ptr->n0_move_x += 0x10000;
            if (swdata & PAD_RIGHT)
                n_reg_ptr->n0_move_x -= 0x10000;

            if (swdata & PAD_A) 
                n_reg_ptr->n0_delta_x += 0x1000;
            if (swdata & PAD_C) 
                n_reg_ptr->n0_delta_x -= 0x1000;

            if (swdata & PAD_X) 
                n_reg_ptr->n0_delta_y += 0x1000;
            if (swdata & PAD_Z) 
                n_reg_ptr->n0_delta_y -= 0x1000;


            if (n_reg_ptr->n0_delta_x > 0x1ffff)
                n_reg_ptr->n0_delta_x -= 0x1000;
            if (n_reg_ptr->n0_delta_y > 0x1ffff)
                n_reg_ptr->n0_delta_y -= 0x1000;


            if (n_reg_ptr->n0_delta_x < 0x8000)
                n_reg_ptr->n0_delta_x += 0x1000;
            if (n_reg_ptr->n0_delta_y < 0x8000)
                n_reg_ptr->n0_delta_y += 0x1000;



            if (swdata & PAD_R) ix->mode = INIT;

            break;
    }

   
}
#if 0
	Fixed32	n0_move_x;		/* NBG0 Scroll Movement X */
	Fixed32	n0_move_y;		/* NBG0 Scroll Movement Y */
	Fixed32	n0_delta_x;		/* NBG0 Scroll Movement X */
	Fixed32	n0_delta_y;		/* NBG0 Scroll Movement Y */
	Fixed32	n1_move_x;		/* NBG1 Scroll Movement X */
	Fixed32	n1_move_y;		/* NBG1 Scroll Movement Y */
	Fixed32	n1_delta_x;		/* NBG1 Scroll Movement X */
	Fixed32	n1_delta_y;		/* NBG1 Scroll Movement Y */
	Uint16	n2_move_x;		/* NBG2 Scroll Movement X */
	Uint16	n2_move_y;		/* NBG2 Scroll Movement Y */
	Uint16	n3_move_x;		/* NBG3 Scroll Movement X */
	Uint16	n3_move_y;		/* NBG3 Scroll Movement Y */
	Uint16	zoomenbl;		/* Zoom Enable */
	Uint16	linecontrl;		/* Normal Extension Control */
	Uint32	celladdr;		/* Cell Scroll Table Address */
	Uint32	lineaddr[2];		/* Line Scroll Table Address x2 */
	Uint32	linecolmode;		/* Line BG Color Mode Address */
	Uint32	backcolmode;		/* Back BG Color Mode Address */
#endif







