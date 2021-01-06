/*----------------------------------------------------------------------------
 *  spr_2c.c -- SPR ���C�u���� 2C ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  ���̃��C�u�����̓X�v���C�g�\���g���i�Q�c�j�������W���[���ŁA�ȉ��̃��[�`��
 *  ���܂ށB
 *
 *  SPR_2Initial            -  �Q�c�X�v���C�g�\���̏���������
 *  SPR_2SetTvMode          -  �s�u���[�h�̃Z�b�g
 *  SPR_2FrameChgIntr       -  �t���[���`�F���WV-BLANK�C���^�[�o�����Z�b�g
 *  SPR_2FrameEraseData     -  �t���[���o�b�t�@�C���[�Y�f�[�^�̃Z�b�g
 *  SPR_2SetGourTbl         -  �O�[���[�e�[�u���̃Z�b�g
 *  SPR_2SetLookupTbl       -  ���b�N�A�b�v�e�[�u���̃Z�b�g
 *  SPR_2SetChar            -  �L�����N�^�̃Z�b�g
 *  SPR_2ClrChar            -  �L�����N�^�G���A�̉��
 *  SPR_2ClrAllChar         -  �S�L�����N�^�G���A�̉��
 *  SPR_2GourTblNo          -  �O�[���[�e�[�u���ԍ����u�q�`�l�A�h���X�ɕϊ�
 *  SPR_2LookupTblNo        -  ���b�N�A�b�v�e�[�u���ԍ����u�q�`�l�A�h���X�ɕϊ�
 *  SPR_2CharNo             -  �L�����N�^�ԍ����u�q�`�l�A�h���X�ɕϊ�
 *  SPR_2OpenCommand        -  �R�}���h�������݂̃I�[�v������
 *  SPR_2CloseCommand       -  �R�}���h�������݂̃N���[�Y����
 *  SPR_2FlushDrawPrty()    -  �R�}���h�`��v���C�I���e�B�`�F�C���̃t���b�V��
 *  SPR_2LocalCoord         -  ���[�J�����W�̃Z�b�g
 *  SPR_2SysClip            -  �V�X�e���N���b�s���O�G���A�̃Z�b�g
 *  SPR_2UsrClip            -  ���[�U�[�N���b�s���O�G���A�̃Z�b�g
 *  SPR_2Line               -  ���C���`��
 *  SPR_2PolyLine           -  �|�����C���̕`��
 *  SPR_2Polygon            -  �|���S���̕`��
 *  SPR_2NormSpr            -  �m�[�}���X�v���C�g�̕`��
 *  SPR_2ScaleSpr           -  �X�P�[���X�v���C�g�̕`��
 *  SPR_2DistSpr            -  �ό`�X�v���C�g�̕`��
 *  SPR_2Cmd                -  �R�}���h�Z�b�g
 *
 *  ���̃��C�u�������g�p����ɂ͎��̃C���N���[�h�t�@�C���ƃ��[�N�G���A���`��
 *  ��K�v������B
 *
 *   #include "sega_scl.h"
 *   #define  _SPR2_
 *   #include "sega_spr.h"
 *
 *   #define CommandMax    100           // �ő�R�}���h��
 *   #define GourTblMax    100           // �ő�O�[���[�e�[�u����
 *   #define LookupTblMax  100           // �ő僋�b�N�A�b�v�e�[�u����
 *   #define CharMax       100           // �ő�L�����N�^��
 *   #define DrawPrtyMax   256           // �ő�v���C�I���e�B�u���b�N��
 *   SPR_2DefineWork(work2D,
 *                 CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)
 *                                       // �Q�c���[�N�G���A��`
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <string.h>

#define  _SPR2_
#include        "sega_spr.h"
#include "sega_scl.h"
#ifdef _SH
#include <machine.h>
#include "sega_dma.h"
#endif

#define USE_DMA_LEVEL         0          /* 0 = DMA no use                   */
                                         /* 1 = DMA use only command tran    */
                                         /* 2 = DMA use only char data tran  */
                                         /* 3 = DMA use cmd & char data tran */

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* #define  SP2_DEBUG */

#define  VRAM_GRAN_MAX         16384     /* VRAM Granularity Max Count       */
#define  SP_SYS_CMD_MAX        4         /* System Command Area Count        */

#if(USE_DMA_LEVEL == 1 || USE_DMA_LEVEL == 3)
#define  GetSpCmdArea(CMD)                                                \
            if(SpDrawPrtyProcFlag) SPR_2SetDrawPrty(drawPrty);            \
            if(bCmdNo >= CMD_BUF_MAX) {                                   \
               CMD = SpCmdTbl + bufTopCmdNo;                              \
               DMA_ScuMemCopy(CMD, cmdBuf, sizeof(SprSpCmd)*CMD_BUF_MAX); \
               DMA_ScuResult();                                           \
               bufTopCmdNo += CMD_BUF_MAX;                                \
               bCmdNo = 0;  crCmdBuf = cmdBuf;                            \
            }                                                             \
            CMD = crCmdBuf++; bCmdNo++;  SpCmdNo++;
#else
#define  GetSpCmdArea(CMD)                                                \
            if(SpDrawPrtyProcFlag) SPR_2SetDrawPrty(drawPrty);            \
            CMD = crCmdTbl++;  SpCmdNo++;
#endif
#define  SetCharPara(CMD)                                                \
            CMD->charAddr = SpCharTbl[charNo].addr;                      \
            CMD->charSize = SpCharTbl[charNo].xySize;                    \
            drawMode = (drawMode & ~DRAW_COLOR) | SpCharTbl[charNo].mode;

#define  SetDrawPara(CMD)                                              \
            CMD->drawMode = drawMode;                                  \
            if((drawMode & DRAW_COLOR) == COLOR_1)                     \
                CMD->color = vramInfo.lookupTblR + color*4;            \
            else                                                       \
                CMD->color = color;

#define  SetDrawParaTex(CMD)                                           \
            CMD->drawMode = drawMode;                                  \
            if(color == 0xffff) color = SpCharTbl[charNo].color;       \
            if((drawMode & DRAW_COLOR) == COLOR_1)                     \
                CMD->color = vramInfo.lookupTblR + color*4;            \
            else                                                       \
                CMD->color = color;

