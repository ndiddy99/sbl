/*----------------------------------------------------------------------------
 *  spr_1c.c -- SPR ���C�u���� 1C ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  ���̃��C�u�����̓X�v���C�g�\����{�������W���[���ŁA�ȉ��̃��[�`�����܂ށB
 *
 *  SPR_Initial             -  �X�v���C�g�\����{�������C�u�����̏���������
 *  SPR_SetTvMode           -  �s�u���[�h�̃Z�b�g
 *  SPR_GetStatus           -  ���X�v���C�g������̎擾
 *  SPR_SetEraseData        -  �t���[���o�b�t�@�̃C���[�Y�f�[�^�Z�b�g
 *  SPR_WaitDrawEnd         -  �t���[���o�b�t�@�`��I���`�F�b�N
 *  SPR_SetEosMode          -  �X�v���C�g�`������E����W�I���r�b�g�̃Z�b�g
 *
 *  ���̃��C�u�������g�p����ɂ͎��̃C���N���[�h�t�@�C�����`����K�v������B
 *
 *  #include "sega_scl.h"
 *  #include "sega_spr.h"
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_spr.h"
#ifdef _SH
#include <machine.h>
#endif


/* #define  VDP2_INIT */    /* ### */

/*
 * GLOBAL DECLARATIONS
 */
Sint32 SpInitialFlag = 0;                   /* VDP1 Initial Flag             */
Sint32 SpScreenX;                           /* TV Screen X Size              */
Sint32 SpScreenX;                           /* TV Screen X Size              */
Sint32 SpScreenY;                           /* TV Screen Y Size              */
Uint16 SpTvMode;                            /* TV Mode                       */
Uint16 SpFbcrMode;                          /* Frame Buffer Chane Mode       */
Uint16 SpDie;                               /* Double Interlace Enable Flag  */
Uint16 SpDil;                               /* Double Interlace Plot Line    */

/*
 * STATIC DECLARATIONS
 */
static Uint16  SpEraseLTPoint = 0;          /* Erase Area Left-Top Point     */
static Uint16  SpEraseRBPoint = 0;          /* Erase Area Right-Bottom Point */
static Uint16  EraseData   = 0;
static Uint16  EraseLeftX  = 0;
static Uint16  EraseTopY   = 0;
static Uint16  EraseRightX = 0;
static Uint16  EraseBotY   = 0;
static Uint16  BitWidth    = 0;


static XyInt   scrSize[8] = { {320, 224},   /* SPR_TV_320X224  */
                              {320, 240},   /* SPR_TV_320X240  */
                              {352, 224},   /* SPR_TV_352X224  */
                              {352, 240},   /* SPR_TV_352X240  */
                              {640, 224},   /* SPR_TV_640X224  */
                              {640, 240},   /* SPR_TV_640X240  */
                              {704, 224},   /* SPR_TV_704X224  */
                              {704, 240} }; /* SPR_TV_704X240  */
    

