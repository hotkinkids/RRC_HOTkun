/**
* @file dsp_com_inf.cpp
* @brief 通信情報の表示
*
* @author Katayama
* @date 2008-03-21
*
* Copyright (C) 2008 TPIP User Community All rights reserved.
* このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
* 使用許諾をします。
*/

#include <windows.h>
#include "TPJT3.h"	// リアルタイム動画伝送関数の include file

/** 通信状態　表示
*  void Dsp_Com_stat(HDC hdc, int x, int y, int req, int stat)
* @param  hdc     : handle of device context　
* @param  x       : 表示位置（ｘ）
* @param  y       : 表示位置（ｙ）
* @param  req     : 通信設定　要求値
* @param  stat    : 通信設定　現在値
*/
void Dsp_Com_stat(HDC hdc, int x, int y, int req, int stat)
{
	COLORREF old_color;

	old_color = SetTextColor(hdc, RGB(0, 0, 0));
	if (req & 0x01) {
		if (stat & 0x01) SetTextColor(hdc, RGB(12, 0, 20));
		else SetTextColor(hdc, RGB(86, 99, 143));
	}
	TextOut(hdc, x, y, " JPEG", 5);

	SetTextColor(hdc, RGB(80, 80, 80));
	if (req & 0x02) {
		if (stat & 0x02) SetTextColor(hdc, RGB(12, 0, 20));
		else SetTextColor(hdc, RGB(86, 99, 143));
	}
	TextOut(hdc, x, y + 20, "  SIO", 5);

	if (req & 0x04) {
		if (stat & 0x04) {
			SetTextColor(hdc, RGB(52, 38, 89));
			TextOut(hdc, x, y + 20, " user", 5);
		}
	}

	SetTextColor(hdc, old_color);
}

/** 画像モード　表示
*  void dsp_com_vga(HDC hdc, int x, int y, int vga)
* @param  hdc     : handle of device context　
* @param  x       : 表示位置（ｘ）
* @param  y       : 表示位置（ｙ）
* @param  vga     : 画像モード( 0: QVGA , 1: VGA)
*/
void dsp_com_vga(HDC hdc, int x, int y, int vga)
{
	char* V_Type;

	SetTextColor(hdc, RGB(0, 0, 0));
	V_Type = "Type[QVGA]";
	if (vga) V_Type = "Type[VGA]";
	TextOut(hdc, x, y, V_Type, lstrlen(V_Type));
}

/** 動画伝送速度　表示
*  void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size)
* @param  hdc      : handle of device context　
* @param  x        : 表示位置（ｘ）
* @param  y        : 表示位置（ｙ）
* @param  kbps     : 動画伝送速度　要求値
* @param  jpeg_size: JPEG data のサイズ
*/
void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size)
{
	static char MsgBuf[256];

	jpeg_size = jpeg_size / 100;
	int rq_speed = kbps / 100;
	int speed = TPJT_get_vspeed() / 100;
	//	wsprintf(MsgBuf,"動画伝送サイズ,%2d.%01d[Kbyte] 要求通信帯域,%2d.%01d[Mbps] 実行通信速度,%2d.%01d[Mbps]  ",
	//		jpeg_size/10, abs(jpeg_size%10), rq_speed/10, abs(rq_speed%10), speed/10, abs(speed%10));
	wsprintf(MsgBuf, "動画伝送サイズ  %02d.%01d[Kbyte]", jpeg_size / 10, abs(jpeg_size % 10));
	TextOut(hdc, x, y, MsgBuf, lstrlen(MsgBuf));
	wsprintf(MsgBuf, "要求通信帯域    %02d.%01d[Mbps]", rq_speed / 10, abs(rq_speed % 10));
	TextOut(hdc, x, y + 20, MsgBuf, lstrlen(MsgBuf));
	wsprintf(MsgBuf, "実行通信速度    %02d.%01d[Mbps]", speed / 10, abs(speed % 10));
	TextOut(hdc, x, y + 40, MsgBuf, lstrlen(MsgBuf));
}
