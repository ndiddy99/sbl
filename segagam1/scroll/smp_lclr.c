
/*	Copyright(c) 1994 SEGA			*/
/*
 *     
 *     ���C���J���[���g�����T���v��
 *
 */

#include "smp_bios.h"
#include "smp_sccg.h"
#include "smp_lclr.h"


#define SMPR_logocharNo 0x40


#define DrawPolMax    0x200   /* �ő�|���S����           */
#define ObjSurfMax     16     /* �P�I�u�W�F�N�g�ő�ʐ�   */
#define ObjVertMax     16     /* �P�I�u�W�F�N�g�ő咸�_�� */

/* �RD���C�u�������[�N��` */
SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax )



/* ���̖ʃf�[�^ */
#define HAKO_DRAW	0x4000 |00 | COLOR_5 | ECDSPD_DISABLE | COMPO_REP
#define HAKO_COLOR 	0x41    /* �e�N�X�`���L�����N�^ No */


/* �v���[���[�̖ʃf�[�^ */
#define PL_DRAW1 	00 | COLOR_0 | ECDSPD_DISABLE | COMPO_REP
#define PL_COLOR1 	RGB16_COLOR(10,31,0)




/* �� �e�N�X�`���e�[�u�� */
const SprTexture YUKATEXT[] = {
    {
	0x41,			/* �L�����N�^No */
	COLOR_5,		/* �J���[���[�h */
        0,
	32,			/* �L�����N�^�� */
	32,			/* �L�����N�^���� */
	&YUKA_cg[0],         	/* �L�����N�^�|�C���^ */
	NULL			/* ���b�N�A�b�v�e�[�u���|�C���^ */
      },
    {
	0xffff,			/* �L�����N�^�ԍ� */
	COLOR_5,		/* �J���[���[�h */
        0,
	0,			/* �L�����N�^�� */
	0,			/* �L�����N�^���� */
	NULL,			/* �L�����N�^�|�C���^ */
	NULL			/* ���b�N�A�b�v�e�[�u���|�C���^ */
    }

};

/* �� ���_���W�e�[�u�� */
const MthXyz HakoVertPoint[] = {
	{MTH_FIXED(-0.1), MTH_FIXED(-0.1), MTH_FIXED(-0.1)}, /* 0 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.1), MTH_FIXED(-0.1)}, /* 1 */
	{MTH_FIXED(0.1),  MTH_FIXED(0.1),  MTH_FIXED(-0.1)}, /* 2 */ 
	{MTH_FIXED(-0.1), MTH_FIXED(0.1),  MTH_FIXED(-0.1)}, /* 3 */

	{MTH_FIXED(-0.1), MTH_FIXED(-0.1), MTH_FIXED(0.1)},  /* 4 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.1), MTH_FIXED(0.1)},  /* 5 */
	{MTH_FIXED(0.1),  MTH_FIXED(0.1),  MTH_FIXED(0.1)},  /* 6 */ 
	{MTH_FIXED(-0.1), MTH_FIXED(0.1),  MTH_FIXED(0.1)},  /* 7 */

};

/* �� �ʒ�`�e�[�u�� */
const SprSurface HakoSurface[] = {
	{	0,1,2,3,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,5,6,7,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,0,3,7,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	1,5,6,2,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	4,5,1,0,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},
	{	7,6,2,3,	/* �ʂ��\�����钸�_�ԍ� */
		HAKO_DRAW | ZSORT_MIN,
		HAKO_COLOR
	},

};

/* �� �ʂ̖@���x�N�g���e�[�u�� */
const MthXyz HakoMenHosen[6] = {
	{MTH_FIXED(0),   MTH_FIXED(0),  MTH_FIXED(-1)},
	{MTH_FIXED(0),   MTH_FIXED(0),  MTH_FIXED(1)},
	{MTH_FIXED(-1),  MTH_FIXED(0),  MTH_FIXED(0)},
	{MTH_FIXED(1),   MTH_FIXED(0),  MTH_FIXED(0)},
	{MTH_FIXED(0),   MTH_FIXED(-1), MTH_FIXED(0)},
	{MTH_FIXED(0),   MTH_FIXED(1),  MTH_FIXED(0)},
};


/* �� �I�u�W�F�N�g��` */
const SprObject3D Hako_Obj = {
        1,                         /* �I�u�W�F�N�g�ԍ�                   */
	DISP_POLYGON,              /* �\���t���O                         */
	8,		           /* ���_��                             */
	6,			   /* �ʐ�                               */
	(MthXyz *)&HakoVertPoint,   /* ���_���W�e�[�u��                   */
	NULL,			   /* ���_�̖@���x�N�g���e�[�u��         */
	(SprSurface *)&HakoSurface, /* �ʒ�`�e�[�u��                     */
	(MthXyz *)&HakoMenHosen,    /* �ʂ̖@���x�N�g���e�[�u��           */
        NULL,                      /* �ʂ̑�\���_���W�e�[�u��           */
	NULL,                      /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u�� */
        MTH_FIXED(1.0),
	NULL		 	   /* ���I�u�W�F�N�g                     */
};




