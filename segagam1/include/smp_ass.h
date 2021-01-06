/*	Copyright(c) 1994 SEGA			*/
/*
 * 
 * CONST DEFINE for SATURN 80% SYSTEM
 *
 */

#ifndef __SMP_ASS_H
#define __SMP_ASS_H

/*-------------------*
 *    SH I/O PORT    *
 *-------------------*/
#define CDMA_OPR    0xffffffb0     /* DMA OPERATION REG         */

#define CDMA0_SRC   0xffffff80     /* DMA CH#0 SOURCE ADD REG   */
#define CDMA0_DST   0xffffff84     /* DMA CH#0 DIST ADD REG     */
#define CDMA0_LEN   0xffffff88     /* DMA CH#0 LENGTH REG       */
#define CDMA0_CNT   0xffffff8c     /* DMA CH#0 CHANNEL CONT REG */

#define CDMA1_SRC   0xffffff90     /* DMA CH#0 SOURCE ADD REG   */
#define CDMA1_DST   0xffffff94     /* DMA CH#0 DIST ADD REG     */
#define CDMA1_LEN   0xffffff98     /* DMA CH#0 LENGTH REG       */
#define CDMA1_CNT   0xffffff9c     /* DMA CH#0 CHANNEL CONT REG */


#define IPRA        0xfffffee2          /* ���荞�ݗD�惌�x���ݒ背�W�X�^�` */
#define IPRB        0xfffffe60          /* ���荞�ݗD�惌�x���ݒ背�W�X�^�a */
#define VCRA        0xfffffe62          /* VECTOR NO SETTEI A               */
#define VCRB        0xfffffe64          /* VECTOR NO SETTEI B               */
#define VCRC        0xfffffe66          /* VECTOR NO SETTEI C               */
#define VCRD        0xfffffe68          /* VECTOR NO SETTEI D               */
#define VCRWDT      0xfffffee4          /* VECTOR NO SETTEI WDT             */
#define ICR         0xfffffee0          /* ���荞�݃R���g���[�����W�X�^     */

#define BSC         0x05ffffa0          /* Bus State Contorl                */
#define PFC         0x05ffffc4          /* Pin function control             */

/*--------------------------------*
 *    SYSTEM CONTROL UNIT (SCU)   *
 *--------------------------------*/
#define SCU_INTMASK 0x25fe00a0      /* INTRPT MASK                      */
#define SCU_INTSTAT 0x25fe00a4      /* INTRPT STATUS                    */

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



/*-------------------*
 *    SPRITE VRAM    *
 *-------------------*/
#define SPR_VRAM	0x25c00000	 /* SPRITE VRAM                     */
#define SPR_ATTR	0x25c00000	 /* SPRITE ATTRIBUTE TABLE          */
#define SPR_GROW        0x25c10000       /* GROW COLOR TABLE TOP            */
#define SPR_LOOKUP      0x25c12000       /* SPRITE LOOKUP COLOR TABLE TOP   */
#define SPR_PTGEN       0x25c14000       /* SPRITE PATTERN GENERATER TOP    */
#define SPR_FB		0x25c80000	 /* �`��t���[���o�b�t�@            */


/*------------------*
 *    SPRITE I/O    *
 *------------------*/
#define SPR_TVMOD	0x25d00000	 /* �s�u���[�h�I��                  */
#define SPR_FBCMOD	0x25d00002	 /* �e�a�؂芷�����[�h              */
#define SPR_PLTTRG	0x25d00004	 /* �v���b�g�g���K                  */
#define SPR_ERSDAT	0x25d00006	 /* �C���[�X���C�g�f�[�^            */
#define SPR_ERSLUP	0x25d00008	 /* �C���[�X���C�g������W          */
#define SPR_ERSRDP	0x25d0000a	 /* �C���[�X���C�g�E�����W          */
#define SPR_DRWBRK	0x25d0000c	 /* �`�拭���I��                    */
#define SPR_TRSSTA	0x25d00010	 /* �]���I�����                    */
#define SPR_BRKTBL	0x25d00012	 /* �������f�e�[�u���A�h���X        */
#define SPR_NOWTBL	0x25d00014	 /* �������e�[�u���A�h���X          */
#define SPR_MODR	0x25d00016	 /* MODE CONDITION REG              */

