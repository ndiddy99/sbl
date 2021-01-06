/*----------------------------------------------------------------------------
 *  spr_3c.c -- SPR ���C�u���� 3C ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  ���̃��C�u�����̓X�v���C�g�R�c�\���������W���[���ŁA�ȉ��̃��[�`�����܂ށB
 *
 *  SPR_3Initial            -  �R�c�X�v���C�g�\���̏�����
 *  SPR_3SetLight           -  �����x�N�^�̃Z�b�g
 *  SPR_3SetView            -  ���_�̃Z�b�g
 *  SPR_3CallAllCluster     -  �S�N���X�^�ɑ΂���t�@���N�V�����R�[��
 *  SPR_3MoveCluster        -  �N���X�^�̈ړ�
 *  SPR_3DrawModel          -  ���f���̓o�^
 *  SPR_3Flush              -  ���f���̕`��
 *  SPR_3SetTexture         -  �e�N�X�`���̃Z�b�g
 *  SPR_3ChangeTexColor     -  �e�N�X�`���J���[�f�[�^�̕ύX
 *  SPR_3ClrTexture         -  �e�N�X�`���G���A�̉��
 *  SPR_3SetClipLevel       -  �N���b�s���O���x���̃Z�b�g
 *  SPR_3SetZSortMinMax     -  ���_���W�n�y�\�[�g�͈͂̍ŏ��E�ő�l���Z�b�g
 *  SPR_3SetPixelCount      -  �P�ʃs�N�Z�����̃Z�b�g
 *  SPR_3SetTransPostRtn    -  ���W�ϊ����ʒʒm���[�`���̃Z�b�g
 *  SPR_3GetStatus          -  ���R�c���f�[�^�̎擾
 *  SPR_3SetSurfNormVect    -  �R�c�I�u�W�F�N�g�̖ʖ@�����v�Z���Z�b�g
 *  SPR_3SetDrawSpeed       -  �u�c�o�P�̍����`��p�����[�^���Z�b�g
 *
 *  ���̃��C�u�������g�p����ɂ͎��̃C���N���[�h�t�@�C���ƃ��[�N�G���A���`��
 *  ��K�v������B
 *
 *   #include "sega_scl.h"
 *   #define  _SPR3_
 *   #include "sega_spr.h"
 *
 *   #define CommandMax    100          // �ő�R�}���h��
 *   #define GourTblMax    100          // �ő�O�[���[�e�[�u����
 *   #define LookupTblMax  100          // �ő僋�b�N�A�b�v�e�[�u����
 *   #define CharMax       100          // �ő�L�����N�^��
 *   #define DrawPrtyMax   256          // �ő�v���C�I���e�B�u���b�N��
 *   SPR_2DefineWork(work2D,
 *                CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)
 *                                      // �Q�c���[�N�G���A��`
 *
 *   #define ObjSurfMax     16          // �R�c�I�u�W�F�N�g�̍ő�ʐ�
 *   #define ObjVertMax     16          // �R�c�I�u�W�F�N�g�̍ő咸�_��
 *   SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)
 *                                      // �R�c���[�N�G���A��`
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */

#include <string.h>

#define  _SPR3_
#include "sega_spr.h"

#ifdef _SH
#include <machine.h>
#endif

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define   SCREEN_W_SIZE     256
#define   SCREEN_H_SIZE     256

#define   FRMBUF_MIN_X     -1000
#define   FRMBUF_MAX_X      1000
#define   FRMBUF_MIN_Y     -1000
#define   FRMBUF_MAX_Y      1000

#define   FIXED_MIN        0x80000000
#define   FIXED_MAX        0x7fffffff
#define   CLIP_MIN_Z       FIXED_MIN
#define   CLIP_MAX_Z       MTH_FIXED(-0.005)
#define   ZSORT_MIN_Z      MTH_FIXED(-10.0)
#define   ZSORT_MAX_Z      MTH_FIXED(0)

#define	WORLD_MATRIX_STACK_SIZE	32

/*
 * DATA TYPE DECLARATIONS
 */
typedef struct PolTbl  PolTbl;
struct PolTbl {
    Uint16     dispFlag;        /* �\���t���O                                */
    Uint16     surfBright;      /* �ʋP�x                                    */
    Uint16     drawMode;        /* �`�惂�[�h                                */
    Uint16     color;           /* �J���[�f�[�^                              */
    Uint16     polyCnt;         /* �Q�c�|���S����                            */
    XyInt      xy[16];          /* �Q�c�|���S�����W                          */
    SprGourTbl gourTbl;         /* �O�[���[�e�[�u��                          */
    Fixed32    z;               /* �`��|���S���y                            */
};
typedef struct DoubleBufArea  DoubleBufArea;
struct DoubleBufArea {
    SprCluster  *cluster;       /* �N���X�^                                  */
    SprObject3D *object;        /* �R�c�I�u�W�F�N�g                          */
    Sint32      *surfBright;    /* �ʋP�x�e�[�u��                            */
    Sint32      *vertBright;    /* ���_�P�x�e�[�u��                          */
    MthXyz      *coordView3D;   /* �R�c���_���W�ϊ���e�[�u��                */
    MthXyz      *coordWorld3D;  /* �R�c���[���h���W�ϊ���e�[�u��            */
};

/*
 * GLOBAL DECLARATIONS
 */
extern Sint32      Sp2OpenFlag;             /* Open Command Exec Flag        */
extern SprCharTbl *SpCharTbl;               /* Character Table Pointer       */
extern Sint32      SpCharMax;               /* Character Table Entry Count   */
extern Sint32      SpDrawPrtyBlkMax;        /* Draw Priority Block Max Count */

/*
 * STATIC DECLARATIONS
 */
static MthMatrix    viewMatrix;
static MthMatrix    lightMatrix;
static MthMatrix    worldMatrix [WORLD_MATRIX_STACK_SIZE];
static MthMatrixTbl viewMatrixTbl;
static MthMatrixTbl lightMatrixTbl;
static MthMatrixTbl worldMatrixTbl;

static MthViewLight      viewLight;
static MthPolyTransParm  polyTransParm;

static DoubleBufArea dBuf[2];      /* ���W�ϊ��_�u���o�b�t�@�G���A�Ǘ�       */
static DoubleBufArea *cBuf;        /* �J�����g���W�ϊ��o�b�t�@               */
static Sint32    doubleBufFlag;    /* �_�u���o�b�t�@�L��t���O               */
static Sint32    bufSW;            /* �J�����g�_�u���o�b�t�@�r�v  0 or 1     */
static Sint32    dbFirstFlag;      /* �_�u���o�b�t�@����t�@�[�X�g�t���O     */
static Sint32    firstDrawFlag;    /* �t���b�V����ŏ��̃��f���`��t���O     */
static MthXy     unitPixel;        /* X,Y 1.0 �ɑ΂���X�N���[���̃s�N�Z���� */
static MthXyz    lightAngle;       /* �����A���O��                           */
static MthXyz	 worldLightVector; /* ���[���h���W�n�ł̌����x�N�g��         */
static MthXyz    viewCoordPoint;   /* ���_���W�n���̎��_�ʒu                 */
static MthXyz    viewPoint;        /* ���_���W                               */
static MthXyz    viewAngle;        /* ���_�A���O��                           */
static Sint32	 viewAngleSeq;     /* ���_�A���O����]����                   */
static XyInt     *coord2D;         /* �Q�c�����ϊ���e�[�u��                 */
static Uint8     *coord2DFlag;     /* �Q�c�����ϊ������t���O�e�[�u��         */
static Sint32    gourTblNo;        /* �t���b�V���P�ʃO�[���[�e�[�u���J�E���^ */
static Fixed32   zSortZMinOrg;     /* �t���b�V���P�ʌ��y���W�ŏ��l           */
static Fixed32   zSortZMaxOrg;     /* �t���b�V���P�ʌ��y���W�ő�l           */
static Fixed32   zSortZMin;        /* �t���b�V���P�ʌ��y���W�ŏ��l           */
static Fixed32   zSortZMax;        /* �t���b�V���P�ʌ��y���W�ő�l           */
static Fixed32   zSortBZMin;       /* �t���b�V���P�ʑO�y���W�ŏ��l           */
static Fixed32   zSortBZMax;       /* �t���b�V���P�ʑO�y���W�ő�l           */
static Uint16    zSortBlkCnt;      /* �y�\�[�g�u���b�N��                     */
static Sint32    zSortMode;        /* �y�\�[�g�y���W�l�̗̍p���[�h           */
static int       blkSftCnt;        /* �y�\�[�g�u���b�N�n�b�V���p�V�t�g��     */
static int       zSftCnt;          /* �y�\�[�g�y�V�t�g��                     */
static Sint16    clipScrXMin;      /* �X�N���[�����E�w�ŏ��l���W             */
static Sint16    clipScrXMax;      /* �X�N���[�����E�w�ő�l���W             */
static Sint16    clipScrYMin;      /* �X�N���[�����E�x�ŏ��l���W             */
static Sint16    clipScrYMax;      /* �X�N���[�����E�x�ő�l���W             */
static Fixed32   clipZMin;         /* ���_���W�n�N���b�v�y�ŏ��l���W         */
static Fixed32   clipZMax;         /* ���_���W�n�N���b�v�y�ő�l���W         */
static Sint32    clipLevel;        /* �N���b�s���O���x��                     */
				   /*   �O���N���b�s���O�Ȃ�                 */
				   /*   �P�����_���W�n�̂y�͈͂ŃN���b�v     */
                                   /*       �i���W�ϊ���j                   */
                                   /*   �Q���X�N���[���Ɋ|����Ȃ��|���S���� */
				   /*       �폜�i�����ϊ���j               */
                                   /*   �R���t���[���o�b�t�@�̋��E�ŃN���b�v */
                                   /*       �i�����ϊ���j                   */
