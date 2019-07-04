
#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// �R���g���[�����͊֐���include file
#include "TPJT3.h"	// ���A���^�C������`���֐��� include file

extern struct GC_STRUCT ps2;	// �Q�[���R���g���[������̓��͒l�@�i�[�ϐ�
extern struct OUT_DT_STR out_dt;// ����o�̓f�[�^
extern struct INP_DT_STR inp_dt;// �Z���T�[���̓f�[�^


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
		if (dt < PWM_MAX || dt < max) out_dt.PWM[p] += 50;//�����l�͓K����
		else out_dt.PWM[p] = max;
	}
	if (ps2.POV_bit & d) {
		dt = out_dt.PWM[p] - 50;
		if (dt > PWM_MIN || dt > min) out_dt.PWM[p] -= 50;
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