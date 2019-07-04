/**
* @file ctrl_PWM.cpp
* @brief 制御ボード通信　データ処理
*
* @author Katayama
* @date 2008-03-21
*
* Copyright (C) 2008 TPIP User Community All rights reserved.
* このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
* 使用許諾をします。
*/
#pragma  warning(disable:4244)
#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// コントローラ入力関数のinclude file
#include "TPJT3.h"	// リアルタイム動画伝送関数の include file
#include "ctrl_PWM_act.h"
#include "ctrl_PWM_hold.h"

//#include ""

extern struct GC_STRUCT ps2;	// ゲームコントローラからの入力値　格納変数
extern struct OUT_DT_STR out_dt;// 制御出力データ
extern struct INP_DT_STR inp_dt;// センサー入力データ


extern int camNum;		//モード用フラグのextern
extern int Ctrlmode;
extern int RNo;


/**********************************************************
/
/	tpip配列　1~20	(0~19ではない)
/	関数の使い方は書いている通り
/	十字キーとスティックとボタンで少し違うので
/	別々に関数を用意した
/	まとめられるのでやりたい人はやってもいい
/
//////////////////////////////////////////////////////////*/
//プロトタイプ宣言
void ctrl_PWM(void);

void PWM_act(int u, int d, int p);				//引数は(増加ボタン,減少ボタン,出力PWMの番号(1～10))	〇×△□用
void PWM_act2(int u, int d, int p);				//十字キー用

void PWM_act(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用
void PWM_act2(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用

													 //サーボ
void PWM_hold(int u, int d, int p, int max, int min);
void PWM_hold2(int u, int d, int p, int max, int min);
void PWM_hold3(int u, int d, int p);

void PWM_switch(int u, int d, int p, int out1, int out2);	//押している間だけ、outの値を出力する
void PWM_switch2(int u, int d, int p, int out1, int out2);	//押している間だけ、outの値を出力する

void PWM_dm(int right, int left);				//以前まで使っていた左右スティックでの操作(DMDなし)
void PWM_dmd(int right, int left);				//dmd用基本関数
void PWM_dmd2(int first, int second);			//十字キーでのdmd制御関数

void PWM_move(int right, int left);				//操作は十字キー、引数はPWM出力

void PWM_move2(int right, int left);				//操作は十字キー、左右超信地のみ
void PWM_move2b(int right, int left);			//操作はボタン、左右超信地のみ

												//MT出力
void PWM_moveEx(int fr, int rr, int fl, int rl);	//メカナムの移動制御関数*超信地はR1R2,希望があれば引数増やした関数作る
void PWM_moveEx2(int fr, int rr, int fl, int rl);	//十字キーでの前後左右とmoveExが内部に入ってる関数(急行用)速い超信地はR2L2

void PWM_moveSL(int fr, int rr, int fl, int rl);

void PWM_moveExre(int fr, int rr, int fl, int rl);
void PWM_moveEx2re(int fr, int rr, int fl, int rl);

void Mode_change(int b);						//引数はボタン,一旦押して離した時に実行します
void cam_change(int b);

//制御
void ctrl_PWM(void)
{
	//制御
	//switch前にはすべてのモードで共通する操作をおく

	Mode_change(BTN_ST);//操作モード切り替え
	cam_change(BTN_SE);//カメラ切り替え 



	switch (Ctrlmode) {//case≒mode、後は各々好きに設定。case増やしても減らしてもいいよ(数の制限はない)
	case 0:
		PWM_moveEx(5, 6, 7, 8);
		//PWM_moveSL(5, 6, 8, 7);		//メカナム　右前、右後、左前、左後
		//PWM_act2(BTN_LU, BTN_LD, 11);	//救助機構上下
		PWM_switch2(BTN_LU, BTN_LD, 11, 700, 500);	//救助機構上下
		PWM_act(BTN_RR, BTN_RD, 10);		//R巻き取り 巻き取り、吐き出し
		PWM_act(BTN_RL, BTN_RU, 9);		//L巻き取り　巻き取り、吐き出し
		PWM_act2(BTN_LL, BTN_LR, 12);    //救助機構前後

		break;
	case 1://反転急行

		PWM_moveEx2re(5, 6, 7, 8);	//反転

		PWM_hold(BTN_RU, BTN_RD, 14, 0, -200);	//スカート
		break;
	case 2:
		PWM_moveSL(5, 6, 7, 8);  //微調整用     

		PWM_act2(BTN_LU, BTN_LD, 13);			//アーム肩
		PWM_hold(BTN_RD, BTN_RR, 2, 1000, -1000);	//アーム手首
		PWM_hold(BTN_RU, BTN_RL, 1, 1000, -1000);	//アームキャッチ
													//PWM_act(BTN_LL, BTN_LR, 10);	//救助前後

													//PWM_act_axis(1, 13);//カメラ上下
		                                            //PWM_dmd(13, 20);

		break;
	default://ここには操作を置かないこと
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
	if ((ps2.BTN_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
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
	if ((ps2.POV_bit & (u + d)) == 0) { //両方のボタンが押されていない時、平均値に
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

	//式

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
		camNum = (camNum + 1) % 3; //%で全カメラ台数を指定
		TPJT_chg_camera(camNum);

		flag = false;
	}

}

void PWM_dmd(int first, int second) {
	first--; second--;
	int ry, ly;

	ly = (ps2.axis[5] * 1100 / AXIS_MAX);
	ry = (ps2.axis[1] * 1100 / AXIS_MAX);
	//しきい値他の処理
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
	//しきい値他の処理
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

	if (ps2.POV_bit & BTN_LU) {//前
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MAX;
		else out_dt.PWM[first] = PWM_MAX;
	}
	if (ps2.POV_bit & BTN_LD) {//後
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MIN;
		else out_dt.PWM[first] = PWM_MIN;
	}
	if (ps2.POV_bit & BTN_LR) {//右超信地
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MAX;
		else out_dt.PWM[first] = PWM_MAX;
	}
	if (ps2.POV_bit & BTN_LL) {//左超信地
		if (dt < PWM_MAX) out_dt.PWM[first] = PWM_MIN;
		else out_dt.PWM[first] = PWM_MIN;
	}


}
