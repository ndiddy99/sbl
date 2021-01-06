/*----------------------------------------------------------------------------
 *  mth_dsps.c -- MTH ���C�u���� �c�r�o�ɂ��R�c���W�ϊ����� ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  ���̃��C�u�����̓V�~�����[�V�����p�c�r�o�ɂ��R�c���W�ϊ��������W���[����
 *  �ȉ��̃��[�`�����܂ށB
 *
 *  MTH_PolyDataTransInit     -  ���W�ϊ������̏�����
 *  MTH_PolyDataTransExec     -  ���W�ϊ��������s
 *  MTH_PolyDataTransCheck    -  ���W�ϊ����������`�F�b�N
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_mth.h"
#ifdef _SH
#include <machine.h>
#endif

static MthMatrix *coordMatrix;
static MthXyz    *lightVector;

static void surfaceBright(Uint16 entryCount, MthXyz *surfacePoint,
			  MthXyz *surfaceNormal, Sint32 *surfaceBright);
static void coordTrans(Uint16 entryCount, MthXyz *src, MthXyz *ans);
static void vertBright(Uint16 entryCount, MthXyz *vertNormal,
					  Sint32 *pointBright);



/*************************************************************************
 *
 * NAME : MTH_PolyDataTransInit  -  Initialize Coord Transfer by DSP
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	�c�r�o�̏������ƍ��W�ϊ������v���O�����̃��[�h�B
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 *
 *************************************************************************
 */
void    MTH_PolyDataTransInit(void)
{
	/** BEGIN ************************************************************/
}


/*************************************************************************
 *
 * NAME : MTH_PolyDataTransExec  -  Execute Coord Transfer by DSP
 *
 * PARAMETERS
 *
 *     (1) MthPolyTransParm *polyTransParm  - <i/o>  ���W�ϊ��p�����[�^�e�[�u��
 *
 * DESCRIPTION
 *
 *	�R�c�X�v���C�g�\�����C�u�����ŗp���Ă���|���S���f�[�^��
 *      �i�R�c�I�u�W�F�N�g�j�ɂ��Ď��̈�A�̏������c�r�o�ɂčs���B
 *
 *      �E�|���S���ʂ̉B�ʔ���ƋP�x�v�Z
 *      �E���_�̎��_���W�n�ւ̕ϊ�����
 *      �E�O�[���[�\���p���_�̋P�x�v�Z�i�I�v�V�����j
 *      �E���_�̃��[���h���W�n�ւ̕ϊ������i�I�v�V�����j
 *
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    MTH_PolyDataTransExec(MthPolyTransParm *polyTransParm)
{
    /** BEGIN ************************************************************/
    coordMatrix = &polyTransParm->viewLight->viewMatrix;
    lightVector = &polyTransParm->viewLight->lightVector;

    /* �ʖ@���e�[�u������ʋP�x�e�[�u���𓾂� */
    surfaceBright(polyTransParm->surfCount,
		  polyTransParm->surfPoint,
		  polyTransParm->surfNormal,
		  polyTransParm->surfBright);

    /* ���_�f�[�^�̎��_���W�ϊ���e�[�u���𓾂� */
    coordTrans(polyTransParm->transViewVertCount,
	       polyTransParm->transViewVertSrc,
	       polyTransParm->transViewVertAns);

    /* �O�[���[�p���_�P�x�e�[�u���𓾂�  */
    if(polyTransParm->gourVertCount)
	vertBright(polyTransParm->gourVertCount,
		   polyTransParm->vertNormal,
		   polyTransParm->vertBright);

    /* ���_�f�[�^�̃��[���h���W�ϊ���e�[�u���𓾂� */
    if(polyTransParm->transWorldVertCount) {
	coordMatrix = &polyTransParm->viewLight->worldMatrix;
	coordTrans(polyTransParm->transWorldVertCount,
		   polyTransParm->transWorldVertSrc,
		   polyTransParm->transWorldVertAns);
    }
}


static void surfaceBright(Uint16 entryCount, MthXyz *surfacePoint,
			  MthXyz *surfaceNormal, Sint32 *surfaceBright)
{
	Sint32	i,j,color;
	MthXyz  viewVect, tSurfPoint, tSurfNormal, *wSurfPoint;

	wSurfPoint = surfacePoint;
	for(i=0;i<entryCount;i++)
	{
	    MTH_CoordTrans(coordMatrix, wSurfPoint, &tSurfPoint);
	    viewVect.x = - tSurfPoint.x;
	    viewVect.y = - tSurfPoint.y;
	    viewVect.z = - tSurfPoint.z;
	    MTH_NormalTrans(coordMatrix, surfaceNormal++, &tSurfNormal);
	    color = MTH_ComputeBright(&tSurfNormal,lightVector);
	    if (MTH_Product((Fixed32*)&viewVect,
			    (Fixed32*)&tSurfNormal) <= 0)
		color |= 0xffff0000;
	    *surfaceBright++ = color;
	    wSurfPoint++;
	}
}


static void coordTrans(Uint16 entryCount, MthXyz *src, MthXyz *ans)
{
	Sint32	i;

	for(i=0;i<entryCount;i++)
	    MTH_CoordTrans(coordMatrix, src++, ans++);
}


static void vertBright(Uint16 entryCount, MthXyz *vertNormal,
					  Sint32 *pointBright)
{
	Uint16	i;
	MthXyz  ans;
	Sint16  color;

	for(i=0;i<entryCount;i++)
	{
	     MTH_NormalTrans(coordMatrix, vertNormal++, &ans);
	     color = MTH_ComputeBright(&ans,lightVector);
	     *pointBright++ = color;
	}
}


/*************************************************************************
 *
 * NAME : MTH_PolyDataTransCheck  -  Check Coord Transfer Complete
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	�c�r�o�ɂ����W�ϊ���������������܂ő҂B
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    MTH_PolyDataTransCheck(void)
{
	/** BEGIN ************************************************************/
}


/*  end of file */