static Uint16   postWorldDataFlag; /* ���[���h���W�ʒm�v�t���O               */
static Sint32   spriteDrawFlag;    /* �ό`�X�v���C�g�`��t���O               */
static Sint32   otherDrawFlag;     /* ���̑��|���S���`��t���O               */

static	MthXyz	orgLightVect =
	    { MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(-1.0)};


static void   initDraw(void);
static void   drawPol(PolTbl *polTbl);
static void   transCluster(SprCluster *cluster);
static void   setInbetPolygons(DoubleBufArea *buf);
static int    clip2DLevel2(PolTbl *polTbl);
static Sint32 clipScrCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt);
static Sint32 outcodeScr(XyInt *xy1, XyInt *xy2);
static int    clip2DLevel3(PolTbl *polTbl);
static Sint32 clipFrmCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt);
static Sint32 outcodeFrm(XyInt *xy1, XyInt *xy2);
static int    clipFrame(PolTbl *polTbl);
static int    frmCrossChk(int frmNo, XyInt *p1, XyInt *p2, XyInt *cp);
static void   transPolygons(DoubleBufArea *buf, XyInt *vert2d);
static void   viewCoordMatrix(void);
static void   setLightAngle(void);
extern Uint16 SPR_3GetShdColor(Uint16 baseRgbColor, Uint16 bright);



/*****************************************************************************
 *
 * NAME:  SPR_3Initial()  - Initial 3D Sprite Display Environment
 *
 * PARAMETERS :
 *
 * �@�@(1) Spr3WorkArea *workArea - <i> �R�c�X�v���C�g���[�N�G���A��`
 *
 * DESCRIPTION:
 *
 *     �R�c�X�v���C�g�\���̏���������
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
SPR_3Initial(Spr3WorkArea  *workArea)
{
    MthXyz  w0,w1;

    /** BEGIN ***************************************************************/
    SPR_3SetDrawSpeed(0, 0, 0);

    dBuf[0].surfBright   = workArea->surfBright[0];
    dBuf[1].surfBright   = workArea->surfBright[1];
    dBuf[0].vertBright   = workArea->vertBright[0];
    dBuf[1].vertBright   = workArea->vertBright[1];
    dBuf[0].coordView3D  = workArea->coordView3D[0];
    dBuf[1].coordView3D  = workArea->coordView3D[1];
    dBuf[0].coordWorld3D = workArea->coordWorld3D[0];
    dBuf[1].coordWorld3D = workArea->coordWorld3D[1];
    if(workArea->surfBright[1])
        doubleBufFlag = 1;
    else
        doubleBufFlag = 0;
    if(workArea->coordWorld3D[0])
        postWorldDataFlag = 1;
    else
        postWorldDataFlag = 0;

    if(!dBuf[1].surfBright)   dBuf[1].surfBright   = dBuf[0].surfBright;
    if(!dBuf[1].vertBright)   dBuf[1].vertBright   = dBuf[0].vertBright;
    if(!dBuf[1].coordView3D)  dBuf[1].coordView3D  = dBuf[0].coordView3D;
    if(!dBuf[1].coordWorld3D) dBuf[1].coordWorld3D = dBuf[0].coordWorld3D;

    coord2D      = workArea->coord2D;
    coord2DFlag  = workArea->coord2DFlag;

    zSortBlkCnt = SpDrawPrtyBlkMax;
    for(blkSftCnt=0; blkSftCnt<16; blkSftCnt++)
       if((zSortBlkCnt<<=1) & 0x8000) break;
    zSortBlkCnt = SpDrawPrtyBlkMax;
    zSortZMin = ZSORT_MIN_Z;
    zSortZMax = ZSORT_MAX_Z;
    zSortMode = ZSORT_FLOAT_MODE;

    clipLevel = 2;
    clipZMin  = CLIP_MIN_Z;
    clipZMax  = CLIP_MAX_Z;

    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    SPR_3SetLight(1, &w0);
    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    w1.x = MTH_FIXED(0.0);   w1.y = MTH_FIXED(0.0);     w1.z = MTH_FIXED(0.0);
    SPR_3SetView(1, &w0, &w1, ROT_SEQ_XYZ, &w0);

    firstDrawFlag = 1;
    MTH_InitialMatrix(&worldMatrixTbl, WORLD_MATRIX_STACK_SIZE,	worldMatrix);
    MTH_PolyDataTransInit();
    SPR_3SetPixelCount(SCREEN_W_SIZE, SCREEN_H_SIZE);
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetLight()  - Set Light Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16      moveKind  - <i> �ړ��ʂ̎�ʁ[�u���|�C���^
 *                       �@�@�@�@�@    0 = ���Έړ��A1 = ��Έړ��^
 *     (2) MthXyz *pLightAngler  - <i>  ���s�����̃A���O��
 *
 * DESCRIPTION:
 *
 *     �����A���O���Z�b�g
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
SPR_3SetLight(Uint16 moveKind, MthXyz *pLightAngle)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    if(moveKind & 0x0002) {
        angleX = MTH_Atan(pLightAngle->y, pLightAngle->z);
        angleY = MTH_Atan(pLightAngle->z, pLightAngle->x);
        angleZ = MTH_Atan(pLightAngle->y, pLightAngle->x);
    } else  {
        angleX = pLightAngle->x;
        angleY = pLightAngle->y;
        angleZ = pLightAngle->z;
    }
    if(moveKind & 0x0001) {
	lightAngle.x  = angleX;
	lightAngle.y  = angleY;
	lightAngle.z  = angleZ;
    } else       {
	lightAngle.x  += angleX;
	lightAngle.y  += angleY;
	lightAngle.z  += angleZ;
    }
    if(lightAngle.x > MTH_FIXED( 180)) lightAngle.x -= MTH_FIXED(360);  else
    if(lightAngle.x < MTH_FIXED(-180)) lightAngle.x += MTH_FIXED(360);
    if(lightAngle.y > MTH_FIXED( 180)) lightAngle.y -= MTH_FIXED(360);  else
    if(lightAngle.y < MTH_FIXED(-180)) lightAngle.y += MTH_FIXED(360);
    if(lightAngle.z > MTH_FIXED( 180)) lightAngle.z -= MTH_FIXED(360);  else
    if(lightAngle.z < MTH_FIXED(-180)) lightAngle.z += MTH_FIXED(360);
    setLightAngle();
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetView()  - Set View Point and Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16  moveKind         - <i> �ړ��ʂ̎�ʁ[�u���|�C���^
 *                       �@�@�@�@�@       0 = ���Έړ��A1 = ��Έړ��^
 *     (2) MthXyz *pViewPoint       - <i> ���_���W
 *     (3) MthXyz *pViewAngle       - <i> ���_�̃A���O���i�x�j
 *     (4) Uint16  pViewAngleSeq    - <i> �A���O���̉�]����
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_ZYX
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_ZXY
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_YZX
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_YXZ
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_XYZ
 *�@�@�@�@�@�@�@�@�@�@�@    �@            ROT_SEQ_XZY
 *     (5) MthXyz *pViewCoordPoint  - <i> ���_���W�n���̎��_�ʒu
 *
 * DESCRIPTION:
 *
 *     ���_�̃Z�b�g
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
SPR_3SetView(Uint16 moveKind, MthXyz *pViewPoint, MthXyz *pViewAngle,
                              Uint16 pViewAngleSeq, MthXyz *pViewCoordPoint)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    viewAngleSeq = pViewAngleSeq;

    if(pViewCoordPoint) {
        if(moveKind & 0x0001) {
	    viewCoordPoint.x  = pViewCoordPoint->x;
	    viewCoordPoint.y  = pViewCoordPoint->y;
	    viewCoordPoint.z  = pViewCoordPoint->z;
        } else       {
	    viewCoordPoint.x += pViewCoordPoint->x;
	    viewCoordPoint.y += pViewCoordPoint->y;
	    viewCoordPoint.z += pViewCoordPoint->z;
        }
    }

    if(pViewPoint) {
        if(moveKind & 0x0001) {
	    viewPoint.x  = pViewPoint->x;
	    viewPoint.y  = pViewPoint->y;
	    viewPoint.z  = pViewPoint->z;
        } else       {
	    viewPoint.x += pViewPoint->x;
	    viewPoint.y += pViewPoint->y;
	    viewPoint.z += pViewPoint->z;
        }
    }

    if(pViewAngle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(pViewAngle->y, pViewAngle->z);
            angleY = MTH_Atan(pViewAngle->z, pViewAngle->x);
            angleZ = MTH_Atan(pViewAngle->y, pViewAngle->x);
        } else  {
            angleX = pViewAngle->x;
            angleY = pViewAngle->y;
            angleZ = pViewAngle->z;
        }
        if(moveKind & 0x0001) {
	    viewAngle.x  = angleX;
	    viewAngle.y  = angleY;
	    viewAngle.z  = angleZ;
        } else       {
	    viewAngle.x += angleX;
	    viewAngle.y += angleY;
	    viewAngle.z += angleZ;
        }
        if(viewAngle.x > MTH_FIXED( 180)) viewAngle.x -= MTH_FIXED(360);  else
        if(viewAngle.x < MTH_FIXED(-180)) viewAngle.x += MTH_FIXED(360);
        if(viewAngle.y > MTH_FIXED( 180)) viewAngle.y -= MTH_FIXED(360);  else
        if(viewAngle.y < MTH_FIXED(-180)) viewAngle.y += MTH_FIXED(360);
        if(viewAngle.z > MTH_FIXED( 180)) viewAngle.z -= MTH_FIXED(360);  else
        if(viewAngle.z < MTH_FIXED(-180)) viewAngle.z += MTH_FIXED(360);
    }

}


