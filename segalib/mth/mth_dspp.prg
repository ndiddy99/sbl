;-----------------------------------------------------------------------------
;   mth_dspp.prg -- MTH ライブラリ DSPP モジュール
;   Copyright(c) 1994 SEGA
;   Written by T.S on 1994-01-24 Ver.0.80
;   Updated by T.S on 1994-07-25 Ver.1.00
;
;   このライブラリはＤＳＰ側の３Ｄ座標変換処理モジュールである。
;
;-----------------------------------------------------------------------------
;
;	DSP (100%) board program
;
;					'94/06/15      F.Sonobe
;
;           RAM0              RAM1              RAM2              RAM3
;       +----------+      +----------+      +----------+      +----------+
;     00|          |    00|       .X |    00|       .X |    00|       .X |
;       | Argv 0-3 |      | src(1).Y |      | ndt(1).Y |      | dst(1).Y |
;       |          |      |       .Z |      |       .Z |      |       .Z |
;       | total cnt|      |----------|      |----------|      |----------|
;       |----------|    03|       .X |    03|       .X |    03|       .X |
;     0E|  entry   |      | src(2).Y |      | ndt(2).Y |      | dst(2).Y |
;       |----------|      |       .Z |      |       .Z |      |       .Z |
;     0F| entry-1  |      |----------|      |----------|      |----------|
;       |----------|    06|     .    |    06|     .    |    06|     .    |
;     11| Current  |      |     .    |      |     .    |      |     .    |
;       |  Matrix1 |      |     .    |      |     .    |      |     .    |
;       |----------|      |     .    |      |     .    |      |     .    |
;     1D|         X|      |     .    |      |     .    |      |     .    |
;       |LightVct Y|      |     .    |      |     .    |      |     .    |
;       |         Z|      |     .    |      |     .    |      |     .    |
;       |----------|      |     .    |      |     .    |      |     .    |
;     20| Current  |      |     .    |      |     .    |      |     .    |
;       |  Matrix2 |      |     .    |      |     .    |      |     .    |
;       |----------|      |     .    |      |     .    |      |     .    |
;     2C| ret adr. |      |----------|      |----------|      |----------|
;       |----------|    3C|       .X |    3C|       .X |    3C|       .X |
;     2D|   tmp    |      |src(21).Y |      |ndt(21).Y |      |dst(21).Y |
;       |          |      |       .Z |      |       .Z |      |       .Z |
;       |----------|      |----------|      |----------|      |----------|
;     3F|   mpt    |    3F|   spt    |    3F|   npt    |    3F|   dpt    |
;       +----------+      +----------+      +----------+      +----------+
;
;
ONE	=	$10000		; =1
MSZ	=	12		; Matrix Size
XYZ	=	3		; Vector Size
PSZ	=	1		; Point Size
MXD	=	21		; MAX data entry
;
;	RAM0
TBL	=	$00		; Argument table
TBL1	=	$01		; Argument table 1
TBL2	=	$05		; Argument table 2
TBL3	=	$08		; Argument table 3
TBL4	=	$0B		; Argument table 4
ECN	=	$0E		; Entry Count
MTX	=	$11		; Current Matrix 1
LV	=	$1D		; Light Vector
MX2	=	$20		; Current Matrix 2
RET	=	$2C		; return address
TMC	=	$2D		; Color Code Temporary
MTM	=	$3F		; Temporary Pointer
;
;	RAM1
SPT	=	$00		; Source Pointer
STM	=	$3F		; Temporary Pointer
;
;	RAM2
NPT	=	$00		; Nomal Vector Pointer
NTM	=	$3F		; Temporary Pointer
;
;	RAM3
DPT	=	$00		; Destination Pointer
DTM	=	$3F		; Temporary pointer
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;
	ORG	0
;
START		; // transfer matrix //
	                                                 MOV TBL,CT0
	                                                 MOV M0,RA0
	                                                 MOV MTX,CT0
	DMA D0,M0,MSZ+XYZ+MSZ
;
;
;    sp3SurfaceBright
;
	                                                 MOV TBL1+3,CT0
	                                                 MOV M0,WA0
SP3_SB
;		// set entry count //
	                                                 MOV DPT,CT3
	                                                 MOV TBL1,MC3
	MVI C_PET,PC
	                                                 MOV DPT,CT3
	JMP Z,SP3_CT
	ADD                                   MOV ALU,A  MOV TBL1+1,CT0
	ADD                                   MOV ALU,A  MOV ALL,MC3
	                                                 MOV DPT,CT3
	                MOV M0,P              MOV M3,A   MOV M0,RA0
	ADD                                              MOV ALL,MC0
	DMA D0,M1,M3
WTA1	JMP T0,WTA1
	                                                 MOV NPT,CT2
	                MOV M0,P                         MOV M0,RA0
	ADD                                              MOV ALL,MC0
	DMA D0,M2,M3