#define  SetGourPara(CMD)                                              \
            if(gourTblNo == NO_GOUR)                                   \
                CMD->grshAddr = 0;                                     \
            else                                                       \
                CMD->grshAddr = crGourTblR + gourTblNo;

/*
 * DATA TYPE DECLARATIONS
 */
struct VramInfo {                           /* VRAM Area Entry Info.         */
    Uint16     cmdTblCnt;                   /* Command Area Max Count        */
    Uint16     cmdTblSize;                  /* Command Area Size (32 byte)   */
    Uint16     gourTblCnt;                  /* Gouraud Shading Table Max Cnt */
    Uint16     gourTblSize;                 /* Gouraud Shading Table Area Siz*/
    Uint16     lookupTblCnt;                /* Color Lookup Table Max Count  */
    Uint16     lookupTblSize;               /* Color Lookup Table Area Size  */
    SprLookupTbl *lookupTbl;                /* Color Lookup Table Top Addr   */
    Vaddr        lookupTblR;                /*      "      Relative Address  */
    Uint8      *blockPool;                  /* Block Pool Area Top Addr      */
    Uint16     blockPoolSize;               /* Block Pool Area Size          */
};

struct VramTbl {                            /* VRAM Area Address Info.       */
    SprSpCmd     *cmdTbl;                   /* Command Table Top Address     */
    Vaddr        cmdTblR;                   /*     "       Relative Address  */
    SprGourTbl   *gourTbl;                  /* Gouraud Shading Table Top Addr*/
    Vaddr        gourTblR;                  /*      "      Relative Address  */
};

struct NullBlock {                          /* Block Null Entry              */
    struct NullBlock *next;                 /* Next Null Block Pointer       */
    Uint16            size;                 /* Block Size (by the 32 byte)   */
};

typedef struct NullBlock NullBlock;

/*
 * GLOBAL DECLARATIONS
 */
Sint32         Sp2OpenFlag;           /* Open Command Exec Flag        */
Sint32         SpLCoordX;             /* Local Origin X Position       */
Sint32         SpLCoordY;             /* Local Origin Y Position       */
extern  Sint32 SpScreenX;             /* TV Screen X Size              */
extern  Sint32 SpScreenY;             /* TV Screen Y Size              */
SprCharTbl     *SpCharTbl;            /* Character Table Pointer       */
Sint32         SpCharMax;             /* Character Table Entry Count   */
Sint32         SpDrawPrtyBlkMax;      /* Draw Priority Block Max Count */
Sint32         SpCmdNo;
Sint32         SpDrawPrtyTopCmdNo;    /* �`��v���C�I���e�B�X�^�[�gCmd�ԍ�  */
Sint32         SpDrawPrtyProcFlag;    /* Draw Priority Process Flag         */
Sint32         SpCmdChainBlkNo;       /* Command Chain Tbl Use Entry No.    */
SprCmdChain    *SpCmdChain;           /* �R�}���h�`�F�C���e�[�u��           */
SprDrawPrtyBlk *SpDrawPrtyBlk;        /* �`��v���C�I���e�B�u���b�N�e�[�u�� */
SprCmdChain    *SpCrCmdChain;
SprSpCmd       *SpCmdTbl;
Vaddr          SpCmdTblR;

/*
 * STATIC DECLARATIONS
 */
static Uint8           *VRAM;
static Sint32          vramSW;
static SprSpCmd        *topSpCmd;
static struct VramInfo vramInfo;
static struct VramTbl  vramTbl[2];
static NullBlock       *nullBlockTop;
static Uint16          eraseData;
static Uint16          displayInterval;
static SprSpCmd        *crCmdTbl;
static Vaddr           crGourTblR;

#if(USE_DMA_LEVEL == 1 || USE_DMA_LEVEL == 3)
#define  CMD_BUF_MAX           80        /* Internal Command Buffer Max Count*/
static SprSpCmd  cmdBuf[CMD_BUF_MAX];    /* Internal Command Buffer          */
static Sint32    bCmdNo;                 /* Buffer Command No                */
static Sint32    bufTopCmdNo;            /* VRAM CMD No of Buf Top CMD       */
static SprSpCmd  *crCmdBuf;
#endif


static Vaddr allocBlock(Uint16 size);
static void  freeBlock(Vaddr addr, Uint16 size);
void  SPR_2SetDrawPrty(int drawPrtyNo);
void  SPR_2FlushDrawPrtyBlock(void);

#ifdef SP2_DEBUG
#include "spr_font.h"
static void dispError(char *rtnName, char *msg1, char *msg2);
#endif


/*****************************************************************************
 *
 * NAME:  SPR_2Initial()  - Initial 2D Sprite Display Environment
 *
 * PARAMETERS :
 *
 * �@�@(1) Spr2WorkArea *workArea - <i> �Q�c�X�v���C�g���[�N�G���A��`
 *
 * DESCRIPTION:
 *
 *     �Q�c�X�v���C�g�\�����̏���������
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
SPR_2Initial(Spr2WorkArea  *workArea)
{
    Uint16    w;
    Uint8     *wp;
    Uint16    gourTblCnt, cmdTblCnt;

    /** BEGIN ***************************************************************/
    SpCharMax = workArea->charMax;
    SpCharTbl = workArea->charTbl;

    gourTblCnt   = (workArea->gourTblMax + 3) & 0xfffc;

    SPR_Initial(&VRAM);

    cmdTblCnt = workArea->commandMax + SP_SYS_CMD_MAX;
    vramInfo.cmdTblCnt     = cmdTblCnt;
    vramInfo.cmdTblSize    = cmdTblCnt;
    vramInfo.gourTblCnt    = gourTblCnt;
    vramInfo.gourTblSize   = gourTblCnt / 4;
    vramInfo.lookupTblCnt  = workArea->lookupTblMax;
    vramInfo.lookupTblSize = workArea->lookupTblMax;
    w = (vramInfo.cmdTblSize + vramInfo.gourTblSize) * 2 + 
         vramInfo.lookupTblSize + 1;
#ifdef SP2_DEBUG
    if(w > VRAM_GRAN_MAX) dispError("SP2INITIAL", "- AREA OVER -", 0);