/*****************************************************************************
 *
 * NAME:  SPR_3CallAllCluster()  - Call User Function with all Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster           - <i> ���[�g�N���X�^�e�[�u���̃|�C���^
 *     (2) void (*userFunc)(SprCluster*) - <i> ���[�U�t�@���N�V����
 *
 * DESCRIPTION:
 *
 *     ���[�g�N���X�^�ɂȂ���S�N���X�^���p�����[�^�Ƃ��Ďw��t�@���N�V����
 *     ���Ăяo��
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3CallAllCluster(SprCluster *cluster, void (*userFunc)(SprCluster*))
{
    /** BEGIN ***************************************************************/
    while(cluster) {
        userFunc(cluster);
	/* �q�N���X�^�̏��� */
	if(cluster->child) SPR_3CallAllCluster(cluster->child, userFunc);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3MoveCluster()  - Move Specify Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster   - <i> �ړ����ׂ��N���X�^�e�[�u���̃|�C���^
 *     (2) Uint16      moveKind  - <i> �ړ��ʂ̎�ʁ[�u���|�C���^
 *                       �@�@�@�@�@    0 = ���Έړ��A1 = ��Έړ��^
 *     (3) MthXyz     *angle     - <i> ��]�ړ��ʁi�x�j
 *     (4) MthXyz     *point     - <i> ���s�ړ��ʃe�[�u���|�C���^
 *
 * DESCRIPTION:
 *
 *     �N���X�^�̈ړ�
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
SPR_3MoveCluster(SprCluster *cluster, Uint16 moveKind,
				      MthXyz *angle, MthXyz *point)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    if(angle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(angle->y, angle->z);
            angleY = MTH_Atan(angle->z, angle->x);
            angleZ = MTH_Atan(angle->y, angle->x);
        } else  {
	    angleX = angle->x;
	    angleY = angle->y;
	    angleZ = angle->z;
        }
        if(moveKind & 0x0001) {
   	    cluster->angle.x  = angleX;
	    cluster->angle.y  = angleY;
	    cluster->angle.z  = angleZ;
        } else       {
	    cluster->angle.x += angleX;
	    cluster->angle.y += angleY;
	    cluster->angle.z += angleZ;
        }
        if(cluster->angle.x > MTH_FIXED( 180))
            cluster->angle.x -= MTH_FIXED(360);
        else
        if(cluster->angle.x < MTH_FIXED(-180))
            cluster->angle.x += MTH_FIXED(360);
        if(cluster->angle.y > MTH_FIXED( 180))
            cluster->angle.y -= MTH_FIXED(360);
        else
        if(cluster->angle.y < MTH_FIXED(-180))
            cluster->angle.y += MTH_FIXED(360);
        if(cluster->angle.z > MTH_FIXED( 180))
            cluster->angle.z -= MTH_FIXED(360);
        else
        if(cluster->angle.z < MTH_FIXED(-180))
            cluster->angle.z += MTH_FIXED(360);
    }
    if(point)
        if(moveKind & 0x0001) {
	    cluster->point.x  = point->x;
	    cluster->point.y  = point->y;
	    cluster->point.z  = point->z;
        } else       {
	    cluster->point.x += point->x;
	    cluster->point.y += point->y;
	    cluster->point.z += point->z;
        }
}


/*****************************************************************************
 *
 * NAME:  SPR_3DrawModel()  - Set Specify Model
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *rootCluster  - <i>  �\�����ׂ����f���̃��[�g�N���X�^
 *                                         �e�[�u���̃|�C���^
 *
 * DESCRIPTION:
 *
 *     ���f���̓o�^
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
SPR_3DrawModel(SprCluster *rootCluster)
{
    /** BEGIN ***************************************************************/
    if(firstDrawFlag) {
	firstDrawFlag = 0;
	viewCoordMatrix();
	initDraw();
    }

    transCluster(rootCluster);
}


