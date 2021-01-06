/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      ��]�ʂ̌W���e�[�u�����g�����T���v��(�X�N���[���ʂ����`�Ɍ�����)
 *
 */

/*
 *  ���\���̂��߂̌W���v�Z
 *
 *  ���̕����� (x - cx)^2 + (y - cy)^2 + (z - cz)^2 = r^2 ��
 *              cx,cy,cz :���S���W r:���a
 *  cz = 0�ɂ����Ƃ���z�ɂ��ĉ����܂��B ����ƁA
 *
 *  z = sqrt(r^2 - (x-cx)^2 - (y-cy^2))... ���P :sqrt()�͕�����
 *
 *  ��]�X�N���[���ʂ̊g��k���W�� k �́A
 *
 *  k = -zp / (zs - zp) ...���Q  : zp:�ϊ���̎��_z���W zs �̓X�N���[��z���W
 *
 *  ����āA
 *
 *  �X�N���[���シ�ׂẴh�b�g�ɂ������Ď��P����z�����߁A���߂�z�����Q��
 *
 *  ������邱�Ƃɂ���ČW�������߂܂��B
 *
 *
 *  ���ׂẴh�b�g�ɂ������Čv�Z���s�Ȃ��̂ŌW���̃A�h���X�����l��������������
 *
 *  �P�^�S�~�������̂݌v�Z���Ă��Ƃ̓R�s�[����ȂǍ����ɍs�Ȃ��ɂ͍H�v��
 *
 *  �K�v�ł��B
 *
 *
 *  �W���A�h���X�����l�́A
 *
 *  �W���A�h���X = (�X�^�[�g�A�h���XKAst) 
 *               + (x�����ldKAx) * x
 *               + (y�����ldKast) * y
 *  
 *  �Ōv�Z���ꂽ�A�h���X�̌W�����L���ɂȂ�܂��B�h�b�g�P�ʂŌW����ǂ܂�����
 *  �Ƃ��́A
 *
 *  �W���A�h���X = (�X�^�[�g�A�h���X)
 *               + (1 * 0x10000) * x
 *               + (���������̃h�b�g�� * 0x10000) * y
 *  �ɂȂ�܂��B
 *
 *
 *  ��A���������R�Q�O�h�b�g�Ńh�b�g���ƂɌW�����E�������ꍇ�́A
 *
 *  �W���A�h���X = (�X�^�[�g�A�h���X)
 *               + (1 * 0x10000) * x
 *               + (320 * 0x10000) * y
 *  �ƂȂ�܂��B
 *
 *  
 *
 */


#include "smp_bios.h"

#include "smp_sccg.h"
#include "smp_ncg.h"
#include "smp_rkei.h"

/*
 *     �O���Q�ƃ��[�N
 */
extern SclRotscl Scl_r_reg;         
extern SclSysreg Scl_s_reg;
extern SclDataset Scl_d_reg;
extern SclNorscl Scl_n_reg;
extern SclWinscl Scl_w_reg;


/* <<<< �֐��v���g�^�C�v >>>> */
void SMRK_cgset(void);
void SMRK_XZRotScrlCnt(Sint16 xdirec, Sint16 zdirec);
void SMRK_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize);
void SMRK_XZRotScrlCnt(Sint16 xdirec, Sint16 zdirec);
void SMRK_VintRotRegSet(void);

void SMRK_ScrlCnt(SMRKACT *ix);
void SMRK_KeisuCnt(void);
void SMRK_KeisuCntX(Sint32 r, Sint32 zoom);
void SMRK_CPUDmaLdir160(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);
void SMRK_KeisuBufErase(void);

void SMRK_ScrlCntX(SMRKACT *ix);
void SMRK_HakeiSet(Sint32 cx, Sint32 cy, Sint32 len, Sint32 time);

void SMRK_SCUDMA_LDIR0(Uint32 src, Uint32 dst, Uint32 trcnt);


/* �X�N���[��VRAM�A�N�Z�X�T�C�N���p�^�[���e�[�u�� */
const Uint16 SMRK_CYC[] = { 0xffff, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff };


/* �萔 */
#define SMRK_KEICNTRL_DAT (0x13) /* �W���e�[�u���R���g���[�����W�X�^�ւ̒l */



/*
 *  ���C�����[�`��
 */
void SMRK_MainMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k, swedge;
    Uint32 times;
    SclRotscl *rotreg;
    ROTPARAM *rp_ptr;

    do {  /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
        SMV1_SprCmdStart();                /* VDP1 SPRITE START */

        swedge = SMPA_pad_edge1;
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SMRK_RotregReq = 0;

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Vdp2Init();
                SCL_SetFrameInterval(0xffff);   /* Intrpt Mode  */


                /* �J���[���[�h�̐ݒ� */
                SCL_SetColRamMode(SCL_CRM15_1024);

                SCL_InitVramConfigTb(&SMV2_sclvram);
                SMV2_sclvram.vramModeA = OFF;
                SMV2_sclvram.vramModeB = ON; 
                SMV2_sclvram.vramA0 = SCL_RBG0_CHAR;
                SMV2_sclvram.vramA1 = SCL_NON;
                SMV2_sclvram.vramB0 = SCL_RBG0_PN;
                SMV2_sclvram.vramB1 = SCL_RBG0_K;
                SCL_SetVramConfig(&SMV2_sclvram);

                SCL_Open(SCL_NBG0);
                SCL_InitConfigTb(&SMV2_nscrlreg0);
                SCL_SetConfig(SCL_NBG0, &SMV2_nscrlreg0);
                SCL_SetCycleTable(SMRK_CYC);
                Scl_s_reg.dispenbl |= 0x1000;
                SCL_Close();

                SCL_Open(SCL_NBG1);
                SCL_InitConfigTb(&SMV2_nscrlreg1);
                SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
                SCL_Close();

                Scl_n_reg.linecolmode = 0x00000000 |
                                         ((SMRK_LCLRTBLADD & 0xfffff) / 2);
                Scl_n_reg.backcolmode = 0x00000000 |
                                         ((SMRK_BCLRTBLADD & 0xfffff) / 2);

                poke_w(SMRK_BCLRTBLADD, 0x0000);

                SCL_Open(SCL_RBG0);
                SCL_InitConfigTb(&SMV2_rscrlreg);
                SMV2_rscrlreg.dispenbl = ON;        
                SMV2_rscrlreg.charsize = SCL_CHAR_SIZE_1X1; 
                SMV2_rscrlreg.pnamesize = SCL_PN2WORD;      
                SMV2_rscrlreg.platesize = SCL_PL_SIZE_1X1;  
                SMV2_rscrlreg.bmpsize = SCL_BMP_SIZE_512X256; 
                SMV2_rscrlreg.coltype = SCL_COL_TYPE_256;     
                SMV2_rscrlreg.datatype = SCL_CELL;    
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_rscrlreg.mapover = SCL_OVER_0;   
                for (i = 0; i < 16; i++) {
                     SMV2_rscrlreg.plate_addr[i] = SMRK_RBG0_PTNAM;
                }
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMRK_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
                SCL_Close();

                /* �v���C�I���e�B�̐ݒ� */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 0);
                SCL_SetPriority(SCL_NBG1, 0);
                SCL_SetPriority(SCL_RBG0, 6);

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Open(SCL_RBG0);
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMRK_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
	            Scl_r_reg.paramode = 0x0; /* ��]�p�����[�^���[�h�R */
                Scl_r_reg.k_contrl = 0x0303;
                Scl_r_reg.k_offset = (0x60000 / 0x20000) * 0x100
                                   + (0x60000 / 0x20000);
                Scl_s_reg.dispenbl |= 0x1000;
                Scl_s_reg.ramcontrl |= 0x0040; /* �W���w�� */
                SCL_Close();

                  
                /* �������e�[�u���̏����� */
                for (i = 0; i < 0x4000; i++) {
                    SMRK_sqrttbl[i] = SMCA_sqrt(i);
                }                

                /* Int Hook1 */
                SMIN_VintHook1  = &SMRK_VintRotRegSet;

                /* CG�f�[�^�̓W�J */
                SMRK_cgset(); 

                /* �A�N�V�����̃Z�b�g */
                SMTA_MakeAction(SMRK_ScrlCnt);
                SMV2_TvOn();               
                SMMA_MainLevel++;

            case MAIN:
                SCL_Open(SCL_RBG0);
                SCL_Close();
                SCL_Open(SCL_NBG0);
                SCL_Close();
                SMTA_ActionLoop();  /* �A�N�V�������s   */
                SMRK_RotregReq = 1; /* ��]�X�N���[���p�����[�^�]���v�� */

                if (swedge & PAD_START) SMMA_MainLevel = NEXT;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    } while (SMMA_MainMode == SMRK_MODE);

}



/*
 *    CGSET
 */
void SMRK_cgset(void)
{
     Uint16 i, j, k;
     Sint16 direc;
     Uint8 *wkptr;
     Uint16 *dstptrb, *dstptr, *page;
     Uint16 wrdata;
     Uint32 *dst32;
     Uint32 dt;

    SMV2_FilMem16((Uint16 *)(SCR_VRAMB1), 0x20000/sizeof(Uint16), 0x400);

    SMV2_FilMem32((Uint32 *)SMRK_RBG0_PTNAM, 64*64, 0x0002);

    SMMI_Ldir8(SNC256_info.cgadd, (Uint8 *)SMRK_RBG0_PTGEN+0x40,
                                                     SNC256_info.cgsize);

    SMRK_MapWrite32((Uint32 *)SMRK_RBG0_PTNAM, SNC256_info.mapadd, 
                                  SNC256_info.hsize, SNC256_info.vsize);

    SMMI_Ldir16(SNC256_info.clradd, (Uint16 *)COLOR_RAM, SNC256_info.clrcnt);


    /* ---- ���C���J���[�̐ݒ� ---- */
    dstptr = (Uint16 *)(COLOR_RAM + 0x400);
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 0 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 1 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 2 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 3 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 4 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 5 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 6 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 7 */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 8 */
    *dstptr++ = (0x01 << 10) + (0x1f << 5) + 0x08; /* 9 */
    *dstptr++ = (0x14 << 10) + (0x00 << 5) + 0x00; /* A */
    *dstptr++ = (0x00 << 10) + (0x1f << 5) + 0x00; /* B */
    *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x1f; /* C */
    *dstptr++ = (0x14 << 10) + (0x00 << 5) + 0x1f; /* D */
    *dstptr++ = (0x14 << 10) + (0x1f << 5) + 0x00; /* E */
    *dstptr++ = (0x00 << 10) + (0x1f << 5) + 0x1f; /* F */

    dstptr = (Uint16 *)(SMRK_LCLRTBLADD);
    for (i = 0; i < 0x100; i++) {
         direc = (i * 0x80) / 0x100;
         *dstptr = (0x400/2); /*  + (0x1f * SMMI_Cosset8(direc)) / 0x100; */
         dstptr++;
    }

    SCL_Open(SCL_NBG0);
    Scl_n_reg.linecolmode = 0x80000000 | ((SMRK_LCLRTBLADD & 0xfffff) / 2);
    SCL_Close();

    /* �J���[���Z���[�h�̐ݒ� */
    SCL_SET_CCMD(1);    /* �J���[���Z���̂܂܉��Z�C�l�[�u�� */
    SCL_SET_CCRTMD(1);  /* ������2nd���Ŏw��             */
    SCL_SET_R0CCRT(0x8);
    SCL_SET_LCCCRT(0x1);
    SCL_SET_N0CCEN(0);  /* NBG0�J���[���Z�C�l�[�u��         */
    SCL_SET_R0CCEN(0);  /* ��]�ʃJ���[���Z�C�l�[�u��       */
    SCL_SET_SPCCEN(0);  /* �X�v���C�g�J���[���Z�C�l�[�u��   */
    SCL_SET_LCCCEN(0);  /* ���C���J���[�C�l�[�u��           */
    SCL_SET_R0LCEN(0);  /* ���C���J���[                     */
    SCL_SET_SPLCEN(0);  /* ���C���J���[                     */
    SCL_SET_EXCCEN(0);  /* �g���J���[���Z                   */

}




/*======================================================
 *  SMRK��p��]SCROLL BOX WRITE(64�Z�����E��Ή� WORD)
 *=====================================================*/
void SMRK_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint32 *dst0, *dst1;
    Uint32 rdata0, rdata1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
             rdata0 = *mapdata;
             mapdata++;
             rdata0 *= 2;
             rdata0 += 0;
 	     *dst1 = rdata0;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}