/* �� �N���X�^��` */
const SprCluster Hako_Cluster = {
        1,                          /* �N���X�^�ԍ�  */
	ROT_SEQ_XYZ,		    /* ��]���� */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	&Hako_Obj,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL     		    /* �q�N���X�^ */
};





/* �v���[���[ ���_���W�e�[�u�� */
const MthXyz PlVertPoint[] = {
	{MTH_FIXED(0),    MTH_FIXED(0.02),  MTH_FIXED(0.5)},    /* 0 */
	{MTH_FIXED(-0.1), MTH_FIXED(-0.02), MTH_FIXED(0.5)},    /* 1 */
	{MTH_FIXED(0.1),  MTH_FIXED(-0.02), MTH_FIXED(0.5)},    /* 2 */
	{MTH_FIXED(0),    MTH_FIXED(0),     MTH_FIXED(0.54)}, /* 3 */
};

/* �v���[���[ �ʒ�`�e�[�u�� */
const SprSurface PlSurface[] = {
	{	0,1,2,0,	/* �ʂ��\�����钸�_�ԍ� */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	0,3,1,0,	/* �ʂ��\�����钸�_�ԍ� */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	0,3,2,0,	/* �ʂ��\�����钸�_�ԍ� */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},
	{	3,1,2,3,	/* �ʂ��\�����钸�_�ԍ� */
		PL_DRAW1 | ZSORT_MIN,
		PL_COLOR1
	},

};

/* �v���[���[ �ʂ̖@���x�N�g���e�[�u�� */
const MthXyz PlMenHosen[4] = {
	{MTH_FIXED(0.0),  MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(-1.0), MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(1.0),  MTH_FIXED(0.0), MTH_FIXED(-1.0)},
	{MTH_FIXED(0.0),  MTH_FIXED(-1.0), MTH_FIXED(-1.0)},
};


/* �v���[���[ �I�u�W�F�N�g��` */
const SprObject3D PL_Obj = {
        1,                          /* �I�u�W�F�N�g�ԍ�                   */
	DISP_POLYLINE,              /* �\���t���O                         */
	4,			    /* ���_��                             */
	4,			    /* �ʐ�                               */
	(MthXyz *)&PlVertPoint,     /* ���_���W�e�[�u��                   */
	NULL,			    /* ���_�̖@���x�N�g���e�[�u��         */
	(SprSurface *)&PlSurface,   /* �ʒ�`�e�[�u��                     */
	(MthXyz *)&PlMenHosen,      /* �ʂ̖@���x�N�g���e�[�u��           */
        NULL,                       /* �ʂ̑�\���_���W�e�[�u��           */
	NULL,                       /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u�� */
        MTH_FIXED(1.0),
	NULL		 	    /* ���I�u�W�F�N�g                     */
};

/* �v���[���[ �N���X�^��` */
const SprCluster Pl_Cluster = {
        1,                          /* �N���X�^�ԍ�  */
	ROT_SEQ_XYZ,		    /* ��]���� */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	&PL_Obj,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL     		    /* �q�N���X�^ */
};



/* 
 *    �O���Q�ƃ��[�N
 */
extern SclSysreg Scl_s_reg;
extern SclNorscl Scl_n_reg;


/*
 *    �֐��v���g�^�C�v
 */
void SMLC_cgset(void);
void SMLC_HakoAct(G3DACT *ix);
void SMLC_PlayerAct(G3DACT *ix);
void SMLC_PutObject(PLYOBJ *obj);



/*
 *  ���C���J���[���g�����T���v��
 */
