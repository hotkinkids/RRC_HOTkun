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
void PWM_act(int u, int d, int p);				//引数は(増加ボタン,減少ボタン,出力PWMの番号(1～10))
void PWM_act2(int u, int d, int p);				//十字キー用

void PWM_act(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用
void PWM_act2(int u, int d, int p, int max, int min);//PWM最大値、最小値を指定する用

													 //サーボ
void PWM_hold(int u, int d, int p, int max, int min);
void PWM_hold2(int u, int d, int p, int max, int min);
void PWM_hold3(int u, int d, int p);

void PWM_dm(int right, int left);				//以前まで使っていた左右スティックでの操作(DMDなし)
void PWM_dmd(int right, int left);				//dmd用基本関数
void PWM_dmd2(int first, int second);			//十字キーでのdmd制御関数
void PWM_move(int right, int left);				//操作は十字キー、引数はPWM出力

void PWM_move2(int right, int left);				//操作は十字キー、左右超信地のみ
void PWM_move2b(int right, int left);			//操作はボタン、左右超信地のみ

												//MT出力
void PWM_moveEx(int fr, int rr, int fl, int rl);	//メカナムの移動制御関数*超信地はR1R2,希望があれば引数増やした関数作る
void PWM_moveEx2(int fr, int rr, int fl, int rl);	//十字キーでの前後左右とmoveExが内部に入ってる関数(急行用)速い超信地はR2L2

													//void PWM_moveExslow(int fr, int rr, int fl, int rl);

void PWM_moveExre(int fr, int rr, int fl, int rl);
void PWM_moveEx2re(int fr, int rr, int fl, int rl);

void PWM_act_axis(int f, int p);

void Mode_change(int b);						//引数はボタン,一旦押して離した時に実行します
void cam_change(int b);

//制御
void ctrl_PWM(void)
{
	//制御
	//switch前にはすべてのモードで共通する操作をおく

	Mode_change(BTN_ST);//操作モード切り替え(startボタン)
	cam_change(BTN_SE);//カメラ切り替え

	switch (Ctrlmode) {//case≒mode、後は各々好きに設定。case増やしても減らしてもいいよ(数の制限はない)

	case 0:

		//*//1

		PWM_moveEx(5, 6, 7, 8);			//メカナム急行用
		PWM_act2(BTN_LU, BTN_LD, 11);	//救助機構上下
		PWM_act2(BTN_LR, BTN_LL, 13);   //回転機構
		PWM_act(BTN_R2, BTN_L2, 9);		//ベルト
		PWM_act(BTN_RU, BTN_RD, 12);     //ベルト左
		PWM_act(BTN_RR, BTN_RL, 10);		//前後

		break;

	case 1://反転急行

		PWM_moveEx2re(5, 6, 7, 8);	//反転

		PWM_hold(BTN_RU, BTN_RD, 3, 600, 0);	//スカート
		break;
	case 2:


		PWM_moveEx2(5, 6, 7, 8);//微調整用

		PWM_act(BTN_RU, BTN_RL, 13);				//アーム肩
		PWM_hold(BTN_RR, BTN_RD, 1, 1000, -1000);	//アーム手首
		PWM_hold(BTN_L3, BTN_R3, 2, 1000, -1000);	//アームキャッチ

		PWM_act_axis(1, 13);//カメラ上下
		PWM_dmd(13, 20);

		break;
		//*/
	default://ここには操作を置かないこと
		Ctrlmode = 0;
		break;
	}

}


/***************************

動作定義

***************************/

void PWM_act(int u, int d, int p) {
	int dt;
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
		if (dt < PWM_MAX) out_dt.PWM[p] = PWM_MAX;//増加値は適当に
		else out_dt.PWM[p] = PWM_MAX;
	}
	if (ps2.POV_bit&d) {
		dt = out_dt.PWM[p] - 10;
		if (dt > PWM_MIN) out_dt.PWM[p] = PWM_MIN;
		else out_dt.PWM[p] = PWM_MIN;
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

void PWM_hold(int u, int d, int p, int max, int min) {
	int dt;
	p--;


	if (ps2.BTN_bit&u) {
		dt = out_dt.PWM[p] + 0;
		if (dt < 0 || dt < max) {
			out_dt.PWM[p] += 30;//増加値は適当に
		}
		else {
			out_dt.PWM[p] = max;
		}
	}
	if (ps2.BTN_bit & d) {
		dt = out_dt.PWM[p] - 0;
		if (dt > 0 || dt > min) {    //ちょっと遅い？調整する
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
		if (dt < PWM_MAX && dt < max) out_dt.PWM[p] += 50;//増加値は適当に
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
		if (dt < PWM_MAX) out_dt.PWM[p] += 50;//増加値は適当に
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

void PWM_move(int right, int left) {//とりあえず十字キーで前、後、超信地
	int ptn = 0x01, dt;

	if (ps2.POV_bit & ptn) {//前
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//右超信地
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//後
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//左超信地
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}

	//ナナメ入力の処理
	if (ps2.POV[0] == 4500 || ps2.POV[0] == 22500) {
		out_dt.PWM[right] = PWM_AVE;
	}
	if (ps2.POV[0] == 13500 || ps2.POV[0] == 31500) {
		out_dt.PWM[left] = PWM_AVE;
	}
	//非入力時平均に戻す
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
	int anv = 0;//角速度

	int erly = 4.5;				//下のPWMコントロール倍率変更分母

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//しきい値他の処理
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
	//前後左右の例外処理(完全に上下左右に入力時)
	//式
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
	if (ps2.BTN_bit & BTN_R1){//右超信地
	out_dt.PWM[fr] = PWM_MIN / 4;
	out_dt.PWM[rr] = PWM_MIN / 4;
	out_dt.PWM[fl] = PWM_MAX / 4;
	out_dt.PWM[rl] = PWM_MAX / 4;
	}

	if (ps2.BTN_bit & BTN_L1){//左超信地
	out_dt.PWM[fr] = PWM_MAX / 4;
	out_dt.PWM[rr] = PWM_MAX / 4;
	out_dt.PWM[fl] = PWM_MIN / 4;
	out_dt.PWM[rl] = PWM_MIN / 4;
	}
	*/

	/*					Mode1	通常走行										*********************************************::*/
	out_dt.PWM[fr] = ((-lx + ly - (a - b)*anv) / erly)*1.2;		//* PWM_MAX*//   //右前
	out_dt.PWM[rr] = ((lx + ly - (a - b)*anv) / erly)*1.2;  //右後
	out_dt.PWM[fl] = ((-lx + ly + (a - b)*anv) / erly)*1.2;   //左後	
	out_dt.PWM[rl] = ((lx + ly + (a - b)*anv) / erly)*1.2;  //左前
															/*						通常走行										************************************************/

	if (ps2.BTN_bit & BTN_R1) {//右超信地
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_L1) {//左超信地
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
	int anv = 0;//角速度

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//しきい値他の処理
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

	out_dt.PWM[fr] = ((-lx + ly - (a - b)*anv) / 6)*1.2;//*1.7; //左後
	out_dt.PWM[rr] = ((lx + ly - (a - b)*anv) / 6)*1.2;//* 1.5; //左前
	out_dt.PWM[fl] = ((-lx + ly + (a - b)*anv) / 6)*1.2; //右前	0.98
	out_dt.PWM[rl] = ((lx + ly + (a - b)*anv) / 6)*1.2; //右後ろ

	if (ps2.BTN_bit & BTN_R1) {//右超信地
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_L1) {//左超信地
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
	int anv = 0;//角速度

	if (ps2.POV[0] == -1) { //両方のボタンが押されていない時、moveExにわたす
		PWM_moveEx(fr + 1, rr + 1, fl + 1, rl + 1);
		lx = 0; ly = 0;
	}
	else {
		if (ps2.POV_bit&BTN_LU) {//上入力時
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

		//しきい値他の処理
		//前後左右の例外処理(完全に上下左右に入力時)
		//ly=0;lx=0;

		//式
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
	if (ps2.BTN_bit & BTN_L1) {//右超信地
		out_dt.PWM[fr] = PWM_MAX / 2;
		out_dt.PWM[rr] = PWM_MAX / 2;
		out_dt.PWM[fl] = PWM_MIN / 2;
		out_dt.PWM[rl] = PWM_MIN / 2;

	}

	if (ps2.BTN_bit & BTN_R1) {//左超信地
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
	int anv = 0;//角速度

	lx = ps2.axis[0] * 600 / AXIS_MAX;
	ly = -(ps2.axis[1] * 600 / AXIS_MAX);



	//しきい値他の処理
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
	//前後左右の例外処理(完全に上下左右に入力時)

	//式
	/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv):(-lx+ly-(a-b)*anv);
	out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv):(lx+ly-(a-b)*anv);
	out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv):(-lx+ly+(a-b)*anv);
	out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv):(lx+ly+(a-b)*anv);
	*/
	out_dt.PWM[fr] = -(-lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[rr] = -(lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[fl] = -(-lx + ly + (a - b)*anv) / 4;
	out_dt.PWM[rl] = -(lx + ly + (a - b)*anv) / 4;

	if (ps2.BTN_bit & BTN_L1) {//右超信地
		out_dt.PWM[fr] = PWM_MIN / 6;
		out_dt.PWM[rr] = PWM_MIN / 6;
		out_dt.PWM[fl] = PWM_MAX / 6;
		out_dt.PWM[rl] = PWM_MAX / 6;
	}

	if (ps2.BTN_bit & BTN_R1) {//左超信地
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
	int anv = 0;//角速度

	if (ps2.POV[0] == -1) { //両方のボタンが押されていない時、moveExにわたす
		PWM_moveExre(fr + 1, rr + 1, fl + 1, rl + 1);
		lx = 0; ly = 0;
	}
	else {
		if (ps2.POV_bit&BTN_LU) {//上入力時
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

		//しきい値他の処理
		//前後左右の例外処理(完全に上下左右に入力時)
		//ly=0;lx=0;

		//式
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
	if (ps2.BTN_bit & BTN_R2) {//右超信地
		out_dt.PWM[fr] = PWM_MAX / 2;
		out_dt.PWM[rr] = PWM_MAX / 2;
		out_dt.PWM[fl] = PWM_MIN / 2;
		out_dt.PWM[rl] = PWM_MIN / 2;

	}

	if (ps2.BTN_bit & BTN_L2) {//左超信地
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
		camNum = (camNum + 1) % 4; //%で全カメラ台数を指定
		if (camNum == 1)camNum += 1;
		TPJT_chg_camera(camNum);
		flag = false;
	}

}


void PWM_move2(int right, int left) {//とりあえず十字キーで前、後、超信地
	int ptn = 0x01, dt;

	if (ps2.POV_bit & ptn) {//前
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//右超信地
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//後
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}
	ptn = ptn << 1;

	if (ps2.POV_bit & ptn) {//左超信地
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}


	//非入力時平均に戻す
	if (ps2.POV[0] == -1) {
		out_dt.PWM[right] = PWM_AVE;
		out_dt.PWM[left] = PWM_AVE;
	}

}
void PWM_move2b(int right, int left) {
	int dt;

	if (ps2.BTN_bit & BTN_RU) {//前
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}

	if (ps2.BTN_bit & BTN_RR) {//右超信地
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] + 10;
		if (dt < PWM_MAX) out_dt.PWM[left] += 10;
		else out_dt.PWM[left] = PWM_MAX;
	}

	if (ps2.BTN_bit & BTN_RD) {//後
		dt = out_dt.PWM[right] - 10;
		if (dt > PWM_MIN) out_dt.PWM[right] -= 10;
		else out_dt.PWM[right] = PWM_MIN;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}

	if (ps2.BTN_bit & BTN_RL) {//左超信地
		dt = out_dt.PWM[right] + 10;
		if (dt < PWM_MAX) out_dt.PWM[right] += 10;
		else out_dt.PWM[right] = PWM_MAX;
		dt = out_dt.PWM[left] - 10;
		if (dt > PWM_MIN) out_dt.PWM[left] -= 10;
		else out_dt.PWM[left] = PWM_MIN;
	}


	//非入力時平均に戻す
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

void PWM_act_axis(int f, int p) {
	int axis;

	axis = (ps2.axis[f] * 1100 / AXIS_MAX);
	//しきい値他の処理
	if (abs(axis)<200) {
		axis = 0;
	}
	else {
		if (axis>0)axis -= 100;
		else axis += 100;
	}
	out_dt.PWM[p] = axis;

}
