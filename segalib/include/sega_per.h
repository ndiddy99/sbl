/*-----------------------------------------------------------------------------
 *  FILE: sega_per.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      システム／ペリフェラルライブラリ使用ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      システム／ペリフェラルライブラリを使用するプログラムにインクルードして
 *      ください。
 *
 *  AUTHOR(S)
 *
 *      1994-05-19  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *      1994-08-26  N.T Ver.1.02
 *
 *-----------------------------------------------------------------------------
 */

#ifndef SEGA_PER_H
#define SEGA_PER_H

/********************************* per_xpt.h *********************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/**** レジスタアドレス *******************************************************/
#ifndef _DEB2
#define PER_REG_COMREG  ((volatile Uint8 *)0x2010001f)   /* コマンドレジスタ          */
#define PER_REG_SR      ((volatile Uint8 *)0x20100061)   /* ステータスレジスタ        */
#define PER_REG_SF      ((volatile Uint8 *)0x20100063)   /* ステータスフラグ          */
#define PER_REG_IREG    ((volatile Uint8 *)0x20100001)   /* IREG                      */
#define PER_REG_OREG    ((volatile Uint8 *)0x20100021)   /* OREG                      */
#else
#define PER_REG_COMREG  ((Uint8 *)0x0603001f)   /* コマンドレジスタ          */
#define PER_REG_SR      ((Uint8 *)0x06030061)   /* ステータスレジスタ        */
#define PER_REG_SF      ((Uint8 *)0x06030063)   /* ステータスフラグ          */
#define PER_REG_IREG    ((Uint8 *)0x06030001)   /* IREG                      */
#define PER_REG_OREG    ((Uint8 *)0x06030021)   /* OREG                      */
#endif

/**** ビット位置 *************************************************************/
#define PER_B_SF        0x1                     /* ステータスフラグ          */

/**** 関数形式マクロ *********************************************************/
#define PER_PokeByte(address,data)  (*(volatile Uint8 *)(address) = (Uint8)(data))
                                                    /* ﾊﾞｲﾄﾃﾞｰﾀｱﾄﾞﾚｽ直接書き-*/
                                                    /* 込み                  */
#define PER_PeekByte(address)   (*(volatile Uint8 *)(address))
                                                    /* ﾊﾞｲﾄﾃﾞｰﾀｱﾄﾞﾚｽ直接読み-*/
                                                    /* 込み                  */

/*
 * TYPEDEFS
 */

/********************************* per_prt.h *********************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* ペリフェラルID */
#define PER_ID_NCON     0xff                    /* 未接続                    */
#define PER_ID_UNKOWN   0xf0                    /* SMPC UNKNOWN              */
#define PER_ID_DGT      0x00                    /* デジタルデバイス          */
#define PER_ID_ANL      0x10                    /* アナログデバイス          */
#define PER_ID_PNT      0x20                    /* ポインティングデバイス    */
#define PER_ID_KBD      0x30                    /* キーボード　　　          */
#define PER_ID_M3BP     0xe1                    /* メガドライブ3ボタンパッド */
#define PER_ID_M6BP     0xe2                    /* メガドライブ6ボタンパッド */

/* ペリフェラルサイズ */
#define PER_SIZE_DGT    2                       /* デジタルデバイス          */
#define PER_SIZE_ANL    5                       /* アナログデバイス          */
#define PER_SIZE_PNT    5                       /* ポインティングデバイス    */
#define PER_SIZE_KBD    4                       /* キーボード　　　          */
#define PER_SIZE_M3BP   1                       /* メガドライブ3ボタンパッド */
#define PER_SIZE_M6BP   2                       /* メガドライブ6ボタンパッド */

/* イントバック種別 */
#define PER_KD_SYS      0                       /* システムデータ取得        */
#define PER_KD_PER      1                       /* ペリフェラルデータ取得    */
#define PER_KD_PERTIM   2                       /* ペリフェラルデータ取得＋  */
                                                /* 時刻データ取得            */