/*****************************************************************************
 *
 * NAME:  SPR_3Flush()  - Draw End All Seted Cluster
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �o�^���ꂽ�S���f���̕`��I������
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
SPR_3Flush(void)
{
    /** BEGIN ***************************************************************/
    if(doubleBufFlag) {
	cBuf = &dBuf[bufSW^1];
	transPolygons(cBuf, coord2D);
    }
    SPR_2FlushDrawPrty();
    firstDrawFlag = 1;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetTexture()  - Set Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  �e�N�X�`���e�[�u���̃|�C���^
 *
 * DESCRIPTION:
 *
 *     �e�N�X�`���̃Z�b�g
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
SPR_3SetTexture(SprTexture *texture)
{
    SprTexture   *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++) {
	SPR_2SetChar(wTex->charNo, wTex->colorMode, wTex->color,
                     wTex->width, wTex->height, wTex->charData);
	if(wTex->lookupTbl)
	    SPR_2SetLookupTbl(wTex->color, wTex->lookupTbl);
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3ChangeTexColor()  - Change Texture Color
 *
 * PARAMETERS :
 *
 *     (1) Uint16      charNo    - <i>  �L�����N�^�ԍ�
 *     (2) Uint16      color     - <i>  �J���[�f�[�^�i�װ��ݸorٯ�����ð���No�j
 *     (2) SprLookupTbl *lookupTbl   - <i>  ���b�N�A�b�v�e�[�u���|�C���^
 *
 * DESCRIPTION:
 *
 *     �e�N�X�`���J���[�f�[�^�̕ύX
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
SPR_3ChangeTexColor(Uint16 charNo, Uint16 color, SprLookupTbl *lookupTbl)
{
    /** BEGIN ***************************************************************/
    SpCharTbl[charNo].color = color;
    if(lookupTbl)
        SPR_2SetLookupTbl(color, lookupTbl);
}


/*****************************************************************************
 *
 * NAME:  SPR_3ClrTexture()  - Clear Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  �e�N�X�`���e�[�u���̃|�C���^
 *
 * DESCRIPTION:
 *
 *     �e�N�X�`���G���A�̉��
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
SPR_3ClrTexture(SprTexture *texture)
{
    SprTexture  *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++)
        SPR_2ClrChar(wTex->charNo);
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetZSortMinMax()  - Set Z Sort Min/Max Z Value
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pZSortMode  - <i>  �y�\�[�g�y���W�l�̗̍p���[�h
 *                                     �O���������[�h
 *                                     �P���Œ胂�[�h
 *     (2) Fixed32  pZSortMin   - <i>  ���_���W�n�y�\�[�g�y�ŏ��l���W
 *     (3) Fixed32  pZSortMax   - <i>  ���_���W�n�y�\�[�g�y�ő�l���W
 *
 *
 * DESCRIPTION:
 *
 *     ���_���W�n�y�\�[�g�͈͂̍ŏ��E�ő�l���Z�b�g
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
SPR_3SetZSortMinMax(Uint16 pZSortMode, Fixed32 pZSortMin, Fixed32 pZSortMax)
{

    /** BEGIN ***************************************************************/
    zSortMode    = pZSortMode;
    zSortZMin    = pZSortMin;
    zSortZMax    = pZSortMax;
    zSortZMinOrg = pZSortMin;
    zSortZMaxOrg = pZSortMax;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetClipLevel()  - Set Cliping Level
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pClipLevel  - <i>  �N���b�s���O�̃��x���ԍ�
 *                                     �O���N���b�s���O�Ȃ�
 *                                     �P�����_���W�n�̂y�͈͂ŃN���b�v
 *                                         �i���W�ϊ���j
 *                                     �Q���X�N���[���Ɋ|����Ȃ��|���S����
 *                                         �폜�i�����ϊ���j
 *                                     �R���t���[���o�b�t�@�̋��E�ŃN���b�v
 *                                         �i�����ϊ���j
 *     (2) Fixed32  pClipZMin   - <i>  ���_���W�n�N���b�v�y�ŏ��l���W
 *     (3) Fixed32  pClipZMax   - <i>  ���_���W�n�N���b�v�y�ő�l���W
 *
 * DESCRIPTION:
 *
 *     �N���b�s���O���x���̃Z�b�g
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
SPR_3SetClipLevel(Uint16 pClipLevel, Fixed32 pClipZMin, Fixed32 pClipZMax)
{

    /** BEGIN ***************************************************************/
    clipLevel = pClipLevel;
    clipZMin  = pClipZMin;
    clipZMax  = pClipZMax;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetPixelCount()  - Set Screen Pixel Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pixelCountX  - <i>  �X�N���[���w�̒P�ʃs�N�Z����
 *     (2) Uint16   pixelCountY  - <i>  �X�N���[���x�̒P�ʃs�N�Z����
 *
 * DESCRIPTION:
 *
 *     �X�N���[���ւ̓����ϊ����̂w�x�e�X 1.0 �ɑ΂���X�N���[���̃s�N�Z����
 *     ��ݒ肷��B
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void SPR_3SetPixelCount(Uint16 pixelCountX, Uint16 pixelCountY)
{

    /** BEGIN ***************************************************************/
    unitPixel.x = MTH_IntToFixed(pixelCountX);
    unitPixel.y = MTH_IntToFixed(pixelCountY);
}


/*****************************************************************************
 *
 * NAME:  SPR_3GetStatus()  - Get 3D Environment Value
 *
 * PARAMETERS :
 *
 *     (1) Spr3dStatus *spr3dStatus  - <o>  �R�c���f�[�^�e�[�u���̃|�C���^
 *
 * DESCRIPTION:
 *
 *     ���R�c���f�[�^�̎擾�B
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
void   SPR_3GetStatus(Spr3dStatus *spr3dStatus)
{
    /** BEGIN ***************************************************************/
    spr3dStatus->lightAngle      = lightAngle;
    spr3dStatus->viewCoordPoint  = viewCoordPoint;
    spr3dStatus->viewPoint       = viewPoint;
    spr3dStatus->viewAngle       = viewAngle;
    spr3dStatus->viewAngleSeq    = viewAngleSeq;
    spr3dStatus->zSortMode       = zSortMode;
    spr3dStatus->zSortZMin       = zSortZMin;
    spr3dStatus->zSortZMax       = zSortZMax;
    spr3dStatus->clipZMin        = clipZMin;
    spr3dStatus->clipZMax        = clipZMax;
    spr3dStatus->clipLevel       = clipLevel;
    spr3dStatus->unitPixel       = unitPixel;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetSurfNormVect() - Compute Surface Normal Vector in the Object
 *
 * PARAMETERS :
 *
 *     (1)  SprObject3D  *obj  - <i/o>  �R�c�I�u�W�F�N�g�̃|�C���^
 *
 * DESCRIPTION:
 *
 *     �w��R�c�I�u�W�F�N�g�̖ʂ̖@�����v�Z���A�Z�b�g����B
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
void   SPR_3SetSurfNormVect(SprObject3D  *obj)
{
    Sint32      i, p1, p2, p3;
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
    MthXyz      *wSurfNormal;

    /** BEGIN ***************************************************************/
    wSurface    = obj->surface;
    wVertPoint  = obj->vertPoint;
    wSurfNormal = obj->surfaceNormal;
    for(i = 0; i < obj->surfaceCount; i++) {
	p1 = wSurface->vertNo[0];
	p2 = wSurface->vertNo[1];
	p3 = wSurface->vertNo[2];
	MTH_ComputeNormVect(obj->surfNormK, &wVertPoint[p1], &wVertPoint[p2],
			                    &wVertPoint[p3], wSurfNormal);
	wSurface++;
	wSurfNormal++;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetDrawSpeed() - Set VDP1 Drawing Speed up Parameter
 *
 * PARAMETERS :
 *
 *     (1)  Sint32    hssFlag  - <i>  �n�C�X�s�[�h�V�������N�t���O
 *                                    �e�N�X�`���|���S���̕`�惂�[�h
 *                                    �O�����x�D��`��i�f�t�H���g�j
 *                                    �P�����x�D��`��
 *     (2)  Sint32    eosFlag  - <i>  hss=1 �ő��x�D��`����w�肵������
 *                                    ���G�e�N�X�`���̃T���v�����O���W
 *                                    �O���������W���T���v�����O�i�f�t�H���g�j
 *                                    �P������W���T���v�����O
 *     (3)  Sint32    pclpFlag - <i>  �v���N���b�s���O�L���E�����t���O
 *                                    �O���L���i�f�t�H���g�j
 *                                    �P������
 *
 *
 * DESCRIPTION:
 *
 *     �u�c�o�P�̍����`��p�����[�^���Z�b�g
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
void   SPR_3SetDrawSpeed(Sint32 hssFlag, Sint32 eosFlag, Sint32 pclpFlag)
{
    Sint32      i, p1, p2, p3;
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
    MthXyz      *wSurfNormal;

    /** BEGIN ***************************************************************/
    if(hssFlag)
       spriteDrawFlag  = HSS_ENABLE;
    else
       spriteDrawFlag  = 0;

    if(pclpFlag) {
       spriteDrawFlag |= PCLP_ENABLE;
       otherDrawFlag   = PCLP_ENABLE;
    } else {
       spriteDrawFlag &= ~PCLP_ENABLE;
       otherDrawFlag   = 0;
    }

    SPR_SetEosMode(eosFlag);
}


/*****************************************************************************
 *
 * NAME:  initDraw()  - Initial Draw Environment
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �R�c�\���̃e�[�u��������
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
static void
initDraw(void)
{
    Fixed32   ztMax;
    Sint32    i;

    /** BEGIN ***************************************************************/
    if(zSortMode) {
        zSortBZMin    = zSortZMinOrg;
        zSortBZMax    = zSortZMaxOrg;
    } else        {
        zSortBZMin    = zSortZMin;
        zSortBZMax    = zSortZMax;
    }
    zSortZMin     = FIXED_MAX;
    zSortZMax     = FIXED_MIN;
    ztMax = zSortBZMax - zSortBZMin;
    for(i=0; i<32; i++)
	if((ztMax<<=1) & 0x80000000) break;
    zSftCnt = 17 - i + blkSftCnt;
    if(zSftCnt < 0) zSftCnt = 0;
    dbFirstFlag = 1;
    bufSW       = 0;
    if(Sp2OpenFlag) {
        gourTblNo   = 0;
        clipScrXMin = -SpLCoordX;
        clipScrXMax = SpScreenX - SpLCoordX - 1;
        clipScrYMin = -SpLCoordY;
        clipScrYMax = SpScreenY - SpLCoordY - 1;
        Sp2OpenFlag = 0;
    }
}


/*****************************************************************************
 *
 * NAME:  drawPol()  - Draw 1 Polygon
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl          - <i>  �|���S���e�[�u���̃|�C���^
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
static void
drawPol(PolTbl *polTbl)
{
    Sint32     drawPrtyBlkNo;
    Sint32     shadingKind, dispKind, drawMode, colOrTex, textCtrl;
    Sint32     surfBright;
    Sint32     mateKind, i;
    XyInt      *xy;
    SprGourTbl *gourTbl;
    Sint32     z;

    /** BEGIN ***************************************************************/
    dispKind    = polTbl->dispFlag & DISP_MASK;
    shadingKind = polTbl->dispFlag & SHADING_MASK;
    mateKind    = polTbl->drawMode & MATE_MASK;
    drawMode    = polTbl->drawMode & DRAW_MODE_MASK;
    surfBright  = polTbl->surfBright;
    gourTbl     = &polTbl->gourTbl;
    colOrTex    = polTbl->color;
    xy          = polTbl->xy;

    if(mateKind == MATE_TEXTURE) {
        textCtrl   = colOrTex & 0xc000;
        textCtrl >>= 10;
        colOrTex  &= 0x3fff;
    }

    /* �y�l�u���b�N�\�[�g�̍ő�ŏ��l�𓾂� */
    if(polTbl->z < zSortZMin) zSortZMin = polTbl->z;
    if(polTbl->z > zSortZMax) zSortZMax = polTbl->z;
    if(polTbl->z > zSortBZMin) {
	drawPrtyBlkNo = (int)((polTbl->z - zSortBZMin) >> zSftCnt);
	if(drawPrtyBlkNo >= zSortBlkCnt)
	    drawPrtyBlkNo = zSortBlkCnt - 1;
    } else
	drawPrtyBlkNo = 0;

    for(i=0; i<polTbl->polyCnt; i++) {
	if(shadingKind == NO_SHADING) {
            if(mateKind == MATE_TEXTURE) {
		SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                  drawMode | spriteDrawFlag,
		   	          0xffff, colOrTex, xy, NO_GOUR);
            } else {
    	        if(dispKind == DISP_POLYGON) {
	            SPR_2Polygon(drawPrtyBlkNo,
	                      drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR);
                } else {
	            SPR_2PolyLine(drawPrtyBlkNo,
	                      drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR);
                }
            }
        } else
	if(shadingKind == FLAT_SHADING) {  /* Color Mode is Only RGB */
            if(mateKind == MATE_TEXTURE) {
	        SPR_2SetGourTbl(gourTblNo, gourTbl);
                SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                             drawMode | DRAW_GOURAU  | spriteDrawFlag,
                              0xffff, colOrTex, xy, gourTblNo);
		gourTblNo++;
            } else {
                if(dispKind == DISP_POLYGON) {
		    SPR_2Polygon(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                 surfBright, xy, NO_GOUR);
                } else {
                    SPR_2PolyLine(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                  surfBright, xy, NO_GOUR);
	        }
	    }
        } else
        if(shadingKind == GOURAUD_SHADING) { /* Color Mode is Only RGB */
            SPR_2SetGourTbl(gourTblNo, gourTbl);
            if(mateKind == MATE_TEXTURE) {
                SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                 drawMode | DRAW_GOURAU | spriteDrawFlag,
		   	                0xffff, colOrTex, xy, gourTblNo);
            } else     {
	        if(dispKind == DISP_POLYGON) {
                    SPR_2Polygon(drawPrtyBlkNo,
                                 drawMode | DRAW_GOURAU | otherDrawFlag,
						colOrTex, xy, gourTblNo);
                } else {
		    SPR_2PolyLine(drawPrtyBlkNo,
                                  drawMode | DRAW_GOURAU | otherDrawFlag,
				          	colOrTex, xy, gourTblNo);
                }
            }
	    gourTblNo++;
        }
        xy += 4;
    }
}


/*****************************************************************************
 *
 * NAME:  transCluster()  - Transfer Cluster Coord
 *
 * PARAMETERS :
 *
 *     (1) SprCluster  *cluster  - <i>  ���[�g�N���X�^�̃|�C���^
 *
 * DESCRIPTION:
 *
 *     �N���X�^�P�ʂ̉B�ʁA���W�ϊ�����
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
static void
transCluster(SprCluster *cluster)
{
    SprObject3D  *obj;
    SprSurface   *wSurface;
    MthXyz       *wVertPoint, *wSurfaceVert;
    Fixed32      *wSurfPoint, *wVPoint;
    Uint8        *wSurfBright;
    Uint16       *wVertNo;
    Sint32 	 i, j;
    const static MthMatrix	unitMatrix
	= { MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0)};

    /** BEGIN ***************************************************************/
    while(cluster) {
        if(cluster->transStart)  cluster->transStart(cluster);

        MTH_PushMatrix(&worldMatrixTbl);

	/* �J�����g�}�g���b�N�X�ɃN���X�^�̈ړ��ʃ}�g���b�N�X�Z�b�g */
	MTH_MoveMatrix(&worldMatrixTbl,
                       cluster->point.x, cluster->point.y, cluster->point.z);
	switch(cluster->angleSeq) {
	    case ROT_SEQ_ZYX :
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
	         MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
                 break;
            case ROT_SEQ_ZXY :
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 break;
            case ROT_SEQ_YZX :
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
                 break;
            case ROT_SEQ_YXZ :
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 break;
	    case ROT_SEQ_XYZ :
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 break;
	    case ROT_SEQ_XZY :
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 break;
	}

	obj = cluster->object;
	if(obj) {
	    if(postWorldDataFlag) {
	        MTH_MulMatrix(&viewMatrix, worldMatrixTbl.current,
						     &viewLight.viewMatrix);
	        memcpy(&viewLight.worldMatrix, worldMatrixTbl.current,
						     sizeof(MthMatrix));
	    } else {
	        memcpy(&viewLight.viewMatrix, worldMatrixTbl.current,
						     sizeof(MthMatrix));
	    }
	}

	while(obj) {
            cBuf = &dBuf[bufSW];
            cBuf->cluster = cluster;
            cBuf->object  = obj;
            if(obj->dispFlag & INBETWEEN_OBJECT) {
		memcpy(&viewLight.viewMatrix, &unitMatrix,sizeof(MthMatrix));
		SPR_3SetSurfNormVect(obj);
            }

	    if(obj->surfaceVert == 0) {
		/* �ʂ̑�\���_�e�[�u���𓾂� */
		wSurfPoint  = (Fixed32*)cBuf->coordView3D;
		wSurface    = obj->surface;
		wVertPoint  = obj->vertPoint;

		for(i = 0; i < obj->surfaceCount; i++) {
		    j = wSurface->vertNo[0];
		    wVPoint = (Fixed32*)&wVertPoint[j];
		    *wSurfPoint++ = *wVPoint++;
		    *wSurfPoint++ = *wVPoint++;
		    *wSurfPoint++ = *wVPoint;
		    wSurface++;
		}
		wSurfaceVert = cBuf->coordView3D;
	    } else
		wSurfaceVert = obj->surfaceVert;

	    /* �ʖ@���e�[�u������ʋP�x�e�[�u���𓾂邽�߂̃p�����[�^�Z�b�g */
	    polyTransParm.viewLight = &viewLight;
	    polyTransParm.surfCount = obj->surfaceCount;
	    polyTransParm.surfPoint = wSurfaceVert;
	    polyTransParm.surfNormal = obj->surfaceNormal;
	    polyTransParm.surfBright = cBuf->surfBright;

	    /* ���_�f�[�^�̎��_���W�ϊ���e�[�u���𓾂邽�߂̃p�����[�^�Z�b�g*/
	    polyTransParm.transViewVertCount = obj->vertCount;
	    polyTransParm.transViewVertSrc = obj->vertPoint;
	    polyTransParm.transViewVertAns = cBuf->coordView3D;

	    /* �O�[���[�p���_�P�x�e�[�u���𓾂邽�߂̃p�����[�^�Z�b�g  */
	    if((obj->dispFlag & SHADING_MASK) == GOURAUD_SHADING) {
                if(obj->dispFlag & INBETWEEN_OBJECT) {
  	            polyTransParm.gourVertCount = 0;
  	            memcpy(cBuf->vertBright,obj->vertNormal,
  	                                  obj->vertCount*sizeof(Sint32));
                } else {
	            polyTransParm.gourVertCount = obj->vertCount;
	            polyTransParm.vertNormal    = obj->vertNormal;
		    polyTransParm.vertBright    = cBuf->vertBright;
		}
            } else
	        polyTransParm.gourVertCount = 0;

	    /* ���_�f�[�^�̃��[���h���W�ϊ���e�[�u���𓾂邽�߂�
                                                        �p�����[�^�Z�b�g*/
	    if(postWorldDataFlag) {
	        polyTransParm.transWorldVertCount = obj->vertCount;
	        polyTransParm.transWorldVertSrc   = obj->vertPoint;
	        polyTransParm.transWorldVertAns   = cBuf->coordWorld3D;
            } else
	        polyTransParm.transWorldVertCount = 0;

	    MTH_PolyDataTransExec(&polyTransParm);

            if(dbFirstFlag) {
		MTH_PolyDataTransCheck();
                if(cluster->transEnd)
                    cluster->transEnd(cluster, obj, worldMatrixTbl.current,
                                                         cBuf->coordWorld3D);
            } else {
		/* �|���S���`��R�}���h�̓o�^���� */
                cBuf = &dBuf[bufSW^1];
		transPolygons(cBuf, coord2D);
		setInbetPolygons(cBuf);
		MTH_PolyDataTransCheck();
                if(cluster->transEnd) {
		    cBuf = &dBuf[bufSW];
                    cluster->transEnd(cluster, obj, worldMatrixTbl.current,
                                                         cBuf->coordWorld3D);
                }
            }

	    if(doubleBufFlag) {
		dbFirstFlag = 0;
                bufSW ^= 1;
	    } else {
                cBuf = &dBuf[bufSW];
		transPolygons(cBuf, coord2D);
                setInbetPolygons(cBuf);
            }

            obj = obj->next;

	}

	/* �q�N���X�^�̏��� */
	if(cluster->child) transCluster(cluster->child);

	MTH_PopMatrix(&worldMatrixTbl);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  setInbetPolygons  - Set Inbetween Polygon Data to Inbet Object
 *
 * PARAMETERS :
 *
 *     (1) DoubleBufArea  *buf    - <i>  �J�����g���W�ϊ��o�b�t�@
 *
 * DESCRIPTION:
 *
 *     �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����_���W�n���_�f�[�^�̓o�^����
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
static void
setInbetPolygons(DoubleBufArea *buf)
{
    Sint32       i;
    SprObject3D  *inbetObj;
    SprInbetInf  *wInbetInf;
    MthXyz       *fromVertPoint;
    Sint32       *fromVertBright;
    Uint16       *fromVertNo;
    MthXyz       *toVertPoint;
    Sint32       *toVertBright;
    Uint16       *toVertNo;
    SprCluster   *cluster;
    SprObject3D  *obj;

    /** BEGIN ***************************************************************/
    cluster = buf->cluster;
    obj     = buf->object;

    /*  �R�c�I�u�W�F�N�g�Ԑڑ��|���S����񂪂���ꍇ     */
    if(cluster->inbetInf) {
	 wInbetInf = cluster->inbetInf;
	 while(wInbetInf) {
	     if(obj == wInbetInf->fromObj) {
		 fromVertPoint = cBuf->coordView3D;
		 fromVertNo    = wInbetInf->fromVertNo;
		 toVertPoint   = wInbetInf->toObj->vertPoint;
		 toVertNo      = wInbetInf->toVertNo;
		 for(i=0 ; i<wInbetInf->vertCount; i++)
		     toVertPoint[toVertNo[i]] = fromVertPoint[fromVertNo[i]];
		 toVertBright  = (Sint32*)wInbetInf->toObj->vertNormal;
		 if(toVertBright) {
		     fromVertBright = cBuf->vertBright;
		     for(i=0 ; i<wInbetInf->vertCount; i++)
		         toVertBright[toVertNo[i]] =
		                              fromVertBright[fromVertNo[i]];
		 }
                 break;
             }
             wInbetInf = wInbetInf->next;
         }
    }
}


/*****************************************************************************
 *
 * NAME:  clip2DLevel2()  - Clip 2D Level 2
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i>  �Q�c�|���S���e�[�u��
 *
 * DESCRIPTION:
 *
 *     �|���S���̂Q�c�N���b�s���O����
 *     �X�N���[���Ɋ|����Ȃ��|���S���̍폜
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  ���^�[���R�[�h
 *                                 �P���`�悵�Ȃ�
 *                                 �O���`�悷��
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clip2DLevel2(PolTbl *polTbl)
{
    Uint32   f1,f2;
    XyInt    *xy,*xyb;
    Sint32   i;

    /** BEGIN ***************************************************************/
    f1 = 0;
    xy = polTbl->xy;
    if(xy->x < clipScrXMin) f1 |= 8;
    if(xy->x > clipScrXMax) f1 |= 4;
    if(xy->y < clipScrYMin) f1 |= 2;
    if(xy->y > clipScrYMax) f1 |= 1;
    for(i=0; i<4; i++) {
	xyb = xy;
	if(i == 3)
	    xy -= 3;
	else
	    xy++;
	f2 = 0;
	if(xy->x < clipScrXMin) f2 |= 8;
	if(xy->x > clipScrXMax) f2 |= 4;
	if(xy->y < clipScrYMin) f2 |= 2;
	if(xy->y > clipScrYMax) f2 |= 1;
	if(!(f1 & f2)) {
	    if(!(f1 | f2)) return 0;
	    if(!clipScrCheck(xyb,xy,8)) return 0;
	}
	f1 = f2;
    }
    return 1;
}

/*****************************************************************************
 *
 * NAME:  clipScrCheck()  - Clip Screen Detail Check
 *
 * PARAMETERS :
 *
 *     (1) XyInt     *xy1   - <i>  ���C���n�_
 *     (2) XyInt     *xy2   - <i>  ���C���I�_
 *     (2) Sint32    chkCnt - <i>  �`�F�b�N�J�E���^
 *
 * DESCRIPTION:
 *
 *     ���C���̒��_���X�N���[�����ɓ����Ă��邩�Q�����@�ɂ�胊�J�[�V�u��
 *     �`�F�b�N����B
 *
 *
 * POSTCONDITIONS:
 *
 *     Sint32   ret         - <o>  ���^�[���R�[�h
 *                                 �O���X�N���[�����ɓ����Ă���
 *                                 �P�������Ă��Ȃ�
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static Sint32
clipScrCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt)
{
   Sint32       ret;
   XyInt     xym;

   if(chkCnt <= 1) return 1;
   xym.x = (xy1->x + xy2->x) >> 1;
   if(xym.x & 0x4000) xym.x |= 0x8000;
   xym.y = (xy1->y + xy2->y) >> 1;
   if(xym.y & 0x4000) xym.y |= 0x8000;
   if((clipScrXMin <= xym.x) && (xym.x <= clipScrXMax) &&
      (clipScrYMin <= xym.y) && (xym.y <= clipScrYMax)) return 0;
   switch(outcodeScr(xy1, &xym)) {
      case -1 : /* just in */
          return 0;
      case  0 : /* nead detail check */
          ret = clipScrCheck(xy1, &xym, chkCnt-1);
          break;
      case  1 : /* just out */
	  ret = clipScrCheck(&xym, xy2, chkCnt-1);
	  break;
   }
   return ret;
}

