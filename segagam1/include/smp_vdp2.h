/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * VDP2(SCROLL) COTROL
 *
 */

#ifndef __SMP_VDP2_H
#define __SMP_VDP2_H



/*
 *    �X�N���[��VRAM�A�T�C�� 
 */
#define NBG_PTGEN (SCR_VRAMA0 + 0x00000)     /* NBG PTGEN                   */
#define NBG1_PTNAMA (SCR_VRAMA0 + 0x14000)   /* NBG1 PTNAM                  */
#define NBG1_PTNAMB (SCR_VRAMA0 + 0x14000)   
#define NBG1_PTNAMC (SCR_VRAMA0 + 0x14000)
#define NBG1_PTNAMD (SCR_VRAMA0 + 0x14000)

#define NBG0_PTNAMA (SCR_VRAMA0 + 0x18000)   /* NBG0 PTNAM                  */
#define NBG0_PTNAMB (SCR_VRAMA0 + 0x18000)
#define NBG0_PTNAMC (SCR_VRAMA0 + 0x18000)
#define NBG0_PTNAMD (SCR_VRAMA0 + 0x18000)

#define NBG0_HLSCLTBL (SCR_VRAMA0 + 0x1c000) /* NBG0 HLINE SCROLL TABLE     */
#define NBG1_HLSCLTBL (SCR_VRAMA0 + 0x1d000) /* NBG1 HLINE SCROLL TABLE     */
#define NBG0_VCSCLTBL (SCR_VRAMA0 + 0x1e000) /* NBG0 VCELL SCROLL TABLE     */
#define NBG1_VCSCLTBL (SCR_VRAMA0 + 0x1e100) /* NBG1 VCELL SCROLL TABLE     */

#define LCLRTBLADD (SCR_VRAMA0 + 0x1e200)    /* Linecolortable 256 * 1 word */
#define BCLRTBLADD (SCR_VRAMA0 + 0x1e400)    /* Backcolortable 256 * 1 word */

#define LWND0TBLADD (SCR_VRAMA0 + 0x1f000)   /* LineWindow #0  256 * 2word  */
#define LWND1TBLADD (SCR_VRAMA0 + 0x1f800)   /* LineWindow #1  256 * 2word  */

#define RBG0_KEISU (SCR_VRAMA1 + 0x00000)    /* RBG0 �W��table 352 * 256 /2 */
#define RBG0_RPAR  (SCR_VRAMA1 + 0x1f000)    /* RBG0 ��]�p�����[�^�e�[�u�� */
#define RBG0_PTGEN (SCR_VRAMB0 + 0x00000)    /* RBG0 PTGEN                  */
#define RBG0_PTNAM (SCR_VRAMB1 + 0x00000)    /* RBG0 PTNAM                  */



/* ��]�X�N���[�����W�X�^�}�b�v�\���� */
typedef struct {
	Sint32 xst;             /* 00 */
	Sint32 yst;             /* 04 */
	Sint32 zst;             /* 08 */
	Sint32 dxst;            /* 0c */
	Sint32 dyst;            /* 10 */
	Sint32 dx;              /* 14 */
	Sint32 dy;              /* 18 */
	Sint32 mtr_a;           /* 1c */
	Sint32 mtr_b;           /* 20 */
	Sint32 mtr_c;           /* 24 */
	Sint32 mtr_d;           /* 28 */
	Sint32 mtr_e;           /* 2c */
	Sint32 mtr_f;           /* 30 */
	Sint16 px;              /* 34 */
	Sint16 py;              /* 36 */
	Sint16 pz;              /* 38 */
	Sint16 pdummy;          /* 3a */
	Sint16 cx;              /* 3c */
	Sint16 cy;              /* 3e */
	Sint16 cz;              /* 40 */
	Sint16 cdummy;          /* 42 */
	Sint32 mx;              /* 44 */
	Sint32 my;              /* 48 */
	Sint32 kx;              /* 4c */
	Sint32 ky;              /* 50 */
	Sint32 kast;            /* 54 �W��RAM�X�^�[�g�A�h���X */
	Sint32 dkasy;           /* 58 �W��RAM�������� �����l  */   
	Sint32 dkasx;           /* 5c �W��RAM�������� �����l  */
    } ROTPARAM;





/*
 *    �֐��v���g�^�C�v
 */
extern void SMV2_IniScroll(void);

extern void SMV2_SetFont4(Uint8 *vramadd, Uint32 fontcolor, Uint32 backcolor);
extern void SMV2_SetFont8(Uint8 *vramadd, Uint32 fontcolor, Uint32 backcolor);
extern void SMV2_PutAsc4(Uint16 *wradd, Uint8 *mesadd);
extern void SMV2_PutAsc8(Uint16 *wradd, Uint8 *mesadd);

extern void SMV2_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                         Uint16 hsize, Uint16 vsize);
extern void SMV2_BoxWrite32(Uint32 *wradd, Uint32 *mapdata, 
                                            Uint16 hsize, Uint16 vsize);

extern void SMV2_BoxFil16(Uint16 *wradd, Uint16 wrdata,
                                               Uint16 hsize, Uint16 vsize);
extern void SMV2_BoxFil32(Uint32 *wradd, Uint32 wrdata,
                                               Uint16 hsize, Uint16 vsize);


extern void SMV2_FilMem32(Uint32 *memadd, Uint32 len, Uint32 fildata);
extern void SMV2_FilMem16(Uint16 *memadd, Uint32 len, Uint16 fildata);

extern void SMV2_TvOn(void);
extern void SMV2_TvOff(void);

/*
 *    ���[�NRAM
 */
GLOBAL SclVramConfig SMV2_sclvram;   /* �X�N���[��VRAM�̎g�p�ړI�A�T�C��    */
GLOBAL SclConfig SMV2_nscrlreg0;     /* NBG0 scroll initial                 */
GLOBAL SclConfig SMV2_nscrlreg1;     /* NBG1 scroll initial                 */
GLOBAL SclConfig SMV2_rscrlreg;      /* RBG0 scroll initial                 */
GLOBAL SclLineparam SMV2_lparam;     /* ���C���X�N���[���e�[�u���o�b�t�@    */
GLOBAL ROTPARAM SMV2_rotparbuf;      /* ��]�p���[���[�^�o�b�t�@            */
GLOBAL Sint32 SMV2_KeisuBuf[0x200];  /* ��]�W���o�b�t�@                    */
GLOBAL Sint16 SMV2_LWnd0Buf[0x200];  /* ���C���E�B���h�E0�e�[�u���o�b�t�@   */
GLOBAL Sint16 SMV2_LWnd1Buf[0x200];  /* ���C���E�B���h�E1�e�[�u���o�b�t�@   */


GLOBAL Uint32 SMV2_fontoffset;


#endif