/*
 *     �R���g���[���A�N�V����
 */
void SMRK_ScrlCnt(SMRKACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};

    Uint16 swdata, swedge;
    Uint32 kadd, i;
    Sint32 hposi, vposi;
    ROTPARAM *rparamA;
    ROTPARAM *rparamB;

    swdata = SMPA_pad_data1;
    swedge = SMPA_pad_edge1;
    rparamA = &SMRK_rotregbufA;
    rparamB = &SMRK_rotregbufB;

    switch (ix->mode) {
        case INIT:
            rparamA->xst = 0x0000;
            rparamA->yst = 0x0000;
            rparamA->zst = 0;
            rparamA->dxst = 0;
            rparamA->dyst = 0x10000;
            rparamA->dx = 0x10000;
            rparamA->dy = 0;
            rparamA->mtr_a = 0x10000;
            rparamA->mtr_b = 0x00000;
            rparamA->mtr_c = 0x00000;
            rparamA->mtr_d = 0x00000;
            rparamA->mtr_e = 0x10000;
            rparamA->mtr_f = 0x00000;
            rparamA->kx = 0x10000;
            rparamA->ky = 0x10000;
            rparamA->mx = 0;   
            rparamA->my = 0;
            rparamA->px = 0xa0;
            rparamA->py = 0x70;
            rparamA->pz = 0x100;
            rparamA->cx = 0xa0;
            rparamA->cy = 0x70;
            rparamA->cz = 0x0;

            rparamB->xst = 0x0000;
            rparamB->yst = 0x0000;
            rparamB->zst = 0;
            rparamB->dxst = 0;
            rparamB->dyst = 0x10000;
            rparamB->dx = 0x10000;
            rparamB->dy = 0;
            rparamB->mtr_a = 0x40000;
            rparamB->mtr_b = 0x00000;
            rparamB->mtr_c = 0x00000;
            rparamB->mtr_d = 0x00000;
            rparamB->mtr_e = 0x40000;
            rparamB->mtr_f = 0x00000;
            rparamB->kx = 0x10000;
            rparamB->ky = 0x10000;
            rparamB->mx = 0;   
            rparamB->my = 0;
            rparamB->px = 0xa0;
            rparamB->py = 0x70;
            rparamB->pz = 0x100;
            rparamB->cx = 0xa0;
            rparamB->cy = 0x70;
            rparamB->cz = 0x0;

            SMRK_FilData = 0x400; /* �{�[���̊O���̌W���f�[�^ */
            SMRK_KeiEnd = 0;
            ix->xposi = 0;
            ix->yposi = 0;
            ix->zposi = 0;
            ix->zspeed = 0;
            ix->zoom = 0x10000;    /* �g�嗦   */
            ix->wreg[0] = 0;       /* ��]�p�x */
            ix->wreg[1] = 0x70;    /* ���a     */
            ix->wreg[2] = 0;       /* �{�[���ړ��p�p�x */
            ix->wcnt[0] = 0;
            ix->mode++;

        case MOVE:

            ix->xspeed = 0;
            ix->yspeed = 0;

            if (swdata & PAD_X) {
                ix->zoom += 0x1000;
                SMRK_KeiEnd = 0;
            }
            if (swdata & PAD_Z) {
                ix->zoom -= 0x1000;
                SMRK_KeiEnd = 0;
            }
            if (ix->zoom < 0x04000) ix->zoom = 0x04000;
            if (ix->zoom > 0x80000) ix->zoom = 0x80000;


            if (swdata & PAD_A) {
                ix->wreg[1] += 0x8;
                SMRK_KeiEnd = 0;
            }
            if (swdata & PAD_C) {
                ix->wreg[1] -= 0x8;
                SMRK_KeiEnd = 0;
            }
            if (ix->wreg[1] < 0x10) ix->wreg[1] = 0x10;
            if (ix->wreg[1] > 0x70) ix->wreg[1] = 0x70;


            if (swdata & PAD_UP)    ix->yspeed =  0x20000;
            if (swdata & PAD_DOWN)  ix->yspeed = -0x20000;
            if (swdata & PAD_LEFT)  ix->xspeed =  0x20000;
            if (swdata & PAD_RIGHT) ix->xspeed = -0x20000;
            
            rparamA->mx += ix->xspeed;
            rparamA->my += ix->yspeed;

            if (swdata & PAD_R) ix->wreg[0] += 4;
            if (swdata & PAD_L) ix->wreg[0] -= 4;
            rparamA->mtr_a =  SMMI_Cosset(ix->wreg[0]) * 4;
            rparamA->mtr_b = -SMMI_Sinset(ix->wreg[0]) * 4;
            rparamA->mtr_d =  SMMI_Sinset(ix->wreg[0]) * 4;
            rparamA->mtr_e =  SMMI_Cosset(ix->wreg[0]) * 4;


            hposi = (SMMI_Sinset8(ix->wreg[2]) * 0x16) / 0x100;
            vposi = (SMMI_Cosset8(ix->wreg[2]) * 0x16) / 0x100;
            ix->wreg[2] += 2;

            kadd = 0x3000 + (hposi * 2) + (vposi * 0xa0 * 2);
            rparamA->kast = kadd * 0x10000;

            if (SMRK_KeiEnd == 0) {
               SMRK_KeisuBufErase();
               SMRK_KeisuCntX((Sint32)ix->wreg[1], ix->zoom);
            }

            break;

    }

}
    


