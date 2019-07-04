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
void PWM_act(int u, int d, int p);				//������(�����{�^��,�����{�^��,�o��PWM�̔ԍ�(1�`10))
void PWM_act2(int u, int d, int p);				//�\���L�[�p

void PWM_act(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p
void PWM_act2(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p

													 //�T�[�{
void PWM_hold(int u, int d, int p, int max, int min);
void PWM_hold2(int u, int d, int p, int max, int min);
void PWM_hold3(int u, int d, int p);

void PWM_dm(int right, int left);				//�ȑO�܂Ŏg���Ă������E�X�e�B�b�N�ł̑���(DMD�Ȃ�)
void PWM_dmd(int right, int left);				//dmd�p��{�֐�
void PWM_dmd2(int first, int second);			//�\���L�[�ł�dmd����֐�
void PWM_move(int right, int left);				//����͏\���L�[�A������PWM�o��

void PWM_move2(int right, int left);				//����͏\���L�[�A���E���M�n�̂�
void PWM_move2b(int right, int left);			//����̓{�^���A���E���M�n�̂�

												//MT�o��
void PWM_moveEx(int fr, int rr, int fl, int rl);	//���J�i���̈ړ�����֐�*���M�n��R1R2,��]������Έ������₵���֐����
void PWM_moveEx2(int fr, int rr, int fl, int rl);	//�\���L�[�ł̑O�㍶�E��moveEx�������ɓ����Ă�֐�(�}�s�p)�������M�n��R2L2

													//void PWM_moveExslow(int fr, int rr, int fl, int rl);

void PWM_moveExre(int fr, int rr, int fl, int rl);
void PWM_moveEx2re(int fr, int rr, int fl, int rl);

void PWM_act_axis(int f, int p);

void Mode_change(int b);						//�����̓{�^��,��U�����ė��������Ɏ��s���܂�
void cam_change(int b);

//����
void ctrl_PWM(void)
{
	//����
	//switch�O�ɂ͂��ׂẴ��[�h�ŋ��ʂ��鑀�������

	Mode_change(BTN_ST);//���샂�[�h�؂�ւ�(start�{�^��)
	cam_change(BTN_SE);//�J�����؂�ւ�

	switch (Ctrlmode) {//case��mode�A��͊e�X�D���ɐݒ�Bcase���₵�Ă����炵�Ă�������(���̐����͂Ȃ�)

	case 0:

		//*//1

		PWM_moveEx(5, 6, 7, 8);			//���J�i���}�s�p
		PWM_act2(BTN_LU, BTN_LD, 11);	//�~���@�\�㉺
		PWM_act2(BTN_LR, BTN_LL, 13);   //��]�@�\
		PWM_act(BTN_R2, BTN_L2, 9);		//�x���g
		PWM_act(BTN_RU, BTN_RD, 12);     //�x���g��
		PWM_act(BTN_RR, BTN_RL, 10);		//�O��

		break;

	case 1://���]�}�s

		PWM_moveEx2re(5, 6, 7, 8);	//���]

		PWM_hold(BTN_RU, BTN_RD, 3, 600, 0);	//�X�J�[�g
		break;
	case 2:


		PWM_moveEx2(5, 6, 7, 8);//�������p

		PWM_act(BTN_RU, BTN_RL, 13);				//�A�[����
		PWM_hold(BTN_RR, BTN_RD, 1, 1000, -1000);	//�A�[�����
		PWM_hold(BTN_L3, BTN_R3, 2, 1000, -1000);	//�A�[���L���b�`

		PWM_act_axis(1, 13);//�J�����㉺
		PWM_dmd(13, 20);

		break;
		//*/
	default://�����ɂ͑����u���Ȃ�����
		Ctrlmode = 0;
		break;
	}

}


/***************************

�����`

***************************/

void PWM_act(int u, int d, int p) {
	int dt;
	p--;
	if (ps2.BTN_bit&u) {
		dt = out_dt.PWM[p] + 10;
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX;//�����l�͓K����
		else out_dt.PWM[p] = PWM_MAX;
	}
	if (ps2.BTN_bit & d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN;
		else out_dt.PWM[p] = PWM_MIN;
	}
	if ((ps2.BTN_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = PWM_AVE;
	}
}

void PWM_act2(int u, int d, int p) {
	int dt;
	p--;
	if (ps2.POV_bit&u) {
		dt = out_dt.PWM[p] + 10;
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX;//�����l�͓K����
		else out_dt.PWM[p] = PWM_MAX;
	}
	if (ps2.POV_bit&d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN;
		else out_dt.PWM[p] = PWM_MIN;
	}
	if ((ps2.POV_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = PWM_AVE;
	}
}

void PWM_act(int u, int d, int p, int max, int min) {
	int dt;
	p--;

	if (max<PWM_AVE) max = PWM_AVE;
	if (min>PWM_AVE) min = PWM_AVE;

	if (ps2.BTN_bit&u) {
		out_dt.PWM[p] = max;
	}
	if (ps2.BTN_bit & d) {
		out_dt.PWM[p] = min;
	}
	if ((ps2.BTN_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = PWM_AVE;
	}
}

void PWM_act2(int u, int d, int p, int max, int min) {
	int dt;
	p--;

	if (max<PWM_AVE) max = PWM_AVE;
	if (min>PWM_AVE) min = PWM_AVE;

	if (ps2.POV_bit&u) {
		dt = out_dt.PWM[p] + 10;
		if (dt < PWM_MAX&& dt < max) out_dt.PWM[p] += 10;//�����l�͓K����
		else out_dt.PWM[p] = max;
	}
	if (ps2.POV_bit & d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN&&dt > min) out_dt.PWM[p] -= 10;
		else out_dt.PWM[p] = min;
	}
	if ((ps2.POV_bit & (u + d)) == 0) { //�����̃{�^����������Ă��Ȃ����A���ϒl��
		out_dt.PWM[p] = PWM_AVE;
	}
}

void PWM_hold(int u, int d, int p, int max, int min) {
	int dt;
	p--;


	if (ps2.BTN_bit&u) {
		dt = out_dt.PWM[p] + 0;
		if (dt < 0 || dt < max) {
			out_dt.PWM[p] += 30;//�����l�͓K����
		}
		else {
			out_dt.PWM[p] = max;
		}
	}
	if (ps2.BTN_bit & d) {
		dt = out_dt.PWM[p] - 0;
		if (dt > 0 || dt > min) {    //������ƒx���H��������
			out_dt.PWM[p] -= 30;
		}
		else {
			out_dt.PWM[p] = min;
		}
	}

}

void PWM_hold2(int u, int d, int p, int max, int min) {
	int dt;
	p--;


	if (ps2.POV_bit&u) {
		dt = out_dt.PWM[p] + 50;
		if (dt < PWM_MAX && dt < max) out_dt.PWM[p] += 50;//�����l�͓K����
		else out_dt.PWM[p] = max;
	}
	if (ps2.POV_bit & d) {
		dt = out_dt.PWM[p] - 50;
		if (dt > PWM_MIN && dt > min) out_dt.PWM[p] -= 50;
		else out_dt.PWM[p] = min;
	}

}

void PWM_hold3(int u, int d, int p) {
	int dt;
	p--;


	if (ps2.BTN_bit&u) {
		dt = out_dt.PWM[p] + 50;
		if (dt < PWM_MAX) out_dt.PWM[p] += 50;//�����l�͓K����
		else out_dt.PWM[p] = PWM_MAX;
	}
	if (ps2.BTN_bit & d) {
		dt = out_dt.PWM[p] - 50;
		if (dt > PWM_MIN) out_dt.PWM[p] -= 50;
		else out_dt.PWM[p] = PWM_MIN;
	}
	if ((ps2.BTN_bit&(u + d)) == 0) {
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

void PWM_move(int right, int left) {//�Ƃ肠�����\���L�[�őO�A��A���M�n
	int ptn = 0x01, dt;

	if (ps2.POV_bit & ptn) {//�O
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//�E���M�n
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//��
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//�����M�n
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}

	//�i�i�����͂̏���
	if (ps2.POV[0] == 4500 || ps2.POV[0] == 22500) {
		out_dt.PWM[right] = PWM_AVE;
	}
	if (ps2.POV[0] == 13500 || ps2.POV[0] == 31500) {
		out_dt.PWM[left] = PWM_AVE;
	}
	//����͎����ςɖ߂�
	if (ps2.POV[0] == -1) {
		out_dt.PWM[right] = PWM_AVE;
		out_dt.PWM[left] = PWM_AVE;
	}

}

void PWM_moveEx(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 220;
	int tread = 290;

	int lx, ly;
	int a = base / 2;
	int b = tread / 2;
	int anv = 0;//�p���x

	int erly = 4.5;				//����PWM�R���g���[���{���ύX����

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//�������l���̏���
	if (abs(lx)<200) {
		lx = 0;
	}
	else {
		if (lx>0)
			lx -= 100;
		else
			lx += 100;
	}
	if (abs(ly)<200) {
		ly = 0;
	}
	else {

		if (ly>0)
			ly -= 100;
		else
			ly += 100;
	}
	//�O�㍶�E�̗�O����(���S�ɏ㉺���E�ɓ��͎�)
	//��
	/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv):(-lx+ly-(a-b)*anv);
	out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv):(lx+ly-(a-b)*anv);
	out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv):(-lx+ly+(a-b)*anv);
	out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv):(lx+ly+(a-b)*anv);
	*/
	/*
	out_dt.PWM[fr] = (-lx + ly - (a - b)*anv) / 2;
	out_dt.PWM[rr] = (lx + ly - (a - b)*anv) / 2;
	out_dt.PWM[fl] = (-lx + ly + (a - b)*anv) / 2;
	out_dt.PWM[rl] = (lx + ly + (a - b)*anv) / 2;
	if (ps2.BTN_bit & BTN_R1){//�E���M�n
	out_dt.PWM[fr] = PWM_MIN / 4;
	out_dt.PWM[rr] = PWM_MIN / 4;
	out_dt.PWM[fl] = PWM_MAX / 4;
	out_dt.PWM[rl] = PWM_MAX / 4;
	}

	if (ps2.BTN_bit & BTN_L1){//�����M�n
	out_dt.PWM[fr] = PWM_MAX / 4;
	out_dt.PWM[rr] = PWM_MAX / 4;
	out_dt.PWM[fl] = PWM_MIN / 4;
	out_dt.PWM[rl] = PWM_MIN / 4;
	}
	*/

	/*					Mode1	�ʏ푖�s										*********************************************::*/
	out_dt.PWM[fr] = ((-lx + ly - (a - b)*anv) / erly)*1.2;		//* PWM_MAX*//   //�E�O
	out_dt.PWM[rr] = ((lx + ly - (a - b)*anv) / erly)*1.2;  //�E��
	out_dt.PWM[fl] = ((-lx + ly + (a - b)*anv) / erly)*1.2;   //����	
	out_dt.PWM[rl] = ((lx + ly + (a - b)*anv) / erly)*1.2;  //���O
															/*						�ʏ푖�s										************************************************/

	if (ps2.BTN_bit & BTN_R1) {//�E���M�n
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_L1) {//�����M�n
		out_dt.PWM[fr] = PWM_MAX / 5.8;
		out_dt.PWM[rr] = PWM_MAX / 5.8;
		out_dt.PWM[fl] = PWM_MIN / 5.8;
		out_dt.PWM[rl] = PWM_MIN / 5.8;
	}
}

void PWM_moveExslow(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 220;
	int tread = 290;

	int lx, ly;
	int a = base / 2;
	int b = tread / 2;
	int anv = 0;//�p���x

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//�������l���̏���
	if (abs(lx)<200) {
		lx = 0;
	}
	else {
		if (lx>0)
			lx -= 100;
		else
			lx += 100;
	}
	if (abs(ly)<200) {
		ly = 0;
	}
	else {

		if (ly>0)
			ly -= 100;
		else
			ly += 100;
	}

	out_dt.PWM[fr] = ((-lx + ly - (a - b)*anv) / 6)*1.2;//*1.7; //����
	out_dt.PWM[rr] = ((lx + ly - (a - b)*anv) / 6)*1.2;//* 1.5; //���O
	out_dt.PWM[fl] = ((-lx + ly + (a - b)*anv) / 6)*1.2; //�E�O	0.98
	out_dt.PWM[rl] = ((lx + ly + (a - b)*anv) / 6)*1.2; //�E���

	if (ps2.BTN_bit & BTN_R1) {//�E���M�n
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_L1) {//�����M�n
		out_dt.PWM[fr] = PWM_MAX / 6;
		out_dt.PWM[rr] = PWM_MAX / 6;
		out_dt.PWM[fl] = PWM_MIN / 6;
		out_dt.PWM[rl] = PWM_MIN / 6;
	}
}


void PWM_moveEx2(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 220;
	int tread = 290;
	int max = 250, min = -250;

	static int lx = 0, ly = 0;
	int a = base / 2;
	int b = tread / 2;
	int anv = 0;//�p���x

	if (ps2.POV[0] == -1) { //�����̃{�^����������Ă��Ȃ����AmoveEx�ɂ킽��
		PWM_moveEx(fr + 1, rr + 1, fl + 1, rl + 1);
		lx = 0; ly = 0;
	}
	else {
		if (ps2.POV_bit&BTN_LU) {//����͎�
			if ((ly + 50)>max) {
				ly = max; lx = 0;
			}
			else {
				ly += 50; lx = 0;
			}
		}
		if (ps2.POV_bit&BTN_LR) {//right
			if ((lx + 50)>max) {
				ly = 0; lx = max;
			}
			else {
				ly = 0; lx += 50;
			}
		}
		if (ps2.POV_bit&BTN_LD) {//down
			if ((ly - 50)<min) {
				ly = min; lx = 0;
			}
			else {
				ly -= 50; lx = 0;
			}
		}
		if (ps2.POV_bit&BTN_LL) {//left
			if ((lx - 50)<min) {
				ly = 0; lx = min;
			}
			else {
				ly = 0; lx -= 50;
			}
		}

		//�������l���̏���
		//�O�㍶�E�̗�O����(���S�ɏ㉺���E�ɓ��͎�)
		//ly=0;lx=0;

		//��
		/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv)*2:(-lx+ly-(a-b)*anv)/2;
		out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv)*2:(lx+ly-(a-b)*anv)/2;
		out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv)*2:(-lx+ly+(a-b)*anv)/2;
		out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv)*2:(lx+ly+(a-b)*anv)/2;
		*/
		out_dt.PWM[fr] = (-lx + ly - (a - b)*anv);
		out_dt.PWM[rr] = (lx + ly - (a - b)*anv);
		out_dt.PWM[fl] = (-lx + ly + (a - b)*anv);
		out_dt.PWM[rl] = (lx + ly + (a - b)*anv);

	}
	if (ps2.BTN_bit & BTN_L1) {//�E���M�n
		out_dt.PWM[fr] = PWM_MAX / 2;
		out_dt.PWM[rr] = PWM_MAX / 2;
		out_dt.PWM[fl] = PWM_MIN / 2;
		out_dt.PWM[rl] = PWM_MIN / 2;

	}

	if (ps2.BTN_bit & BTN_R1) {//�����M�n
		out_dt.PWM[fr] = PWM_MIN / 2;
		out_dt.PWM[rr] = PWM_MIN / 2;
		out_dt.PWM[fl] = PWM_MAX / 2;
		out_dt.PWM[rl] = PWM_MAX / 2;
	}

}

