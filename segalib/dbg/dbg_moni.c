/*----------------------------------------------------------------------------
 *  dbg_moni.c -- DBG ライブラリ Moni モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-07-04 Ver.0.90
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  このライブラリはデバッグサポートライブラリで、以下のルーチンを含む。
 *
 *  DBG_Monitor       -  簡易デバッグ処理
 *
 *----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */
#include <stdio.h>
#include <string.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_dbg.h"

/*
 * DATA TYPE DEFINES
 */
#define NO_SPECIFY  0x48415255

struct RegArea {
    Uint32     pr;
    Uint32     gbr;
    Uint32     vbr;
    Uint32     mach;
    Uint32     macl;
    Uint32     r15;
    Uint32     r14;
    Uint32     r13;
    Uint32     r12;
    Uint32     r11;
    Uint32     r10;
    Uint32     r9;
    Uint32     r8;
    Uint32     r7;
    Uint32     r6;
    Uint32     r5;
    Uint32     r4;
    Uint32     r3;
    Uint32     r2;
    Uint32     r1;
    Uint32     r0;
    Uint32     pc;
    Uint32     sr;
};

struct RegArea *DbgRegAreaP = 0;
Uint32          DbgIntrKind = 0;

void DBG_IntrProc(void);

/*
 * STATIC DECLARATIONS
 */
static void memModify(Uint32 adr, Uint32 data, Uint32 type);
static Sint32  strToHex(Uint8 *str, Uint32 *hex);
static Uint32 getData(Uint32 adr, Uint32 type);
static void putData(Uint32 adr, Uint32 data, Uint32 type);
static Uint32 memDump(Uint32 adr1, Uint32 adr2, Uint32 size, Uint32 type);
static void dispReg(struct RegArea *reg);
static Sint32 dispMem(char *mem, Sint32 len, Sint32 mode);
static void setHex(Uint32 *val, Uint8 c);
static Sint32 evalCommand(Uint8 *cmd,
                       Uint32 *adr1, Uint32 *adr2, Uint32 *size, Uint32 *type);


/*****************************************************************************
 *
 * NAME:  DBG_IntrProc()  - Program Error Interrupt Proc
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     以下の内容を表示し、簡易モニタを呼び出す。
 *
 *     （１）例外割り込み要因
 *     （２）現レジスタ内容
 *     （３）スタック内容
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void DBG_IntrProc(void)
{
    Uint8  cmd[80];
    Uint32 adr1, adr2, size, type, nextDumpAdr, nextDumpType;

    /** BEGIN ***************************************************************/
    DBG_DisplayOn();
    switch(DbgIntrKind) {
        case 0 :
             DBG_Printf("\n!!! General Operation Error !!!\n\n");
             break;
        case 1 :
             DBG_Printf("\n!!! Slot Operation Error !!!\n\n");
             break;
        case 2 :
             DBG_Printf("\n!!! CPU Address Error !!!\n\n");
             break;
        case 3 :
             DBG_Printf("\n!!! DMA Address Error !!!\n\n");
             break;
    }
    dispReg(DbgRegAreaP);
    DBG_Printf("\n---- Stack Area ----\n");
    memDump(DbgRegAreaP->r15, NO_SPECIFY, 0x50, 2);
    for(;;) DBG_Monitor();
}


