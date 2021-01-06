/*	Copyright(c) 1994 SEGA			*/
/*--------------------------------------------------------------------------
 * �A�N�V�����Ǘ�
 *--------------------------------------------------------------------------
 * �A�N�V�����̓Q�[���̃L�����N�^�Ȃǂň�x�ɑ����̃v���O�����𓮂�������
 * �Ƃ��Ɏg���܂��B
 *--------------------------------------------------------------------------
 */


#include "smp_bios.h"



ACTWK SMTA_ActionBuf[ACTWKMAX];
ACTWK SMTA_DummyActwk;


/*--------------------------------------------------------------
 *  �A�N�V�������[�N�̏�����
 *--------------------------------------------------------------
 *  �z��̕K�v�ŏ����̓��e�����N���A���܂���
 *  
 *  
 *--------------------------------------------------------------
 */
void SMTA_ActWkInit(void)
{
    ACTWK *ix;
    Uint16 i;

    ix = &SMTA_ActionBuf[0];
    for (i = 0; i < ACTWKMAX; i++) {
        ix->actid = 0;
        ix->pcbuff = NULL;
        ix->mode = 0;
        ix->status = 0;
        ix->level = 0;
        ix++;
     }
   
}




/*--------------------------------------------------------------
 *  �A�N�V�����𔭐�������(���ׂẴA�N�V�����z����T�[�`����)
 *--------------------------------------------------------------
 *  IN : ���s�A�h���X
 *  OUT: �A�N�V�����z�񂪊m�ۂł�����A���̔z��̃A�h���X��
 *       �m�ۂł��Ȃ��Ƃ��́ASMTA_Dummy_Actwk�̃|�C���^��Ԃ�
 *--------------------------------------------------------------
 */
void* SMTA_MakeAction(void *exeadd)
{
    ACTWK *iy;
    Uint16 i;

    iy = &SMTA_ActionBuf[0];
    for (i = 0; i < ACTWKMAX; i++) {
         if (iy->actid == 0) {
             iy->actid = 1;
             iy->pcbuff = exeadd;
             iy->mode = 0;
             iy->status = 0;
             iy->level = 0;
             return iy;
         }
         iy++;
     }
     return &SMTA_DummyActwk;
}




/*--------------------------------------------------------------
 *  �A�N�V�����𔭐�������(�T�[�`�G���A�A�T�[�`�J�E���g�w��)
 *--------------------------------------------------------------
 *  IN : ���s�A�h���X�A�T�[�`���J�n����z��̃C���f�b�N�X
 *       �T�[�`�J�E���^���Z�b�g����
 *  OUT: �A�N�V�����z�񂪊m�ۂł�����A���̔z��̃A�h���X��
 *       �m�ۂł��Ȃ��Ƃ��́ASMTA_Dummy_Actwk�̃|�C���^��Ԃ�
 *--------------------------------------------------------------
 */
void* SMTA_MakeActionX(void *exeadd, Uint8 start, Uint8 count)
{
    ACTWK *iy;
    Uint16 i;

    iy = &SMTA_ActionBuf[start];
    for (i = 0; i < count; i++) {
         if (iy->actid == 0) {
             iy->actid = 1;
             iy->pcbuff = exeadd;
             iy->mode = 0;
             iy->status = 0;
             iy->level = 0;
             return iy;
         }
         iy++;
     }
     return &SMTA_DummyActwk;
}


/*--------------------------------------------------------------
 *  �A�N�V�����𔭐�������(�w�肵���z��ɋ����ɃZ�b�g����)
 *--------------------------------------------------------------
 *  IN : ���s�A�h���X�A�Z�b�g����z��̃C���f�b�N�X
 *  OUT: �m�ۂ����z��̃A�h���X��Ԃ�
 *--------------------------------------------------------------
 */
void* SMTA_SetAction(void *exeadd, Uint8 start)
{
    ACTWK *iy;
    Uint16 i;

    iy = &SMTA_ActionBuf[start];
    iy->actid = 1;
    iy->pcbuff = exeadd;
    iy->mode = 0;
    iy->status = 0;
    iy->level = 0;
    return iy;
}






/*--------------------------------------------------------------
 *  �A�N�V���������s����
 *--------------------------------------------------------------
 *  �z��̐擪����actid���O���ǂ������ׂĂO�łȂ��Ƃ�
 *   pcbuff�̃A�h���X���R�[�����܂�
 *--------------------------------------------------------------
 */
void SMTA_ActionLoop(void)
{
    ACTWK *ix;

    ix = &SMTA_ActionBuf[0];

    do {
        if (ix->actid != 0) {        /* actid���O���ǂ���    */
            if (ix->pcbuff != NULL)  /* pcbuff��NULL���ǂ��� */
                (ix->pcbuff)(ix);    /* pcbuff����R�[������ */
        }
        ix++;
    } while (ix != &SMTA_ActionBuf[ACTWKMAX]);

}





