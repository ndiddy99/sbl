/********************************************************************
*  FILE:	btest.c
*
*	Copyright(c) 1994 SEGA
*
*  PURPOSE:
*	「バックアップライブラリ」テストプログラム
*
*  AUTHOR(S):
*	K.M
*		
*  MOD HISTORY:
*	Written by K.M on 1994-08-27 Ver.1.00
*	Updated by K.M on 1994-09-12 Ver.1.00
*	Updated by M.K on 1994-09-12 Ver.1.10
********************************************************************/

#include	<stdio.h>
#include	<machine.h>
#include	"sega_scl.h"
#include	"sega_per.h"
#include	"sega_bup.h"
#include	"..\v_blank\v_blank.h"
#include	"font\smp_font.h"

#define 	BUP_START_ADDR	0x6070000

#define		CLOCK_26	0
#define		CLOCK_28	1


void	DispInit(Uint32 clock);
Uint32	MainMenu(Uint32 *sel);
void	PadInputWait(void);
void	ClrVram(Uint8 *buff);
void	err_disp(Sint32 err,Sint32 sw);
void	BackUpInit(BupConfig cntb[3]);
Sint32	BackUpWrite(Uint32 device,BupDir *dir,Uint8 *data,Uint8 sw);
Sint32	BackUpDelete(Uint32 device,Uint8 *filename);
Sint32	BackUpFormat(Uint32 device);
void	DispDate(char *comment,Uint16 x,Uint16 y);

	Uint32	BackUpRamWork[2048];

	BupConfig	CnTb[3];
	BupDir		DirTb[50];
	BupDate		DateTb;
	BupStat		StTb;
	Uint32		device;
	Uint32		clock;


Uint8	DummyData[] = "Dummy Data";
Uint8	DummyData2[] = "Bench Test";

Uint8	Week[7][4] = {
	"SUN",
	"MON",
	"TUE",
	"WED",
	"THU",
	"FRI",
	"SAT"
};