/*===========================
 *    ���`�p�W���v�Z�֐�
 *==========================*/
void SMRK_KeisuCnt(void)
{
    ROTPARAM *rregptr;
    Sint16 *keisuptr;
    Sint32 sx, sy, dt0;
    Sint32 r2, ux2, uy2, sqans, sqqst;
    Sint32 cx, cy;

    rregptr = &SMRK_rotregbufA;
	
    rregptr->dkasy = 0xa00000;
    rregptr->dkasx = 0x008000;

    r2 = 0x50* 0x50;
    cx = 0xa0;
    cy = 0x70;
    keisuptr = &SMRK_KeisuBuf[0];
    for (sy = 0; sy < 0x100; sy++) {
         uy2 = (sy - cy)  * (sy - cy);
         for (sx = 0; sx < 0x140; sx += 2) {
             ux2 = (sx - cx) * (sx - cx);
             sqqst = r2 - ux2 - uy2;
             if (sqqst > 0) {
                sqans = -SMCA_sqrt((Uint32)sqqst);
                dt0 = (-0x70 * 0x400) / ((Sint32)sqans - 0x70);
                if (dt0  < 0) dt0 = SMRK_FilData;
                if (dt0 == 0) dt0 = SMRK_FilData;
             } else
                dt0 = SMRK_FilData;

             *keisuptr = (Sint16)dt0;
             keisuptr++;
         }
    }
    SMRK_KeiEnd = 1;

}



