/*----------------------------------------------------------------------------
 *  mth_dsp.c -- MTH ���C�u���� �c�r�o�ɂ��R�c���W�ϊ����� ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-03-29 Ver.0.80
 *  Updated by H.E on 1994-03-29 Ver.0.80
 *
 *  ���̃��C�u�����͂c�r�o�ɂ��R�c���W�ϊ��������W���[����
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
#include <string.h>
#include "sega_mth.h"
#include "sega_dsp.h"
#include "sega_csh.h"
#ifdef _SH
#include <machine.h>
#endif

static Uint32 dspProgram[] = {
#include "mth_dspp.cod"
};

static MthPolyTransParm iPolyTransParm;


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
    DSP_LoadProgram(0, dspProgram, 256);
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
    MthPolyTransParm wPolyTransParm;
    Uint32 w;

    /** BEGIN ************************************************************/
#if 0
    if(polyTransParm->surfCount)
       CSH_Purge(polyTransParm->surfBright,
                 polyTransParm->surfCount*sizeof(Sint32));
    if(polyTransParm->transViewVertCount)
       CSH_Purge(polyTransParm->transViewVertAns,
                 polyTransParm->transViewVertCount*sizeof(MthXyz));
    if(polyTransParm->gourVertCount)
       CSH_Purge(polyTransParm->vertBright,
                 polyTransParm->gourVertCount*sizeof(Sint32));
    if(polyTransParm->transWorldVertAns)
       CSH_Purge(polyTransParm->transWorldVertAns,
                 polyTransParm->transWorldVertCount*sizeof(MthXyz));
#endif
    memcpy(&wPolyTransParm,polyTransParm,sizeof(MthPolyTransParm));

    w = ((Uint32)wPolyTransParm.viewLight        ) >> 2;
    wPolyTransParm.viewLight         = (MthViewLight*)w;
    w = ((Uint32)wPolyTransParm.surfPoint        ) >> 2;
    wPolyTransParm.surfPoint	     = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.surfNormal       ) >> 2;
    wPolyTransParm.surfNormal        = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.surfBright       ) >> 2;
    wPolyTransParm.surfBright        = (Sint32*)w;
    w = ((Uint32)wPolyTransParm.transViewVertSrc ) >> 2;
    wPolyTransParm.transViewVertSrc  = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.transViewVertAns ) >> 2;
    wPolyTransParm.transViewVertAns  = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.vertNormal       ) >> 2;
    wPolyTransParm.vertNormal        = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.vertBright       ) >> 2;
    wPolyTransParm.vertBright        = (Sint32*)w;
    w = ((Uint32)wPolyTransParm.transWorldVertSrc) >> 2;
    wPolyTransParm.transWorldVertSrc = (MthXyz*)w;
    w = ((Uint32)wPolyTransParm.transWorldVertAns) >> 2;
    wPolyTransParm.transWorldVertAns = (MthXyz*)w;
    DSP_WriteData(DSP_RAM_0 | 0, (Uint32*)&wPolyTransParm,
                                              sizeof(MthPolyTransParm)/4);
    DSP_Start(0);
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
    while(DSP_CheckEnd() == DSP_NOT_END); 
}

/*  end of file */