#endif
    vramInfo.blockPoolSize = VRAM_GRAN_MAX - w;
    
    vramTbl[0].cmdTbl    = (SprSpCmd*)VRAM + 1;
    vramTbl[0].gourTbl   = (SprGourTbl*)  (vramTbl[0].cmdTbl    +
                                        vramInfo.cmdTblSize);
    vramTbl[1].cmdTbl    = (SprSpCmd*)    (vramTbl[0].gourTbl +
                                        vramInfo.gourTblSize * 4);
    vramTbl[1].gourTbl   = (SprGourTbl*)  (vramTbl[1].cmdTbl    +
                                        vramInfo.cmdTblSize);
    vramInfo.lookupTbl   = (SprLookupTbl*)(vramTbl[1].gourTbl   +
                                        vramInfo.gourTblSize * 4);
    wp                   = (Uint8*)    (vramInfo.lookupTbl +
                                        vramInfo.lookupTblSize);
    vramInfo.blockPool   = wp;

    vramTbl[0].cmdTblR   = 4;
    vramTbl[0].gourTblR  = vramTbl[0].cmdTblR    + vramInfo.cmdTblSize*4;
    vramTbl[1].cmdTblR   = vramTbl[0].gourTblR   + vramInfo.gourTblSize*4;
    vramTbl[1].gourTblR  = vramTbl[1].cmdTblR    + vramInfo.cmdTblSize*4;
    vramInfo.lookupTblR  = vramTbl[1].gourTblR   + vramInfo.gourTblSize*4;

    SPR_2ClrAllChar();

    topSpCmd = (SprSpCmd*)VRAM;
    topSpCmd->control = CTRL_END;

    SpCmdChain           = workArea->cmdChain;
    SpDrawPrtyBlk        = workArea->drawPrtyBlk;
    SpDrawPrtyBlkMax     = workArea->drawPrtyBlkMax;
    SpDrawPrtyProcFlag   = 0;
    memset(SpDrawPrtyBlk,0xff,sizeof(SprDrawPrtyBlk)*SpDrawPrtyBlkMax);

    vramSW = 0;
    SpCmdNo  = SP_SYS_CMD_MAX;
    SPR_2FrameEraseData(RGB16_COLOR(0,0,0));
    SpLCoordX = SpLCoordY = 0;

#if 0
xprintf("\n*** VramInfo ***\n");
xprintf("  cmdTblCnt     = %d\n",   vramInfo.cmdTblCnt);
xprintf("  cmdTblSize    = %d\n",   vramInfo.cmdTblSize);
xprintf("  gourTblCnt    = %d\n",   vramInfo.gourTblCnt);
xprintf("  gourTblSize   = %d\n",   vramInfo.gourTblSize);
xprintf("  lookupTblCnt  = %d\n",   vramInfo.lookupTblCnt);
xprintf("  lookupTblSize = %d\n",   vramInfo.lookupTblSize);
xprintf("  blockPool     = %lxH\n", vramInfo.blockPool);
xprintf("  blockPoolSize = %d\n",   vramInfo.blockPoolSize);
xprintf("\n*** VramTbl - 0 ***\n");
xprintf("  cmdTbl     = %lxH\n", vramTbl[0].cmdTbl);
xprintf("  cmdTblR    = %xH\n",  vramTbl[0].cmdTblR);
xprintf("  gourTbl    = %lxH\n", vramTbl[0].gourTbl);
xprintf("  gourTblR   = %xH\n",  vramTbl[0].gourTblR);
xprintf("  lookupTbl  = %lxH\n", vramTbl[0].lookupTbl);
xprintf("  lookupTblR = %xH\n",  vramTbl[0].lookupTblR);
xprintf("\n*** VramTbl - 1 ***\n");
xprintf("  cmdTbl     = %lxH\n", vramTbl[1].cmdTbl);
xprintf("  cmdTblR    = %xH\n",  vramTbl[1].cmdTblR);
xprintf("  gourTbl    = %lxH\n", vramTbl[1].gourTbl);
xprintf("  gourTblR   = %xH\n",  vramTbl[1].gourTblR);
xprintf("  lookupTbl  = %lxH\n", vramTbl[1].lookupTbl);
xprintf("  lookupTblR = %xH\n",  vramTbl[1].lookupTblR);
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetTvMode()  - Set TV Mode
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
SPR_2SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace)
{

    /** BEGIN ***************************************************************/
    SPR_SetTvMode(mode, screenSize, doubleInterlace);
}


/*****************************************************************************
 *
 * NAME:  SPR_2FrameChgIntr()  - Set Frame Change Interval Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16  interval   - <i>  �u�|�a�k�`�m�j�C���^�[�o����
 *                                0      = �X�v���C�g�̃t���[���؂�ւ����I�[�g
 *                                         �ɂ��āA�C���^�[�o�����P�ɂ���
 *                                         SCL_DisplayFrame()�ɂ�铯���Ȃ�
 *                                1      = �X�v���C�g�̃t���[���؂�ւ����I�[�g
 *                                         �ɂ��āA�C���^�[�o�����P�ɂ���
 *                                         SCL_DisplayFrame()�ɂ�铯������
 *                                0xffff = �C���^�[�o���𖳌��ɂ��ăt���[���؂�
 *                                         �ւ��v�����ɑ��؂�ւ���
 *                                0xfffe = �C���^�[�o���𖳌��ɂ��ăt���[���؂�
 *                                         �ւ��v�����ɂu�u�����N�C���[�X���s
 *                                         ���đ��؂�ւ���
 *                                ���̑� = �w��C���^�[�o���Ńt���[���؂�ւ�
 *                                         ���s��
 *                                         b15 = 0 : �C���[�X���C�g���s��
 *                                             = 1 : �C���[�X���C�g���s��Ȃ�
*
 * DESCRIPTION:
 *
 *     �t���[���`�F���W�u�|�a�k�`�m�j�C���^�[�o�����̃Z�b�g
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
SPR_2FrameChgIntr(Uint16 interval)
{

    /** BEGIN ***************************************************************/
    displayInterval = interval;
    SCL_SetFrameInterval(interval);
}