/*----------------------------*
 *    SPRITE COMMAND TABLE    *
 *----------------------------*/

/* Function No */
#define SPR_normal        0x0000          /* ��`�X�v���C�g�`��             */
#define SPR_scaled        0x0001          /* �g�k�X�v���C�g�`��             */
#define SPR_distorted     0x0002          /* �ό`�X�v���C�g�`��             */
#define SPR_polygon	  0x0004          /* �|���S��                       */
#define SPR_polyline      0x0005          /* �|�����C��                     */
#define SPR_line	  0x0006          /* ���C��                         */
#define SPR_usrclip	  0x0008          /* ���[�U�N���b�s���O���W         */
#define SPR_sysclip	  0x0009          /* �V�X�e���N���b�s���O���W       */
#define SPR_local	  0x000a          /* ���[�J�����W                   */

/* HV Reverse */
#define SPR_Hrev          0x0010          /* ���E���]                       */
#define SPR_Vrev          0x0020          /* �㉺���]                       */

/* ZOOM Point */
#define SPR_ul		0x0500		  /* ���㒸�_                       */
#define SPR_uc		0x0600		  /* ��Ӓ���                       */
#define SPR_ur		0x0700		  /* �E�㒸�_                       */
#define SPR_cl		0x0900		  /* ���Ӓ���                       */
#define SPR_cc		0x0a00		  /* ����                           */
#define SPR_cr		0x0b00		  /* �E�Ӓ���                       */
#define SPR_dl		0x0d00		  /* �������_                       */
#define SPR_dc		0x0e00		  /* ���Ӓ���                       */
#define SPR_dr		0x0f00		  /* �E�����_                       */

/* COMMAND JUMP */
#define SPR_next	0x0000		  /* JUMP NEXT                      */
#define SPR_assign	0x1000		  /* JUMP ASSIGN                    */
#define SPR_call	0x2000		  /* JUMP CALL                      */
#define SPR_return	0x3000		  /* JUMP RETURN                    */
#define SPR_skipnext	0x4000		  /* SKIP NEXT                      */
#define SPR_skipassign	0x5000		  /* SKIP ASSIGN                    */
#define SPR_skipcall	0x6000		  /* SKIP CALL                      */
#define SPR_skipreturn	0x7000		  /* SKIP RETURN                    */
#define SPR_end		0x8000		  /* �����I��                       */

/* SPRITE PMODE ASSIGN */
#define SPM_mon   0x8000                  /* FRAME BUFFER MSB ON            */
#define SPM_gg    0x0800                  /* GRAYSCALE GROW SHADING         */
#define SPM_clip  0x0400                  /* UserCLip enable                */
#define SPM_cmod  0x0200                  /* UserClip CLIPMODE              */
#define SPM_mesh  0x0100                  /* Mesh Enable                    */
#define SPM_ecd   0x0080                  /* SPRITE ENDCODE Disable         */
#define SPM_sdd   0x0040                  /* �����J���[����                 */
#define SPM_grow  0x0004                  /* �O���[�V�F�[�h                 */
#define SPM_half  0x0002                  /* ���P�x ���G1/2                 */
#define SPM_shd   0x0001                  /* �V���h�E ���G1/2               */


/*-------------------------------*
 *    VDP2(�X�N���[��VDP�֘A)    *
 *-------------------------------*/
#define SCR_VRAM	0x25e00000        /* SCROLL VRAM TOP                */
#define SCR_VRAMA0      0x25e00000        /* SCROLL VRAM A bank 0           */
#define SCR_VRAMA1      0x25e20000        /* SCROLL VRAM A bank 1           */
#define SCR_VRAMB0      0x25e40000        /* SCROLL VRAM B bank 0           */
#define SCR_VRAMB1      0x25e60000        /* SCROLL VRAM B bank 1           */
#define COLOR_RAM       0x25f00000        /* COLOR RAM                      */
#define SCRL_REG        0x25f80000        /* SCROLL REGISTER                */


