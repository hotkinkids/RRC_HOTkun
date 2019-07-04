/**
* @file ctrl_PWM.cpp
* @brief ����{�[�h�ʐM�@�f�[�^����
*
* @author Katayama
* @date 2008-03-21
*
* Copyright (C) 2008 TPIP User Community All rights reserved.
* ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
* �g�p���������܂��B
*/
#pragma  warning(disable:4244)
#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// �R���g���[�����͊֐���include file
#include "TPJT3.h"	// ���A���^�C������`���֐��� include file
#include "ctrl_PWM_act.h"
#include "ctrl_PWM_hold.h"

//#include ""

extern struct GC_STRUCT ps2;	// �Q�[���R���g���[������̓��͒l�@�i�[�ϐ�
extern struct OUT_DT_STR out_dt;// ����o�̓f�[�^
extern struct INP_DT_STR inp_dt;// �Z���T�[���̓f�[�^


extern int camNum;		//���[�h�p�t���O��extern
extern int Ctrlmode;
extern int RNo;


/**********************************************************
/
/	tpip�z��@1~20	(0~19�ł͂Ȃ�)
/	�֐��̎g�����͏����Ă���ʂ�
/	�\���L�[�ƃX�e�B�b�N�ƃ{�^���ŏ����Ⴄ�̂�
/	�ʁX�Ɋ֐���p�ӂ���
/	�܂Ƃ߂���̂ł�肽���l�͂���Ă�����
/
//////////////////////////////////////////////////////////*/
//�v���g�^�C�v�錾
void ctrl_PWM(void);

void PWM_act(int u, int d, int p);				//������(�����{�^��,�����{�^��,�o��PWM�̔ԍ�(1�`10))	�Z�~�����p
void PWM_act2(int u, int d, int p);				//�\���L�[�p

