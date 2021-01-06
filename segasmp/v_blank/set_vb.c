/*----------------------------------------------------------------------------
 *  Set_VB.c -- V-BlankäÑÇËçûÇ›ÉãÅ[É`ÉìÇÃìoò^
 *  Copyright(c) 1994 SEGA
 *  Written by K.M on 1994-05-16 Ver.0.90
 *  Updated by K.M on 1994-10-04 Ver.1.02
 *----------------------------------------------------------------------------
 */

#include <sega_xpt.h>
#include <sega_int.h>
#include <sega_per.h>

extern	void  UsrVblankStart(void);
extern	void  UsrVblankEnd(void);

extern	Uint32	PadWorkArea[7];

volatile Sint32	perFlag;

void   CheckVblankEnd(void);

void SetVblank(void)
{
	/* V_Blank Out äÑÇËçûÇ›Çë“Ç¬ */
	
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_OUT,CheckVblankEnd);
	INT_ChgMsk(INT_MSK_VBLK_OUT,INT_MSK_NULL);

	perFlag = 1;
	while(perFlag);
    
	PER_Init(PER_KD_PERTIM,2,PER_ID_DGT,PER_SIZE_DGT,PadWorkArea,0);

	/* V-BlankäÑÇËçûÇ›ÉãÅ[É`ÉìÇÃìoò^ */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_IN,UsrVblankStart);
	INT_SetScuFunc(INT_SCU_VBLK_OUT,UsrVblankEnd);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);
}


void   CheckVblankEnd(void)
{
	perFlag = 0;
}