void SMLC_LclrMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k;
    Uint32 times;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    XyInt reg_xy;

    while (SMMA_MainMode == LCLR_MODE) {
        SMV1_SprCmdStart();
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SPR_3Initial(&work3D);
                SCL_SetPriority(SCL_SP0, 6);
                SCL_SetPriority(SCL_NBG0, 5);
                SCL_SetPriority(SCL_NBG1, 0);
                SCL_SetPriority(SCL_RBG0, 0);


                /* �J���[���[�h�̐ݒ� */
                SCL_SetColRamMode(SCL_CRM15_2048);
                
                /* �X�v���C�g�f�[�^�^�C�v�̐ݒ� */
                SCL_SetSpriteMode(SCL_TYPE7, SCL_MIX, SCL_SP_WINDOW);

                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* CG�f�[�^�̓W�J */
                SMLC_cgset();

                /* �v���[���[�A�N�V�����̃Z�b�g */
                SMTA_MakeAction(SMLC_PlayerAct);

                SMV2_TvOn();               
                SMMA_MainLevel++;
            case MAIN:
                /* �o�b�N�J���[�Z�b�g */
                poke_w(BCLRTBLADD, ((31 << 10) + (31 << 5) + 31));    

                /* ���[�J�����W�i�X�v���C�g���_�j�̃Z�b�g */
                reg_xy.x = WIDTH_H / 2;
                reg_xy.y = WIDTH_V / 2;
                SPR_2LocalCoord(SPR_2MOST_FAR+3, &reg_xy);

                /* �𑜓x�Z�b�g */
                SPR_3SetPixelCount(320, 224);

                /* �N���b�v���x���̐ݒ� */
                SPR_3SetClipLevel(3, MTH_FIXED(-20.0), MTH_FIXED(-0.1));

                /* �A�N�V�������s */
                SMTA_ActionLoop();

                /* �|���S���`��   */
                SPR_3Flush();

                /* �X�^�[�g�����ꂽ�甲���� */
                if (SMPA_pad_edge1 & PAD_START)
                    SMMA_MainLevel++;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    }

}

	    
/*
 *   SPRITE CG SET
 */
void SMLC_cgset(void)
{
    const Uint16 nbg1_map[] = {
        0x1082,0x1084,0x1086,0x1088,
        0x108a,0x108c,0x108e,0x1090,
        0x1092,0x1094,0x1096,0x1098,
        0x109a,0x109c,0x109e,0x10a0,
    };

    Uint16 *dst16, *src16, *temp16, *wrptr16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;
    Uint32 direc;


    /* �X�v���C�g�o�^�L�����N�^������ */
    SPR_2ClrAllChar();

    /* �A�X�L�[�t�H���g�A�J���[�Z�b�g */
    SMV1_SetFont4sp(0,1,0);
    SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
    poke_w(COLOR_RAM+0x20, 0x7000);
    poke_w(COLOR_RAM+0x22, 0x739c);

    /* �e�N�X�`���f�[�^�Z�b�g */
    SPR_3SetTexture(YUKATEXT);

    /* �o�b�N�J���[�Z�b�g */
    poke_w(BCLRTBLADD, ((31 << 10) + (31 << 5) + 31));    

    /* ���C���J���[�̐ݒ� */
    wrptr16 = (Uint16 *)(COLOR_RAM + 0x7c0);
    for (i = 0; i < 0x20; i++)
         *wrptr16++ = (i << 10) + (i << 5) + i;

    wrptr16 = (Uint16 *)(LCLRTBLADD);
    for (i = 0; i < 0xe0; i++) {
         direc = (i * 0x80) / 0xe0;
         *wrptr16++ = 0x3e0 + (0x1f * SMMI_Sinset8(direc)) / 0x100;
    }

    SCL_Open(SCL_NBG0);
    Scl_n_reg.linecolmode = 0x80000000 | ((LCLRTBLADD & 0xfffff) / 2);
    SCL_Close();

    /* �J���[���Z���[�h�̐ݒ� */
    SCL_SET_CCMD(1);
    SCL_SET_SPCCEN(1);
    SCL_SET_LCCCEN(1);
    SCL_SET_EXCCEN(0);
    SCL_SET_SPLCEN(1);  /* ���C���J���[ */
    SCL_SET_SPCCCS(3);  /* �X�v���C�g�F���Z���� */


}
    




/*
 *     ��
 */
void SMLC_HakoAct(G3DACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};


    Sint32 ld0, ld1, ld2;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    G3DACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->obj.clsptr = &Hako_Cluster;
            ix->obj.direc.x = MTH_FIXED(0);
            ix->obj.direc.y = MTH_FIXED(0.0); 
            ix->obj.direc.z = MTH_FIXED(0);
            ix->obj.speed.x = 0;
            ix->obj.speed.y = 0;
            ix->obj.speed.z = 0;
            ix->mode++;
        case MOVE:
            /* ix->msttbl �ɂ� �v���[���[�A�N�V�����̃|�C���^�������Ă��� */
            iy = ix->msttbl;
            ix->obj.speed.z = iy->obj.speed.z;

            ix->obj.posi.z -= ix->obj.speed.z;
            if (ix->obj.posi.z < MTH_FIXED(-3.0))
                SMTA_KillAction(ix);
            else
                SMLC_PutObject(&ix->obj);
    }
   
}



/*
 *    �v���[���[�A�N�V����
 */
