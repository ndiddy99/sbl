/*----------------------------------------------------------------------------
 *  sega_mth.h -- MTH ���C�u���� �C���N���[�h�t�@�C��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-28 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  �ȉ��̌v�Z���s�����w�v�Z���C�u�����p�C���N���[�h�t�@�C���ł��B
 *
 *                                      ------- �\�[�X�t�@�C���� -------
 *                                        �r�g�Q�p    �V�~�����[�V�����p
 *    �E�O�p�֐�                        mth_tri.src      mth_tri.c
 *    �E�}�g���b�N�X���Z����            mth_mtrx.src     mth_mtrx.c
 *    �E�c�r�o�ɂ��R�c���W�ϊ�����    mth_dsp.c        mth_dsps.c
 *    �E�����ϊ�����                    mth_ps2d.src     mth_ps2d.c
 *    �E������������                    mth_rand.c       ����
 *    �E�X�v���C���Ȑ��v�Z����          mth_curv.c       ����
 *    �E�Œ菬���_���Z                  mth_fixd.src     mth_fixd.c
 *    �E�@���x�N�^�v�Z����              mth_norm.src     mth_nrms.c
 *
 *----------------------------------------------------------------------------
 */

#ifndef SEGA_MTH_H
#define SEGA_MTH_H

#include        "sega_xpt.h"
#include        "sega_def.h"

/*
 * �f�[�^�^�C�v��`
 */

typedef Sint32           Fixed32;    /* Fixed-point representation           */
				     /* Radix point is between bit 15 & 16   */

typedef struct  MthXy {              /* 2 Dimension Point Data               */
	 Fixed32         x;
	 Fixed32         y;
} MthXy;

typedef struct  MthXyz {             /* 3 Dimension Point Data               */
	 Fixed32         x;
	 Fixed32         y;
	 Fixed32         z;
} MthXyz;

typedef struct  MthMatrix {          /* 3 Dimension Matrix Data              */
	 Fixed32         val[3][4];
} MthMatrix;

typedef struct  MthMatrixTbl {       /* Matrix Stack Management Table        */
	 Uint16          stackSize;  /* Number of Matrix Stack Entry         */
	 MthMatrix       *current;   /* Current Matrix Pointer               */
	 MthMatrix       *stack;     /* Matrix Stack Area                    */
} MthMatrixTbl;

typedef struct  MthViewLight {       /* Coord Tran Matrix and Light Parm     */
	 MthMatrix     viewMatrix;   /* View Coord Transfer Matrix           */
	 MthXyz        lightVector;  /* Light Vector in View Coord           */
	 MthMatrix     worldMatrix;  /* World Coord Transfer Matrix          */
} MthViewLight;

typedef struct  MthPolyTransParm {   /* Polygon Data Coord Transfer Parm     */
	 MthViewLight  *viewLight;   /* Coord Tran Matrix and Light Parm     */
	 Uint32        surfCount;    /* Number of Polygon Surface            */
	 MthXyz        *surfPoint;   /* Surface Point for Compute Bright     */
	 MthXyz        *surfNormal;  /* Surface Normal Vector                */
	 Sint32        *surfBright;  /* Result of Surface Compute Bright     */
	 Uint32        transViewVertCount;  /* Number of Vert for View Tran  */
	 MthXyz        *transViewVertSrc;   /* Source Vert for View Tran     */
	 MthXyz        *transViewVertAns;   /* Answer Vert for View Tran     */
	 Uint32        gourVertCount;/* Number of Vert for Compute Bright    */
	 MthXyz        *vertNormal;  /* Vertices Bright Normal Vector        */
	 Sint32        *vertBright;  /* Result of Vertices Compute Bright    */
	 Uint32        transWorldVertCount; /* Number of Vert for World Tran */
	 MthXyz        *transWorldVertSrc;  /* Source Vert for World Tran    */
	 MthXyz        *transWorldVertAns;  /* Answer Vert for World Tran    */
} MthPolyTransParm;


/*
 * �O�p�֐��̒�`
 */
Fixed32 MTH_Sin(Fixed32 degree);
Fixed32 MTH_Cos(Fixed32 degree);
Fixed32 MTH_Atan(Fixed32 y, Fixed32 x);

/*
 * �}�g���b�N�X���Z����
 */
void    MTH_InitialMatrix(MthMatrixTbl *matrixTbl, Uint16 stackSize,
						   MthMatrix *matrix);
