/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   ウィンドウ機能を使ったサンプル
 *
 */


#ifndef __SMP_WIND_H
#define __SMP_WIND_H


/*
 *     アクションワーク構造体
 */
SMTA_DefActWk(WINDACT,                     /* ACTWK SYSTEM  0x10 bytes */
         SPRITE sprite;
         Sint16 wreg[4];                    /* etc work      0x08 bytes */
         Sint16 wcnt[4];                    /* etc work      0x08 bytes */
);


SMTA_DefActWk(NBWINDACT,                    /* ACTWK SYSTEM  0x10 bytes */
         Sint32 W0posi[2];      /* 窓０用XYポジション */
         Sint16 W0speed[2];     /* 窓０用XYスピード   */
         Sint16 W0size[2];      /* 窓０用XYサイズ     */
         Sint32 W1posi[2];      /* 窓１用XYポジション */
         Sint16 W1speed[2];     /* 窓１用XYスピード   */
         Sint16 W1size[2];      /* 窓１用XYサイズ     */
         Sint16 wcnt[4];        /* 汎用カウンタ       */
);


/*
 *     関数プロトタイプ
 */
void SMWI_WindMode(void);



#endif