WTA2	JMP T0,WTA2
;
;		// Entry Count の４倍ー１を LOP にセット //
	                                                 MOV ECN,CT0
	                                      MOV M0,A   MOV DTM,CT3
	SL                                    MOV ALU,A  MOV NTM,CT2
	SL                                    MOV ALU,A  MOV #1,PL
	SUB                                              MOV ALL,LOP
	MVI ONE,MC3
;
;		// src と dst のポイントをセット //
	                                               
	                                                 MOV MTM,CT0
	                                                 MOV DPT,MC0
	                                                 MOV SPT,MC2
	                                                 MOV DTM-XYZ,CT3
;
;		// CoordTransSub の呼出し //
SB_LP
	MVI C_TRN,PC
	                                                 MOV NTM,CT2
	                                                 MOV DTM-XYZ,CT3
	                MOV MC3,P             CLR A      MOV MTM-XYZ,CT0
	SUB             MOV MC3,P             CLR A      MOV ALL,MC0
	SUB             MOV MC3,P             CLR A      MOV ALL,MC0
	SUB                                              MOV ALL,MC0

;		// src と dst のポイントをセット //
;;;	                                                 MOV STM,CT1 -> C_TRN
	                                                 MOV M2,MC1
;
;		// NormalTrans の呼出し //
	MVI N_TRN,PC
	                                                 MOV STM,CT1
;		// ComputBrightness の呼出し //
	MVI C_BRNE,PC
	                                                 MOV DTM-XYZ,CT3
;
;		// src ポインタを次にセット //
	                                                 MOV SB_LP,TOP
	                                      MOV M0,A   MOV PSZ,PL
	ADD                                              MOV ALL,MC0
	                                                 MOV STM,CT1
	                                      MOV M1,A   MOV XYZ,PL
	ADD                                              MOV ALL,MC1
	                                      MOV M2,A   MOV XYZ,PL
	BTM
	ADD                                              MOV ALL,MC2
;
;		// 演算結果をＤＭＡ転送 //
	                                                 MOV ECN,CT0
	                                                 MOV DPT,CT3
	JMP SP3_SB
	DMA M3,D0,M0
;............................................................................
;
;    sp3CoordTrans
;
SP3_CT
	                                                 MOV RET,CT0
	                                                 MOV SP3_VB,MC0
	                                                 MOV TBL2+1,CT0
	                                                 MOV M0,RA0
	                                                 MOV TBL2+2,CT0
	                                                 MOV M0,WA0
CT_BLK
;		// set entry count //
	                                                 MOV DPT,CT3
	                                                 MOV TBL2,MC3
	MVI C_PET,PC
	                                                 MOV DPT,CT3
	JMP Z,RET_CT
	ADD                                   MOV ALU,A  MOV RET,CT0
	ADD                                   MOV ALU,A  MOV ALL,MC3
	                                                 MOV DPT,CT3
	DMA D0,M1,M3
	                                                 MOV ECN+1,CT0
	                                                 MOV M3,MC0
;
;		// Entry Count の２倍ー１を LOP にセット //
	                                                 MOV ECN,CT0
	                                      MOV M0,A   MOV NTM,CT2
	SL                                    MOV ALU,A  MOV #1,PL
	SUB                                              MOV ALL,LOP
;
;		// src と dst のポイントをセット //
	                                                 MOV SPT,MC2
WTB	JMP T0,WTB
	                                                 MOV DPT,CT3
;
;		// CoordTransSub の呼出し //
CT_LP
	MVI C_TRN,PC
	                                                 MOV NTM,CT2
	                                                 MOV CT_LP,TOP
;
;		// src ポインタを次にセット //
	                                      MOV M2,A   MOV XYZ,PL
	BTM
	ADD                                              MOV ALL,MC2
	                                                 MOV ECN+1,CT0
	                                                 MOV DPT,CT3
	JMP CT_BLK
	DMA M3,D0,M0
;
RET_CT
	                                                 MOV MC0,TOP
	                                                 MOV #1,LOP
	BTM
;		// #この次の命令も実行されるので注意!! //
;............................................................................
;
;    sp3VertBright
;
SP3_VB
	                                                 MOV TBL3+1,CT0
	                                                 MOV M0,RA0
	                                                 MOV TBL3+2,CT0
	                                                 MOV M0,WA0
VB_BLK
	                                                 MOV DPT,CT3
	                                                 MOV TBL3,MC3
	MVI C_PET,PC
	                                                 MOV DPT,CT3
	JMP Z,SP3_CT2
	ADD                                   MOV ALU,A  MOV NPT,CT2
	ADD                                   MOV ALU,A  MOV ALL,MC3
	                                                 MOV DPT,CT3
	DMA D0,M2,M3