/*------------------------------*
 *     VDP2�V�X�e�����W�X�^     *
 *------------------------------*/
#define SCRL_TVMOD      (SCRL_REG+0x00)   /* �e���r��ʃ��[�h               */
#define SCRL_EXENB      (SCRL_REG+0x02)   /* �O���M���C�l�[�u��             */
#define SCRL_TVSTS      (SCRL_REG+0x04)   /* ��ʃX�e�[�^�X                 */
#define SCRL_HCNT       (SCRL_REG+0x08)   /* �O���M���ɂ��H�J�E���^�[      */
#define SCRL_VCNT       (SCRL_REG+0x0a)   /* �O���M���ɂ��V�J�E���^�[      */

/*-------------------------------------*
 *    VDP2 VRAM�R���g���[�����W�X�^    *
 *-------------------------------------*/
#define SCRL_VRAMCHG    (SCRL_REG+0x0c)   /* VRAM�ؑւ����W�X�^             */
#define SCRL_VRAMCNT    (SCRL_REG+0x0e)   /* VRAM�R���g���[�����W�X�^       */

#define SCRL_CYCA00 (SCRL_REG+0x10)       /* VRAM�T�C�N���p�^�[�����W�X�^   */
#define SCRL_CYCA01 (SCRL_REG+0x12)   
#define SCRL_CYCA10 (SCRL_REG+0x14)   
#define SCRL_CYCA11 (SCRL_REG+0x16)   
#define SCRL_CYCB00 (SCRL_REG+0x18)   
#define SCRL_CYCB01 (SCRL_REG+0x1a)   
#define SCRL_CYCB10 (SCRL_REG+0x1c)   
#define SCRL_CYCB11 (SCRL_REG+0x1e)   


/*------------------------------------*
 *     VDP2�X�N���[���R���g���[��     *
 *------------------------------------*/
#define SCRL_DSPCNT     (SCRL_REG+0x20)   /* ��ʕ\���C�l�[�u�����W�X�^    */


/*-----------------------------*
 *    ���U�C�N �R���g���[��    *
 *-----------------------------*/
#define SCRL_MZCNT      (SCRL_REG+0x22)   /* SCROLL MOSAIC CONTROL REG      */
#define MZE_N0 0x0001                     /* MOZAIC ON/OFF BIT              */
#define MZE_N1 0x0002
#define MZE_N2 0x0004
#define MZE_N3 0x0008
#define MZE_R0 0x0010



/*---------------------------------------*
 *     VDP2 �X�N���[���L�����N�^�ݒ�     *
 *---------------------------------------*/
#define SCRL_CHRCNT0    (SCRL_REG+0x28)   /* �L�����N�^�R���g���[��         */
#define SCRL_CHRCNT1    (SCRL_REG+0x2a)   /* �L�����N�^�R���g���[��         */

#define SCRL_NBGBMPPLT  (SCRL_REG+0x2c)   /* NBG BITMAP PALEETE NO REG      */
#define SCRL_RBGBMPPLT  (SCRL_REG+0x2e)   /* RBG BITMAP PALETTE NO REG      */


/*-----------------------------------------*
 *     VDP2 �X�N���[���p�^�[�����̐ݒ�     *
 *-----------------------------------------*/
#define SCRL_PTNCNTN0   (SCRL_REG+0x30)   /* NGB0 PATTERN NAME CONTROL REG  */
#define SCRL_PTNCNTN1   (SCRL_REG+0x32)   /* NBG1 PATTERN NAME CONTROL REG  */
#define SCRL_PTNCNTN2   (SCRL_REG+0x34)   /* NBG2 PATTERN NAME CONTROL REG  */
#define SCRL_PTNCNTN3   (SCRL_REG+0x36)   /* NBG3 PATTERN NAME CONTROL REG  */
#define SCRL_PTNCNTR0   (SCRL_REG+0x38)   /* RBG0 PATTERN NAME CONTROL REG  */


