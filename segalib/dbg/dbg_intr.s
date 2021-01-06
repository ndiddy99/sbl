!----------------------------------------------------------------------------
!  dbg_moni.c -- DBG ���C�u���� Intr ���W���[��
!  Copyright(c) 1994 SEGA
!  Written by H.E on 1994-07-25 Ver.1.00
!  Updated by H.E on 1994-07-25 Ver.1.00
!
!  ���̃��C�u�����̓f�o�b�O�T�|�[�g���C�u�����̗�O���荞�ݏ����̃G���g����
!  �܂ށB
!
!  DBG_IntrGenOpErr        -  ��ʕs�����ߏ���
!  DBG_IntrSlotOpErr       -  �X���b�g�s�����ߏ���
!  DBG_IntrCpuAddrErr      -  �b�o�t�A�h���X�G���[����
!  DBG_IntrDmaAddrErr      -  �c�l�`�A�h���X�G���[����
!
!----------------------------------------------------------------------------
          .extern     _DbgRegAreaP
          .extern     _DbgIntrKind
          .extern     _DBG_IntrProc

          .global     _DBG_IntrGenOpErr
          .global     _DBG_IntrSlotOpErr
          .global     _DBG_IntrCpuAddrErr
          .global     _DBG_IntrDmaAddrErr
!
          .section    .text

					.align 2
!
!*****************************************************************************
!* NAME:  _DBG_IntrGenOpErr        -   ��ʕs�����ߏ����G���g��
!*****************************************************************************
_DBG_IntrGenOpErr:
!
          MOV.L       r0,@-r15          ! push R0
          MOV         #0,r0
          BRA         intrProc
          NOP
!
!*****************************************************************************
!* NAME:  _DBG_IntrSlotOpErr       -  �X���b�g�s�����ߏ����G���g��
!*****************************************************************************
_DBG_IntrSlotOpErr:
!
          MOV.L       r0,@-r15          ! push R0
          MOV         #1,r0
          BRA         intrProc
          NOP
!
!*****************************************************************************
!* NAME:  _DBG_IntrCpuAddrErr      -  �b�o�t�A�h���X�G���[�����G���g��
!*****************************************************************************
_DBG_IntrCpuAddrErr:
!
          MOV.L       r0,@-r15          ! push R0
          MOV         #2,r0
          BRA         intrProc
          NOP
!
!*****************************************************************************
!* NAME:  _DBG_IntrDmaAddrErr      -  �c�l�`�A�h���X�G���[�����G���g��
!*****************************************************************************
_DBG_IntrDmaAddrErr:
!
          MOV.L       r0,@-r15          ! push R0
          MOV         #3,r0
          BRA         intrProc
          NOP
!
!*****************************************************************************
!*      ��O���荞�ݏ���
!*****************************************************************************
intrProc:
                                        ! R0 : intr kind
          MOV.L       r1,@-r15          ! push R1
          MOV.L       r2,@-r15          ! push R2
          MOV.L       r3,@-r15          ! push R3
          MOV.L       r4,@-r15          ! push R4
          MOV.L       r5,@-r15          ! push R5
          MOV.L       r6,@-r15          ! push R6
          MOV.L       r7,@-r15          ! push R7
          MOV.L       r8,@-r15          ! push R8
          MOV.L       r9,@-r15          ! push R9
          MOV.L       r10,@-r15         ! push R10
          MOV.L       r11,@-r15         ! push R11
          MOV.L       r12,@-r15         ! push R12
          MOV.L       r13,@-r15         ! push R13
          MOV.L       r14,@-r15         ! push R14
          MOV         r15,r1
          ADD         #72,r1
          MOV.L       r1,@-r15          ! push R15 (���荞�ݑO�̒l�j
          STS.L       macl,@-r15        ! push MACL
          STS.L       mach,@-r15        ! push MACH
          STC.L       vbr,@-r15         ! push VBR
          STC.L       gbr,@-r15         ! push GBR
          STS.L       pr,@-r15          ! push PR
!
          MOV.L       DbgIntrKind,r7
          MOV.L       r0,@r7            ! DbgIntrKind = R0
!
          MOV.L       DbgRegAreaP,r7
          MOV.L       r15,@r7           ! DbgRegAreaP = Register save area
!
          MOV.L       DBG_IntrProc,r3
          JSR         @r3               ! DBG_IntrProc()
          NOP
!
	  .ALIGN      2
DbgIntrKind:
          .long     _DbgIntrKind
DbgRegAreaP:
          .long     _DbgRegAreaP
DBG_IntrProc:
          .long     _DBG_IntrProc
!
          .END