WTC	JMP T0,WTC
;
;		// Entry Count の２倍ー１を LOP にセット //
	                                                 MOV ECN,CT0
	                                      MOV M0,A   MOV MTM,CT0
	SL                                    MOV ALU,A  MOV #1,PL
	SUB                                              MOV ALL,LOP
	MVI ONE,MC0
;
;		// src と dst のポイントをセット //
	                                                 MOV STM-1,CT1
	                                                 MOV NPT,MC1
	                                                 MOV DPT,MC1
;
;		// NormalTrans の呼出し //
VB_LP
	MVI N_TRN,PC
	                                                 MOV STM-1,CT1
	                                      MOV MC1,A  MOV NTM,CT2
	                                                 MOV MTM,CT0
	                                                 MOV DTM-XYZ,CT3
	     MOV M0,X              MOV M0,Y              MOV LV,CT0
	     MOV MC0,X  MOV MUL,P  MOV MC3,Y  CLR A      MOV #$10,MC2
	AD2  MOV MC0,X  MOV MUL,P  MOV MC3,Y  MOV ALU,A  MOV NTM,CT2
	AD2  MOV MC0,X  MOV MUL,P  MOV MC3,Y  MOV ALU,A  MOV M1,CT3
	AD2             MOV MUL,P  MOV M2,Y   MOV ALU,A  MOV #$1F,MC3
	AD2                                              MOV ALH,RX 
	                MOV MUL,P             CLR A      MOV M1,CT3
	AD2             MOV M3,P              MOV ALU,A  MOV ALH,MC2
	JMP S,VB_FN    ; // color < 0 //
	                                                 MOV #$00,MC3
	                                                 MOV NTM,CT2
	                                      MOV M2,A   MOV M1,CT3
	SUB                                              MOV #$1F,MC3
	JMP NZS,VB_FN  ; // color > 31 //
	                                                 MOV M1,CT3
	                                                 MOV M2,MC3
;
;		// src ポインタを次にセット //
VB_FN
	                                                 MOV VB_LP,TOP
	                                                 MOV STM-1,CT1
	                                      MOV M1,A   MOV XYZ,PL
	ADD                                              MOV ALL,MC1
	                                      MOV M1,A   MOV PSZ,PL
	BTM
	ADD                                              MOV ALL,MC1

	                                                 MOV ECN,CT0
	                                                 MOV DPT,CT3
	JMP VB_BLK
	DMA M3,D0,M0

SP3_CT2
	                                                 MOV MX2,CT0
	                                                 MOV DPT,CT3
	                                                 MOV MSZ-1,LOP
	LPS
	                                                 MOV MC0,MC3
	                                                 MOV DPT,CT3
	                                                 MOV MTX,CT0
	                                                 MOV MSZ-1,LOP
	LPS
	                                                 MOV MC3,MC0
	                                                 MOV RET,CT0
	                                                 MOV LAST,MC0
	                                                 MOV TBL4+1,CT0
	                                                 MOV M0,RA0
	                                                 MOV TBL4+2,CT0
	                                                 MOV M0,WA0
	                                                 MOV TBL4,CT0
	                MOV M0,P              CLR A      MOV TBL2,CT0
	JMP CT_BLK
	ADD                                              MOV ALL,MC0

LAST
	ENDI

;
;////////////////////////////////////////////////////////////////////////////
;
;    Coord TransSub
;
;    IN  : CT2 = Source XYZ pointer's address.
;        : CT3 = Destination pointer.
;
C_TRN
	                                                 MOV M2,CT1
	                                                 MOV MTX,CT0
	     MOV MC1,X             MOV MC0,Y
	     MOV MC1,X  MOV MUL,P  MOV MC0,Y  CLR A
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A  MOV M2,CT1
	AD2             MOV MUL,P  MOV MC0,Y  MOV ALU,A
	MVI ONE,RX
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  CLR A      MOV ALH,MC3
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A  MOV M2,CT1
	AD2             MOV MUL,P  MOV MC0,Y  MOV ALU,A
	MVI ONE,RX
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  CLR A      MOV ALH,MC3
	AD2  MOV MC1,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2             MOV MUL,P  MOV MC0,Y  MOV ALU,A  MOV STM,CT1
	MVI ONE,RX
	AD2             MOV MUL,P             MOV ALU,A
	BTM
	AD2                                              MOV ALH,MC3