static
Sint32 outcodeScr(XyInt *xy1, XyInt *xy2)
{
   Uint32 f1, f2;

   f1 = f2 = 0;
   if(xy1->x < clipScrXMin) f1 |= 8;
   if(xy1->x > clipScrXMax) f1 |= 4;
   if(xy1->y < clipScrYMin) f1 |= 2;
   if(xy1->y > clipScrYMax) f1 |= 1;
   if(xy2->x < clipScrXMin) f2 |= 8;
   if(xy2->x > clipScrXMax) f2 |= 4;
   if(xy2->y < clipScrYMin) f2 |= 2;
   if(xy2->y > clipScrYMax) f2 |= 1;
   if(f1 & f2)
       return  1; /* just out          */
   else
   if(f1 | f2)
       return  0; /* need detail check */
   else
       return -1; /* just in           */
}


/*****************************************************************************
 *
 * NAME:  clipFrmCheck()  - Clip Frame Buffer Detail Check
 *
 * PARAMETERS :
 *
 *     (1) XyInt  *xy1   - <i>  ���C���n�_
 *     (2) XyInt  *xy2   - <i>  ���C���I�_
 *     (2) Sint32    chkCnt - <i>  �`�F�b�N�J�E���^
 *
 * DESCRIPTION:
 *
 *     ���C���̒��_���t���[���o�b�t�@���ɓ����Ă��邩�Q�����@�ɂ�胊�J�[�V�u��
 *     �`�F�b�N����B
 *
 *
 * POSTCONDITIONS:
 *
 *     Sint32   ret         - <o>  ���^�[���R�[�h
 *                                 �O���X�N���[�����ɓ����Ă���
 *                                 �P�������Ă��Ȃ�
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static Sint32
clipFrmCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt)
{
   Sint32       ret;
   XyInt     xym;

   if(chkCnt <= 1) return 1;
   xym.x = (xy1->x + xy2->x) >> 1;
   if(xym.x & 0x4000) xym.x |= 0x8000;
   xym.y = (xy1->y + xy2->y) >> 1;
   if(xym.y & 0x4000) xym.y |= 0x8000;
   if((FRMBUF_MIN_X <= xym.x) && (xym.x <= FRMBUF_MAX_X) &&
      (FRMBUF_MIN_Y <= xym.y) && (xym.y <= FRMBUF_MAX_Y)) return 0;
   switch(outcodeFrm(xy1, &xym)) {
      case -1 : /* just in */
          return 0;
      case  0 : /* nead detail check */
          ret = clipFrmCheck(xy1, &xym, chkCnt-1);
	  break;
      case  1 : /* just out */
          ret = clipFrmCheck(&xym, xy2, chkCnt-1);
          break;
   }
   return ret;
}