void PWM_moveExre(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 220;
	int tread = 290;

	int lx, ly, re;
	int a = base / 2;
	int b = tread / 2;
	int anv = 0;//�p���x

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//�������l���̏���
	if (abs(lx)<200) {
		lx = 0;
	}
	else {
		if (lx>0)
			lx += 100;
		else
			lx -= 100;
	}
	if (abs(ly)<200) {
		ly = 0;
	}
	else {

		if (ly>0)
			ly += 100;
		else
			ly -= 100;
	}
	//�O�㍶�E�̗�O����(���S�ɏ㉺���E�ɓ��͎�)

	//��
	/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv):(-lx+ly-(a-b)*anv);
	out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv):(lx+ly-(a-b)*anv);
	out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv):(-lx+ly+(a-b)*anv);
	out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv):(lx+ly+(a-b)*anv);
	*/
	out_dt.PWM[fr] = -(-lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[rr] = -(lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[fl] = -(-lx + ly + (a - b)*anv) / 4;
	out_dt.PWM[rl] = -(lx + ly + (a - b)*anv) / 4;

	if (ps2.BTN_bit & BTN_L1) {//�E���M�n
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_R1) {//�����M�n
		out_dt.PWM[fr] = PWM_MAX / 6;
		out_dt.PWM[rr] = PWM_MAX / 6;
		out_dt.PWM[fl] = PWM_MIN / 6;
		out_dt.PWM[rl] = PWM_MIN / 6;
	}
}

void PWM_moveEx2re(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 220;
	int tread = 290;
	int max = 1000, min = -1000;

	static int lx = 0, ly = 0;
	int a = base / 2;
	int b = tread / 2;
	int anv = 0;//�p���x

	if (ps2.POV[0] == -1) { //�����̃{�^����������Ă��Ȃ����AmoveEx�ɂ킽��
		PWM_moveExre(fr + 1, rr + 1, fl + 1, rl + 1);
		lx = 0; ly = 0;
	}
	else {
		if (ps2.POV_bit&BTN_LU) {//����͎�
			if ((ly + 50)>max) {
				ly = max; lx = 0;
			}
			else {
				ly += 50; lx = 0;
			}
		}
		if (ps2.POV_bit&BTN_LR) {//right
			if ((lx + 50)>max) {
				ly = 0; lx = max;
			}
			else {
				ly = 0; lx += 50;
			}
		}
		if (ps2.POV_bit&BTN_LD) {//down
			if ((ly - 50)<min) {
				ly = min; lx = 0;
			}
			else {
				ly -= 50; lx = 0;
			}
		}
		if (ps2.POV_bit&BTN_LL) {//left
			if ((lx - 50)<min) {
				ly = 0; lx = min;
			}
			else {
				ly = 0; lx -= 50;
			}
		}

		//�������l���̏���
		//�O�㍶�E�̗�O����(���S�ɏ㉺���E�ɓ��͎�)
		//ly=0;lx=0;

		//��
		/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv)*2:(-lx+ly-(a-b)*anv)/2;
		out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv)*2:(lx+ly-(a-b)*anv)/2;
		out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv)*2:(-lx+ly+(a-b)*anv)/2;
		out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv)*2:(lx+ly+(a-b)*anv)/2;
		*/
		out_dt.PWM[fr] = -(-lx + ly - (a - b)*anv);
		out_dt.PWM[rr] = -(lx + ly - (a - b)*anv);
		out_dt.PWM[fl] = -(-lx + ly + (a - b)*anv);
		out_dt.PWM[rl] = -(lx + ly + (a - b)*anv);

	}
	if (ps2.BTN_bit & BTN_R2) {//�E���M�n
		out_dt.PWM[fr] = PWM_MAX / 2;
		out_dt.PWM[rr] = PWM_MAX / 2;
		out_dt.PWM[fl] = PWM_MIN / 2;
		out_dt.PWM[rl] = PWM_MIN / 2;

	}

	if (ps2.BTN_bit & BTN_L2) {//�����M�n
		out_dt.PWM[fr] = PWM_MIN / 2;
		out_dt.PWM[rr] = PWM_MIN / 2;
		out_dt.PWM[fl] = PWM_MAX / 2;
		out_dt.PWM[rl] = PWM_MAX / 2;
	}

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
		camNum = (camNum + 1) % 4; //%�őS�J�����䐔���w��
		if (camNum == 1)camNum += 1;
		TPJT_chg_camera(camNum);
		flag = false;
	}

}


