/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * SEGA共通定数＆マクロ
 *
 */

#ifndef __SMP_DEF_H
#define __SMP_DEF_H

#include <machine.h>

/*#####[ 定数宣言 ]#####*/
#define COMMENT     0                       /* コメント         */
#define NO          FALSE                   /* 違               */
#define YES         TRUE                    /* 正               */
#define DEBUG       YES                     /* デバッグスイッチ */
#define NUL         '\0'                    /* ヌル文字         */

#define SEC	60
#define WIDTH_H	320
#define WIDTH_V	224



/*#####[ マクロ宣言 ]#####*/
/*--------------------------------------------------------
    静的変数を一箇所に集めるためのマクロ 1993/12/13追加
  --------------------------------------------------------*/
#ifndef __SMP_WKRM_C
    #define GLOBAL extern
#else
    #define GLOBAL
#endif

#define const

/*--------------------------------------
    ＲＧＢコードを作り出すマクロ
    Uint32 rgb(Uint8, Uint8, Uint8)
    Uint32 rgb15(Uint8, Uint8, Uint8)
    Uint32 rgb24(Uint8, Uint8, Uint8)
  --------------------------------------*/
#define rgb(_r_, _g_, _b_) rgb15(_r_, _g_, _b_)

#define rgb15(_r_, _g_, _b_)						\
    (0x8000 | (int)(_b_)<<10 | (int)(_g_)<<5 | (int)(_r_))

#define rgb24(_r_, _g_, _b_)						\
    ((int)(_b_)<<16 | (int)(_g_)<<8 | (int)(_r_))


/*---------------------------------
    インタラプトを許可するマクロ
    void ei(void)
  ---------------------------------*/
#define ei() set_imask(0x00)


/*---------------------------------
    インタラプトを禁止するマクロ
    void di(void)
  --------------------------------*/
#define di() set_imask(0x0f)



/*---------------------------------
    メモリから直接読み込むマクロ
    Uint8  peek_b(volatile Uint8 *)
    Uint16 peek_w(volatile Uint16*)
    Uint32 peek_l(volatile Uint32*)
  --------------------------------*/
#define peek_b(_address_) (*(volatile Uint8 *)(_address_))
#define peek_w(_address_) (*(volatile Uint16*)(_address_))
#define peek_l(_address_) (*(volatile Uint32*)(_address_))



/*-----------------------------------
    メモリに直接書き込むマクロ
    Uint8  poke_b(volatile Uint8 *, Uint8 )
    Uint16 poke_w(volatile Uint16*, Uint16)
    Uint32 poke_l(volatile Uint32*, Uint32)
  -----------------------------------*/
#define poke_b(_address_, _data_) (*(volatile Uint8 *)(_address_) = (_data_))
#define poke_w(_address_, _data_) (*(volatile Uint16*)(_address_) = (_data_))
#define poke_l(_address_, _data_) (*(volatile Uint32*)(_address_) = (_data_))



/*-----------------------------------
    Vdp2書き込み後のウエイト
    void Vdp2Wait(void)
  -----------------------------------*/
#define Vdp2Wait() (set_imask(get_imask()))
#define VdpWait() (set_imask(get_imask()))




/*<<<<<<<< Coordinates >>>>>>>>*/
#define	X	0
#define	Y	1
#define	Z	2
#define	XY	2
#define	XYZ	3

#define H	0
#define V	1
#define HV	2




/*---------------------------------
    CGデータ定義用構造体集
  --------------------------------*/
typedef struct {
    Uint8 *cgadd;           /* CG DATA ADD                  */
    Uint32 cgsize;          /* CG DATA SIZE                 */                 
    Uint16 *clradd;         /* COLOR DATA ADDRESS           */
    Uint16 clrcnt;
    } SPRCG;
  
typedef struct {
    Uint8 *cgadd;                       /* CG DATA ADD                  */
    Sint32 cgsize;                      /* CG DATA SIZE                 */
    Uint16 *mapadd;                     /* MAP DATA ADD                 */
    Uint16 *clradd;                     /* COLOR DATA ADDRESS           */
    Uint16 clrcnt;
    Uint8 vsize;                        /* MAP Hcell SIZE               */
    Uint8 hsize;                        /* MAP Vcell SIZE               */
    } FIXCG;


typedef struct {
    Uint16 *mapadd;                     /* MAP DATA ADD                 */
    Uint8 vsize;                        /* MAP Hcell SIZE               */
    Uint8 hsize;                        /* MAP Vcell SIZE               */
    } BOXMAP;


typedef struct {
    Uint8 *cgadd;                       /* CG DATA ADD                  */
    Sint32 cgsize;                      /* CG DATA SIZE                 */                     
    } FONTCG;

typedef struct {
    Uint16 *cgadd;                       /* CG DATA ADD                  */
    Sint32 cgsize;                      /* CG DATA SIZE                 */                     
    } WORDBLOCK;


#endif