static
Sint32 outcodeFrm(XyInt *xy1, XyInt *xy2)
{
   Uint32 f1, f2;

   f1 = f2 = 0;
   if(xy1->x < FRMBUF_MIN_X) f1 |= 8;
   if(xy1->x > FRMBUF_MAX_X) f1 |= 4;
   if(xy1->y < FRMBUF_MIN_Y) f1 |= 2;
   if(xy1->y > FRMBUF_MAX_Y) f1 |= 1;
   if(xy2->x < FRMBUF_MIN_X) f2 |= 8;
   if(xy2->x > FRMBUF_MAX_X) f2 |= 4;
   if(xy2->y < FRMBUF_MIN_Y) f2 |= 2;
   if(xy2->y > FRMBUF_MAX_Y) f2 |= 1;
   if(f1 & f2)
       return  1; /* just out          */
   else
   if(f1 | f2)
       return  0; /* need detail check */
   else
       return -1; /* just in           */
}


/*****************************************************************************
 *
 * NAME:  clip2DLevel3()  - Clip 2D Level 3
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i>  �Q�c�|���S���e�[�u��
 *
 * DESCRIPTION:
 *
 *     �|���S���̂Q�c�N���b�s���O����
 *     �t���[���o�b�t�@�̋��E�ŃN���b�v
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  ���^�[���R�[�h
 *                                 �P���`�悵�Ȃ�
 *                                 �O���`�悷��
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clip2DLevel3(PolTbl *polTbl)
{
    Uint32   f1,f2;
    Sint32   i,justInCnt;
    XyInt *xy, *xyb;

    /** BEGIN ***************************************************************/
    f1 = 0;
    xy = polTbl->xy;
    justInCnt = 0;
    if(xy->x < FRMBUF_MIN_X) f1 |= 8;
    if(xy->x > FRMBUF_MAX_X) f1 |= 4;
    if(xy->y < FRMBUF_MIN_Y) f1 |= 2;
    if(xy->y > FRMBUF_MAX_Y) f1 |= 1;
    for(i=0; i<4; i++) {
	xyb = xy;
	if(i == 3)
	    xy -= 3;
	else
	    xy++;
	f2 = 0;
	if(xy->x < FRMBUF_MIN_X) f2 |= 8;
	if(xy->x > FRMBUF_MAX_X) f2 |= 4;
	if(xy->y < FRMBUF_MIN_Y) f2 |= 2;
	if(xy->y > FRMBUF_MAX_Y) f2 |= 1;
	if(!(f1 & f2)) { /* just in or between */
	    if(!(f1 | f2))
		justInCnt++;
	    else
		if(!clipFrmCheck(xyb,xy,8))  return clipFrame(polTbl);
	}
	f1 = f2;
    }
    if(justInCnt >=  4)  return 0;
    return 1;
}