void SMLC_PlayerAct(G3DACT *ix)
{

    enum {
        INIT,
        MOVE,
        NEXT
	};

    Uint16 swdata, swdatax, xlp, ylp;
    Sint32 ld0, ld1, ld2;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    G3DACT *iy;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case INIT:
            ix->obj.clsptr = &Pl_Cluster;
            ix->obj.posi.x = MTH_FIXED(0.5);
            ix->obj.posi.y = MTH_FIXED(0.25);
            ix->obj.posi.z = MTH_FIXED(-3); 
            ix->obj.direc.x = MTH_FIXED(0);
            ix->obj.direc.y = MTH_FIXED(0.1);
            ix->obj.direc.z = MTH_FIXED(0.0);
            ix->obj.speed.x = MTH_FIXED(0.0);
            ix->obj.speed.y = MTH_FIXED(0.0);
            ix->obj.speed.z = MTH_FIXED(0.0);
            ix->obj.acc.x = MTH_FIXED(0.0);
            ix->obj.acc.y = MTH_FIXED(0.0);
            ix->obj.acc.z = MTH_FIXED(0.0);
            ix->lreg[0] = MTH_FIXED(0.0);
            ix->lreg[1] = MTH_FIXED(0.0);
            ix->lreg[2] = MTH_FIXED(0.0);
            ix->lreg[3] = 0;
        
            ix->mode++;

        case MOVE:

            if (swdata & PAD_C) {
                ix->obj.speed.z += MTH_FIXED(0.01);
                if (ix->obj.speed.z > MTH_FIXED(0.1))
                    ix->obj.speed.z = MTH_FIXED(0.1);
            } else {
                ix->obj.speed.z -= MTH_FIXED(0.01);
                if (ix->obj.speed.z < MTH_FIXED(0.03))
                    ix->obj.speed.z = MTH_FIXED(0.03);
            }


            switch (swdata & (PAD_LEFT + PAD_RIGHT)) {
                case PAD_LEFT:
                    ix->obj.speed.x = MTH_FIXED(0.02);
                    ix->obj.direc.z -= MTH_FIXED(2.0);
                    if (ix->obj.direc.z < MTH_FIXED(-10.0))
                        ix->obj.direc.z = MTH_FIXED(-10.0);
                    break;

                case PAD_RIGHT:
                    ix->obj.speed.x = MTH_FIXED(-0.02);
                    ix->obj.direc.z += MTH_FIXED(2.0);
                    if (ix->obj.direc.z > MTH_FIXED(10.0))
                        ix->obj.direc.z = MTH_FIXED(10.0);
                    break;

                case 0:
                    ix->obj.speed.x = MTH_FIXED(0.0);

                    if (ix->obj.direc.z > MTH_FIXED(0)) {
                        ix->obj.direc.z -= MTH_FIXED(1.0);
                        if (ix->obj.direc.z < MTH_FIXED(0))
                            ix->obj.direc.z = MTH_FIXED(0);
                    }

                    if (ix->obj.direc.z < MTH_FIXED(0)) {
                        ix->obj.direc.z += MTH_FIXED(1.0);
                        if (ix->obj.direc.z > MTH_FIXED(0))
                            ix->obj.direc.z = MTH_FIXED(0);
                    }
                    break;
            }
         

           
            ix->obj.posi.x += ix->obj.speed.x;
            ix->obj.posi.y += ix->obj.speed.y;

            if (ix->obj.posi.x < MTH_FIXED(0))
                ix->obj.posi.x = MTH_FIXED(0);

            if (ix->obj.posi.x > MTH_FIXED(1.0))
                ix->obj.posi.x = MTH_FIXED(1.0);


            /* ���_�̐ݒ� */
            ix->lreg[0] = ix->obj.direc.x;
            ix->lreg[1] = ix->obj.direc.y;
            ix->lreg[2] = MTH_FIXED(0.0) - ix->obj.direc.z;
            SPR_3SetView(1, &ix->obj.posi, 
                            (MthXyz *)&ix->lreg[0], ROT_SEQ_XYZ,0);

            /* �����̐ݒ� */
            SPR_3SetLight(1, &ix->obj.direc);

            /* lreg[3]������l���z����Ɣ����Z�b�g���� */
            ix->lreg[3] += ix->obj.speed.z;
            if (ix->lreg[3] > MTH_FIXED(0.5)) {
                ix->lreg[3] = MTH_FIXED(0.0);
                for (xlp = 0; xlp < 3; xlp++) {
                    for (ylp = 0; ylp < 2; ylp++) {
                        iy = SMTA_MakeAction(SMLC_HakoAct);
                        iy->obj.posi.x = MTH_FIXED(0.5) * xlp;
                        iy->obj.posi.y = MTH_FIXED(0.5) * ylp;
                        iy->obj.posi.z = MTH_FIXED(0.5) * 2;
                        iy->msttbl = ix;
                    }
                }
            }

            break;
    }
   
}




/*
 * �|���S���\���֐�
 */
void SMLC_PutObject(PLYOBJ *obj)
{
    SPR_3MoveCluster(obj->clsptr, 1, &obj->direc, &obj->posi);
    SPR_3DrawModel(obj->clsptr);
}








