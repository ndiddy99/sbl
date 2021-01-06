!-----------------------------------------------------------------------------
!   spr_fmat.src -- SPR ���C�u���� FMAT ���W���[��
!   Copyright(c) 1994 SEGA
!   Written by T.S on 1994-01-24 Ver.0.80
!   Updated by H.E on 1994-02-15 Ver.0.80
!
!   ���̃��C�u�����͓����ϊ��������W���[���ŁA
!   �ȉ��̃��[�`�����܂ށB
!
!   MTH_Pers2D              -  �����ϊ�����
!   MTH_ComputeBright       -  �|���S���̉B�ʔ���ƋP�x�v�Z
!
!-----------------------------------------------------------------------------
!
	.global _MTH_Pers2D
	.global _MTH_ComputeBright
	.section .text

!
!-----------------------------------------------------------------------------
!
!  NAME : MTH_Pers2D  -  Persepect 3D to 2D
!
!        void  MTH_Pers2D(MthXyz *p3d, MthXy *unitPixel, XyInt *p2d)!
!
!  PARAMETERS
!
!  �@�@(1) MthXyz   *p3d        - <i> r4 ���_���W�n�R�c���_���W
!  �@�@(2) MthXy    *unitPixel  - <i> r5 �X�N���[���w�x�̒P�ʃs�N�Z����
!  �@�@(3) XyInt    *p2d        - <o> r6 �����ϊ���X�N���[���Q�c���W
!
!  DESCRIPTION
!
! 	���_���W�n�̌��_�����_�Ƃ��� -1.0 �̂Ƃ���ɃX�N���[����ݒ肵�ĂR�c
!       ����Q�c�ւ̓����ϊ����s���B�X�N���[����� 1.0 �̑傫�����X�N���[��
!       �w�x�̒P�ʃs�N�Z�����ɑΉ�����B
!
!  POSTCONDITIONS
!
!  CAVEATS
!
!-----------------------------------------------------------------------------
!
	.align 2

_MTH_Pers2D:
 	STS.L	mach,@-r15
 	STS.L	macl,@-r15
!
! hz = MTH_Div(MTH_FIXED(1),p3d->z)!
!
	MOV.L	A_DIV,r0           !* FIXED(1) / p3d->z *
        MOV.L   @(8,r4),r1
	MOV.L	r1,@r0             ! <- p3d->z
	MOV	#1,r1
	MOV.L	r1,@(0x10,r0)      ! <- 0x00000001 (upper)
	MOV  	#0,r1
	MOV.L	r1,@(0x14,r0)      ! <- 0x00000000 (lower)
!
! wx = MTH_Mul(p3d->x,unitPixel->x)!
!
        MOV.L   @r4+,r1            ! r1 = p3d->x
        MOV.L   @r5+,r2            ! r2 = unitPixel->x
	DMULS.L r1,r2              ! mach,L = r1 * r2
 	STS 	mach,r1
 	STS 	macl,r2            ! r1,r2 = mach,macl
 	XTRCT	r1,r2	           ! -> r2
!
! wy = MTH_Mul(p3d->y,unitPixel->y)!
!
        MOV.L   @r4+,r3            ! r3 = p3d->x
        MOV.L   @r5+,r4            ! r4 = unitPixel->x
	DMULS.L r3,r4              ! mach,L = r3 * r4
 	STS	mach,r3
 	STS	macl,r4            ! r3,r4 = mach,macl
 	XTRCT	r3,r4	           ! -> r4
!
 	MOV.L	@(0x14,r0),r1      ! -> r1 (= 1/z)
!
! p2d->x = MTH_FixedToInt(MTH_Mul(wx,hz))!
!
	DMULS.L r1,r2              ! mach,L = x * (1/z)
 	STS	mach,r2
 	STS	macl,r3            ! r1,r2 = mach,macl
 	XTRCT	r2,r0	           ! -> r3
	SHLR16	r0
	MOV.W	r0,@r6             ! r3 -> pers X
!
! p2d->y = MTH_FixedToInt(MTH_Mul(wy,hz))!
!
	DMULS.L r1,r4              ! mach,L = x * (1/z)
 	STS	mach,r2
 	STS	macl,r3            ! r1,r2 = mach,macl
 	XTRCT	r2,r0	           ! -> r3
 	LDS.L	@r15+,macl
 	LDS.L	@r15+,mach
	SHLR16	r0
	RTS
	MOV.W	r0,@(2,r6)         ! r3 -> pers Y
!
	.align 2

A_DIV:	.long	0xFFFFFF00
!
!
!--------------------------------------------------------------------------
!
! NAME : MTH_ComputeBright  -  Compute Brightness
!
! PARAMETERS
!
! �@�@(1) MthXyz   *lightVector   - <i> �����x�N�g��
! �@�@(2) MthXyz   *normalVector  - <i> �|���S���̖@���x�N�g��
!
! DESCRIPTION
!
!      �����x�N�g������|���S���ʂ̋P�x��Ԃ��B
!
! POSTCONDITIONS
!
!      Sint32   bright  :  0�i�Áj ���� 31�i���j
!
! CAVEATS
!
!-------------------------------------------------------------------------
!
	.align 2

_MTH_ComputeBright:
 	STS	mach,r1
 	STS	macl,r2
!
! product = MTH_Product((Fixed32*)normVector, (Fixed32*)lightVector)!
!
	CLRMAC
	MAC.L   @r4+,@r5+
	MAC.L   @r4+,@r5+
	MAC.L   @r4+,@r5+
 	STS	mach,r3
 	STS	macl,r0
!
! color = MTH_FixedToInt((product + MTH_FIXED(1.0)) * (32/2))!
!
	ADD	#1,r3
 	XTRCT	r3,r0	! xtract for Fixed32
	MOV	#16,r3
	DMULS.L r3,r0
 	STS	mach,r3
 	STS	macl,r0
	SHLR16	r0
!
! return  color & 0x1f!
!
 	LDS	r1,mach
 	LDS	r2,macl
	RTS
	AND	#0x1f,r0
	NOP
!
!---------------------------------------------------------------------
	.end
!---------------------------------------------------------------------
!   end of file
