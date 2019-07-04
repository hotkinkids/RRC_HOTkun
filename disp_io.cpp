
#pragma  warning(disable:4244)
#include <windows.h>
#include <stdio.h>
#include "TPIP.h"
#include "TPJT3.h"	// リアルタイム動画伝送関数の include file
#include "TPGC_11dll.h"

extern struct GC_STRUCT ps2;	// ゲームコントローラからの入力値　格納変数
extern struct OUT_DT_STR out_dt;
extern struct INP_DT_STR inp_dt;

//モード用フラグのextern
extern int Ctrlmode;

//dsp_game_ctrlよりコピペ&必要なところだけ
static void dsp_bit(HDC hdc, int x, int y, int bit, int btn, int btn2)
{
	char* msg;

	msg = "-";
	if (bit & btn) msg = "O";
	TextOut(hdc, x, y, msg, 3);
	x += 10;
	msg = "/";
	TextOut(hdc, x, y, msg, 3);
	x += 10;
	msg = "-";
	if (bit & btn2) msg = "O";
	TextOut(hdc, x, y, msg, 3);

}

static void pov_bit(HDC hdc, int x, int y)
{
	int ptn;
	char* msg;
	ptn = 0x01;

	switch (ps2.POV[0]) {
	case 0:
		msg = "8";
		break;
	case 4500:
		msg = "9";
		break;
	case 9000:
		msg = "6";
		break;
	case 13500:
		msg = "3";
		break;
	case 18000:
		msg = "2";
		break;
	case 22500:
		msg = "1";
		break;
	case 27000:
		msg = "4";
		break;
	case 31500:
		msg = "7";
		break;
	default:
		msg = "-";
		break;
	}
	TextOut(hdc, x, y, msg, 3);

}

static void dsp_axis(HDC hdc, int x, int y, int LR) {
	HBRUSH hBrush;

	double LX = ps2.axis[0] * 49 / AXIS_MAX;
	double LY = ps2.axis[1] * 49 / AXIS_MAX;
	double RX = ps2.axis[2] * 49 / AXIS_MAX;
	double RY = ps2.axis[5] * 49 / AXIS_MAX;  



	if (LR == 1) {
		hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, y, x + 100, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		hBrush = CreateSolidBrush(RGB(255, 0, 0));;  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, (int)((y + 48) + LY), x + 100, (int)((y + 52) + LY));
		Rectangle(hdc, (int)((x + 48) + LX), y, (int)((x + 52) + LX), y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

	}
	else if (LR == 2) {
		hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, y, x + 100, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		hBrush = CreateSolidBrush(RGB(255, 0, 0));;  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, (int)((y + 48) + RY), x + 100, (int)((y + 52) + RY));
		Rectangle(hdc, (int)((x + 48) + RX), y, (int)((x + 52) + RX), y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
	}
	else if (LR == 3) {
		hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, y, x + 30, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		hBrush = CreateSolidBrush(RGB(255, 0, 0));;  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, (int)((y + 48) + LY), x + 30, (int)((y + 52) + LY));
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
	}
	else if (LR == 4) {
		hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, y, x + 30, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		hBrush = CreateSolidBrush(RGB(255, 0, 0));;  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, (int)((y + 48) + RY), x + 30, (int)((y + 52) + RY));
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
	}
	else if (LR == 6) {
		hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x, y + 30, x + 100, y + 60);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		hBrush = CreateSolidBrush(RGB(255, 0, 0));;  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, (int)((x + 48) + RX), y + 30, (int)((x + 52) + RX), y + 60);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
	}

}

static void dsp_sensbar(HDC hdc, int x, int y, int a) {
	HBRUSH hBrush;

	double LX = 0;

	if (a <= 2) {
		LX = (0.0025 * (inp_dt.AI[a - 1] - 20000)) + 50;
	}
	else {
		LX = (-0.0025 * inp_dt.AI[a - 1]) + 100;
	}

	hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);				// 灰色のストックブラシを取得する
	SelectObject(hdc, hBrush);									// 取得したブラシを使用する
	Rectangle(hdc, x, y, x + 100, y + 20);
	DeleteObject(hBrush);										// 作成した論理ブラシを削除する

	hBrush = CreateSolidBrush(RGB(255, 0, 0));					// 灰色のストックブラシを取得する
	SelectObject(hdc, hBrush);									// 取得したブラシを使用する
	Rectangle(hdc, ((int)(x - 2) + LX), y, ((int)(x + 2) + LX), y + 20);
	DeleteObject(hBrush);										// 作成した論理ブラシを削除する

}