void    MTH_ClearMatrix(MthMatrixTbl *matrixTbl);
void    MTH_PushMatrix(MthMatrixTbl *matrixTbl);
void    MTH_PopMatrix(MthMatrixTbl *matrixTbl);
void    MTH_MoveMatrix(MthMatrixTbl *matrixTbl,
				   Fixed32 x, Fixed32 y, Fixed32 z);
void    MTH_RotateMatrixX(MthMatrixTbl *matrixTbl, Fixed32 xDegree);
void    MTH_RotateMatrixY(MthMatrixTbl *matrixTbl, Fixed32 yDegree);
void    MTH_RotateMatrixZ(MthMatrixTbl *matrixTbl, Fixed32 zDegree);
void    MTH_ReverseZ(MthMatrixTbl *matrixTbl);
void    MTH_MulMatrix(MthMatrix *a, MthMatrix *b, MthMatrix *c);
void    MTH_CoordTrans(MthMatrix *matrix, MthXyz *src, MthXyz *ans);
void    MTH_NormalTrans(MthMatrix *matrix, MthXyz *src, MthXyz *ans);

/*
 * �c�r�o�ɂ��R�c���W�ϊ�����
 */
void    MTH_PolyDataTransInit(void);
void    MTH_PolyDataTransExec(MthPolyTransParm *polyTransParm);
void    MTH_PolyDataTransCheck(void);

/*
 * �����ϊ�����
 */
void    MTH_Pers2D(MthXyz *p3d, MthXy *unitPixel, XyInt *p2d);
Sint32  MTH_ComputeBright(MthXyz *lightVector, MthXyz *normalVector);
void    MTH_ComputeNormVect(Fixed32 surfNormK, MthXyz *p0, MthXyz *p1,
					 MthXyz *p2, MthXyz *normal);

/*
 * ������������
 */
void    MTH_InitialRand(Uint32 initVal);
Uint32  MTH_GetRand(void);

/*
 * �X�v���C���Ȑ��v�Z����
 */
typedef struct {
	Uint32 num;
	Fixed32 *MTH_crvh;
	Fixed32 *MTH_crvd;
	Fixed32 *MTH_crvx;
	Fixed32 *MTH_crvy;
	Fixed32 *MTH_crvz;
	Fixed32 *MTH_crvp;
	Fixed32 *MTH_crva;
	Fixed32 *MTH_crvb;
	Fixed32 *MTH_crvc;
} MthCurveWork;

#define MTH_INIT_CURVE(wk, n) \
static Fixed32 MTH_crvh[n]; \
static Fixed32 MTH_crvd[n]; \
static Fixed32 MTH_crvx[n]; \
static Fixed32 MTH_crvy[n]; \
static Fixed32 MTH_crvz[n]; \
static Fixed32 MTH_crvp[n]; \
static Fixed32 MTH_crva[n]; \
static Fixed32 MTH_crvb[n]; \
static Fixed32 MTH_crvc[n]; \
static MthCurveWork wk = { n, MTH_crvh, MTH_crvd, MTH_crvx, MTH_crvy, MTH_crvz, MTH_crvp, MTH_crva, MTH_crvb, MTH_crvc }

Uint32  MTH_Curve2(MthCurveWork *work, MthXy *in_aray,
 Uint32 in_n, Uint32 step, MthXy *out_aray);
Uint32  MTH_Curve2t(MthCurveWork *work, MthXy *in_aray, Uint32 in_n,
 Uint32 step, MthXy *out_aray, MthXy *tan_aray);
Uint32  MTH_Curve3(MthCurveWork *work, MthXyz *in_aray,
 Uint32 in_n, Uint32 step, MthXyz *out_aray);
Uint32  MTH_Curve3t(MthCurveWork *work, MthXyz *in_aray, Uint32 in_n,
 Uint32 step, MthXyz *out_aray, MthXyz *tan_aray);
Fixed32 MTH_Sqrt(Fixed32 x);
Fixed32 MTH_Hypot(Fixed32 x, Fixed32 y);

/*
 * �Œ菬���_���Z�̊֐��ƃ}�N����`
 */

#define MTH_FLOAT(x)          ((Float32)((long)(x) / 65536.0))
#define MTH_FIXED(x)	      ((Fixed32)((x) * 65536.0))
Fixed32 MTH_IntToFixed(Sint32 x);
Sint32  MTH_FixedToInt(Fixed32 x);
Fixed32 MTH_Mul(Fixed32 a, Fixed32 b);
Fixed32 MTH_Div(Fixed32 a, Fixed32 b);
Fixed32 MTH_Product(Fixed32 *a, Fixed32 *b);

#endif /* ifndef SEGA_MTH_H */

/*  end of file */