/*
 *   �W���o�b�t�@������
 */
void SMRK_KeisuBufErase(void)
{
    Uint32 i;
    Sint16 *wrptr16;

    wrptr16 = SMRK_KeisuBuf;
    while (((Uint32)wrptr16 & 0xf) != 0) {
        *wrptr16 = SMRK_FilData;
        wrptr16++;
    }
    for (i = 0; i < 0x10; i++) {
         *wrptr16 = SMRK_FilData;
         wrptr16++;
    }    
    SMRK_CPUDmaLdir160((Uint32)wrptr16 - 0x10, 
                       (Uint32)wrptr16, 0x14000);
}

/*
 *   ���`�W���v�Z�֐����ǔ�
 */
void SMRK_KeisuCntX(Sint32 r, Sint32 zoom)
{
    ROTPARAM *rregptr;
    Sint32 sx, sy, dt0;
    Sint32 r2, ux2, uy2, sqans, sqqst;
    Sint32 cx, cy;
    Uint32 i,pnt, pntbase;
    Sint16 *wrptr16;

    rregptr = &SMRK_rotregbufA;
    rregptr->dkasy = 0xa00000;
    rregptr->dkasx = 0x008000;

    r2 = r * r;
    cx = 0xa0;
    cy = 0x70;

    pntbase = (cx / 2) + (0xa0 * cy);

    for (sy = 0; sy < r; sy++) {
        uy2 = sy * sy;
        for (sx = 0; sx < r; sx += 2) {
            ux2 = sx * sx;
            sqqst = r2 - ux2 - uy2;
             if (sqqst > 0) {
                sqans = -SMCA_sqrt((Uint32)sqqst);
                dt0 = (-0x70 * 0x400) / ((Sint32)sqans - 0x70);
                if (dt0 <= 0) dt0 = SMRK_FilData;
                dt0 = (dt0 * zoom) / 0x10000;
             } else
                dt0 = SMRK_FilData;

             pnt = pntbase + (sx / 2) + (0xa0 * sy);
             SMRK_KeisuBuf[pnt] = (Sint16)dt0;

             pnt = pntbase + (sx / 2) - (0xa0 * sy);
             SMRK_KeisuBuf[pnt] = (Sint16)dt0;

             pnt = pntbase - (sx / 2) - (0xa0 * sy);
             SMRK_KeisuBuf[pnt] = (Sint16)dt0;

             pnt = pntbase - (sx / 2) + (0xa0 * sy);
             SMRK_KeisuBuf[pnt] = (Sint16)dt0;
         }
    }
    SMRK_KeiEnd = 1;

}