void main(void)
{
	Uint32		i,fp;
	Sint32		num,ret;
	Uint32		sel,dummy,size,count;
	Uint16		part;
	Uint8		stat,work;
	Uint8		buff[100];


	i=0;
	dummy = 0;
	sel   = 0;
	part  = 0;
	size  = 0;

	clock = CLOCK_26;
	DispInit(clock);

	/* バックアップライブラリ初期化 */
	BackUpInit(CnTb);

	while(1) switch(MainMenu(&sel)){
		case	1:
			/* バックアップライブラリ初期化 */
			BackUpInit(CnTb);
			ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);
			sprintf((char *)buff,"Main Unit:Id=%d,Part=%d",CnTb[0].unit_id,CnTb[0].partition);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,40,2,0);
			sprintf((char *)buff,"Cartridge:Id=%d,Part=%d",CnTb[1].unit_id,CnTb[1].partition);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,60,2,0);
			sprintf((char *)buff,"Serial   :Id=%d,Part=%d",CnTb[2].unit_id,CnTb[2].partition);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,80,2,0);
			part=0;
			PadInputWait();/* パッド入力待ち */
			dummy = 0;
			break;
		case	2:
			/* パーティション選択 */
			if(device==2){
				if(part==0)	part = 1;
				else		part = 0;
				err_disp( BUP_SelPart(device,part),ON);
			}
			break;
		case	3:
			/* Ｆｏｒｍａｔ */
			ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);
			sprintf((char *)buff,"Format ");
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 40,2,0);
			err_disp( BackUpFormat(device),ON);
			break;
		case	4:
			/* ステイタス取得 */
			err_disp( BUP_Stat(device,10,&StTb),OFF);
			sprintf((char *)buff,"Total Size  :%10ld",StTb.totalsize);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 40,2,0);
			sprintf((char *)buff,"Total Block :%10ld",StTb.totalblock);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 60,2,0);
			sprintf((char *)buff,"Block Size  :%10ld",StTb.blocksize);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 80,2,0);
			sprintf((char *)buff,"Free Size   :%10ld",StTb.freesize);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,100,2,0);
			sprintf((char *)buff,"Free Block  :%10ld",StTb.freeblock);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,120,2,0);
			sprintf((char *)buff,"Data Num    :%10ld",StTb.datanum);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20,140,2,0);
			PadInputWait();
			break;
		case	5:
			/* ファイル書き込み */
			sprintf((char *)DirTb[0].filename,"DUMMYDAT%03d",dummy);
			sprintf((char *)DirTb[0].comment,"ｺﾒﾝﾄ%03d",dummy);
			DirTb[0].language = BUP_JAPANESE;
			DirTb[0].datasize = sizeof(DummyData);
			DirTb[0].date = 0;
			err_disp(BackUpWrite(device,&DirTb[0],DummyData,(Uint8 )ON),ON);
			dummy++;
			break;
		case	6:
			/* ファイル読み込み */
			err_disp( BUP_Read(device,DirTb[fp].filename,(Uint8 *)0x6080100),OFF);
			work = ((Uint8 *)0x6080100)[100];
			((Uint8 *)0x6080100)[100] = 0;
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,(Uint8 *)0x6080100,20,40,2,0);
			PadInputWait();
			((Uint8 *)0x6080100)[100] = work;
			break;
		case	7:
			/* ファイルの削除 */
			err_disp( BackUpDelete(device,DirTb[fp].filename),ON );
			break;
		case	8:
			/* ファイル一覧取得 */
			num = BUP_Dir(device,(Uint8 *)"",50,DirTb);
			ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);

			sprintf((char *)buff,"Hit:%08ld",num);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,10,2,0);
			if(!num)	PadInputWait();

			fp = 0;
			for(i=0;i<num;i++){
				sprintf((char *)buff,"FileName :%11s",DirTb[i].filename);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 40,2,0);
				sprintf((char *)buff,"Comment  :%10s",DirTb[i].comment);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 60,2,0);
				sprintf((char *)buff,"Language :%10d",DirTb[i].language);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 80,2,0);
				BUP_GetDate(DirTb[i].date,&DateTb);
				sprintf((char *)buff,"Date     :%04d-%02d-%02d(%s)  %2d:%02d",
						((Uint16)DateTb.year)+1980,
						DateTb.month,DateTb.day,
						Week[DateTb.week],
						DateTb.time,DateTb.min);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 100,2,0);
				sprintf((char *)buff,"Size     :%10ld",DirTb[i].datasize);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 120,2,0);
				sprintf((char *)buff,"BlockSize:%10ld",DirTb[i].blocksize);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,20, 140,2,0);
				PadInputWait();
				if(PadData1 & PAD_B){
					fp=i;
					i=num;
				}
			}
			break;
		case	9:
			/* ベリファイ */
			err_disp( BUP_Verify(device,DirTb[fp].filename,(Uint8 *)0x6080100),ON);
			break;
		case	10:
			/* クロックチェンジ */
			if(clock == CLOCK_26)	clock = CLOCK_28;
			else			clock = CLOCK_26;
			DispInit(clock);
			BackUpInit(CnTb);
			break;
		case	15:
			BUP_Stat(device,0,&StTb);
			size = StTb.freesize;
		case	11:if(!size)	size =    100;
		case	12:if(!size)	size =   1000;
		case	13:if(!size)	size =  10000;
		case	14:if(!size)	size = 100000;
			/* ファイル書き込み */
			sprintf((char *)DirTb[0].filename,"DUMMYDAT%03d",dummy);
			sprintf((char *)DirTb[0].comment,"ｺﾒﾝﾄ%03d",dummy);
			DirTb[0].language = BUP_JAPANESE;
			DirTb[0].datasize = size;
			size = 0;
			DirTb[0].date = 0;
			err_disp(BackUpWrite(device,&DirTb[0],(Uint8 *)0x00200000,(Uint8 )ON),ON);
			dummy++;
			break;
		case	16:
			/* ファイルの無限オーバーライト */
			ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"ﾊﾞｯｸｱｯﾌﾟﾗｲﾌﾞﾗﾘ Test Program",40,10,2,0);
			dummy = 0;
			count = 0;
			DispDate("Start:",50,100);

			DirTb[0].language = BUP_JAPANESE;
			DirTb[0].datasize = sizeof(DummyData2);
			DirTb[0].date = 0;
			PadData1E = 0;
			while( !(PadData1E & PAD_B) ){
				sprintf((char *)DirTb[0].filename,"DUMMYDA%04d",dummy);
				sprintf((char *)DirTb[0].comment,"ｺﾒﾝﾄ%04d",dummy);
				ret = BackUpWrite(device,&DirTb[0],DummyData2,(Uint8 )OFF);
				sprintf((char *)buff,"Write :%s",DirTb[0].filename);
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,40,2,0);
				if(ret == BUP_NOT_ENOUGH_MEMORY){
					count++;
					sprintf((char *)buff,"Count :%11ld",count);
					FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,60,2,0);
					if( (PadData1E & PAD_C) )	PadData1E |= PAD_B;
					else for(i=0;i<dummy;i++){
						sprintf((char *)DirTb[0].filename,"DUMMYDA%04d",i);
						BackUpDelete(device,DirTb[0].filename);
						sprintf((char *)buff,"Delete:%s",DirTb[0].filename);
						FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,40,2,0);
					}
					DispDate("Now  :",50,120);
					sprintf((char *)buff,"Sector:%11ld",dummy);
					FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,80,2,0);
					dummy = 0;
					if( (PadData1E & PAD_A) )
						PadData1E |= PAD_B;
				}
				else	dummy++;
			}
			DispDate("End  :",50,120);
			PadInputWait();
			break;
	}
}