/*****************************************************************************
 *
 * NAME:  SPR_2FrameEraseData()  - Set Frame Buffer Erase Data
 *
 * PARAMETERS :
 *
 *     (1) Uint16  rgbColor      - <i> RGB �C���[�Y�f�[�^
 *                                     0 = �����R�[�h
 *
 * DESCRIPTION:
 *
 *     �t���[���o�b�t�@�C���[�Y�f�[�^�̃Z�b�g
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
SPR_2FrameEraseData(Uint16 rgbColor)
{

    /** BEGIN ***************************************************************/
    SPR_SetEraseData(rgbColor, 0, 0, SpScreenX-1, SpScreenY-1);
    eraseData = rgbColor;
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetGourTbl()  - Set Gouraud Shading Table in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16  gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *     (2) SprGourTbl *gourTbl   - <i> �O�[���[�e�[�u���|�C���^
 *
 * DESCRIPTION:
 *
 *     �O�[���[�e�[�u�����u�q�`�l�ɃZ�b�g
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
SPR_2SetGourTbl(Uint16 gourTblNo, SprGourTbl *gourTbl)
{
    Uint16      *ws, *wd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(gourTblNo >= vramInfo.gourTblCnt)
        dispError("SP2SETGOURTBL", "- GOURAUD TABLE OVER -", 0);
#endif
    wd = (Uint16*)(vramTbl[vramSW].gourTbl + gourTblNo);
    ws = (Uint16*)gourTbl;
    *wd++ = *ws++;
    *wd++ = *ws++;
    *wd++ = *ws++;
    *wd   = *ws  ;
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetLookupTbl()  - Set Color Lookup Table in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16        lookupTblNo   - <i>  ���b�N�A�b�v�e�[�u���ԍ�
 *     (2) SprLookupTbl *lookupTbl     - <i>  ���b�N�A�b�v�e�[�u���|�C���^
 *
 * DESCRIPTION:
 *
 *     �J���[���b�N�A�b�v�e�[�u�����u�q�`�l�ɃZ�b�g
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
SPR_2SetLookupTbl(Uint16 lookupTblNo, SprLookupTbl *lookupTbl)
{
    SprLookupTbl  *dLookupTbl;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(lookupTblNo >= vramInfo.lookupTblCnt)
        dispError("SP2SETLOOKUPTBL", "- LOOKUP TABLE OVER -", 0);
#endif
    dLookupTbl = vramInfo.lookupTbl + lookupTblNo;
#ifdef USE_DMA
    DMA_ScuMemCopy(dLookupTbl, lookupTbl, sizeof(SprLookupTbl));
    DMA_ScuResult();
#else
    memcpy(dLookupTbl, lookupTbl, sizeof(SprLookupTbl));
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetChar()  - Set Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo       - <i>  �L�����N�^�ԍ�
 *     (2) Uint16  colorMode    - <i>  �J���[���[�h�i�O�|�T�j
 *     (3) Uint16  color        - <i>  �J���[�f�[�^�i�װ��ݸorٯ�����ð���No�j
 *     (4) Uint16  width        - <i>  �w�T�C�Y
 *     (5) Uint16  height       - <i>  �x�T�C�Y
 *     (6) Uint8  *charImage    - <i>  �L�����N�^�f�[�^
 *
 * DESCRIPTION:
 *
 *     �L�����N�^�f�[�^���u�q�`�l�ɃZ�b�g
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
SPR_2SetChar(Uint16 charNo, Uint16 colorMode, Uint16 color,
                       Uint16 width, Uint16 height, Uint8 *charImage)
{
    Uint32   w, charSize;
    Uint8    *charPtr;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(charNo >= SpCharMax)
        dispError("SP2SETCHAR", "- CHAR NO. OVER -", 0);
#endif
    w = (Uint32)width * (Uint32)height;
    if(colorMode >= COLOR_5)
       w <<= 1;
    else
    if(colorMode <= COLOR_1)
       w >>= 1;
    w = (w + 31) >> 5;
    charSize = w << 4;
    if(SpCharTbl[charNo].addr == 0) {
        SpCharTbl[charNo].addr = allocBlock(w);
#ifdef SP2_DEBUG
        if(SpCharTbl[charNo].addr == 0)
            dispError("SP2SETCHAR", "- CHAR AREA NOTHING -", 0);
#endif
        SpCharTbl[charNo].size = w;
    }
    if(w <= SpCharTbl[charNo].size) {
        charPtr = VRAM + (((Uint32)SpCharTbl[charNo].addr)<<3);
        if(charImage) {
#if(USE_DMA_LEVEL == 2 || USE_DMA_LEVEL == 3)
            DMA_ScuMemCopy(charPtr, charImage, charSize*2);
            DMA_ScuResult();
#else
            memcpy(charPtr, charImage, charSize*2);
#endif
        }
        SpCharTbl[charNo].mode   = colorMode;
        SpCharTbl[charNo].color  = color;
	SpCharTbl[charNo].xySize = (width << 5) + height;
    } else {
#ifdef SP2_DEBUG
        dispError("SP2SETCHAR", "- CHAR OVER WRITE SIZE -", 0);
#endif
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_2ClrChar()  - Clear Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo       - <i>  �L�����N�^�ԍ�
 *
 * DESCRIPTION:
 *
 *     �u�q�`�l����L�����N�^�f�[�^�G���A�����
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
SPR_2ClrChar(Uint16 charNo)
{

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(charNo >= SpCharMax)
        dispError("SP2CLRCHAR", "- CHAR NO. OVER -", 0);
#endif
    if(SpCharTbl[charNo].addr) {
       freeBlock(SpCharTbl[charNo].addr, SpCharTbl[charNo].size);
       SpCharTbl[charNo].addr = 0;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_2ClrAllChar()  - Clear All Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �u�q�`�l����S�L�����N�^�f�[�^�G���A�����
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
SPR_2ClrAllChar(void)
{
    Sint32    i;

    /** BEGIN ***************************************************************/
    for(i=0; i<SpCharMax; i++)
        SpCharTbl[i].addr = 0;

    if(vramInfo.blockPoolSize) {
        nullBlockTop       = (struct NullBlock*)vramInfo.blockPool;
        nullBlockTop->next = 0;
        nullBlockTop->size = vramInfo.blockPoolSize;
    } else
        nullBlockTop = 0;
}


/*****************************************************************************
 *
 * NAME:  SPR_2GourTblNoToVram()  - Convert Gouraud Shading Table No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  gourTblNo    - <i>  �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �O�[���[�e�[�u���ԍ����u�q�`�l�A�h���X�ɕϊ�
 *
 * POSTCONDITIONS:
 *
 *     �u�q�`�l�����΃A�h���X�^�W
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2GourTblNoToVram(Uint16 gourTblNo)
{

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(gourTblNo >= vramInfo.gourTblCnt)
        dispError("SP2GOURTBLNOTOVRAM", "- GOURAUD TABLE OVER -", 0);
#endif
    return vramTbl[vramSW].gourTblR + gourTblNo;
}


/*****************************************************************************
 *
 * NAME:  SPR_2LookupTblNoToVram()  - Convert Color Lookup Table No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  lookupTblNo  - <i>  ���b�N�A�b�v�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     ���b�N�A�b�v�e�[�u���ԍ����u�q�`�l�A�h���X�ɕϊ�
 *
 * POSTCONDITIONS:
 *
 *     �u�q�`�l�����΃A�h���X�^�W
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2LookupTblNoToVram(Uint16 lookupTblNo)
{

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(lookupTblNo >= vramInfo.lookupTblCnt)
        dispError("SP2LOOKUPTBLNOTOVRAM", "- LOOKUP TABLE OVER -", 0);
#endif
    return  vramInfo.lookupTblR + lookupTblNo*4;
}


/*****************************************************************************
 *
 * NAME:  SPR_2CharNoTblNoToVram()  - Convert Character No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo    - <i>  �L�����N�^�ԍ�
 *
 * DESCRIPTION:
 *
 *     �L�����N�^�ԍ����u�q�`�l�A�h���X�ɕϊ�
 *
 * POSTCONDITIONS:
 *
 *     �u�q�`�l�����΃A�h���X�^�W
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2CharNoToVram(Uint16 charNo)
{

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(charNo >= SpCharMax)
        dispError("SP2CHARNOTOVRAM", "- CHAR NO. OVER -", 0);
#endif
    return  SpCharTbl[charNo].addr; 
}


/*****************************************************************************
 *
 * NAME:  SPR_2GetAreaSizeInfo()  - Get Work Area Size Info.
 *
 * PARAMETERS :
 *
 *     (1) SprAreaSize  *areaSizeInfo   - <o>  �G���A�T�C�Y��񃊃^�[���G���A
 *
 * DESCRIPTION:
 *
 *     �e���[�N�G���A�̃T�C�Y�y�юg�p�T�C�Y��Ԃ�
 *
 * POSTCONDITIONS:
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_2GetAreaSizeInfo(SprAreaSize  *areaSizeInfo)
{
    NullBlock  *nullBlock;
    Uint16     nullBlkMaxSize, totalNullBlkCount;

    /** BEGIN ***************************************************************/
    nullBlock         = nullBlockTop;
    nullBlkMaxSize    = 0;
    totalNullBlkCount = 0;
    while(nullBlock) {
        if(nullBlkMaxSize < nullBlock->size)
            nullBlkMaxSize = nullBlock->size;
        totalNullBlkCount += nullBlock->size;
        nullBlock = nullBlock->next;
    }
    areaSizeInfo->charRemainBlkCount = totalNullBlkCount;
    areaSizeInfo->charNullBlkMaxSize = nullBlkMaxSize;
    areaSizeInfo->commandMax         = vramInfo.cmdTblCnt;
    areaSizeInfo->gourTblMax         = vramInfo.gourTblCnt;
    areaSizeInfo->lookupTblMax       = vramInfo.lookupTblCnt;
    areaSizeInfo->charMax            = SpCharMax;
    areaSizeInfo->drawPrtyBlkMax     = SpDrawPrtyBlkMax;
    areaSizeInfo->useCommandCount    = SpCmdNo;
}


/*****************************************************************************
 *
 * NAME:  SPR_2OpenCommand()  - Open Sprite Command Write
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �R�}���h�������݂̃I�[�v������
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
SPR_2OpenCommand(Uint16 drawPrtyFlag)
{
    SprSpCmd  *spCmd;
    /** BEGIN ***************************************************************/
    Sp2OpenFlag         = 1;
    vramSW             ^= 1;
    SpCmdNo             = SP_SYS_CMD_MAX;
    SpDrawPrtyProcFlag  = drawPrtyFlag;
    SpCmdChainBlkNo     = 0;
    SpCrCmdChain        = SpCmdChain;
    SpDrawPrtyTopCmdNo  = SpCmdNo;
    SpCmdTbl            = vramTbl[vramSW].cmdTbl;
    SpCmdTblR           = vramTbl[vramSW].cmdTblR;
    crGourTblR          = vramTbl[vramSW].gourTblR;
    crCmdTbl            = SpCmdTbl + SpCmdNo;
    spCmd               = crCmdTbl;
    spCmd->control      = CTRL_SKIP;
    crCmdTbl++;
    SpCmdNo++;
#if(USE_DMA_LEVEL == 1 || USE_DMA_LEVEL == 3)
    bCmdNo              = 0;
    bufTopCmdNo         = SpCmdNo;
    crCmdBuf            = cmdBuf;
#endif    
}


/*****************************************************************************
 *
 * NAME:  SPR_2CloseCommand()  - Close Sprite Command Write
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �R�}���h�������݂̃N���[�Y����
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
SPR_2CloseCommand(void)
{
    SprSpCmd    *spCmd;
    Uint16      xy[8];
    Uint16      *w;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2CLOSECOMMAND", "- COMMAND OVER -", 0);
#endif

    SPR_2FlushDrawPrty();

    spCmd = SpCmdTbl + SpCmdNo;
    spCmd->control = CTRL_END;
    topSpCmd->control = CTRL_SKIP | JUMP_ASSIGN;
    if(displayInterval == 0xffff) {
        SpCmdNo = 0;
	w = xy;
        *w++ = 0;
        *w++ = 0;
        *w++ = SpScreenX-1;
        *w++ = 0;
        *w++ = SpScreenX-1;
        *w++ = SpScreenY-1;
        *w++ = 0;
        *w   = SpScreenY-1;

        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control = JUMP_NEXT | FUNC_LCOORD;
        spCmd->ax      = 0;
        spCmd->ay      = 0;

        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control  = JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON;
        spCmd->drawMode =  ECDSPD_DISABLE | COLOR_5 | COMPO_REP;
        spCmd->color    = eraseData;
        memcpy(&spCmd->ax, xy, sizeof(XyInt)*4);
        spCmd->grshAddr = 0;

        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control = JUMP_NEXT | FUNC_LCOORD;
        spCmd->ax      = SpLCoordX;
        spCmd->ay      = SpLCoordY;

        spCmd          = SpCmdTbl + SpCmdNo;
        spCmd->control = CTRL_SKIP | JUMP_ASSIGN;
        spCmd->link    = SpCmdTblR + SP_SYS_CMD_MAX*4;
        topSpCmd->link = SpCmdTblR;
    } else
        topSpCmd->link = SpCmdTblR + SP_SYS_CMD_MAX*4;
}


/*****************************************************************************
 *
 * NAME:  SPR_2FlushDrawPrty()  - Flush Draw Priority Command Chain
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �R�}���h�`��v���C�I���e�B�`�F�C���̃t���b�V��
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
SPR_2FlushDrawPrty(void)
{
    SprSpCmd       *spCmd;

    /** BEGIN ***************************************************************/
#if(USE_DMA_LEVEL == 1 || USE_DMA_LEVEL == 3)
    if(bCmdNo) {
        spCmd = SpCmdTbl + bufTopCmdNo;
        DMA_ScuMemCopy(spCmd, cmdBuf, sizeof(SprSpCmd)*bCmdNo);
        DMA_ScuResult();
    }
#endif
    if(SpCmdChainBlkNo == 0) return;
    SPR_2FlushDrawPrtyBlock();
    crCmdTbl    = SpCmdTbl + SpCmdNo;
#if(USE_DMA_LEVEL == 1 || USE_DMA_LEVEL == 3)
    bCmdNo      = 0;
    bufTopCmdNo = SpCmdNo;
    crCmdBuf    = cmdBuf;
#endif    
}


/*****************************************************************************
 *
 * NAME:  SPR_2LocalCoord()  - Set Local Coord
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) XyInt   xy         - <i> ���[�J�����W�̑��΍��W�@
 *
 * DESCRIPTION:
 *
 *     ���[�J�����W�̃Z�b�g
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
SPR_2LocalCoord(Sint32 drawPrty, XyInt *xy)
{
    SprSpCmd  *spCmd;
    Uint16    *w;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2LOCALCOORD", "- COMMAND OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT | FUNC_LCOORD;
    w = (Uint16*)xy;
    SpLCoordX = spCmd->ax = *w++;
    SpLCoordY = spCmd->ay = *w;
}


/*****************************************************************************
 *
 * NAME:  SPR_2SysClip()  - Set System Cliping Area
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) XyInt   xy         - <i> �E�����W
 *
 * DESCRIPTION:
 *
 *     �V�X�e���N���b�s���O�G���A�̃Z�b�g
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
SPR_2SysClip(Sint32 drawPrty, XyInt *xy)
{
    SprSpCmd  *spCmd;
    Uint16    *w;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2SYSCLIP", "- COMMAND OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT  | FUNC_SCLIP;
    w = (Uint16*)xy;
    spCmd->cx = *w++;
    spCmd->cy = *w;
}


/*****************************************************************************
 *
 * NAME:  SPR_2UserClip()  - Set User Cliping Area
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty    - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) XyInt   xy[2]       - <i>  xy[0] = ������W
 *                                    xy[1] = �E�����W
 *
 * DESCRIPTION:
 *
 *     ���[�U�[�N���b�s���O�G���A�̃Z�b�g
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
SPR_2UserClip(Sint32 drawPrty, XyInt xy[2])
{
    SprSpCmd  *spCmd;
    Uint16 *w;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2USERCLIP", "- COMMAND OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT  | FUNC_UCLIP;
    w = (Uint16*)xy;
    spCmd->ax = *w++;
    spCmd->ay = *w++;
    spCmd->cx = *w++;
    spCmd->cy = *w;
}


/*****************************************************************************
 *
 * NAME:  SPR_2Line()  - Draw Line
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  drawMode   - <i> �`�惂�[�h
 *     (3) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (4) XyInt   xy[2]      - <i> ���C���̂Q�_
 *     (5) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     ���C���̕`��
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
SPR_2Line(Sint32 drawPrty,
          Uint16 drawMode, Uint16 color, XyInt xy[2], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;
    Sint16 *s, *d;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2LINE", "- COMMAND OVER -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2LINE", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT | FUNC_LINE;
    SetDrawPara(spCmd);
    s = (Sint16*)xy;
    d = (Sint16*)&spCmd->ax;
    *d++ = *s++;
    *d++ = *s++;
    *d++ = *s++;
    *d   = *s  ;
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2PolyLine()  - Draw Polyline
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  drawMode   - <i> �`�惂�[�h
 *     (3) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (4) XyInt   xy[4]      - <i> �|�����C���̂S�_
 *     (5) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �|�����C���̕`��
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
SPR_2PolyLine(Sint32 drawPrty, 
              Uint16 drawMode, Uint16 color, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2POLYLINE", "- COMMAND OVER -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2POLYLINE", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT | FUNC_POLYLINE;
    SetDrawPara(spCmd);
    memcpy(&spCmd->ax, xy, sizeof(XyInt)*4);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2Polygon()  - Draw Polygon
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  drawMode   - <i> �`�惂�[�h
 *     (3) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (4) XyInt   xy[4]      - <i> �|���S���̂S�_
 *     (5) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �|���S���̕`��
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
SPR_2Polygon(Sint32 drawPrty, 
             Uint16 drawMode, Uint16 color, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2POLYGON", "- COMMAND OVER -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2POLYGON", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON;
    SetDrawPara(spCmd);
    memcpy(&spCmd->ax, xy, sizeof(XyInt)*4);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2NormSpr()  - Draw Normal Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  dir        - <i> �L�����N�^���]�w��
 *     (3) Uint16  drawMode   - <i> �`�惂�[�h
 *     (4) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (5) Uint16  charNo     - <i> �L�����N�^�ԍ�
 *     (6) XyInt   xy�@�@     - <i> ������W
 *     (7) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �m�[�}���X�v���C�g�̕`��
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
SPR_2NormSpr(Sint32 drawPrty, Uint16 dir, Uint16 drawMode, Uint16 color, 
                  Uint16 charNo, XyInt *xy, Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2NORMSPR", "- COMMAND OVER -", 0);
    if(charNo >= SpCharMax)
        dispError("SP2NORMSPR", "- CHAR NO. OVER -", 0);
    if(SpCharTbl[charNo].addr == 0)
        dispError("SP2NORMSPR", "- CHAR NO SET -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2NORMSPR", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control =  ((JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_NORMALSP)
                        & ~CTRL_DIR) | dir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
    spCmd->ax = xy->x;
    spCmd->ay = xy->y;
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2ScaleSpr()  - Draw Scale Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  zoomDir    - <i> �Y�[���ƃL�����N�^���]�w��
 *     (3) Uint16  drawMode   - <i> �`�惂�[�h
 *     (4) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (5) Uint16  charNo     - <i> �L�����N�^�ԍ�
 *     (6) XyInt   xy[2]      - <i> �Q���_���W�A�܂��͕s���_���W�E�\����
 *     (7) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �X�P�[���X�v���C�g�̕`��
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
SPR_2ScaleSpr(Sint32 drawPrty, Uint16 zoomDir, Uint16 drawMode,
              Uint16 color, Uint16 charNo, XyInt xy[2], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;
    Sint16 *w;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2SCALESPR", "- COMMAND OVER -", 0);
    if(charNo >= SpCharMax)
        dispError("SP2SCALESPR", "- CHAR NO. OVER -", 0);
    if(SpCharTbl[charNo].addr == 0)
        dispError("SP2SCALESPR", "- CHAR NO SET -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2SCALESPR", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control =  ((JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP)
                       & ~CTRL_ZOOM & ~CTRL_DIR ) | zoomDir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
    w = (Sint16*)xy;
    if(zoomDir & CTRL_ZOOM) {
        spCmd->ax = *w++;
        spCmd->ay = *w++;
        spCmd->bx = *w++;
        spCmd->by = *w;
    } else {
        spCmd->ax = *w++;
        spCmd->ay = *w++;
        spCmd->cx = *w++;
        spCmd->cy = *w;
    }
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2DistSpr()  - Draw Distorted Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) Uint16  dir        - <i> �L�����N�^���]�w��
 *     (3) Uint16  drawMode   - <i> �`�惂�[�h
 *     (4) Uint16  color      - <i> �J���[�R�[�h�܂��̓��b�N�A�b�v�e�[�u���ԍ�
 *     (5) Uint16  charNo     - <i> �L�����N�^�ԍ�
 *     (6) XyInt   xy[4]      - <i> �S���_���W
 *     (7) Uint16�@gourTblNo  - <i> �O�[���[�e�[�u���ԍ�
 *
 * DESCRIPTION:
 *
 *     �ό`�X�v���C�g�̕`��
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
SPR_2DistSpr(Sint32 drawPrty, Uint16 dir, Uint16 drawMode, Uint16 color,
                       Uint16 charNo, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2DISTSPR", "- COMMAND OVER -", 0);
    if(charNo >= SpCharMax)
        dispError("SP2DISTSPR", "- CHAR NO. OVER -", 0);
    if(SpCharTbl[charNo].addr == 0)
        dispError("SP2DISTSPR", "- CHAR NO SET -", 0);
    if((gourTblNo != NO_GOUR) && (gourTblNo >= vramInfo.gourTblCnt))
        dispError("SP2DISTSPR", "- GOURAUD TABLE OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    spCmd->control = ((JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP)
                      & ~CTRL_DIR) | dir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
    memcpy(&spCmd->ax, xy, sizeof(XyInt)*4);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2Cmd()  - Set Sprite Command
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty    - <i>  �R�}���h�`��v���C�I���e�B�ԍ�
 *     (2) SprSpCmd  *spCmd    - <i> �R�Q�o�C�g�̃X�v���C�g�R�}���h
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g�R�}���h���u�q�`�l�ɃZ�b�g
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
SPR_2Cmd(Sint32 drawPrty, SprSpCmd *pSpCmd)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
#ifdef SP2_DEBUG
    if(SpCmdNo >= vramInfo.cmdTblCnt)
        dispError("SP2CMD", "- COMMAND OVER -", 0);
#endif
    GetSpCmdArea(spCmd);
    memcpy(spCmd, pSpCmd, sizeof(SprSpCmd));
}


/*****************************************************************************
 *
 * NAME:  allocBlock()  - Allocate Block in The VRAM Area
 *
 * PARAMETERS :
 *
 *     (1) Uint16  size    - <i>  �u���b�N�T�C�Y�i�R�Q�o�C�g���P�P�ʂƂ���j
 *
 * DESCRIPTION:
 *
 *     �u�q�`�l���̃u���b�N�v�[���G���A����w��T�C�Y�̃u���b�N���l������
 *
 * POSTCONDITIONS:
 *
 *     �u�q�`�l���̊l���u���b�N�G���A�ʒu�𑊑΃A�h���X�^�W�ŕԂ�
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static Vaddr
allocBlock(Uint16 size)
{
    NullBlock  *nullBlock, *minNullBlock;
    NullBlock  *befNullBlock, *befMinNullBlock;
    Uint16     minSize;
    Uint32     w;
    Vaddr      addr;

    /** BEGIN ***************************************************************/
    nullBlock = nullBlockTop;
    addr = 0;
    minSize = 0xffff;
    minNullBlock = 0;
    befNullBlock = 0;
    while(nullBlock) {
        if(size <= nullBlock->size)
            if(minSize > nullBlock->size) {
                minSize      = nullBlock->size;
                minNullBlock = nullBlock;
                befMinNullBlock = befNullBlock;
            }
        befNullBlock = nullBlock;
        nullBlock = nullBlock->next;
    }
    if(minNullBlock) {
        minNullBlock->size -= size;
        w  = (Uint32)minNullBlock + (Uint32)minNullBlock->size * 32;
        w -= (Uint32)VRAM;
        addr = w >> 3;
        if(minNullBlock->size == 0)
            if(befMinNullBlock)  befMinNullBlock->next = minNullBlock->next;
    }
    return addr;
}


/*****************************************************************************
 *
 * NAME:  freeBlock()  - Free Block in The VRAM Area
 *
 * PARAMETERS :
 *
 *     (1) Vaddr  addr    - <i>  �u���b�N�A�h���X
 *     (2) Uint16 size    - <i>  �u���b�N�T�C�Y�i�R�Q�o�C�g���P�P�ʂƂ���j
 *
 * DESCRIPTION:
 *
 *     �u�q�`�l�u���b�N�v�[�����̎w��u���b�N�G���A���������
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
freeBlock(Vaddr addr, Uint16 size)
{
    NullBlock  *nullBlock;
    NullBlock  *blockTop, *blockNext;
    NullBlock  *vramTop, *vramBot;
    NullBlock  *pt1, *pt2, *pt1Next;
    Uint32     w;

    /** BEGIN ***************************************************************/
    blockTop  = (NullBlock*)((Uint32)VRAM     + ((Uint32)addr)*8);
    blockNext = (NullBlock*)((Uint32)blockTop + ((Uint32)size)*32);
    
    vramTop = (NullBlock*)VRAM;
    vramBot = (NullBlock*)((Uint32)VRAM       + (Uint32)VRAM_GRAN_MAX*32);
    pt1 = vramTop;
    pt2 = nullBlockTop;
    while(1) {
        if((pt1 <= blockTop) && (blockTop < pt2)) {
            if(pt1 == vramTop) {
                if(pt2 == blockNext) {
                    blockTop->next = pt2->next;
                    blockTop->size = pt2->size + size;
                    nullBlockTop   = blockTop;
                } else {
                    blockTop->next = nullBlockTop;
                    blockTop->size = size;
                    nullBlockTop   = blockTop;
                }
            } else
            if(pt2 == vramBot) {
                pt1Next = (NullBlock*)((Uint32)pt1 + ((Uint32)(pt1->size))*32);
                if(pt1Next == blockTop) {
                    pt1->size += size;
                } else {
                    pt1->next      = blockTop;
                    blockTop->next = 0;
                    blockTop->size = size;
                } 
            } else {
                pt1Next = (NullBlock*)((Uint32)pt1 + ((Uint32)(pt1->size))*32);
                if((pt1Next == blockTop) && (blockNext <  pt2)) {
                    pt1->size += size;
                } else
                if((pt1Next <  blockTop) && (blockNext == pt2)) {
                    blockTop->next = pt2->next;
                    blockTop->size = pt2->size + size;
                    pt1->next      = blockTop;
                } else
                if((pt1Next <  blockTop) && (blockNext <  pt2)) {
                    blockTop->next = pt2;
                    blockTop->size = size;
                    pt1->next      = blockTop;
                } else {
                    pt1->next  = pt2->next;
                    pt1->size += size;
                    pt1->size += pt2->size;
                }
            }
            break;
        }
        if(pt2 == vramBot) break;
        pt1 = pt2;
        pt2 = pt2->next;
        if(pt2 == 0)  pt2 = vramBot;
    }

#if 0
    nullBlock = nullBlockTop;
    xprintf("\n");
    while(nullBlock) {
       xprintf("(%lxH,%d)->",nullBlock,nullBlock->size);
       nullBlock = nullBlock->next;
    }
#endif
}


#ifdef SP2_DEBUG
/*****************************************************************************
 *
 * NAME:  dispError()  - Display Error Message to Screen
 *
 * PARAMETERS :
 *
 *     (1) char  *rtnName - <i>  ���[�`����
 *     (2) char  *msg1    - <i>  �G���[���b�Z�[�W�P
 *     (3) char  *msg2    - <i>  �G���[���b�Z�[�W�Q
 *
 * DESCRIPTION:
 *
 *     �X�v���C�g��ʂɃG���[���b�Z�[�W��\�����Ė������[�v�ɓ���
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
dispError(char *rtnName, char *msg1, char *msg2)
{
    Uint8   *w;
    XyInt   xy[4];
    Uint16  i, sw;
    char    *msg;

    /** BEGIN ***************************************************************/
    PriorityInit();
    SetPriorityNum(0xf300);
    SetColorMode(0);          /* 16 bit mode */
    SetMapColor(1,  0, 0, 0);
    SetMapColor(2, 31, 0, 0);

    SPR_2Initial(256,0,0);

    w = Font8x8;
    for(i=0; i<0x40; i++) {
        SPR_2SetChar(i, COLOR_2,8,8,w);
        w += 64;
    }

    SPR_2FrameEraseData(RGB16_COLOR(0,0,0));
    SPR_2FrameChgIntr(10);

    i  = 0;
    sw = 0;
    for(;;) {
        SPR_2OpenCommand();

        xy[0].x = SpScreenX-1;
        xy[0].y = SpScreenY-1;
        SPR_2SysClip(xy);

        xy[0].x = 0;
        xy[0].y = 0;
        SPR_2LocalCoord(xy);
        
	if(++i > 4) {
	    sw ^= 1;
	    i = 0;
        }

        if(sw) {
            xy[0].x = 36;
            xy[0].y = 48;
            xy[1].x = 36 + 48 - 1;
            xy[1].y = 48 + 48 - 1;
            for(msg  = "ERROR!"; *msg != 0; msg++) {
                SPR_2ScaleSpr( ZOOM_NOPOINT | DIR_NOREV,
                             ECDSPD_DISABLE | COMPO_REP,
                             0, *msg - 0x20, xy, NO_GOUR);
                xy[0].x += 48;
                xy[1].x += 48;
            }
	}

        if(rtnName) {
            xy[0].x = 8;
            xy[0].y = 120;
            for(msg  = rtnName; *msg != 0; msg++) {
                SPR_2NormSpr( DIR_NOREV, ECDSPD_DISABLE | COMPO_REP,
                            0, *msg - 0x20, xy, NO_GOUR);
                xy[0].x += 8;
            }
        }

        if(msg1) {
            xy[0].x = 8;
            xy[0].y = 136;
            for(msg  = msg1; *msg != 0; msg++) {
                SPR_2NormSpr( DIR_NOREV, ECDSPD_DISABLE | COMPO_REP,
                            0, *msg - 0x20, xy, NO_GOUR);
                xy[0].x += 8;
            }
        }

        if(msg2) {
            xy[0].x = 8;
            xy[0].y = 152;
            for(msg  = msg2; *msg != 0; msg++) {
                SPR_2NormSpr( DIR_NOREV, ECDSPD_DISABLE | COMPO_REP,
                            0, *msg - 0x20, xy, NO_GOUR);
                xy[0].x += 8;
            }
        }

        SPR_2CloseCommand();
        VBL_VdpDisplayFrame();
    }
}
#endif

/*  end of file */