void PWM_move2(int right, int left) {//�Ƃ肠�����\���L�[�őO�A��A���M�n
	int ptn = 0x01, dt;

	if (ps2.POV_bit & ptn) {//�O
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//�E���M�n
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//��
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//�����M�n
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}


	//����͎����ςɖ߂�
	if (ps2.POV[0] == -1) {
		out_dt.PWM[right] = PWM_AVE;
		out_dt.PWM[left] = PWM_AVE;
	}

}
void PWM_move2b(int right, int left) {
	int dt;

	if (ps2.BTN_bit & BTN_RU) {//�O
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}

	if (ps2.BTN_bit & BTN_RR) {//�E���M�n
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}

	if (ps2.BTN_bit & BTN_RD) {//��
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}

	if (ps2.BTN_bit & BTN_RL) {//�����M�n
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}


	//����͎����ςɖ߂�
	if (ps2.POV[0] == -1) {
		out_dt.PWM[right] = PWM_AVE;
		out_dt.PWM[left] = PWM_AVE;
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

void PWM_act_axis(int f, int p) {
	int axis;

	axis = (ps2.axis[f] * 1100 / AXIS_MAX);
	//�������l���̏���
	if (abs(axis)<200) {
		axis = 0;
	}
	else {
		if (axis>0)axis -= 100;
		else axis += 100;
	}
	out_dt.PWM[p] = axis;

}