/*=================================
 *   VBlankIn�֐�������Ă΂��
 *===============================*/ 
void SMRK_VintRotRegSet(void)
{

    poke_w(SMRK_BCLRTBLADD, 0x7000);

    if (SMRK_RotregReq) {

        SMMI_CPUDmaLdirL0((Uint32)&SMRK_rotregbufA,
                                 (Uint32)(SMRK_RBG0_RPAR+0x00), 0x60);
        SMMI_CPUDmaLdirL0((Uint32)&SMRK_rotregbufB,
                                 (Uint32)(SMRK_RBG0_RPAR+0x80), 0x60);
        if (SMRK_KeiEnd == 1) {
            SMMI_CPUDmaLdirL0((Uint32)&SMRK_KeisuBuf[0],
                                     (Uint32)SMRK_RBG0_KEISU,0x14000);
            SMRK_KeiEnd = 2;
        }

        SMRK_RotregReq = 0;
    }

    poke_w(SMRK_BCLRTBLADD, 0x0000);

}


#if 0
#define SCU_DMA0REG 0x25fe0000      /* LEVEL 0 DMA �Z�b�g���W�X�^       */
#define SCU_DMA1REG 0x25fe0020      /* LEVEL 1 DMA �Z�b�g���W�X�^       */
#define SCU_DMA2REG 0x25fe0040      /* LEVEL 2 DMA �Z�b�g���W�X�^       */
#define SCUDMA_SRCADD 0x0000        /* �\�[�X�A�h���X                   */
#define SCUDMA_DSTADD 0x0004        /* �f�B�X�g�l�[�V�����A�h���X       */
#define SCUDMA_SIZE   0x0008        /* �]���T�C�Y                       */
#define SCUDMA_INC    0x000c        /* �I���R�[�h�A���Z�l���W�X�^       */
#define SCUDMA_ENBL   0x0010        /* DMA�����W�X�^                  */
#define SCUDMA_MODE   0x0014        /* DMA���[�h���W�X�^                */