/*-------------------------------------------*
 *     VDP2 �X�N���[���v���[���T�C�Y�ݒ�     *
 *-------------------------------------------*/
#define SCRL_PLNSIZE    (SCRL_REG+0x3a)   /* SCROLL PLANE SIZE CONTROLL     */


/*-----------------------------------*
 *     VDP2 �X�N���[���}�b�v�ݒ�     *
 *-----------------------------------*/
#define SCRL_NBGMAPOFST (SCRL_REG+0x3c)   /* NBG�p�}�b�v�I�t�Z�b�g          */
#define SCRL_RBGMAPOFST (SCRL_REG+0x3e)   /* RGB�p�}�b�v�I�t�Z�b�g          */

#define SCRL_NBG0PLNADD0 (SCRL_REG+0x40)   /* NBG0-PLANE A B MAP REG        */
#define SCRL_NBG0PLNADD1 (SCRL_REG+0x42)   /* NBG0-PLANE C D MAP REG        */

#define SCRL_NBG1PLNADD0 (SCRL_REG+0x44)   /* NBG1-PLANE A B MAP REG        */
#define SCRL_NBG1PLNADD1 (SCRL_REG+0x46)   /* NBG1-PLANE C D MAP REG        */

#define SCRL_NBG2PLNADD0 (SCRL_REG+0x48)   /* NBG2-PLANE A B MAP REG        */
#define SCRL_NBG2PLNADD1 (SCRL_REG+0x4a)   /* NBG2-PLANE C D MAP REG        */

#define SCRL_NBG3PLNADD0 (SCRL_REG+0x4c)   /* NBG3-PLANE A B MAP REG        */
#define SCRL_NBG3PLNADD1 (SCRL_REG+0x4e)   /* NBG3-PLANE C D MAP REG        */


#define SCRL_RBG0MAPREG0 (SCRL_REG+0x50)  /* RBG PARAMATER A-PLANE A B MAP  */
#define SCRL_RBG0MAPREG1 (SCRL_REG+0x52)  /* RBG PARAMATER A-PLANE C D MAP  */
#define SCRL_RBG0MAPREG2 (SCRL_REG+0x54)  /* RBG PARAMATER A-PLANE E F MAP  */
#define SCRL_RBG0MAPREG3 (SCRL_REG+0x56)  /* RBG PARAMATER A-PLANE G H MAP  */
#define SCRL_RBG0MAPREG4 (SCRL_REG+0x58)  /* RBG PARAMATER A-PLANE I J MAP  */
#define SCRL_RBG0MAPREG5 (SCRL_REG+0x5a)  /* RBG PARAMATER A-PLANE K L MAP  */
#define SCRL_RBG0MAPREG6 (SCRL_REG+0x5c)  /* RBG PARAMATER A-PLANE M N MAP  */
#define SCRL_RBG0MAPREG7 (SCRL_REG+0x5e)  /* RBG PARAMATER A-PLANE O P MAP  */

#define SCRL_RBG1MAPREG0 (SCRL_REG+0x60)  /* RBG PARAMATER B-PLANE A B MAP  */
#define SCRL_RBG1MAPREG1 (SCRL_REG+0x62)  /* RBG PARAMATER B-PLANE C D MAP  */
#define SCRL_RBG1MAPREG2 (SCRL_REG+0x64)  /* RBG PARAMATER B-PLANE E F MAP  */
#define SCRL_RBG1MAPREG3 (SCRL_REG+0x66)  /* RBG PARAMATER B-PLANE G H MAP  */
#define SCRL_RBG1MAPREG4 (SCRL_REG+0x68)  /* RBG PARAMATER B-PLANE I J MAP  */
#define SCRL_RBG1MAPREG5 (SCRL_REG+0x6a)  /* RBG PARAMATER B-PLANE K L MAP  */
#define SCRL_RBG1MAPREG6 (SCRL_REG+0x6c)  /* RBG PARAMATER B-PLANE M N MAP  */
#define SCRL_RBG1MAPREG7 (SCRL_REG+0x6e)  /* RBG PARAMATER B-PLANE O P MAP  */