/*****************************************************************************
 *
 * NAME:  DBG_Monitor()  - Very Tiny Monitor Routine
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     以下の機能を有す。
 *
 *     （１）メモリ内容の表示
 *           D fromAdr [toAdr/@size] [; B/W/L] 
 *
 *           0000000000111111111122222222223333333333
 *           0123456789012345678901234567890123456789
 *          +----------------------------------------+
 *          |:D xxxxxx                               |
 *          |<ADDR>       <D A T A>                  |
 *          |xxxxxxxx xx xx xx xx xx xx xx xx xxxxxxx|  B
 *          |xxxxxxxx xxxx xxxx xxxx xxxx  xxxxxxxx  |  W
 *          |xxxxxxxx xxxxxxxx xxxxxxxx  xxxxxxxx    |  L
 *          |                                        |
 *          +----------------------------------------+
 *
 *     （２）メモリ内容の変更
 *           M toAddr [data] [; B/W/L]
 *
 *           0000000000111111111122222222223333333333
 *           0123456789012345678901234567890123456789
 *          +----------------------------------------+
 *          |:M xxxxxx;w                             |
 *          |xxxxxxxx xx       ? xx                  |  B
 *          |xxxxxxxx xxxx     ? xxxx                |  W
 *          |xxxxxxxx xxxxxxxx ? xxxxxxxx            |  L
 *          |                                        |
 *          +----------------------------------------+
 *
 *     （３）例外処理ルーチンから呼び出された場合、レジスタ内容の表示
 *           R <rtn>
 *
 *           0000000000111111111122222222223333333333
 *           0123456789012345678901234567890123456789
 *          +----------------------------------------+
 *          |:R                                      |
 *          |PC=xxxxxxxx SR=xxxxxxxx                 |
 *          |PR=xxxxxxxx GBR=xxxxxxxx VBR=xxxxxxxx   |
 *          |MACH=xxxxxxxx MACL=xxxxxxxx             |
 *          |R0 - xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx|
 *          |R4 - xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx|
 *          |R8 - xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx|
 *          |R12- xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx|
 *          |                                        |
 *          +----------------------------------------+
 *
 *     （４）本ルーチンの終了
 *           Q <rtn>
 *
 *     （５）ヘルプ表示
 *           H <rtn>
 *
 *           *** Command ***
 *           (1) Dump
 *               D fromAdr [toAdr/@size] [;B/W/L] <rtn> 
 *           (2) Memory Modify
 *               M toAddr [data] [;B/W/L] <rtn>
 *           (3) Register Dump
 *               R <rtn>
 *           (4) Quit
 *               Q <rtn>
 *           (5) Help
 *               H <rtn>
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void DBG_Monitor(void)
{
    Uint8  cmd[80];
    Uint32 adr1, adr2, size, type, nextDumpAdr, nextDumpType;
    Sint32 rtncd;
    int    i, j;

    /** BEGIN ***************************************************************/
    DBG_DisplayOn();
    nextDumpAdr = 0;
    for(;;) {
        DBG_Printf(">");
        DBG_GetKeyStr((char*)cmd);
	rtncd = evalCommand(cmd, &adr1, &adr2, &size, &type);
	switch(rtncd) {
	    case 0 : /* コマンド なし */
                if(nextDumpAdr)
                    nextDumpAdr  = memDump(nextDumpAdr, NO_SPECIFY,
                                                 0x40,nextDumpType);
		break;
	    case 1 : /* Ｄコマンド    */
                nextDumpAdr  = memDump(adr1,adr2,size,type);
                nextDumpType = type;
		break;
	    case 2 : /* Ｍコマンド    */
                nextDumpAdr = 0;
                memModify(adr1,adr2,type);
		break;
	    case 3 : /* Ｒコマンド    */
                nextDumpAdr = 0;
		if(DbgRegAreaP)
		    dispReg(DbgRegAreaP);
		break;
	    case 4 : /* Ｑコマンド    */
		return;
	    case 5 : /* Ｈコマンド    */
                nextDumpAdr = 0;
                DBG_Printf("*** Command ***\n");
                DBG_Printf("(1) Dump\n");
                DBG_Printf("    D fromAdr [toAdr/@size] [;B/W/L]\n");
                DBG_Printf("(2) Memory Modify\n");
                DBG_Printf("    M toAddr [data] [;B/W/L]\n");
                DBG_Printf("(3) Register Dump\n");
                DBG_Printf("    R\n");
                DBG_Printf("(4) Quit\n");
                DBG_Printf("    Q\n");
                DBG_Printf("(5) Help\n");
                DBG_Printf("    H\n");
		break;
	    case 9 : /* コマンドエラー  */
		DBG_Printf("** Command Error **\n");
		break;
	}
    }
}