#define SCU_DMASET  0x25fe0060      /* DMA�Z�b�g���W�X�^                */
#define SCU_DMASTS  0x25fe0070      /* DMA�X�e�[�^�X���W�X�^            */
#define SCUDMA_STSRC 0x0000         /* ���f���̃\�[�X�A�h���X           */
#define SCUDMA_STDST 0x0004         /* ���f���̃f�B�X�g�l�[�V�����A�h���X */
#define SCUDMA_STSIZ 0x0008         /* ���f���̃R���g���[���T�C�Y         */
#define SCUDMA_CNTRL 0x000c         /* �A�N�Z�X,���f,�ҋ@,���샌�W�X�^    */

#endif

/*
 *   SCU_DMA
 */
void SMRK_SCUDMA_LDIR0(Uint32 src, Uint32 dst, Uint32 trcnt)
{
    Uint32 *wrptr32;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SRCADD);
    *wrptr32 = src;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_DSTADD);
    *wrptr32 = dst;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SIZE);
    *wrptr32 = trcnt;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_INC);
    *wrptr32 = (1 << 8) + 1; /* 4byte add mode  */

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_MODE);
    *wrptr32 = (0 << 24) + (0 << 16) + (0 << 8) + (7);

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_ENBL);
    *wrptr32 = (1 << 8) + (1);  /* DMA START */

}