/*-----------------------------------------*
 *     VDP2 �m�[�}���X�N���[�����W�X�^     *
 *-----------------------------------------*/
#define NBG0_XST        (SCRL_REG+0x70)   /* NBG0 X scroll reg (long)       */
#define NBG0_YST        (SCRL_REG+0x74)   /* NBG0 Y scroll reg (long)       */
#define NBG0_DX         (SCRL_REG+0x78)   /* NBG0 Xzoubunn     (long)       */
#define NBG0_DY         (SCRL_REG+0x7c)   /* NBG0 Yzoubunn     (long)       */

#define NBG1_XST        (SCRL_REG+0x80)   /* NBG1 X scroll reg (long)       */
#define NBG1_YST        (SCRL_REG+0x84)   /* NBG1 Y scroll reg (long)       */
#define NBG1_DX         (SCRL_REG+0x88)   /* NBG1 X zoubunn    (long)       */
#define NBG1_DY         (SCRL_REG+0x8c)   /* NBG1 Y zoubunn    (long)       */

#define NBG2_XST        (SCRL_REG+0x90)   /* NBG2 X scroll reg (word)       */
#define NBG2_YST        (SCRL_REG+0x92)   /* NBG2 Y scroll reg (word)       */

#define NBG3_XST        (SCRL_REG+0x94)   /* NBG3 X scroll reg (word)       */
#define NBG3_YST        (SCRL_REG+0x96)   /* NBG3 Y scroll reg (word)       */


/*-----------------------------------------*
 *     VDP2 �m�[�}���X�N���[���k���ݒ�     *
 *-----------------------------------------*/
#define NBG_ZOOMCNT     (SCRL_REG+0x98)   /* NBG �k���C�l�[�u��             */


/*-----------------------------------------------------*
 *     VDP2 �m�[�}���X�N���[�����C���X�N���[���ݒ�     *
 *-----------------------------------------------------*/
#define NBG_LINESCRLCNT (SCRL_REG+0x9a)  /* ���C���X�N���[���R���g�[��      */
#define NBG_VCTBLADD    (SCRL_REG+0x9c)  /* �c�Z���X�N���[���e�[�u���A�h���X*/
#define NBG0_HLTBLADD   (SCRL_REG+0xa0)  /* NBG0�p Hline scroll table add   */
#define NBG1_HLTBLADD   (SCRL_REG+0xa4)  /* NBG1�p Hline scroll table add   */


/*-------------------------------*
 *     VDP2 ���C���J���[�ݒ�     *
 *-------------------------------*/
#define LCLR_TBLADD     (SCRL_REG+0xa8)   /* ���C���J���[�R���g�[��         */


/*-------------------------------*
 *     VDP2 �o�b�N�J���[�ݒ�     *
 *-------------------------------*/
#define BKCL_TBLADD     (SCRL_REG+0xac)   /* �o�b�N�J���[�R���g���[��       */


/*-----------------------------------------*
 *     VDP2 ��]�X�N���[���V�X�e���ݒ�     *
 *-----------------------------------------*/
#define SCRL_RPMMODE    (SCRL_REG+0xb0)   /* ��]�p�����[�^���[�h           */
#define SCRL_RPMCNT     (SCRL_REG+0xb2)   /* ��]�p�����[�^���[�h����       */
#define SCRL_RKEICNT    (SCRL_REG+0xb4)   /* �W���e�[�u���R���g���[��       */
#define SCRL_RKEIOFSET  (SCRL_REG+0xb6)   /* �W���e�[�u���A�h���X�I�t�Z�b�g */
#define SCRL_RAOVRPN    (SCRL_REG+0xb8)   /* ��ʃI�[�o�[���W�X�^           */
#define SCRL_RBOVRPN    (SCRL_REG+0xba)   /* ��ʃI�[�o�[���W�X�^           */
#define SCRL_RTBLADD    (SCRL_REG+0xbc)   /* ��]�p�����[�^�e�[�u���A�h���X */