static void dsp_PWM(HDC hdc, int x, int y, int p) {
	char msg[20];

	wsprintf(msg, "[%02d]:%04d", p, out_dt.PWM[p - 1]);
	//if(p<0)	wsprintf(msg,"MT :%04d",out_dt.MT);		//TPIP2用 MT関数(廃止済)

	TextOut(hdc, x, y, msg, lstrlen(msg));
}

static void dsp_SENS(HDC hdc, int x, int y, int a) {
	char msg[20];

	SetTextColor(hdc, RGB(255, 0, 0));

	wsprintf(msg, "□");
	TextOut(hdc, x, y, msg, lstrlen(msg));

	SetTextColor(hdc, RGB(0, 0, 0));

}

void dsp_SENSr(HDC hdc, int x, int y, int a) {
	char msg[20];

	SetTextColor(hdc, RGB(255, 0, 0));

	wsprintf(msg, "■");
	TextOut(hdc, x, y, msg, lstrlen(msg));

	SetTextColor(hdc, RGB(0, 0, 0));

}

static void dsp_SENSn(HDC hdc, int x, int y, int a) {
	char msg[20];

	wsprintf(msg, "%d : %d", a, inp_dt.AI[a - 1]);
	TextOut(hdc, x, y, msg, lstrlen(msg));

}



void disp_sensor(HDC hdc, int x, int y, int RoboNo) {
	char msg[256];
	HBRUSH hBrush;
	int i = x;

	wsprintf(msg, "側方センサ");
	TextOut(hdc, x, y - 45, msg, lstrlen(msg));

	x += 24;
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);            // 取得したブラシを使用する
	Rectangle(hdc, x - 4, y - 4, x + 60, y + 70);
	DeleteObject(hBrush);

	x -= 24;
	//左前側方センサ
	if (inp_dt.AI[0] < 20000) {
		dsp_SENS(hdc, x, y + 5, 1);
	}
	else {
		dsp_SENSr(hdc, x, y + 5, 1);
	}
	//左後ろ側方センサ
	if (inp_dt.AI[1] < 20000) {
		dsp_SENS(hdc, x, y + 45, 2);
	}
	else {
		dsp_SENSr(hdc, x, y + 45, 2);
	}

	x += 88;
	//右前側方センサ
	if (inp_dt.AI[2] < 20000) {
		dsp_SENS(hdc, x, y + 5, 3);
	}
	else {
		dsp_SENSr(hdc, x, y + 5, 3);
	}
	//左後ろ側方センサ
	if (inp_dt.AI[3] < 20000) {
		dsp_SENS(hdc, x, y + 45, 4);
	}
	else {
		dsp_SENSr(hdc, x, y + 45, 4);
	}

	//1,2 Sensor and Sensorber
	x = i;

	x -= 60;
	dsp_SENSn(hdc, x, y, 1);
	dsp_SENSn(hdc, x, y + 40, 2);

	x -= 20;
	y -= 5;
	dsp_sensbar(hdc, x, y - 20, 1);
	dsp_sensbar(hdc, x, y + 75, 2);

	//3,4 Sensor and Sensorber
	x = i;

	x += 108;

	dsp_SENSn(hdc, x, y, 3);
	dsp_SENSn(hdc, x, y + 60, 4);

	x -= 24;

	dsp_sensbar(hdc, x, y - 20, 3);
	dsp_sensbar(hdc, x, y + 75, 4);

}
void disp_siren() {

}
void disp_number(HDC hdc, int x, int y, int a) {
	char msg[256];
	HBRUSH hBrush;
	int i;
	for (int i = 0; i < 20; i++) {
		dsp_PWM(hdc, x + 200, y + i * 20, i + 1);
	}
}
//mode番号
void mode_number(HDC hdc, int x, int y, int RoboNo) {
	char msg[256];
	HBRUSH hBrush;
	int i;

	wsprintf(msg, "Mode:");
	TextOut(hdc, x, y, msg, lstrlen(msg));
	wsprintf(msg, "%d", Ctrlmode + 1);
	TextOut(hdc, x + 50, y, msg, lstrlen(msg));
}