/*===========================================================================
 *    ���������������ԂP�U�o�C�g���[�hCPUDMA�]�� (CH#0 �T�C�N���X�`�[�����[�h)
 *    ���ӁI�I: SDRAM�ȊO�ł̓���͒m��Ȃ�
 *              �K���P�U�o�C�g���E�̃A�h���X�Z�b�g���邱��
 *    srcadd : �]�����A�h���X
 *    dstadd : �]����A�h���X
 *    trcnt  : �]���T�C�Y(�o�C�g�T�C�Y)
 *==========================================================================*/
void SMRK_CPUDmaLdir160(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt)
{
    volatile Uint32 d0, d1;

    d0 = *(volatile Uint32 *)CDMA_OPR;
    d0 &= 0x1;
    d0 |= 0x1;
    *(volatile Uint32 *)CDMA_OPR = d0;

    *(volatile Uint32 *)CDMA0_SRC = srcadd;
    *(volatile Uint32 *)CDMA0_DST = dstadd;
    *(volatile Uint32 *)CDMA0_LEN = (trcnt+3)>>2;
    d0 = *(volatile Uint32 *)CDMA0_CNT;
    *(volatile Uint32 *)CDMA0_CNT = 0x5e01;        /* src++ dst++ NoINT */
	
    do {
        d0 = *(volatile Uint32 *)CDMA0_CNT;
    } while ((d0 & 2) == 0);
	
}






/*
 *     �R���g���[���A�N�V����
 */