/*****************************************************************************
 *
 * NAME:  clipFrame()  - Clip Frame Buffer
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i/o>  �Q�c�|���S���e�[�u��
 *
 * DESCRIPTION:
 *
 *     �|���S�����t���[���o�b�t�@���E�ŃN���b�s���O���A�����̃|���S��
 *     �𐶐�����
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  ���^�[���R�[�h
 *                                 �P���`�悵�Ȃ�
 *                                 �O���`�悷��
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clipFrame(PolTbl *polTbl)
{
    Sint32   i, j, in, out, visible;
    XyInt *pxy, *qxy, *wxy, w1xy[16], w2xy[16], fxy, sxy;

    /** BEGIN ***************************************************************/
    pxy = polTbl->xy;          /* ���͑��p�` */
    qxy = w1xy;                /* �o�͑��p�` */
    in  = 4;                   /* ���͒��_�� */
    for(i=0; i<4; i++) {       /* �t���[���o�b�t�@���E�ӂɂ��Ẵ��[�v */
	out = 0;               /* �o�͒��_�� */
	for(j=0; j<in; j++) {  /* �\�����p�`�̕ӂɂ��Ẵ��[�v */
	    if(j == 0) {       /* ���p�`�̍ŏ��̓_�̏ꍇ */
		fxy.x = pxy->x;
		fxy.y = pxy->y;
	    } else     {
		if(frmCrossChk(i,&sxy,&pxy[j],&qxy[out]))  out++;
	    }
	    sxy.x = pxy[j].x;
	    sxy.y = pxy[j].y;
	    visible = 0;
	    /* �t���[���o�b�t�@���E�̓����Ƀ|�C���g�������Ă��邩���`�F�b�N*/
	    switch(i) {
		case 0 : /* �t���[���o�b�t�@���ӂƂ̃`�F�b�N */
		     if(FRMBUF_MIN_X <= sxy.x)  visible = 1;
		     break;
		case 1 : /* �t���[���o�b�t�@��ӂƂ̃`�F�b�N */
		     if(FRMBUF_MIN_Y <= sxy.y)  visible = 1;
		     break;
		case 2 : /* �t���[���o�b�t�@�E�ӂƂ̌����`�F�b�N */
		     if(sxy.x <= FRMBUF_MAX_X)  visible = 1;
		     break;
		case 3 : /* �t���[���o�b�t�@���ӂƂ̌����`�F�b�N */
		     if(sxy.y <= FRMBUF_MAX_Y)  visible = 1;
		     break;
	    }
	    if(visible) {
		qxy[out].x = sxy.x;
		qxy[out].y = sxy.y;
		out++;
	    }
	}
	if(out) {    /* �ŏI�ӂɂ��Ă̏��� */
	    if(frmCrossChk(i,&sxy,&fxy,&qxy[out]))  out++;  /* 1 = cross */
	}
	in = out;
	if(i == 0) {
	    pxy = qxy;
	    qxy = w2xy;
	} else {
	    wxy = pxy;
	    pxy = qxy;
	    qxy = wxy;
	}
    }
    if(out >= 4) {   /* ���p�`�|���S�����S�p�`�|���S���ɕ��� */
	qxy = pxy;
	wxy = polTbl->xy;
	for(i=0; i<4; i++,wxy++,qxy++) {
	    wxy->x = qxy->x;
	    wxy->y = qxy->y;
	}
	out -= 4;
	j = 1;
	qxy--;
	while(out) {
	    if(out >= 2) {
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy  ->y;
	       out -= 2;
	    } else       {
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy  ->y;
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       out -= 1;
	    }
	    j++;
	}
	polTbl->polyCnt = j;
	return 0;
    } else
    if(out >= 3) {
	qxy = pxy;
	wxy = polTbl->xy;
	for(i=0; i<3; i++,wxy++,qxy++) {
	    wxy->x = qxy->x;
	    wxy->y = qxy->y;
	}
	wxy->x = pxy->x;
	wxy->y = pxy->y;
	return 0;
    } else
	return 1;
}


/*****************************************************************************
 *
 * NAME:  frmCrossChk()  - Frame Buffer Cross Check
 *
 * PARAMETERS :
 *
 *     (1) int       frmNo  - <i>  �t���[���o�b�t�@���E�̕Ӕԍ�
 *                                 �O������
 *                                 �P�����
 *                                 �Q���E��
 *                                 �R������
 *     (2) XyInt  *p1    - <i>  ���C���̃X�^�[�g�|�C���g
 *     (2) XyInt  *p2    - <i>  ���C���̃G���h�|�C���g
 *     (2) XyInt  *cp    - <o>  ���E�ƃ��C���̃N���X�|�C���g
 *
 *
 * DESCRIPTION:
 *
 *     �w��t���[���o�b�t�@�̋��E�Ǝw�胉�C�����������Ă��邩�`�F�b�N���A
 *     �������Ă���ꍇ���̌�_��Ԃ�
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  ���^�[���R�[�h
 *                                 �P���������Ă���
 *                                 �O���������Ă��Ȃ�
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
frmCrossChk(int frmNo, XyInt *p1, XyInt *p2, XyInt *cp)
{
    Sint32   w1, w2, w3;

    /** BEGIN ***************************************************************/
    switch(frmNo) {
	case 0 : /* �t���[���o�b�t�@���ӂƂ̌����`�F�b�N */
	     w1 = FRMBUF_MIN_X - p1->x;
	     w2 = FRMBUF_MIN_X - p2->x;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->x - p1->x;
	     if(w3 == 0) return 0;
	     cp->x = FRMBUF_MIN_X;
	     cp->y = p1->y + ((Sint32)(p2->y - p1->y)) * w1 / w3;
          /* if(cp->y < FRMBUF_MIN_Y) cp->y = FRMBUF_MIN_Y; else
             if(cp->y > FRMBUF_MAX_Y) cp->y = FRMBUF_MAX_Y; */
	     break;
	case 1 : /* �t���[���o�b�t�@��ӂƂ̌����`�F�b�N */
	     w1 = FRMBUF_MIN_Y - p1->y;
	     w2 = FRMBUF_MIN_Y - p2->y;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->y - p1->y;
	     if(w3 == 0) return 0;
	     cp->x = p1->x + ((Sint32)(p2->x - p1->x)) * w1 / w3;
	     cp->y = FRMBUF_MIN_Y;
          /* if(cp->x < FRMBUF_MIN_X) cp->x = FRMBUF_MIN_X; else
             if(cp->x > FRMBUF_MAX_X) cp->x = FRMBUF_MAX_X; */
	     break;
	case 2 : /* �t���[���o�b�t�@�E�ӂƂ̌����`�F�b�N */
	     w1 = FRMBUF_MAX_X - p1->x;
	     w2 = FRMBUF_MAX_X - p2->x;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->x - p1->x;
	     if(w3 == 0) return 0;
	     cp->x = FRMBUF_MAX_X;
	     cp->y = p1->y + ((Sint32)(p2->y - p1->y)) * w1 / w3;
          /* if(cp->y < FRMBUF_MIN_Y) cp->y = FRMBUF_MIN_Y; else
             if(cp->y > FRMBUF_MAX_Y) cp->y = FRMBUF_MAX_Y; */
	     break;
	case 3 : /* �t���[���o�b�t�@���ӂƂ̌����`�F�b�N */
	     w1 = FRMBUF_MAX_Y - p1->y;
	     w2 = FRMBUF_MAX_Y - p2->y;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->y - p1->y;
	     if(w3 == 0) return 0;
	     cp->x = p1->x + ((Sint32)(p2->x - p1->x)) * w1 / w3;
	     cp->y = FRMBUF_MAX_Y;
          /* if(cp->x < FRMBUF_MIN_X) cp->x = FRMBUF_MIN_X; else
             if(cp->x > FRMBUF_MAX_X) cp->x = FRMBUF_MAX_X; */
	     break;
    }
    return 1;
}


