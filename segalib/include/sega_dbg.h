/*----------------------------------------------------------------------------
 *  sega_dbg.h -- DBG ���C�u���� �C���N���[�h�t�@�C��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-05-23 Ver.0.90
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  �f�o�b�O�T�|�[�g���C�u�����̃C���N���[�h�t�@�C��
 *
 *----------------------------------------------------------------------------
 */

#ifndef SEGA_DBG_H
#define SEGA_DBG_H

#include "sega_xpt.h"
#include "sega_def.h"
#define _MD_PAD
#define _STN_PAD
#include "sega_per.h" 

/*
 * EXTERNAL FUNCTION PROTOTYPE DECLARATIONS
 */
void DBG_Initial (Uint16 *padData, Rgb16 charColor, Rgb16 backColor);
void DBG_DisplayOn(void);
void DBG_DisplayOff(void);
void DBG_ClearScreen(void);
void DBG_SetCursol(Uint16 x, Uint16 y);
void DBG_Printf(const char *control, ...);
char *DBG_GetKeyStr(char *keyStr);
void DBG_Monitor(void);

#endif /* ifndef SEGA_DBG_H */

/*  end of file */
