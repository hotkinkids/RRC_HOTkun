#include <windows.h>
#include <stdlib.h>
#include "TPIP.h"
#include "TPGC_11dll.h"	// コントローラ入力関数のinclude file
#include "TPJT3.h"	// リアルタイム動画伝送関数の include file

extern struct GC_STRUCT ps2;	// ゲームコントローラからの入力値　格納変数
extern struct OUT_DT_STR out_dt;// 制御出力データ
extern struct INP_DT_STR inp_dt;// センサー入力データ

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

//Mode1とMode3
void PWM_moveEx(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 233;
	int tread = 405;

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
	//前後左右の例外処理(完全に上下左右に入力時)

	//式
	/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv):(-lx+ly-(a-b)*anv);
	out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv):(lx+ly-(a-b)*anv);
	out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv):(-lx+ly+(a-b)*anv);
	out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv):(lx+ly+(a-b)*anv);
	*/
	out_dt.PWM[fr] = (-lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[rr] = (lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[fl] = (-lx + ly + (a - b)*anv) / 4;
	out_dt.PWM[rl] = (lx + ly + (a - b)*anv) / 4;

	if (ps2.BTN_bit & BTN_R2) {//右超信地
		out_dt.PWM[fr] = PWM_MIN / 4;
		out_dt.PWM[rr] = PWM_MIN / 4;
		out_dt.PWM[fl] = PWM_MAX / 4;
		out_dt.PWM[rl] = PWM_MAX / 4;
	}

	if (ps2.BTN_bit & BTN_L2) {//左超信地
		out_dt.PWM[fr] = PWM_MAX / 4;
		out_dt.PWM[rr] = PWM_MAX / 4;
		out_dt.PWM[fl] = PWM_MIN / 4;
		out_dt.PWM[rl] = PWM_MIN / 4;
	}
}

void PWM_moveEx2(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 315;
	int tread = 440;
	int max = 1000, min = -1000;

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
		out_dt.PWM[fr] = (-lx + ly - (a - b)*anv) / 2;
		out_dt.PWM[rr] = (lx + ly - (a - b)*anv) / 2;
		out_dt.PWM[fl] = (-lx + ly + (a - b)*anv) / 2;
		out_dt.PWM[rl] = (lx + ly + (a - b)*anv) / 2;

	}
	if (ps2.BTN_bit & BTN_L2) {//右超信地
		out_dt.PWM[fr] = PWM_MAX / 3;
		out_dt.PWM[rr] = PWM_MAX / 3;
		out_dt.PWM[fl] = PWM_MIN / 3;
		out_dt.PWM[rl] = PWM_MIN / 3;

	}

	if (ps2.BTN_bit & BTN_R2) {//左超信地
		out_dt.PWM[fr] = PWM_MIN / 3;
		out_dt.PWM[rr] = PWM_MIN / 3;
		out_dt.PWM[fl] = PWM_MAX / 3;
		out_dt.PWM[rl] = PWM_MAX / 3;
	}

}

void PWM_moveSL(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 347;
	int tread = 555;

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
	//前後左右の例外処理(完全に上下左右に入力時)

	//式
	/*	out_dt.PWM[fr]=abs(-lx+ly-(a-b)*anv)==500?(-lx+ly-(a-b)*anv):(-lx+ly-(a-b)*anv);
	out_dt.PWM[rr]=abs(lx+ly-(a-b)*anv)==500?(lx+ly-(a-b)*anv):(lx+ly-(a-b)*anv);
	out_dt.PWM[fl]=abs(-lx+ly+(a-b)*anv)==500?(-lx+ly+(a-b)*anv):(-lx+ly+(a-b)*anv);
	out_dt.PWM[rl]=abs(lx+ly+(a-b)*anv)==500?(lx+ly+(a-b)*anv):(lx+ly+(a-b)*anv);
	*/
	out_dt.PWM[fr] = (-lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[rr] = (lx + ly - (a - b)*anv) / 4;
	out_dt.PWM[fl] = (-lx + ly + (a - b)*anv) / 4;
	out_dt.PWM[rl] = (lx + ly + (a - b)*anv) / 4;

	if (ps2.BTN_bit & BTN_L2) {//右超信地
		out_dt.PWM[fr] = PWM_MAX / 9;
		out_dt.PWM[rr] = PWM_MAX / 9;
		out_dt.PWM[fl] = PWM_MIN / 9;
		out_dt.PWM[rl] = PWM_MIN / 9;

	}

	if (ps2.BTN_bit & BTN_R2) {//左超信地
		out_dt.PWM[fr] = PWM_MIN / 9;
		out_dt.PWM[rr] = PWM_MIN / 9;
		out_dt.PWM[fl] = PWM_MAX / 9;
		out_dt.PWM[rl] = PWM_MAX / 9;
	}
}

void PWM_moveExre(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 260;
	int tread = 336.5;

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

	if (ps2.BTN_bit & BTN_L2) {//右超信地
		out_dt.PWM[fr] = PWM_MIN / 4;
		out_dt.PWM[rr] = PWM_MIN / 4;
		out_dt.PWM[fl] = PWM_MAX / 4;
		out_dt.PWM[rl] = PWM_MAX / 4;
	}

	if (ps2.BTN_bit & BTN_R2) {//左超信地
		out_dt.PWM[fr] = PWM_MAX / 4;
		out_dt.PWM[rr] = PWM_MAX / 4;
		out_dt.PWM[fl] = PWM_MIN / 4;
		out_dt.PWM[rl] = PWM_MIN / 4;
	}
}

void PWM_moveEx2re(int fr, int rr, int fl, int rl) {
	fr--; rr--; fl--; rl--;
	int base = 347;
	int tread = 555;
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
		out_dt.PWM[fr] = -(-lx + ly - (a - b)*anv) / 6;
		out_dt.PWM[rr] = -(lx + ly - (a - b)*anv) / 6;
		out_dt.PWM[fl] = -(-lx + ly + (a - b)*anv) / 6;
		out_dt.PWM[rl] = -(lx + ly + (a - b)*anv) / 6;

	}
	if (ps2.BTN_bit & BTN_L2) {//右超信地
		out_dt.PWM[fr] = PWM_MAX / 5;
		out_dt.PWM[rr] = PWM_MAX / 5;
		out_dt.PWM[fl] = PWM_MIN / 5;
		out_dt.PWM[rl] = PWM_MIN / 5;

	}

	if (ps2.BTN_bit & BTN_R2) {//左超信地
		out_dt.PWM[fr] = PWM_MIN / 5;
		out_dt.PWM[rr] = PWM_MIN / 5;
		out_dt.PWM[fl] = PWM_MAX / 5;
		out_dt.PWM[rl] = PWM_MAX / 5;
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