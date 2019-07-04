
#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// コントローラ入力関数のinclude file
#include "TPJT3.h"	// リアルタイム動画伝送関数の include file

extern struct GC_STRUCT ps2;	// ゲームコントローラからの入力値　格納変数
extern struct OUT_DT_STR out_dt;// 制御出力データ
extern struct INP_DT_STR inp_dt;// センサー入力データ

void PWM_act(int u, int d, int p);				//引数は(増加ボタン,減少ボタン,出力PWMの番号(1～10))	〇×△□用
void PWM_act2(int u, int d, int p);				//十字キー用

void PWM_act(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用
void PWM_act2(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用

void PWM_act(int u, int d, int p) {
	int dt;  //童貞の略
	p--;
	if (ps2.BTN_bit&u) {
		dt = out_dt.PWM[p] + 10;
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX;//増加値は適当に
		else out_dt.PWM[p] = PWM_MAX;
	}
	if (ps2.BTN_bit & d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN;
		else out_dt.PWM[p] = PWM_MIN;
	}
	if ((ps2.BTN_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
		out_dt.PWM[p] = PWM_AVE;
	}
}

void PWM_act2(int u, int d, int p) {
	int dt;
	p--;
	if (ps2.POV_bit&u) {
		dt = out_dt.PWM[p] + 10;
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX / 2;//増加値は適当に
		else out_dt.PWM[p] = PWM_MAX / 2;
	}
	if (ps2.POV_bit&d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN / 2;
		else out_dt.PWM[p] = PWM_MIN / 2;
	}
	if ((ps2.POV_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
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
	if ((ps2.BTN_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
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
		if (dt < PWM_MAX&& dt < max) out_dt.PWM[p] += 10;//増加値は適当に
		else out_dt.PWM[p] = max;
	}
	if (ps2.POV_bit & d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN&&dt > min) out_dt.PWM[p] -= 10;
		else out_dt.PWM[p] = min;
	}
	if ((ps2.POV_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
		out_dt.PWM[p] = PWM_AVE;
	}
}