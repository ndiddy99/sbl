/*----------------------------------------------------------------------------
 *  V_Blank.c -- V-Blank割り込み処理内ルーチンサンプル
 *  Copyright(c) 1994 SEGA
 *  Written by K.M on 1994-05-16 Ver.1.00
 *  Updated by K.M on 1994-09-21 Ver.1.00
 *
 *  UsrVblankStart()	：V-Blank開始割り込み処理サンプル
 *  UsrVblankEnd()	：V-Blank終了割り込み処理サンプル
 *
 *----------------------------------------------------------------------------
 */

#include <machine.h>
#include <sega_spr.h>
#include <sega_scl.h>
#include <sega_per.h> 


volatile Uint16	PadData1  = 0x0000;
volatile Uint16	PadData1E = 0x0000;
volatile Uint16	PadData2  = 0x0000;
volatile Uint16	PadData2E = 0x0000;

Uint32	PadWorkArea[7];

void   UsrVblankStart(void)
{
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankStart();
}


void   UsrVblankEnd(void)
{
	PerDgtInfo	*pad;

	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankEnd();

	/* パッドデータ取得 */

	PER_GetPer((PerGetPer **)&pad);
	if( pad != NULL ){
		PadData1   = pad[0].data ^ 0xffff;
		PadData1E |= pad[0].push ^ 0xffff;
		PadData2   = pad[1].data ^ 0xffff;
		PadData2E |= pad[1].push ^ 0xffff;
	}

}