/*-----------------------------------------*
 *     VDP2 �m�[�}����`�E�B���h�E�ݒ�     *
 *-----------------------------------------*/
#define SCRL_WIND_XST   (SCRL_REG+0xc0)  /* BOX WINDOW X START POSI        */
#define SCRL_WIND_YST   (SCRL_REG+0xc2)  /* BOX WINDOW Y START POSI        */
#define SCRL_WIND_XED   (SCRL_REG+0xc4)  /* BOX WINDOW X END POSI          */
#define SCRL_WIND_YED   (SCRL_REG+0xc6)  /* BOX WINDOW Y END POSI          */


/*---------------------------------------------*
 *     VDP2 �E�B���h�E�R���g���[�����W�X�^     *
 *---------------------------------------------*/
#define SCRL_WIND_CNT0   (SCRL_REG+0xd0)  /* WINDOW CONTROL NBG0-1          */
#define SCRL_WIND_CNT1   (SCRL_REG+0xd2)  /* WINDOW CONTROL NBG2-3          */
#define SCRL_WIND_CNT2   (SCRL_REG+0xd4)  /* WINDOW CONTROL SPR & ROT-A     */
#define SCRL_WIND_CNT3   (SCRL_REG+0xd6)  /* WINDOW CONTROL COLOR& ROT      */


/*-----------------------------------------------*
 *     VDP2 ���C���E�B���h�E�e�[�u���A�h���X     *
 *-----------------------------------------------*/
#define SCRL_WIND0_TBLADD (SCRL_REG+0xd8)  /* W0���C���e�[�u���A�h���X      */
#define SCRL_WIND1_TBLADD (SCRL_REG+0xdc)  /* W1���C���e�[�u���A�h���X      */


/*-----------------------------------------------*
 *     VDP2 �X�v���C�g�R���g���[��     *
 *-----------------------------------------------*/
#define SCRL_SPRCNT      (SCRL_REG+0xe0)   /* �X�v���C�g�R���g���[�����W�X�^*/


/*-------------------------------------*
 *     VDP2 ���C���J���[�C�l�[�u��     *
 *-------------------------------------*/
#define LCLR_ENABLE      (SCRL_REG+0xe8)  /* LINE COLOR ENABLE REG          */

#define SCRL_SPRIOMODE   (SCRL_REG+0xea)  /* SPECIAL PRIOLITY MODE          */

#define SCRL_PRIO0       (SCRL_REG+0xf8)  /* SCROLL PRIOLITY NO REG NBG 0 1 */
#define SCRL_PRIO1       (SCRL_REG+0xfa)  /* SCROLL PRIOLITY NO REG NBG 2 3 */
#define SCRL_PRIO2       (SCRL_REG+0xfc)  /* SCROLL PRIOLITY NO REG RBG     */


#define CROF_ENABLE      (SCRL_REG+0x110) /* COLOR OFFSET ENABLE            */
#define CROF_SELECT      (SCRL_REG+0x112) /* COLOR OFFSET SELECT            */
#define CROF_REDA        (SCRL_REG+0x114) /* COLOR OFFSET REG-A RED         */
#define CROF_GREENA      (SCRL_REG+0x116) /* COLOR OFFSET REG_A GREEN       */
#define CROF_BLUEA       (SCRL_REG+0x118) /* COLOR OFFSET REG-A BLUE        */

#define CROF_REDB        (SCRL_REG+0x11a) /* COLOR OFFSET REG-B RED         */
#define CROF_GREENB      (SCRL_REG+0x11c) /* COLOR OFFSET REG-B GREEN       */
#define CROF_BLUEB       (SCRL_REG+0x11e) /* COLOR OFFSET REG-B BLUE        */







#endif /* #ifndef __assign_h */


