/*****************************************************************************
 *
 * NAME:  evalCommand()  - Evaluate Command String 
 *
 * PARAMETERS :
 *
 *     (1) Uint8  *cmd    - <i> コマンド文字列
 *     (2) Uint32 *adr1   - <o> アドレス１  NO_SPECIFY = 指定なし
 *     (3) Uint32 *adr2   - <o> アドレス２  NO_SPECIFY = 指定なし
 *     (4) Uint32 *size   - <o> サイズ               0 = 指定なし
 *     (5) Uint32 *type   - <o> データタイプ  0 = Byte, 1 = Word, 2 = Long
 *
 * DESCRIPTION:
 *
 *     コマンド文字列の解析
 *
 * POSTCONDITIONS:
 *
 *     return code   0 = コマンド なし
 *                   1 = Ｄコマンド 
 *                   2 = Ｍコマンド 
 *                   3 = Ｒコマンド 
 *                   4 = Ｑコマンド 
 *                   5 = Ｈコマンド 
 *                   9 = コマンドエラー 
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
static Sint32 evalCommand(Uint8 *cmd,
		       Uint32 *adr1, Uint32 *adr2, Uint32 *size, Uint32 *type)
{
    struct Stbl {
	Uint8   cc;
	Uint8   jump;
    };
#define NOP      0
#define EXIT     1
#define PROC     2
#define STOP     0
#define OTH      0x80
#define HEX      0x81
#define BWL      0x82
#define CONT     0x83
    static struct Stbl stbl[][7] = {
/*       -PROC-      -1-       -2-       -3-       -4-       -5-       -6-  */
/*00*/{{ NOP, 0},{ 'D',11},{ 'M',22},{ 'R', 3},{ 'Q', 5},{ 'H', 7},{CONT, 1}},
/*01*/{{ NOP, 0},{ ' ', 0},{STOP,30},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},