/*****************************************************************************
 *
 * NAME:  transPolygons  - Set Polygon Data to polTbl
 *
 * PARAMETERS :
 *
 *     (1) DoubleBufArea  *buf    - <i>  �J�����g���W�ϊ��o�b�t�@
 *     (12 XyInt          *vert2d - <O>  �Q�c���W�ۑ����[�N�G���A
 *
 * DESCRIPTION:
 *
 *     �|���S���`��R�}���h�̓o�^����
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
static void
transPolygons(DoubleBufArea *buf, XyInt *vert2d)
{
    Sint32     i, j;
    Sint32     shadingKind, mateKind, bothFace, dispFlag, sBright;
    Uint32     zSortKind;
    Sint32     *wSurfBright, *vertBright, l;
    SprSurface *wSurface;
    SprObject3D *obj;
    PolTbl     polTbl;
    Uint16     *gour, w;
    Uint16     *wVertNo;
    Uint16     *shadingTbl;
    XyInt      *wxy;
    MthXyz     xyz[4], *wxyz, *coordView3D;
    Fixed32    zMax, *wFix1, *wFix2;

    static Uint16    grayCode[32] = {
        RGB16_COLOR( 0, 0, 0), RGB16_COLOR( 1, 1, 1),
        RGB16_COLOR( 2, 2, 2), RGB16_COLOR( 3, 3, 3),
        RGB16_COLOR( 4, 4, 4), RGB16_COLOR( 5, 5, 5),
        RGB16_COLOR( 6, 6, 6), RGB16_COLOR( 7, 7, 7),
        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 9, 9, 9),
        RGB16_COLOR(10,10,10), RGB16_COLOR(11,11,11),
        RGB16_COLOR(12,12,12), RGB16_COLOR(13,13,13),
        RGB16_COLOR(14,14,14), RGB16_COLOR(15,15,15),
        RGB16_COLOR(16,16,16), RGB16_COLOR(17,17,17),
        RGB16_COLOR(18,18,18), RGB16_COLOR(19,19,19),
        RGB16_COLOR(20,20,20), RGB16_COLOR(21,21,21),
        RGB16_COLOR(22,22,22), RGB16_COLOR(23,23,23),
        RGB16_COLOR(24,24,24), RGB16_COLOR(25,25,25),
        RGB16_COLOR(26,26,26), RGB16_COLOR(27,27,27),
        RGB16_COLOR(28,28,28), RGB16_COLOR(29,29,29),
        RGB16_COLOR(30,30,30), RGB16_COLOR(31,31,31)
    };

    static Uint16    texGrayCode[32] = {
        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 8, 8, 8),
        RGB16_COLOR( 9, 9, 9), RGB16_COLOR( 9, 9, 9),
        RGB16_COLOR(10,10,10), RGB16_COLOR(10,10,10),
        RGB16_COLOR(11,11,11), RGB16_COLOR(11,11,11),
        RGB16_COLOR(12,12,12), RGB16_COLOR(12,12,12),
        RGB16_COLOR(13,13,13), RGB16_COLOR(13,13,13),
        RGB16_COLOR(14,14,14), RGB16_COLOR(14,14,14),
        RGB16_COLOR(15,15,15), RGB16_COLOR(15,15,15),
        RGB16_COLOR(16,16,16), RGB16_COLOR(16,16,16),
        RGB16_COLOR(17,17,17), RGB16_COLOR(17,17,17),
        RGB16_COLOR(18,18,18), RGB16_COLOR(18,18,18),
        RGB16_COLOR(19,19,19), RGB16_COLOR(19,19,19),
        RGB16_COLOR(20,20,20), RGB16_COLOR(20,20,20),
        RGB16_COLOR(21,21,21), RGB16_COLOR(21,21,21),
        RGB16_COLOR(22,22,22), RGB16_COLOR(22,22,22),
        RGB16_COLOR(23,23,23), RGB16_COLOR(23,23,23),
    };

    /** BEGIN ***************************************************************/
    obj         = buf->object;
    coordView3D = buf->coordView3D;
    vertBright  = buf->vertBright;
    wSurfBright = buf->surfBright;
    wSurface    = obj->surface;
    shadingKind = obj->dispFlag & SHADING_MASK;
    bothFace    = obj->dispFlag & BOTH_FACE;
    wxy         = vert2d;
    for(i=0; i < obj->vertCount; i++) {
	wxy->x = 0x7fff;
	wxy++;
    }

    for(i = 0; i < obj->surfaceCount; i++, wSurfBright++, wSurface++) {
	sBright = *wSurfBright & 0xff;
	dispFlag = 1;
	if(*wSurfBright & 0x80000000)
	    if(bothFace) {
	       sBright = 31 - sBright;
	       dispFlag = 2;
	    } else
	       dispFlag = 0;

 /* *(Sint32*)0x6060008 += 1; */ /* ### */
	if(dispFlag) {
 /* *(Sint32*)0x606000c += 1; */ /* ### */
	    /* ���_�f�[�^�̎��o�� */
	    wVertNo  = wSurface->vertNo;
	    wFix1 = (Fixed32*)xyz;
	    for(j = 0; j < 4; j++, wVertNo++) {
		wFix2 = (Fixed32*)&coordView3D[*wVertNo];
		*wFix1++ = *wFix2++;   /* X */
		*wFix1++ = *wFix2++;   /* Y */
		*wFix1++ = *wFix2;     /* Z */
	    }

	    /* ��\�y�l�ɂ��R�c�N���b�s���O */
	    zMax = MAX(MAX(MAX(xyz[0].z,xyz[1].z),xyz[2].z),xyz[3].z);
	    if(clipLevel > 0)
		if((zMax < clipZMin) || (zMax > clipZMax)) continue;

	    /* ��\�y�l�̎��o�� */
	    zSortKind = wSurface->drawMode & ZSORT_MASK;
	    if(zSortKind == ZSORT_MIN)
		polTbl.z = MIN(MIN(MIN(xyz[0].z,xyz[1].z),xyz[2].z),xyz[3].z);
	    else
	    if(zSortKind == ZSORT_MAX)
		polTbl.z = zMax;
	    else
		polTbl.z = MTH_Mul(xyz[0].z+xyz[1].z+xyz[2].z+xyz[3].z,
		                   MTH_FIXED(0.25));

	    /* �����ϊ� */
	    wVertNo = wSurface->vertNo;
	    wxyz    = xyz;
	    wxy     = polTbl.xy;
	    for(j = 0; j < 4; j++, wVertNo++) {
		if(vert2d[*wVertNo].x != 0x7fff) {
		    wxy->x = vert2d[*wVertNo].x;
		    wxy->y = vert2d[*wVertNo].y;
		} else {
		    MTH_Pers2D(wxyz, &unitPixel, wxy);
		    coord2D[*wVertNo].x = wxy->x;
		    coord2D[*wVertNo].y = wxy->y;
		}
		wxyz++;
		wxy++;
	    }

	    /* �Q�c�N���b�s���O */
	    polTbl.polyCnt = 1;
	    if(clipLevel == 2) {
		if(clip2DLevel2(&polTbl)) continue;
	    } else
	    if(clipLevel == 3) {
		if(clip2DLevel3(&polTbl)) continue;
	    }

	    if(shadingKind == GOURAUD_SHADING) {
		gour    = (Uint16*)&polTbl.gourTbl;
		wVertNo = wSurface->vertNo;
		if(obj->shdIdxTbl)
		    shadingTbl = obj->shdIdxTbl[0];
		else
		    shadingTbl = grayCode;
                if(dispFlag == 1) {
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour   = shadingTbl[vertBright[*wVertNo  ]];
                } else {
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour   = shadingTbl[31 - vertBright[*wVertNo  ]];
                }
	    }

	    polTbl.dispFlag = obj->dispFlag;
            mateKind = wSurface->drawMode & MATE_MASK;
	    if(shadingKind == FLAT_SHADING) {
		if(mateKind == MATE_SHADING_TBL) {
		    shadingTbl = obj->shdIdxTbl[wSurface->color];
		    polTbl.surfBright = shadingTbl[sBright];
		} else
		if(mateKind == MATE_TEXTURE) {
		    gour    = (Uint16*)&polTbl.gourTbl;
		    if(obj->shdIdxTbl) {
		        shadingTbl = obj->shdIdxTbl[0];
		        w = shadingTbl[sBright];
		    } else
		        w = texGrayCode[sBright];
		    *gour++ = w;
		    *gour++ = w;
		    *gour++ = w;
		    *gour   = w;
		} else {
		    polTbl.surfBright =
			    SPR_3GetShdColor(wSurface->color,sBright);
		}
	    }
	    polTbl.drawMode   = wSurface->drawMode;
	    polTbl.color      = wSurface->color;

	    drawPol(&polTbl);
	}
    }
}


/*****************************************************************************
 *
 * NAME:  viewCoordMatrix()  - set View Coord Convert Matrix To
 *                                                    current matrix
 *
 * PARAMETERS :
 *
 * �@�@�Ȃ�
 *
 *
 * DESCRIPTION:
 *
 *     �J�����g�}�g���b�N�X�Ɏ��_���W�n�ւ̕ϊ��}�g���b�N�X���Z�b�g
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
static void viewCoordMatrix(void)
{
    MTH_InitialMatrix(&viewMatrixTbl, 1, &viewMatrix);
    MTH_ReverseZ(&viewMatrixTbl);
    MTH_MoveMatrix(&viewMatrixTbl, viewCoordPoint.x,
				   viewCoordPoint.y,
				   viewCoordPoint.z);
    switch(viewAngleSeq) {
        case ROT_SEQ_ZYX :
   	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_ZXY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_YZX :
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_YXZ :
             MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_XYZ :
  	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
        case ROT_SEQ_XZY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
    }
    MTH_MoveMatrix(&viewMatrixTbl, -viewPoint.x, -viewPoint.y, -viewPoint.z);
    MTH_NormalTrans(&viewMatrix, &worldLightVector, &viewLight.lightVector);
    if(postWorldDataFlag) {
        MTH_ClearMatrix(&worldMatrixTbl);
    } else {
	memcpy(worldMatrixTbl.current, &viewMatrix, sizeof(MthMatrix));
    }

}


/*****************************************************************************
 *
 * NAME:  lightAngle()  -
 *
 * PARAMETERS :
 *
 * �@�@MthXyz *lightAngle
 *
 *
 * DESCRIPTION:
 *
 *     �����̃A���O����ݒ肷��
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
static void setLightAngle(void)
{
    MTH_InitialMatrix(&lightMatrixTbl, 1, &lightMatrix);
    MTH_RotateMatrixZ(&lightMatrixTbl, lightAngle.z);
    MTH_RotateMatrixY(&lightMatrixTbl, lightAngle.y);
    MTH_RotateMatrixX(&lightMatrixTbl, lightAngle.x);
    MTH_NormalTrans(&lightMatrix, &orgLightVect, &worldLightVector);
}

/*  end of file */