/*****************************************************************************
 *
 * NAME:  SPR_Initial()  - Initialize Sprite Environment
 *
 * PARAMETERS :
 *
 *     (1) Uint8  **VRAM       - <o> VRAM �A�h���X���^�[���G���A�̃|�C���^
 *     (2) Uint8  **CharVRAM   - <o> �L�����N�^�p�^�[�� VRAM �A�h���X���^�[��
 *                                   �G���A�̃|�C���^
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g���̏�����
 *
 *     (1) �X�v���C�g�s�u���[�h���m�[�}���i�R�Q�O���Q�Q�S�j�ɐݒ�
 *     (2) �����E����W�I���r�b�g���O�ɐݒ�
 *     (3) �f�t�H���g�C���[�Y�f�[�^�ƃC���[�Y�G���A�̐ݒ�
 *         �C���[�Y�f�[�^ �� 0x8000(��)
 *         �C���[�Y�G���A �� 0x0 - 319x223
 *     (4) �f�t�H���g�t���[���؂�ւ����[�h�Ƃ��ăI�[�g��ݒ�
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void
SPR_Initial(Uint8 **VRAM)
{
    Uint32  vbr, *irqVector;
    int     iMask;
    Uint32  *l, i;

    /** BEGIN ***************************************************************/
    iMask = get_imask();
    set_imask(15);                           /* interrupt disable           */

    /* FBCR Flag clear */
    SPR_SetEosMode(0);

    /* NTSC, no-turn-around, 16bit/pixel */
    SPR_SetTvMode(SPR_TV_NORMAL, SPR_TV_320X224, 0);

    /* change frame buffer to auto change mode  */
    SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_AUTO | SpFbcrMode);

    /* trig set to auto start drawing */
    SPR_WRITE_REG(SPR_W_PTMR, 0x0002);

    /* set erase write color to Black */
    SPR_WRITE_REG(SPR_W_EWDR, RGB16_COLOR(0,0,0));

    /* set e/w left top point to 0 */
    SPR_WRITE_REG(SPR_W_EWLR, 0x0000);

    /* set e/w right bottom point to ffff */
    SPR_WRITE_REG(SPR_W_EWRR, 0xffff);

#ifdef VDP2_INIT  /* VDP2 + Priority initial */
    /* TV mode  show screen & 320*224 */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x000000), 0x8000);

    /* Sprite Control */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000e0), 0x0020);

    /* Set Priority */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f0), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f2), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f4), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f6), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f8), 0x0000);
#endif

    SPR_SetEraseData(RGB16_COLOR(0,0,0), 0, 0, SpScreenX - 1, SpScreenY - 1);

    *VRAM     = (Uint8*)VRAM_ADDR;

    *(*VRAM)     = 0x80;                 /* set END command to VRAM top area */
    *((*VRAM)+1) = 0x00;

    set_imask(iMask);                    /* interrupt enable             */

    SpInitialFlag = 1;                   /* VDP1 Initial Flag ON             */
}


/*****************************************************************************
 *
 * NAME:  SPR_SetTvMode()  - Set TV Mode
 *
 * PARAMETERS :
 *
 *     (1) Uint16  mode            - <i>  �s�u���[�h��`�l
 *     (2) Uint16  screenSize      - <i>  ��ʂ̉𑜓x
 *     (3) Uint16  doubleInterlace - <i>  �{���C���^���[�X�t���O
 *
 * DESCRIPTION:
 *
 *     �s�u���[�h��ݒ肷��
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace)
{
    Uint16  vcnt;

    /** BEGIN ***************************************************************/
    SpTvMode = 0x0007 & mode;
    SPR_WRITE_REG(SPR_W_TVMR, SpTvMode);
    if(screenSize > SPR_TV_704X240)
        screenSize = SPR_TV_320X224;
    SpScreenX = scrSize[screenSize].x;
    SpScreenY = scrSize[screenSize].y;
    if(doubleInterlace == ON) SpScreenY <<= 1;

    if((mode == SPR_TV_HIRESO) || (mode == SPR_TV_ROT8))
        BitWidth = 1;  /* 8 bit mode */
    else
        BitWidth = 0;  /* 16 bit mode */

    if(doubleInterlace)
        SpDie = SPR_FBCR_DIE;
    else
        SpDie = 0;
    SpDil = 0;
}


/*****************************************************************************
 *
 * NAME:  SPR_GetStatus()  - Get Sprite Current status
 *
 * PARAMETERS :
 *
 *     (1) struct SpStatus *spStatus  - <o> ���^�[���X�e�[�^�X�G���A
 *
 * DESCRIPTION:
 *
 *     ���݂̃X�v���C�g����̊e���Ԃ�Ԃ�
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_GetStatus(SprSpStatus *spStatus)
{

    /** BEGIN ***************************************************************/
    spStatus->frameChgMode   = SpFrameChgMode;
    spStatus->frameEraseMode = SpFrameEraseMode;
    spStatus->vbInterval     = VBInterval;      
    spStatus->eraseData      = EraseData;
    spStatus->eraseLeftX     = EraseLeftX;
    spStatus->eraseTopY      = EraseTopY;
    spStatus->eraseRightX    = EraseRightX;
    spStatus->eraseBotY      = EraseBotY;

}