;////////////////////////////////////////////////////////////////////////////
;
;    Normal Vector Transformed
;
;    IN  : CT1   = Normal Vector pointer
;        : R0:0E = Matrix
;        : R2    = Normal Vector
;    OUT : R3:3C = Surface Nomal Vector
;
N_TRN
	                                                 MOV M1,CT2
	                                                 MOV MTX,CT0
	     MOV MC2,X             MOV MC0,Y             MOV DTM-XYZ,CT3
	     MOV MC2,X  MOV MUL,P  MOV MC0,Y  CLR A
	AD2  MOV MC2,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2             MOV MUL,P  MOV MC0,Y  MOV ALU,A  MOV M1,CT2
	AD2  MOV MC2,X             MOV MC0,Y             MOV ALH,MC3
	AD2  MOV MC2,X  MOV MUL,P  MOV MC0,Y  CLR A    
	AD2  MOV MC2,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2             MOV MUL,P  MOV MC0,Y  MOV ALU,A  MOV M1,CT2
	AD2  MOV MC2,X             MOV MC0,Y             MOV ALH,MC3
	AD2  MOV MC2,X  MOV MUL,P  MOV MC0,Y  CLR A    
	AD2  MOV MC2,X  MOV MUL,P  MOV MC0,Y  MOV ALU,A
	AD2             MOV MUL,P             MOV ALU,A
	BTM
	AD2                                              MOV ALH,MC3

;////////////////////////////////////////////////////////////////////////////
;
;    Compute Brightness
;
;    IN  : R0:3C = View Vector
;        : R0:1A = Light Vector
;        : R0:3F = Surface Bright Pointer
;        : R3:3C = Nomal Vector
;        : R3:3F = $10000
;        ; CT3   = 3C
;    OUT : R3    = Surface Bright
;    TMP : R1:0  = Color No. etc.
;
C_BRNE
	                                                 MOV MTM-XYZ,CT0
	     MOV MC0,X             MOV MC3,Y             MOV SPT,CT1
	     MOV MC0,X  MOV MUL,P  MOV MC3,Y  CLR A      MOV #$10,MC1
	AD2  MOV M0,X   MOV MUL,P  MOV MC3,Y  MOV ALU,A  MOV MTM,CT0
	AD2  MOV M3,X   MOV MUL,P  MOV M3,Y   MOV ALU,A  MOV M0,CT3
	AD2                                   MOV MC3,A  MOV LV,CT0
	MVI #0,MC3,NZS
	MVI #$70000,MC3,ZS
	                                                 MOV DTM-XYZ,CT3
	     MOV MC0,X  MOV MUL,P  MOV MC3,Y  CLR A      MOV SPT,CT1
	AD2  MOV MC0,X  MOV MUL,P  MOV MC3,Y  MOV ALU,A  MOV NTM,CT2
	AD2  MOV M0,X   MOV MUL,P  MOV MC3,Y  MOV ALU,A  MOV MTM,CT0
	AD2             MOV MUL,P  MOV M1,Y   MOV ALU,A  MOV M0,CT3
	                                                 MOV #$1F,MC3
	AD2                                              MOV ALH,RX 
	                MOV MUL,P             CLR A      MOV M0,CT3
	AD2             MOV M3,P                         MOV ALH,MC1
	JMP S,COL_FN
	                                                 MOV #$00,MC3
	                                                 MOV SPT,CT1
	                                      MOV M1,A   MOV M0,CT3
	SUB                                              
	JMP NZS,COL_FN
	                                                 MOV #$1F,MC3
	                                                 MOV M0,CT3
	                                                 MOV M1,MC3
COL_FN
	                MOV M3,P                         MOV M0,CT3
	                                      MOV M3,A   MOV M0,CT3
	OR                                               MOV ALL,MC3
	BTM
	                                                 MOV MTM,CT0

;////////////////////////////////////////////////////////////////////////////
;
;    Compute Point Entry
;
;    IN  : R3:0  = Table Address
;    OUT : Z     = Check Data Size
;
C_PET
	                                                 MOV #1,LOP
	                                                 MOV M3,CT0
	                                                 MOV SPT,CT1
	                                      MOV M0,A   MOV MXD,PL
	SUB                                              MOV ALL,MC1
	                                                 MOV M0,MC1
	                                                 MOV SPT,CT1
	                                                 MOV MC1,MC0
	                                                 MOV ECN,CT0
	                                                 MOV MC1,MC0
	                                                 MOV M3,CT0
	MVI #0,MC0,S
	                                                 MOV ECN,CT0
	MVI MXD,MC0,NS
	                                                 MOV ECN,CT0
WT0	JMP T0,WT0
	                                      CLR A      MOV M0,PL
	BTM
	ADD                                   MOV ALU,A  MOV SPT,CT1
;
;
IF 0
;
;////////////////////////////////////////////////////////////////////////////
;
;    Program Loader
;
;
	ORG	$FA
LD_PRO
	                                                 MOV TBL,CT0
	                                                 MOV M0,RA0
	DMA D0,PRG,LD_PRO
	MVI START,PC
	NOP
	END
;
ENDIF
;
 	ENDS
;
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