void disp_io(HDC hdc, int x, int y, int RoboNo) {
	char msg[256];
	HBRUSH hBrush;
	int i;

	wsprintf(msg, "Mode:");
	TextOut(hdc, x, y, msg, lstrlen(msg));
	wsprintf(msg, "%d", Ctrlmode + 1);
	TextOut(hdc, x + 50, y, msg, lstrlen(msg));

	//stickbar
	wsprintf(msg, "L-stick");
	TextOut(hdc, x, y + 260, msg, lstrlen(msg));
	dsp_axis(hdc, x, y + 280, 1);
	wsprintf(msg, "R-stick");
	TextOut(hdc, x + 130, y + 260, msg, lstrlen(msg));
	dsp_axis(hdc, x + 130, y + 280, 4);

	wsprintf(msg, "LX %6d", ps2.axis[0]);
	TextOut(hdc, x + 170, y + 280, msg, lstrlen(msg));
	wsprintf(msg, "LY %6d", ps2.axis[1]);
	TextOut(hdc, x + 170, y + 300, msg, lstrlen(msg));
	wsprintf(msg, "RX %6d", ps2.axis[2]);
	TextOut(hdc, x + 170, y + 320, msg, lstrlen(msg));
	wsprintf(msg, "RY %6d", ps2.axis[5]);
	TextOut(hdc, x + 170, y + 340, msg, lstrlen(msg));

	switch (Ctrlmode) {
		//3号機
	case 0:

		//ここから
		wsprintf(msg, "救助用");
		TextOut(hdc, x + 70, y, msg, lstrlen(msg));
		y += 20;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
		wsprintf(msg, "<足回り>");

		TextOut(hdc, x, y, msg, lstrlen(msg));

		y += 20;
		wsprintf(msg, "移動  左前:");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		wsprintf(msg, "移動  左後ろ");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右前");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右後ろ");
		TextOut(hdc, x, y + 60, msg, lstrlen(msg));
		dsp_PWM(hdc, x + 200, y, 5);
		dsp_PWM(hdc, x + 200, y + 20, 6);
		dsp_PWM(hdc, x + 200, y + 40, 7);
		dsp_PWM(hdc, x + 200, y + 60, 8);

		y += 90;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 80);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		wsprintf(msg, "救助機構上下　　　↑ ↓");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		if (out_dt.PWM[11] == 1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[11] == -1000) {

			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		wsprintf(msg, "R巻き取り　     　　 〇 ×");
		TextOut(hdc, x, y+20 , msg, lstrlen(msg));
		if (out_dt.PWM[9] == 1000) {
			wsprintf(msg, "巻き取り");
			TextOut(hdc, x + 150, y+20, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[9] == -1000) {

			wsprintf(msg, "吐き出し");
			TextOut(hdc, x + 150, y+20, msg, lstrlen(msg));
		}
		wsprintf(msg, "L巻き取り　     　　 □ △");
		TextOut(hdc, x, y+40, msg, lstrlen(msg));
		if (out_dt.PWM[8] == 1000) {
			wsprintf(msg, "巻き出し");
			TextOut(hdc, x + 150, y+40, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[8] == -1000) {

			wsprintf(msg, "吐き出し");
			TextOut(hdc, x + 150, y+40, msg, lstrlen(msg));
		}
		wsprintf(msg, "救助機構前後　　　← →");
		TextOut(hdc, x, y+60, msg, lstrlen(msg));
		if (out_dt.PWM[12] == 1000) {
			wsprintf(msg, "前");
			TextOut(hdc, x + 150, y+60, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[12] == -1000) {

			wsprintf(msg, "後ろ");
			TextOut(hdc, x + 150, y+60, msg, lstrlen(msg));
		}
		dsp_PWM(hdc, x + 200, y, 11);
		dsp_PWM(hdc, x + 200, y + 20, 10);
		dsp_PWM(hdc, x + 200, y + 40, 9);
		dsp_PWM(hdc, x + 200, y + 60, 12);
		break;
		/*wsprintf(msg, "<肩>");
		TextOut(hdc, x, y, msg, lstrlen(msg));

		y += 20;
		wsprintf(msg, "右肩　×△");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		if (out_dt.PWM[4] == 1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[4] == -1000) {

			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		wsprintf(msg, "左肩　上下");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		if (out_dt.PWM[5] == 1000) {
			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[5] == -1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		wsprintf(msg, "スカート　R2L2");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		if (out_dt.PWM[11] > 0) {
			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[11] < 0) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}
		dsp_PWM(hdc, x + 200, y, 5);
		dsp_PWM(hdc, x + 200, y + 20, 6);
		dsp_PWM(hdc, x + 200, y + 40, 12);*/
	case 1:
		wsprintf(msg, "移動用");
		TextOut(hdc, x + 70, y, msg, lstrlen(msg));
		y += 20;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 130);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
		wsprintf(msg, "<足回り>");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		y += 20;
		wsprintf(msg, "移動  左前:");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		wsprintf(msg, "移動  左後ろ");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右前");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右後ろ");
		TextOut(hdc, x, y + 60, msg, lstrlen(msg));
		dsp_PWM(hdc, x + 200, y, 5);
		dsp_PWM(hdc, x + 200, y + 20, 6);
		dsp_PWM(hdc, x + 200, y + 40, 7);
		dsp_PWM(hdc, x + 200, y + 60, 8);

		y += 90;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 20);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		wsprintf(msg, "スカート　　　　　　　△ ×");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		if (out_dt.PWM[14] == 1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[14] == -1000) {

			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}

		dsp_PWM(hdc, x + 200, y, 14);
		break;
		/*wsprintf(msg, "<アーム制御>");
		TextOut(hdc, x, y, msg, lstrlen(msg));

		y += 20;
		wsprintf(msg, "左肘 ↑↓");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		if (out_dt.PWM[0] == 1000) {
			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[0] == -1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		wsprintf(msg, "右ひじ△×");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		if (out_dt.PWM[1] > 0) {
			wsprintf(msg, "up");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[1] < 0) {
			wsprintf(msg, "down");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		wsprintf(msg, "左手首←→");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		if (out_dt.PWM[8] > 0) {
			wsprintf(msg, "up");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[8] < 0) {
			wsprintf(msg, "down");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}

		wsprintf(msg, "つまみR2L2");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		if (out_dt.PWM[9] > 0) {
			wsprintf(msg, "open");
			TextOut(hdc, x + 150, y + 60, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[9] < 0) {
			wsprintf(msg, "close");
			TextOut(hdc, x + 150, y + 60, msg, lstrlen(msg));
		}
		wsprintf(msg, "右手首○□");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		if (out_dt.PWM[10] > 0) {
			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y + 80, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[10] < 0) {
			wsprintf(msg, "down");
			TextOut(hdc, x + 150, y + 80, msg, lstrlen(msg));
		}*/

		
	case 2:

		//ここから
		wsprintf(msg, "アーム用");
		TextOut(hdc, x + 70, y, msg, lstrlen(msg));
		y += 20;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));  // 灰色のストックブラシを取得する
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 100);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する
		wsprintf(msg, "<足回り>");

		TextOut(hdc, x, y, msg, lstrlen(msg));

		y += 20;
		wsprintf(msg, "移動  左前:");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		wsprintf(msg, "移動  左後ろ");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右前");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		wsprintf(msg, "移動	 右後ろ");
		TextOut(hdc, x, y + 60, msg, lstrlen(msg));
		dsp_PWM(hdc, x + 200, y, 5);
		dsp_PWM(hdc, x + 200, y + 20, 6);
		dsp_PWM(hdc, x + 200, y + 40, 7);
		dsp_PWM(hdc, x + 200, y + 60, 8);

		y += 90;
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);            // 取得したブラシを使用する
		Rectangle(hdc, x - 4, y - 4, x + 280, y + 60);
		DeleteObject(hBrush);                 // 作成した論理ブラシを削除する

		wsprintf(msg, "アーム肩　   　　　　　↑ ↓");
		TextOut(hdc, x, y, msg, lstrlen(msg));
		if (out_dt.PWM[13] == 1000) {
			wsprintf(msg, "DOWN");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[13] == -1000) {

			wsprintf(msg, "UP");
			TextOut(hdc, x + 150, y, msg, lstrlen(msg));
		}
		wsprintf(msg, "アーム手首　   　　　 × 〇");
		TextOut(hdc, x, y + 20, msg, lstrlen(msg));
		if (out_dt.PWM[2] == 1000) {
			wsprintf(msg, "左");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[2] == -1000) {

			wsprintf(msg, "右");
			TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
		}
		wsprintf(msg, "アームキャッチ　  　　□ △");
		TextOut(hdc, x, y + 40, msg, lstrlen(msg));
		if (out_dt.PWM[1] == 1000) {
			wsprintf(msg, "開き");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}
		else if (out_dt.PWM[1] == -1000) {

			wsprintf(msg, "閉まり");
			TextOut(hdc, x + 150, y + 40, msg, lstrlen(msg));
		}
		dsp_PWM(hdc, x + 200, y, 13);
		dsp_PWM(hdc, x + 200, y + 20, 2);
		dsp_PWM(hdc, x + 200, y + 40, 1);
		//dsp_PWM(hdc, x + 200, y + 60, 10);
		//dsp_PWM(hdc, x + 200, y + 80, 11);
		break;


	}
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkColor(hdc, RGB(255, 255, 255));
}