/*02*/{{EXIT, 9},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*03*/{{ NOP, 0},{ ' ', 3},{STOP, 4},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*04*/{{EXIT, 3},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*05*/{{ NOP, 0},{ ' ', 5},{STOP, 6},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*06*/{{EXIT, 4},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*07*/{{ NOP, 0},{ ' ', 7},{STOP, 8},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*08*/{{EXIT, 5},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*09*/{{EXIT, 1},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*10*/{{EXIT, 2},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},

/*11*/{{ NOP, 0},{ ' ',11},{ HEX,12},{STOP, 2},{ OTH, 2},{   0, 0},{   0, 0}},
/*12*/{{PROC, 0},{ ' ',13},{ HEX,12},{STOP, 9},{ ';',19},{ OTH, 2},{   0, 0}},
/*13*/{{ NOP, 0},{ ' ',13},{ HEX,14},{STOP, 2},{ '@',16},{ ';',18},{ OTH, 2}},
/*14*/{{PROC, 1},{ ' ',15},{ HEX,14},{STOP, 9},{ ';',19},{ OTH, 2},{   0, 0}},
/*15*/{{ NOP, 0},{ ' ',15},{ ';',19},{STOP, 2},{ OTH, 2},{   0, 0},{   0, 0}},
/*16*/{{ NOP, 0},{ HEX,17},{ ';',19},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*17*/{{PROC, 2},{ ' ',18},{ HEX,17},{STOP, 9},{ ';',19},{ OTH, 2},{   0, 0}},
/*18*/{{ NOP, 2},{ ' ',18},{ ';',19},{STOP, 9},{ OTH, 2},{   0, 0},{   0, 0}},
/*19*/{{ NOP, 0},{ ' ',19},{ BWL,20},{STOP, 9},{ OTH, 2},{   0, 0},{   0, 0}},
/*20*/{{PROC, 3},{ ' ',21},{STOP, 9},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*21*/{{ NOP, 0},{ ' ',21},{STOP, 9},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},

/*22*/{{ NOP, 0},{ ' ',22},{ HEX,23},{STOP, 2},{ OTH, 2},{   0, 0},{   0, 0}},
/*23*/{{PROC, 0},{ ' ',24},{ HEX,23},{STOP,10},{ ';',27},{ OTH, 2},{   0, 0}},
/*24*/{{ NOP, 0},{ ' ',24},{ HEX,25},{STOP,10},{ ';',27},{ OTH, 2},{   0, 0}},
/*25*/{{PROC, 1},{ ' ',26},{ HEX,25},{STOP,10},{ ';',27},{ OTH, 2},{   0, 0}},
/*26*/{{ NOP, 0},{ ' ',26},{ ';',27},{STOP,10},{ OTH, 2},{   0, 0},{   0, 0}},
/*27*/{{ NOP, 0},{ ' ',27},{ BWL,28},{STOP,10},{ OTH, 2},{   0, 0},{   0, 0}},
/*28*/{{PROC, 3},{ ' ',28},{STOP,10},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},
/*29*/{{ NOP, 0},{ ' ',29},{STOP,10},{ OTH, 2},{   0, 0},{   0, 0},{   0, 0}},

/*30*/{{EXIT, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0},{   0, 0}},
};
    Sint32  sc, i, n;
    Uint8   c;

    /** BEGIN ***************************************************************/
    *adr1 = NO_SPECIFY;
    *adr2 = NO_SPECIFY;
    *size = 0;
    *type = 0;
    sc = 0;
    for(;;) {
       c = *cmd++;
       if((0x61 <= c) && (c <= 0x7a)) c -= 0x20;
       for(i=1; i<7; i++) {
           if(stbl[sc][i].cc == c)  break;
               else
	   if(stbl[sc][i].cc == HEX) {
               if((('0' <= c) && (c <= '9')) ||
                  (('A' <= c) && (c <= 'F')))   break;
	   }   else
           if(stbl[sc][i].cc == BWL) {
               if((c == 'B') || (c == 'W') ||(c == 'L'))  break;
           }   else
           if(stbl[sc][i].cc == CONT) {
               --cmd;
               break;
           }   else
           if(stbl[sc][i].cc == OTH)  break;
       }
       sc = stbl[sc][i].jump;
       switch(stbl[sc][0].cc) {
           case NOP  :
                break;
           case PROC :
                switch(stbl[sc][0].jump) {
                    case 0 : /* set adr1 */
                        setHex(adr1, c);
                        break;
		    case 1 : /* set adr2 */
                        setHex(adr2, c);
                        break;
		    case 2 : /* set size */
                        setHex(size, c);
                        break;
                    case 3 : /* set type */
			if(c == 'B') *type = 0;
			else
			if(c == 'W') *type = 1;
			else
			if(c == 'L') *type = 2;
                        break;
                }
		break;
           case EXIT :
                return stbl[sc][0].jump;
		break;
       }
    }
}

static void setHex(Uint32 *val, Uint8 c)
{
   Uint32 n;

   if(*val == NO_SPECIFY) *val = 0;
   if(c >= 'A')
       n = c - 0x37;
   else
       n = c - 0x30;
   *val = (*val << 4) + n;
}


/*****************************************************************************
 *
 * NAME:  memModify()  - Memory Modify
 *
 * PARAMETERS :
 *
 *     (1) Uint32  adr    - <i> 変更開始アドレス
 *     (2) Uint32  data   - <i> 変更データ
 *     (3) Uint32  type   - <i> データ表示モード
 *                             ０＝バイト
 *                             １＝ワード
 *                             ２＝ロングワード
 *
 * DESCRIPTION:
 *
 *     メモリ内容の変更
 *
 * POSTCONDITIONS:
 *
 *     なし
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
static void memModify(Uint32 adr, Uint32 data, Uint32 type)
{
    Uint32  len, newData, oldData;
    Uint8   cmd[64];

    /** BEGIN ***************************************************************/
    if(type == 2)
	if(adr & 0x00000003) goto error;
    if(type == 1)
	if(adr & 0x00000001) goto error;
        
    if(data != NO_SPECIFY) {
        putData(adr, data, type);
        return;
    }

    for(;;) {
       oldData = getData(adr, type);
       switch(type) {
           case 0: /* byte mode */
	          DBG_Printf("%08lx %02x       ? ", adr, oldData);
                  len = 1;
	          break;
           case 1: /* word mode */
	          DBG_Printf("%08lx %04x     ? ", adr, oldData);
                  len = 2;
                  break;
           case 2: /* long mode */
	          DBG_Printf("%08lx %08lx ? ", adr, oldData);
                  len = 4;
                  break;
       }
       DBG_GetKeyStr((char*)cmd);
       if(*cmd == '.') return;
       else
       if(*cmd == 0)   adr += len;
       else
       if(*cmd == '^') adr -= len;
       else {
           if(strToHex(cmd, &newData)) 
               DBG_Printf("** Data Error **\n");
           else {
               putData(adr, newData, type);
               adr += len;
           }
       }
    }
error:
    DBG_Printf("** Command Error **\n");
}

static Sint32  strToHex(Uint8 *str, Uint32 *hex)
{
   Uint32 n;
   Uint8  c;

   *hex = 0;
   while(*str != 0) {
       c = *str++;
       if((0x61 <= c) && (c <= 0x7a)) c -= 0x20;
       if((('0' <= c) && (c <= '9')) ||
          (('A' <= c) && (c <= 'F'))) {
          if(c >= 'A')
             n = c - 0x37;
          else
             n = c - 0x30;
          *hex = (*hex << 4) + n;
       } else
          return 1;
   }
   return 0;
}

static Uint32 getData(Uint32 adr, Uint32 type)
{
    Uint32 data;
    Uint8  *b;
    Uint16 *w;
    Uint32 *l;

    switch(type) {
       case 0: /* byte mode */
	      b = (Uint8*)adr;
              data = *b;
	      break;

       case 1: /* word mode */
	      w = (Uint16*)adr;
              data = *w;
              break;

       case 2: /* long mode */
	      l = (Uint32*)adr;
              data = *l;
              break;
    }
    return data;
}

static void putData(Uint32 adr, Uint32 data, Uint32 type)
{
    Uint8  *b;
    Uint16 *w;
    Uint32 *l;

    switch(type) {
       case 0: /* byte mode */
	      b = (Uint8*)adr;
              *b = data;
	      break;

       case 1: /* word mode */
	      w = (Uint16*)adr;
              *w = data;
              break;

       case 2: /* long mode */
	      l = (Uint32*)adr;
              *l = data;
              break;
    }
}


/*****************************************************************************
 *
 * NAME:  memDump()  - Memory Dump
 *
 * PARAMETERS :
 *
 *     (1) Uint32  adr1   - <i> ダンプ開始アドレス
 *     (2) Uint32  adr2   - <i> ダンプ終了アドレス
 *     (2) Uint32  size   - <i> データサイズ
 *     (3) Uint32  type   - <i> データ表示モード
 *                             ０＝バイト
 *                             １＝ワード
 *                             ２＝ロングワード
 *
 * DESCRIPTION:
 *
 *     メモリ内容の編集表示
 *
 * POSTCONDITIONS:
 *
 *     nextDumpAdr   次のダンプアドレス
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
static Uint32 memDump(Uint32 adr1, Uint32 adr2, Uint32 size, Uint32 type)
{
    Uint32  n;

    /** BEGIN ***************************************************************/
    if(adr2 != NO_SPECIFY) {
        if(adr1 > adr2) goto error;
        size = adr2 - adr1 + 1;
    }
    if(size == 0) size = 0x40;
    DBG_Printf("<ADDR>       <D A T A>\n");
    while(size) {
       if(size < 8)
          n = size;
       else
          n = 8;
       if(dispMem((char *)adr1, n, type)) goto error;
       adr1 += n;
       size -= n;
    }
    return adr1;
error:
    DBG_Printf("** Command Error **\n");
    return 0;
}


/*****************************************************************************
 *
 * NAME:  dispMem()  - Display Memory
 *
 * PARAMETERS :
 *
 *     (1) char   *mem   - <i> メモリエリア
 *     (2) Sint32 len    - <i> データサイズ (<=8)
 *     (3) Sint32 mode   - <i> データ表示モード
 *                             ０＝バイト
 *                             １＝ワード
 *                             ２＝ロングワード
 *
 * DESCRIPTION:
 *
 *     メモリ内容の１行編集表示
 *
 * POSTCONDITIONS:
 *
 *     return code   0=OK,  1=NG(address error)
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
static Sint32 dispMem(char *mem, Sint32 len, Sint32 mode)
{
    Uint8  *b;
    Uint16 *w;
    Uint32 *l;
    int     i, n;
    Uint8   cbuf[16];

    /** BEGIN ***************************************************************/
    if(mode == 2)
	if((Uint32)mem & 0x00000003) return 1;
    if(mode == 1)
	if((Uint32)mem & 0x00000001) return 1;

    DBG_Printf("%08lx ",(Uint32)mem);

    switch(mode) {
       case 0: /* byte mode */
	      b = (Uint8*)mem;
	      for(i=0; i<len; i++)
		  DBG_Printf("%02x ",*b++);
	      for(i=0; i<8-len; i++)  DBG_Printf("   ");
	      if(len >= 8) --len;
	      break;

       case 1: /* word mode */
	      w = (Uint16*)mem;
	      n = (len + 1) >> 1;
              len = n << 1;
              for(i=0; i<n; i++)
                  DBG_Printf("%04x ",*w++);
	      for(i=0; i<4-n; i++)  DBG_Printf("     ");
              break;

       case 2: /* long mode */
	      l = (Uint32*)mem;
              n = (len + 3) >> 2;
              len = n << 2;
              for(i=0; i<n; i++)
                  DBG_Printf("%08lx ",*l++);
	      for(i=0; i<2-n; i++)  DBG_Printf("         ");
              break;
    }

    memcpy(cbuf,mem,len);
    b = cbuf;
    for(i=0; i<len; i++) {
       if(*b < 0x20 || *b > 0x7f) *b = '.';
       b++;
    }
    *b = 0;
    if((mode == 0) && (len == 7))
        DBG_Printf("%s",cbuf);
    else
        DBG_Printf("%s\n",cbuf);
    return 0;
}


/*****************************************************************************
 *
 * NAME:  dispReg()  - Display Register Contents 
 *
 * PARAMETERS :
 *
 *     (1) struct RegArea  *reg   - <i> レジスタエリア
 *
 * DESCRIPTION:
 *
 *     レジスタ内容の編集表示
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
static void dispReg(struct RegArea *reg)
{
    /** BEGIN ***************************************************************/
    DBG_Printf("PC=%08lx SR=%08lx\n",reg->pc,reg->sr);
    DBG_Printf("PR=%08lx GBR=%08lx VBR=%08lx\n",reg->pr,reg->gbr,reg->vbr);
    DBG_Printf("MACH=%08lx MACL=%08lx\n",reg->mach,reg->macl);
    DBG_Printf("R0 = %08lx %08lx %08lx %08lx",
		reg->r0,reg->r1,reg->r2,reg->r3);
    DBG_Printf("R4 = %08lx %08lx %08lx %08lx",
		reg->r4,reg->r5,reg->r6,reg->r7);
    DBG_Printf("R8 = %08lx %08lx %08lx %08lx",
		reg->r8,reg->r9,reg->r10,reg->r11);
    DBG_Printf("R12= %08lx %08lx %08lx %08lx",
                reg->r12,reg->r13,reg->r14,reg->r15);
}

/*  end of file */