void PWM_act(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p
void PWM_act2(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p

													 //�T�[�{
void PWM_hold(int u, int d, int p, int max, int min);
void PWM_hold2(int u, int d, int p, int max, int min);
void PWM_hold3(int u, int d, int p);

void PWM_switch(int u, int d, int p, int out1, int out2);	//�����Ă���Ԃ����Aout�̒l���o�͂���
void PWM_switch2(int u, int d, int p, int out1, int out2);	//�����Ă���Ԃ����Aout�̒l���o�͂���

void PWM_dm(int right, int left);				//�ȑO�܂Ŏg���Ă������E�X�e�B�b�N�ł̑���(DMD�Ȃ�)
void PWM_dmd(int right, int left);				//dmd�p��{�֐�
void PWM_dmd2(int first, int second);			//�\���L�[�ł�dmd����֐�

void PWM_move(int right, int left);				//����͏\���L�[�A������PWM�o��

void PWM_move2(int right, int left);				//����͏\���L�[�A���E���M�n�̂�
void PWM_move2b(int right, int left);			//����̓{�^���A���E���M�n�̂�

												//MT�o��
void PWM_moveEx(int fr, int rr, int fl, int rl);	//���J�i���̈ړ�����֐�*���M�n��R1R2,��]������Έ������₵���֐����
void PWM_moveEx2(int fr, int rr, int fl, int rl);	//�\���L�[�ł̑O�㍶�E��moveEx�������ɓ����Ă�֐�(�}�s�p)�������M�n��R2L2

void PWM_moveSL(int fr, int rr, int fl, int rl);

void PWM_moveExre(int fr, int rr, int fl, int rl);
void PWM_moveEx2re(int fr, int rr, int fl, int rl);

void Mode_change(int b);						//�����̓{�^��,��U�����ė��������Ɏ��s���܂�
void cam_change(int b);

//����
void ctrl_PWM(void)
{
	//����
	//switch�O�ɂ͂��ׂẴ��[�h�ŋ��ʂ��鑀�������

	Mode_change(BTN_ST);//���샂�[�h�؂�ւ�
	cam_change(BTN_SE);//�J�����؂�ւ� 



	switch (Ctrlmode) {//case��mode�A��͊e�X�D���ɐݒ�Bcase���₵�Ă����炵�Ă�������(���̐����͂Ȃ�)
	case 0:
		PWM_moveEx(5, 6, 7, 8);
		//PWM_moveSL(5, 6, 8, 7);		//���J�i���@�E�O�A�E��A���O�A����
		//PWM_act2(BTN_LU, BTN_LD, 11);	//�~���@�\�㉺
		PWM_switch2(BTN_LU, BTN_LD, 11, 700, 500);	//�~���@�\�㉺
		PWM_act(BTN_RR, BTN_RD, 10);		//R������� �������A�f���o��
		PWM_act(BTN_RL, BTN_RU, 9);		//L�������@�������A�f���o��
		PWM_act2(BTN_LL, BTN_LR, 12);    //�~���@�\�O��

		break;
	case 1://���]�}�s

		PWM_moveEx2re(5, 6, 7, 8);	//���]

		PWM_hold(BTN_RU, BTN_RD, 14, 0, -200);	//�X�J�[�g
		break;
	case 2:
		PWM_moveSL(5, 6, 7, 8);  //�������p     

		PWM_act2(BTN_LU, BTN_LD, 13);			//�A�[����
		PWM_hold(BTN_RD, BTN_RR, 2, 1000, -1000);	//�A�[�����
		PWM_hold(BTN_RU, BTN_RL, 1, 1000, -1000);	//�A�[���L���b�`
													//PWM_act(BTN_LL, BTN_LR, 10);	//�~���O��

													//PWM_act_axis(1, 13);//�J�����㉺
		                                            //PWM_dmd(13, 20);

		break;
	default://�����ɂ͑����u���Ȃ�����
		Ctrlmode = 0;
		break;
	}

}

/**********************************************************************************************************************/
void PWM_switch(int u, int d, int p, int out1, int out2) {
	int dt;
	p--;

	if (ps2.BTN_bit&u) {
		out_dt.PWM[p] = out1;
	}
	if (ps2.BTN_bit & d) {
		out_dt.PWM[p] = -out2;
	}
	if ((ps2.BTN_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = 0;
	}
}

void PWM_switch2(int u, int d, int p, int out1, int out2) {
	int dt;
	p--;

	if (ps2.POV_bit&u) {
		out_dt.PWM[p] = out1;
	}
	if (ps2.POV_bit & d) {
		out_dt.PWM[p] = -out2;
	}
	if ((ps2.POV_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = 0;
	}
}

void PWM_dm(int right, int left) {
	right--;
	left--;
	int dr, dl;
	int rx, ly;

	ly = -(ps2.axis[1] * 1100 / AXIS_MAX);
	rx = (ps2.axis[2] * 1100 / AXIS_MAX);

	if (abs(ly)<100) {
		ly = 0;
	}
	else {
		if (ly>0)ly -= 100;
		else	ly += 100;
	}
	if (abs(rx)<100) {
		rx = 0;
	}
	else {
		if (rx>0)rx -= 100;
		else	rx += 100;
	}

	//��

	if (abs(ly) >= abs(rx)) {

		if (ly >= 0) {
			if (rx>0) {
				dr = ly - rx;
				dl = ly;
			}
			else if (rx <= 0) {
				dr = ly;
				dl = ly + rx;
			}
			/*		if(rx==1000){
			dr=PWM_MIN;
			}else if(rx==-1000){
			dl=PWM_MIN;
			}
			*/
		}
		else if (ly<0) {
			if (rx>0) {
				dr = ly + rx;
				dl = ly;
			}
			else if (rx <= 0) {
				dr = ly;
				dl = ly - rx;
			}
			if (rx == 1000) {
				dr = PWM_MAX;
			}
			else if (rx == -1000) {
				dl = PWM_MAX;
			}

		}
	}
	else {
		dr = 0;
		dl = 0;
	}

	if (ps2.BTN_bit & BTN_L1) {
		dr = 700; dl = -700;
	}
	else if (ps2.BTN_bit & BTN_R1) {
		dr = -700; dl = 700;
	}
	out_dt.PWM[right] = dr;
	out_dt.PWM[left] = dl;


}

void Mode_change(int b) {
	static bool flag = false;
	if (ps2.BTN_bit&b)
		flag = true;
	if (!(ps2.BTN_bit&b) && flag) {
		Ctrlmode++;
		flag = false;
	}

}
void cam_change(int b) {
	static bool flag = false;
	if (ps2.BTN_bit&b)
		flag = true;
	if (!(ps2.BTN_bit&b) && flag) {
		camNum = (camNum + 1) % 3; //%�őS�J�����䐔���w��
		TPJT_chg_camera(camNum);

		flag = false;
	}

}

void PWM_dmd(int first, int second) {
	first--; second--;
	int ry, ly;

	ly = (ps2.axis[5] * 1100 / AXIS_MAX);
	ry = (ps2.axis[1] * 1100 / AXIS_MAX);
	//�������l���̏���
	if (abs(ry)<200) {
		ry = 0;
	}
	else {
		if (ry>0)ry -= 100;
		else ry += 100;
	}
	if (abs(ly)<200) {
		ly = 0;
	}
	else {

		if (ly>0)ly -= 100;
		else ly += 100;
	}

	out_dt.PWM[first] = ly;
	out_dt.PWM[second] = ry;

}
void PWM_dmd_back(int first, int second) {
	first--; second--;
	int ry, ly;

	ly = (ps2.axis[1] * 1100 / AXIS_MAX);
	ry = (ps2.axis[5] * 1100 / AXIS_MAX);
	//�������l���̏���
	if (abs(ry)<200) {
		ry = 0;
	}
	else {
		if (ry>0)ry -= 100;
		else ry += 100;
	}
	if (abs(ly)<200) {
		ly = 0;
	}
	else {

		if (ly>0)ly -= 100;
		else ly += 100;
	}

	out_dt.PWM[first] = ly;
	out_dt.PWM[second] = ry;



}

void PWM_dmd2(int first, int second) {
	int dt = 0;

	if (ps2.POV_bit & BTN_LU) {//�O
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MAX;
		else out_dt.PWM[first] = PWM_MAX;
	}
	if (ps2.POV_bit & BTN_LD) {//��
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MIN;
		else out_dt.PWM[first] = PWM_MIN;
	}
	if (ps2.POV_bit & BTN_LR) {//�E���M�n
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MAX;
		else out_dt.PWM[first] = PWM_MAX;
	}
	if (ps2.POV_bit & BTN_LL) {//�����M�n
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MIN;
		else out_dt.PWM[first] = PWM_MIN;
	}


}