void SMRK_ScrlCntX(SMRKACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};

    Uint16 swdata;
    Uint32 kadd, i;
    Sint32 hposi, vposi;
    ROTPARAM *rparamA;
    ROTPARAM *rparamB;

    swdata = SMPA_pad_data1;
    rparamA = &SMRK_rotregbufA;
    rparamB = &SMRK_rotregbufB;

    switch (ix->mode) {
        case INIT:
            rparamA->xst = 0x0000;
            rparamA->yst = 0x0000;
            rparamA->zst = 0;
            rparamA->dxst = 0;
            rparamA->dyst = 0x10000;
            rparamA->dx = 0x10000;
            rparamA->dy = 0;
            rparamA->mtr_a = 0x10000;
            rparamA->mtr_b = 0x00000;
            rparamA->mtr_c = 0x00000;
            rparamA->mtr_d = 0x00000;
            rparamA->mtr_e = 0x10000;
            rparamA->mtr_f = 0x00000;
            rparamA->kx = 0x10000;
            rparamA->ky = 0x10000;
            rparamA->mx = 0;   
            rparamA->my = 0;
            rparamA->px = 0xa0;
            rparamA->py = 0x70;
            rparamA->pz = 0x100;
            rparamA->cx = 0xa0;
            rparamA->cy = 0x70;
            rparamA->cz = 0x0;

            rparamB->xst = 0x0000;
            rparamB->yst = 0x0000;
            rparamB->zst = 0;
            rparamB->dxst = 0;
            rparamB->dyst = 0x10000;
            rparamB->dx = 0x10000;
            rparamB->dy = 0;
            rparamB->mtr_a = 0x40000;
            rparamB->mtr_b = 0x00000;
            rparamB->mtr_c = 0x00000;
            rparamB->mtr_d = 0x00000;
            rparamB->mtr_e = 0x40000;
            rparamB->mtr_f = 0x00000;
            rparamB->kx = 0x10000;
            rparamB->ky = 0x10000;
            rparamB->mx = 0;   
            rparamB->my = 0;
            rparamB->px = 0xa0;
            rparamB->py = 0x70;
            rparamB->pz = 0x100;
            rparamB->cx = 0xa0;
            rparamB->cy = 0x70;
            rparamB->cz = 0x0;

            rparamA->kast = 0x3000 * 0x10000;

            SMRK_FilData = 0x400; /* �{�[���̊O���̌W���f�[�^ */
            SMRK_KeiEnd = 0;
            ix->xposi = 0;
            ix->yposi = 0;
            ix->zposi = 0;
            ix->zspeed = 0;
            ix->zoom = 0x10000;    /* �g�嗦   */
            ix->wcnt[0] = 4;
            ix->wreg[0] = 0;  
            ix->mode++;
        case MOVE:

            if (swdata & PAD_UP)   ix->yposi -= 0x100000;
            if (swdata & PAD_DOWN) ix->yposi += 0x100000;

            if (swdata & PAD_LEFT)  ix->xposi -= 0x100000;
            if (swdata & PAD_RIGHT) ix->xposi += 0x100000;

            if (ix->xposi < 0x100000) ix->xposi = 0x100000;
            if (ix->xposi > 0x1300000) ix->xposi = 0x1300000;
            if (ix->yposi < 0x100000) ix->yposi = 0x100000;
            if (ix->yposi > 0xd00000) ix->yposi = 0xd00000;

            if (swdata & PAD_B)
                ix->wcnt[0] += 0x10;
            else
                ix->wcnt[0] += 4;

            if (swdata & PAD_A) ix->wreg[0]--;
            if (swdata & PAD_C) ix->wreg[0]++;

            if (ix->wreg[0] < 0) ix->wreg[0] = 0;
            if (ix->wreg[0] > 8) ix->wreg[0] = 8;

            hposi = *(Sint16 *)&ix->xposi;
            vposi = *(Sint16 *)&ix->yposi;

            SMRK_KeiEnd = 0;
            SMRK_KeisuBufErase();
            SMRK_HakeiSet(hposi, vposi, ix->wreg[0], ix->wcnt[0]);


            break;

    }

}
    


/*===========================
 *    �T�C���J�[�u
 *==========================*/
void SMRK_HakeiSet(Sint32 cx, Sint32 cy, Sint32 len, Sint32 time)
{
    ROTPARAM *rregptr;
    Sint16 *keisuptr;
    Sint32 sx, sy, dt0;
    Sint32 r2, ux2, uy2, sqans, sqqst;


    rregptr = &SMRK_rotregbufA;
	
    rregptr->dkasy = 0xa00000;
    rregptr->dkasx = 0x008000;

    keisuptr = &SMRK_KeisuBuf[0];
    for (sy = 0; sy < 0x100; sy++) {
         uy2 = (sy - cy)  * (sy - cy);
         for (sx = 0; sx < 0x140; sx += 2) {
             ux2 = (sx - cx) * (sx - cx);
             sqqst = ux2 + uy2;

             if (sqqst > 0x4000)
                 sqans = SMCA_sqrt((Uint32)sqqst);
             else
                 sqans = SMRK_sqrttbl[sqqst];

             dt0 = ((time - sqans) * len) & 0x7f;

             dt0 = -SMMI_Sinset8(dt0);
             dt0 = (-0x200 * 0x400) / ((Sint32)dt0 - 0x200);
             if (dt0  < 0) dt0 = SMRK_FilData;
             if (dt0 == 0) dt0 = SMRK_FilData;

             *keisuptr = (Sint16)dt0;
             keisuptr++;
         }
    }
    SMRK_KeiEnd = 1;

}