/*****************************************************************************
 *
 * NAME:  SPR_SetEraseData()  - Set Frame Buffer Erase Data & Area
 *
 * PARAMETERS :
 *
 *     (1) Uint16  eraseData     - <i> RGB �C���[�Y�f�[�^
 *     (2) Uint16  leftX         - <i> �C���[�Y�G���A���[�w���W
 *     (3) Uint16  topY          - <i> �C���[�Y�G���A��[�x���W
 *     (4) Uint16  rightX        - <i> �C���[�Y�G���A�E�[�w���W
 *     (5) Uint16  botY          - <i> �C���[�Y�G���A���[�x���W
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g�h�b�ɃC���[�Y�f�[�^�ƃC���[�Y�G���A�̐ݒ������
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_SetEraseData(Uint16 eraseData, Uint16 leftX, Uint16 topY,
                             Uint16 rightX, Uint16 botY)
{
    /** BEGIN ***************************************************************/
     EraseData   = eraseData;
     EraseLeftX  = leftX;
     EraseTopY   = topY;
     EraseRightX = rightX;
     EraseBotY   = botY;

     SPR_WRITE_REG(SPR_W_EWDR, eraseData);      /* set erase write data */

     if(BitWidth)
	 leftX >>= 4;
     else
	 leftX >>= 3;
     if(SpDie) topY >>= 1;
     SpEraseLTPoint = (leftX << 9) + topY;
     SPR_WRITE_REG(SPR_W_EWLR, SpEraseLTPoint); /* set erase screen left top */

     rightX++;
     if(BitWidth)
	 rightX >>= 4;
     else
	 rightX >>= 3;
     if(SpDie) botY >>= 1;
     SpEraseRBPoint = (rightX << 9) + botY;
     SPR_WRITE_REG(SPR_W_EWRR, SpEraseRBPoint);
                                            /* set erase screen right bottom */

}


/*****************************************************************************
 *
 * NAME:  SPR_WaitDrawEnd()  - Wait Frame Baffer Draw End
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g�h�b���t���[���o�b�t�@�ւ̕`����I������̂�҂�
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *     �{���[�`���͂u�|�a�����������荞�݃��[�`������Ăяo�����
 *
 *
 *****************************************************************************
 */
void
SPR_WaitDrawEnd(void)
{

    /** BEGIN ***************************************************************/
    while((SPR_READ_REG(SPR_R_EDSR) & 0x0002) == 0);
}


/*****************************************************************************
 *
 * NAME:  SPR_SetEosMode()  - Set EOS Bit
 *
 * PARAMETERS :
 *
 *     (1)  Sint32    eosFlag  - <i>  �`�惂�[�h���[�h�ő��x�D��`����w�肵��
 *                                    ���̌��G�e�N�X�`���̃T���v�����O���W
 *                                    �O���������W���T���v�����O�i�f�t�H���g�j
 *                                    �P������W���T���v�����O
 *
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g�`������E����W�I���r�b�g�̃Z�b�g
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *
 *****************************************************************************
 */
void
SPR_SetEosMode(Sint32 eosFlag)
{

    /** BEGIN ***************************************************************/
    if(eosFlag)
        SpFbcrMode = SPR_FBCR_EOS;
    else
        SpFbcrMode = 0;
}


#if 0
static Uint16
getPortW(Uint16 *portAddr)
{
   return *portAddr;
}

static void
putPortW(Uint16 *portAddr, Uint16 c)
{
   *portAddr = c;
}
#endif

/*  end of file */