void	ClrVram(Uint8 *buff)
{
	Uint32	i;

	for(i=0;i<(512 * 256);i++)	buff[i]=0x00;
}


static	Uint32	color[7] = {
	0x00000000,/* 透明色になるところだから何を入れても良い */
	0x00000000,/* 黒 */
	0x00FFFFFF,/* 白 */
	0x00FF0000,/* 青 */
	0x0000FF00,/* 緑 */
	0x000000FF,/* 赤 */
	0x00008000 /* 深緑 */
};

#define	BLACK	1
#define	WHITE	2
#define	BLUE	3
#define	GREEN	4
#define	RED	5
#define	DGREEN	6


void	DispInit(Uint32 clock)
{
	Uint32		i;
	SclConfig	Nbg0Scfg;
	Uint16		BackCol;

	*(Uint16 *)0x25F80000 &= 0x7fff;/* 画面表示をＯＦＦにする */
	SCL_Vdp2Init();
	if(clock == CLOCK_26)
		SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
	else	SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_B);
	PER_SMPC_RES_ENA();/* リセットボタン有効 */

	SCL_SetColRamMode(SCL_CRM24_1024);
	BackCol = RGB16_COLOR(0,0,0) & 0x7fff;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	FNT_SetBuffSize(512,256,FNT_JAPAN);
	SetVblank();
	set_imask(0);
	SCL_SetFrameInterval(1);
	SCL_DisplayFrame();

	SCL_AllocColRam(SCL_NBG0,256,OFF);
	SCL_SetColRam(SCL_NBG0,0,7,color);

	SCL_SetPriority(SCL_NBG0,5);

	/* サイクルパターン設定 */
	Scl_s_reg.vramcyc[0] = 0x44FF;	/* NBG0 256 Bitmap */

	SCL_InitConfigTb(&Nbg0Scfg);
	    Nbg0Scfg.dispenbl = ON;
	    Nbg0Scfg.coltype  = SCL_COL_TYPE_256;
	    Nbg0Scfg.datatype = SCL_BITMAP;
	    for(i=0;i<4;i++)
		Nbg0Scfg.plate_addr[i] = SCL_VDP2_VRAM_A0;
	SCL_SetConfig(SCL_NBG0, &Nbg0Scfg);

	/**************************************
	*	スクロール画面の初期描画      *
	**************************************/
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0), FIXED(0));
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
	SCL_DisplayFrame();
}