/* ビット位置 ****************************************************************/
/* デジタルデバイスデータ型	 */
#define	PER_DGT_U   (1 << 12)                   /* UP                        */
#define	PER_DGT_D   (1 << 13)                   /* DOWN                      */
#define	PER_DGT_R   (1 << 15)                   /* RIGHT                     */
#define	PER_DGT_L   (1 << 14)                   /* LEFT                      */
#define	PER_DGT_A   (1 << 10)                   /* A                         */
#define	PER_DGT_B   (1 << 8)                    /* B                         */
#define	PER_DGT_C   (1 << 9)                    /* C                         */
#define	PER_DGT_S   (1 << 11)                   /* START                     */
#define	PER_DGT_X   (1 << 6)                    /* X                         */
#define	PER_DGT_Y   (1 << 5)                    /* Y                         */
#define	PER_DGT_Z   (1 << 4)                    /* Z                         */
#define	PER_DGT_TR  (1 << 7)                    /* トリガ RIGHT              */
#define	PER_DGT_TL  (1 << 3)                    /* トリガ LEFT               */

/* ポインティングデバイスデータ型 */
/***** データ */
#define PER_PNT_R   (1 << 1)                    /* RIGHT                     */
#define PER_PNT_L   (1 << 0)                    /* LEFT                      */
#define PER_PNT_MID (1 << 2)                    /* MIDDLE                    */
#define PER_PNT_CNT (1 << 3)                    /* CENTER                    */
#define PER_PNT_XO  (1 << 6)                    /* X軸オーバフロー           */
#define PER_PNT_YO  (1 << 7)                    /* Y軸オーバフロー           */

/* キーボードデバイスデータ型 */
/***** 特殊キー */
#define PER_KBD_CL  (1 << 6)                    /* Caps Lock                 */
#define PER_KBD_NL  (1 << 5)                    /* Num Lock                  */
#define PER_KBD_SL  (1 << 4)                    /* Scrool Lock               */
#define PER_KBD_MK  (1 << 3)                    /* Make                      */
#define PER_KBD_BR  (1 << 0)                    /* Break                     */

/* メガドライブ3ボタンパッドデータ型 */
#define	PER_M3BP_U  PER_DGT_U                   /* UP                        */
#define	PER_M3BP_D  PER_DGT_D                   /* DOWN                      */
#define	PER_M3BP_R  PER_DGT_R                   /* RIGHT                     */
#define	PER_M3BP_L  PER_DGT_L                   /* LEFT                      */
#define	PER_M3BP_A  PER_DGT_A                   /* A                         */
#define	PER_M3BP_B  PER_DGT_B                   /* B                         */
#define	PER_M3BP_C  PER_DGT_C                   /* C                         */
#define	PER_M3BP_S  PER_DGT_S                   /* START                     */

/* メガドライブ6ボタンパッドデータ型 */
#define	PER_M6BP_U  PER_DGT_U                    /* UP                        */
#define	PER_M6BP_D  PER_DGT_D                    /* DOWN                      */
#define	PER_M6BP_R  PER_DGT_R                    /* RIGHT                     */
#define	PER_M6BP_L  PER_DGT_L                    /* LEFT                      */
#define	PER_M6BP_A  PER_DGT_A                    /* A                         */
#define	PER_M6BP_B  PER_DGT_B                    /* B                         */
#define	PER_M6BP_C  PER_DGT_C                    /* C                         */
#define	PER_M6BP_S  PER_DGT_S                    /* START                     */
#define	PER_M6BP_X  PER_DGT_X                    /* X                         */
#define	PER_M6BP_Y  PER_DGT_Y                    /* Y                         */
#define	PER_M6BP_Z  PER_DGT_Z                    /* Z                         */
#define	PER_M6BP_MD PER_DGT_TR                   /* MODE                      */

/* システムデータ出力 */
/* システムステータス */
#define PER_SS_DOTSEL   (1 <<  6)               /* カートリッジコード        */
#define PER_SS_SYSRES   (1 <<  1)               /* エリアコード              */
#define PER_SS_MSHNMI   (1 <<  3)               /* システムステータス        */
#define PER_SS_SNDRES   (1 <<  0)               /* SMPCメモリ                */
#define PER_SS_CDRES    (1 << 14)               /* SMPCステータス            */

/* SMPCメモリ */
/***** マスク */
#define PER_MSK_LANGU   (0xf << 0)              /* 言語マスク                */
#define PER_MSK_SE      (0x1 << 8)              /* SE                        */
#define PER_MSK_STEREO  (0x1 << 9)              /* STEREO or MONO            */
#define PER_MSK_HELP    (0x1 << 10)             /* HELP                      */
/***** 言語 */
#define PER_ENGLISH     0x0                     /* English(英語)             */
#define PER_DEUTSCH     0x1                     /* Deutsch(ドイツ語)         */
#define PER_FRANCAIS    0x2                     /* francais(フランス語)      */
#define PER_ESPNOL      0x3                     /* Espnol(スペイン語)        */
#define PER_ITALIANO    0x4                     /* Italiano(イタリア語)      */
#define PER_JAPAN       0x5                     /* Japan(日本語)             */

