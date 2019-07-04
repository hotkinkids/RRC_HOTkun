
#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// �R���g���[�����͊֐���include file
#include "TPJT3.h"	// ���A���^�C������`���֐��� include file

extern struct GC_STRUCT ps2;	// �Q�[���R���g���[������̓��͒l�@�i�[�ϐ�
extern struct OUT_DT_STR out_dt;// ����o�̓f�[�^
extern struct INP_DT_STR inp_dt;// �Z���T�[���̓f�[�^

void PWM_act(int u, int d, int p);				//������(�����{�^��,�����{�^��,�o��PWM�̔ԍ�(1�`10))	�Z�~�����p
void PWM_act2(int u, int d, int p);				//�\���L�[�p

void PWM_act(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p
void PWM_act2(int u, int d, int p, int max, int min);//PWM�ő�l�A�ŏ��l���w�肷��p

void PWM_act(int u, int d, int p) {
	int dt;  //����̗�
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
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX / 2;//�����l�͓K����
		else out_dt.PWM[p] = PWM_MAX / 2;
	}
	if (ps2.POV_bit&d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN / 2;
		else out_dt.PWM[p] = PWM_MIN / 2;
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