Uint32	MainMenu(Uint32	*sel)
{
	Uint32	sw,i;
	Uint16	back[20];
	Uint16	PadData1EW;

	ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);
	FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)"ﾊﾞｯｸｱｯﾌﾟﾗｲﾌﾞﾗﾘ Test Program",40,10,2,0);
	for(i=0;i<16;i++)	back[i] = 0;

	PadData1E  = 0;
	PadData1EW = PadData1E;
	sw=1;
	while( !(PadData1EW & (PAD_A | PAD_B | PAD_C)) ){
		PadData1EW = PadData1E;
		PadData1E  = 0;

		if(PadData1 & PAD_X){
			device = 0;
			sw = 1;
		}
		if(PadData1 & PAD_Y){
			device = 1;
			sw = 1;
		}
		if(PadData1 & PAD_Z){
			device = 2;
			sw = 1;
		}

		if(PadData1EW & PAD_R){
			if(*sel < 8)	*sel += 8;
			sw=1;
		}else if(PadData1EW & PAD_L){
			if(*sel >= 8)	*sel -= 8;
			sw=1;
		}else if(PadData1EW & PAD_U){
			if(*sel < 8)	*sel=(*sel+7)%8;
			else		*sel=8+(*sel-8+7)%8;
			sw=1;
		}else if(PadData1EW & PAD_D){
			if(*sel < 8)	*sel=(*sel+1)%8;
			else		*sel=8+(*sel-8+1)%8;
			sw=1;
		}

		if(sw){
			if(clock == CLOCK_26)
				FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
					(Uint8 *)"CPU Clock:26MHz", 20, 40,GREEN,0);
			else	FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
					(Uint8 *)"CPU Clock:28MHz", 20, 40,BLACK,GREEN);

			switch(device){
				case	0:
					back[*sel] = BLUE;
					FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
						(Uint8 *)"ﾅｲｿﾞｳ ﾒﾓﾘ ",200, 40,WHITE,0);
					break;
				case	1:
					back[*sel] = DGREEN;
					FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
						(Uint8 *)"ｶｰﾄﾘｯｼﾞ   ",200, 40,WHITE,0);
					break;
				case	2:
					back[*sel] = RED;
					FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
						(Uint8 *)"ｼﾘｱﾙ      ",200, 40,WHITE,0);
					break;
			}

			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Init    ", 20, 60,WHITE,back[ 0]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_SelPart ", 20, 80,WHITE,back[ 1]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Format  ", 20,100,WHITE,back[ 2]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Stat    ", 20,120,WHITE,back[ 3]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Write   ", 20,140,WHITE,back[ 4]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Read    ", 20,160,WHITE,back[ 5]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Delete  ", 20,180,WHITE,back[ 6]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Dir     ", 20,200,WHITE,back[ 7]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"BUP_Verify  ",180, 60,WHITE,back[ 8]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Clock Change",180, 80,WHITE,back[ 9]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy    100",180,100,WHITE,back[10]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy   1000",180,120,WHITE,back[11]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy  10000",180,140,WHITE,back[12]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy 100000",180,160,WHITE,back[13]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy   FULL",180,180,WHITE,back[14]);
			FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,
				(Uint8 *)"Dummy  Bench",180,200,WHITE,back[15]);
			back[*sel] = 0;
			sw = 0;
		}
		SCL_DisplayFrame();
	}
	return(*sel+1);
}

/* パッド入力待ち */
void	PadInputWait(void)
{
	PadData1E = 0;
	while( !(PadData1E) );
}