/* SMPCステータス */
#define PER_SS_RESET    (1 <<  6)               /* リセットマスク状態        */
#define PER_SS_SETTIM   (1 <<  7)               /* 時刻設定状態              */

/* アクセスマクロ ************************************************************/
/* デジタルデバイスデータ型 */
#define PER_DGT(data)   ((PerDgtInfo *)(data))

/* アナログデバイスデータ型  */
#define PER_ANL(data)   ((PerAnlInfo *)(data))

/* ポインティングデバイスデータ型 */
#define PER_PNT(data)   ((PerPntInfo *)(data))

/* キーボードデバイスデータ型 */
#define PER_KBD(data)   ((PerKbdInfo *)(data))

/* メガドライブ3ボタンパッドデータ型 */
#define PER_M3BP(data)  ((PerM3bpInfo *)(data))

/* メガドライブ6ボタンパッドデータ型 */
#define PER_M6BP(data)  ((PerM6bpInfo *)(data))

/* システムデータ出力 */
#define PER_GS_CC(data)         ((data)->cc)    /* カートリッジコード        */
#define PER_GS_AC(data)         ((data)->ac)    /* エリアコード              */
#define PER_GS_SS(data)         ((data)->ss)    /* システムステータス        */
#define PER_GS_SM(data)         ((data)->sm)    /* SMPCメモリ                */
#define PER_GS_SMPC_STAT(data)  ((data)->stat)  /* SMPCステータス            */

/* 定数 **********************************************************************/
#define PER_HOT_RES_ON  0x1                     /* ホットリセットON          */
#define PER_HOT_RES_OFF 0x0                     /* ホットリセットOFF         */

/* イントバック実行状態      */
#define PER_INT_OK      0x0                     /* 正常                      */
#define PER_INT_ERR     0x1                     /* エラー                    */

/******************************************************************************
 *
 * NAME:    PER_GET_TIM             -   時刻取得
 *
 ******************************************************************************
 */
#define PER_GET_TIM()  (per_get_time_adr)

/******************************************************************************
 *
 * NAME:    PER_GET_SYS             -   システムデータ取得
 *
 ******************************************************************************
 */

#define PER_GET_SYS()   ((per_set_sys_flg == OFF) ? NULL : &per_get_sys_data)

#ifndef _PER_HOT_ENA
/******************************************************************************
 *
 * NAME:    PER_GET_HOT_RES         -   ホットリセット取得
 *
 ******************************************************************************
 */
#define PER_GET_HOT_RES()  (per_hot_res)
#endif  /* _PER_HOT_ENA */

/*
 * TYPEDEFS
 */
/* デバイスデータ型 */
typedef Uint16  PerDgtData;                     /* デジタルデバイスデータ型  */

typedef struct  {                               /* アナログデバイスデータ型  */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Sint16      x;                              /* X軸絶対値(0～255)         */
    Sint16      y;                              /* Y軸絶対値(0～255)         */
    Sint16      z;                              /* Z軸絶対値(0～255)         */
}PerAnlData;

typedef struct  {                       /* ポインティングデバイスデータ型    */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Uint16      data;                           /* データ                    */
    Sint16      x;                              /* X軸移動量(-128～127)      */
    Sint16      y;                              /* Y軸移動量(-128～127)      */
}PerPntData;

typedef struct  {                           /* キーボードデバイスデータ型    */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Uint8       skey;                           /* 特殊キー                  */
    Uint8       key;                            /* キー                      */
}PerKbdData;

typedef Uint8   PerM3bpData;            /* メガドライブ3ボタンパッドデータ型 */
typedef Uint16  PerM6bpData;            /* メガドライブ6ボタンパッドデータ型 */
typedef Uint8   PerId;                  /* ペリフェラルID                    */
typedef Uint8   PerSize;                /* ペリフェラルサイズ                */
typedef Uint8   PerKind;                        /* イントバック種別          */
typedef Uint16  PerNum;                         /* 必要ペリフェラル数        */

/* ペリフェラルデータ出力 */
typedef struct  {                               /* デジタルデバイス          */
    PerDgtData  data;                           /* 現在ペリフェラルデータ    */
    PerDgtData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerDgtInfo;

typedef struct  {                               /* アナログデバイス          */
    PerAnlData  data;                           /* 現在ペリフェラルデータ    */
    PerAnlData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerAnlInfo;

typedef struct  {                               /* ポインティングデバイス    */
    PerPntData  data;                           /* 現在ペリフェラルデータ    */
    PerPntData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerPntInfo;

typedef struct  {                               /* キーボードデバイス        */
    PerKbdData  data;                           /* 現在ペリフェラルデータ    */
    PerKbdData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerKbdInfo;

typedef struct  {                               /* メガドライブ3ボタンパッド */
    PerM3bpData data;                           /* 現在ペリフェラルデータ    */
    PerM3bpData push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerM3bpInfo;

typedef struct  {                               /* メガドライブ6ボタンパッド */
    PerM6bpData data;                           /* 現在ペリフェラルデータ    */
    PerM6bpData push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerM6bpInfo;

typedef struct  {                               /* システム出力              */
    Uint8   cc;                                 /* カートリッジコード        */
    Uint8   ac;                                 /* エリアコード              */
    Uint16  ss;                                 /* システムステータス        */
    Uint32  sm;                                 /* SMPCメモリ                */
    Uint8   stat;                               /* SMPCステータス            */
}PerGetSys;

typedef void PerGetPer;                         /* ペリフェラルデータ出力    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
#ifndef _PER_HOT_ENA
extern Uint8 per_hot_res;                       /* ホットリセット状態        */
#endif  /* _PER_HOT_ENA */
extern Uint8 *per_get_time_adr;                 /* 時刻データ取得先頭アドレス*/
extern PerGetSys per_get_sys_data;              /* システムデータ取得格納    */
extern Uint8 per_set_sys_flg;                   /* システムデータ設定フラグ  */
extern Uint8 per_time_out_flg;                  /* タイムアウトフラグ        */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
Uint32 PER_Init(PerKind, PerNum, PerId, PerSize, Uint32 *, Uint8);
                                                /* イントバック初期化        */
Uint32 PER_GetPer(PerGetPer **);                /* ペリフェラルデータ取得    */
PerGetSys PER_GetSys(void);                     /* システムデータ取得        */
void PER_IntFunc(void);                         /* SMPC割り込み処理          */

/********************************* per_smp.h *********************************/
/*
 * GLOBAL DEFINESMACROS DEFINES
 */

/**** コマンド ***************************************************************/
#define PER_SM_MSHON    0x00                    /* マスタSH ON               */
#define PER_SM_SSHON    0x02                    /* スレーブSH ON             */
#define PER_SM_SSHOFF   0x03                    /* スレーブSH OFF            */
#define PER_SM_SNDON    0x06                    /* サウンドON                */
#define PER_SM_SNDOFF   0x07                    /* サウンドOFF               */
#define PER_SM_CDON     0x08                    /* CD ON                     */
#define PER_SM_CDOFF    0x09                    /* CD OFF                    */
#define PER_SM_SYSRES   0x0d                    /* システム全体リセット      */
#define PER_SM_NMIREQ   0x18                    /* NMIリクエスト             */
#define PER_SM_RESENA   0x19                    /* ホットリセットイネーブル  */
#define PER_SM_RESDIS   0x1a                    /* ホットリセットディセーブル*/
#define PER_SM_SETSM    0x17                    /* SMPCメモリ設定            */
#define PER_SM_SETTIM   0x16                    /* 時刻設定                  */

/*****************************************************************************/
/*****************************************************************************/
/**** 低水準マクロ ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_WAIT() - SMPC待ち時間処理
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      SMPCを正しく実行するための、待ち時間を取る。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      PER_SMPC_SET_IREG(),PER_SMPCCmdGo()の前に必ず実行すること。
 *
 ******************************************************************************
 */

#define PER_SMPC_WAIT()                        /* SMPC待ち時間処理         */\
            do{                                                               \
                while((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF);\
                                                /* SFが"1"の間繰り返し      */\
                PER_PokeByte(PER_REG_SF, PER_B_SF);\
                                                /* SF<-"1"                  */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_GO_CMD() - SMPCコマンド実行
 *
 * PARAMETERS :
 *      (1) Uint8   smpc_cmd    - <i>   SMPCマクロ名
 *
 * DESCRIPTION:
 *      取得したSMPCマクロ名をSMPCのCOMREG（コマンドレジスタ）へセットする。
 *  （セットすることにより、SMPCハードがCOMREGに書かれたコマンドを実行する）
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_GO_CMD(smpc_cmd)                 /* SMPCコマンド実行         */\
            do{                                                               \
             PER_PokeByte(PER_REG_COMREG, smpc_cmd);/* COMREGにコマンドWRITE*/\
             while(PER_PeekByte(PER_REG_SF) & PER_B_SF);\
                                                    /* SFが"1"でなくなるまで*/\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_STATS_GET()  - SMPCステータス取得
 *
 * PARAMETERS :
 *      (1) Uint8   stats_reg   - <o>   コマンド実行ステータス
 *
 * DESCRIPTION:
 *      コマンド実行後のステータスを取得する
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_STATS_GET(stats_reg)           /* SMPCステータス取得       */\
            do{                                                               \
                stats_reg = PER_PeekByte(PER_SR);                             \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_IREG()   - SMPC IREGセット
 *
 * PARAMETERS :
 *      (1) Uint32  ireg_no     - <o>   IREG番号
 *      (2) Uint8   ireg_prm    - <i>   IREGセット値
 *
 * DESCRIPTION:
 *      指定されたIREG番号アドレスにIREG値をセットする。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_IREG(ireg_no, ireg_prm)    /* SMPC IREGセット          */\
            do{                                                               \
                PER_PokeByte((PER_REG_IREG + (ireg_no * 2)), ireg_prm);   \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NO_IREG()    - IREG設定無しコマンド実行
 *
 ******************************************************************************
 */

#define PER_SMPC_NO_IREG(com)\
            do{                                                               \
                PER_SMPC_WAIT();                /* SMPC待ち時間処理         */\
                PER_SMPC_GO_CMD(com);           /* SMPCコマンド実行         */\
            }while(FALSE)

/*****************************************************************************/
/*****************************************************************************/
/**** 高水準マクロ ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_MSH_ON()     - マスタSH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_MSH_ON()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_MSHON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_ON()     - スレーブSH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_ON()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_OFF()    - スレーブSH OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_OFF()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHOFF);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_ON()     - サウンドON
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_ON()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_OFF()    - サウンドOFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_OFF()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDOFF);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_ON()      - CD ON
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_ON()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDON);    /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_OFF()     - CD OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_OFF()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDOFF);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SYS_RES()    - システム全体リセット
 *
 ******************************************************************************
 */

#define PER_SMPC_SYS_RES()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SYSRES);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NMI_REQ()    - NMIリクエスト
 *
 ******************************************************************************
 */

#define PER_SMPC_NMI_REQ()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_NMIREQ);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_ENA()    - ホットリセットイネーブル
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_ENA()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESENA);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_DIS()    - ホットリセットディセーブル
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_DIS()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESDIS);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_SM()     - SMPCメモリ設定
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_SM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC待ち時間処理         */\
                PER_SMPC_SET_IREG(0, (ireg) >> 24); /* IREG0セット          */\
                PER_SMPC_SET_IREG(1, (ireg) >> 16); /* IREG0セット          */\
                PER_SMPC_SET_IREG(2, (ireg) >>  8); /* IREG0セット          */\
                PER_SMPC_SET_IREG(3, (ireg) >>  0); /* IREG0セット          */\
                PER_SMPC_GO_CMD(PER_SM_SETSM);    /* SMPCコマンド実行         */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_TIM()    - 時刻設定
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_TIM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC待ち時間処理         */\
                PER_SMPC_SET_IREG(6, *(ireg));    /* IREG6セット              */\
                PER_SMPC_SET_IREG(5, *(ireg + 1));/* IREG5セット              */\
                PER_SMPC_SET_IREG(4, *(ireg + 2));/* IREG4セット              */\
                PER_SMPC_SET_IREG(3, *(ireg + 3));/* IREG3セット              */\
                PER_SMPC_SET_IREG(2, *(ireg + 4));/* IREG2セット              */\
                PER_SMPC_SET_IREG(1, *(ireg + 5));/* IREG1セット              */\
                PER_SMPC_SET_IREG(0, *(ireg + 6));/* IREG0セット              */\
                PER_SMPC_GO_CMD(PER_SM_SETTIM);   /* SMPCコマンド実行         */\
            }while(FALSE)

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */

#endif  /* ifndef SEGA_PER_H */
/******************************* end of file *********************************/