void	err_disp(Sint32 err,Sint32 wait)
{
	Uint8	buff[40];

	ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);

	switch(err){
		case	BUP_NON:
			sprintf((char *)buff,"BUP_NON");
			break;
		case	BUP_UNFORMAT:
			sprintf((char *)buff,"BUP_UNFORMAT");
			break;
		case	BUP_WRITE_PROTECT:
			sprintf((char *)buff,"BUP_WRITE_PROTECT");
			break;
		case	BUP_NOT_ENOUGH_MEMORY:
			sprintf((char *)buff,"BUP_NOT_ENOUGH_MEMORY");
			break;
		case	BUP_NOT_FOUND:
			sprintf((char *)buff,"BUP_NOT_FOUND");
			break;
		case	BUP_FOUND:
			sprintf((char *)buff,"BUP_FOUND");
			break;
		case	BUP_NO_MATCH:
			sprintf((char *)buff,"BUP_NO_MATCH");
			break;
		case	BUP_BROKEN:
			sprintf((char *)buff,"BUP_BROKEN");
			break;
		default:
			sprintf((char *)buff,"ErrNO:%08x",err);
			break;
	}
	FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,50,10,2,0);
	if(wait)	PadInputWait();
}


/****************************/
/* 書き込み中断防止特別処理 */
/****************************/
void	BackUpInit(BupConfig cntb[3])
{
	PER_SMPC_RES_DIS();/* リセットボタン無効 */
	BUP_Init((Uint32 *)BUP_START_ADDR,BackUpRamWork,cntb);
	PER_SMPC_RES_ENA();/* リセットボタン有効 */
}

void	DispDate(char *comment,Uint16 x,Uint16 y)
{
	Uint8	*time;
	Uint8	buff[32];

	time = PER_GET_TIM();
	sprintf(buff,"%s%02x%02x-%02x-%02x %02x:%02x:%02x",comment,
			time[6],time[5],time[4] & 0x0F,time[3],
			time[2],time[1],time[0]);
	FNT_Print256((Uint8 *)SCL_VDP2_VRAM_A0,buff,x,y,2,0);
}


Sint32	BackUpWrite(Uint32 device,BupDir *dir,Uint8 *data,Uint8 sw)
{
	Sint32	ret;
	Uint8	*time;
	BupDate	date;

	/* 日付データが０だったら現在の日付を入力する */
	if(!dir->date){
		time = PER_GET_TIM();
		date.year = (Uint8 )( (Uint16 )(time[6]>>4) * 1000
				+ (Uint16 )(time[6] & 0x0F) * 100
				+ (Uint16 )(time[5]>>4)     * 10
				+ (Uint16 )(time[5] & 0x0F) - 1980);
		date.month = time[4] & 0x0F;
		date.day   = (time[3]>>4)*10 + (time[3] & 0x0F);
		date.time  = (time[2]>>4)*10 + (time[2] & 0x0F);
		date.min   = (time[1]>>4)*10 + (time[1] & 0x0F);
		dir->date = BUP_SetDate(&date);
	}

	PER_SMPC_RES_DIS();/* リセットボタン無効 */
	ret = BUP_Write(device,dir,data,sw);
	PER_SMPC_RES_ENA();/* リセットボタン有効 */

	return(ret);
}

Sint32	BackUpDelete(Uint32 device,Uint8 *filename)
{
	Sint32	ret;

	PER_SMPC_RES_DIS();/* リセットボタン無効 */
	ret = BUP_Delete(device,filename);
	PER_SMPC_RES_ENA();/* リセットボタン有効 */

	return(ret);
}

Sint32	BackUpFormat(Uint32 device)
{
	Sint32	ret;

	PER_SMPC_RES_DIS();/* リセットボタン無効 */
	ret = BUP_Format(device);
	PER_SMPC_RES_ENA();/* リセットボタン有効 */

	return(ret);
}
