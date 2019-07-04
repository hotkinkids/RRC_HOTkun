/**
* @file dsp_com_inf.cpp
* @brief �ʐM���̕\��
*
* @author Katayama
* @date 2008-03-21
*
* Copyright (C) 2008 TPIP User Community All rights reserved.
* ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
* �g�p���������܂��B
*/

#include <windows.h>
#include "TPJT3.h"	// ���A���^�C������`���֐��� include file

/** �ʐM��ԁ@�\��
*  void Dsp_Com_stat(HDC hdc, int x, int y, int req, int stat)
* @param  hdc     : handle of device context�@
* @param  x       : �\���ʒu�i���j
* @param  y       : �\���ʒu�i���j
* @param  req     : �ʐM�ݒ�@�v���l
* @param  stat    : �ʐM�ݒ�@���ݒl
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

/** �摜���[�h�@�\��
*  void dsp_com_vga(HDC hdc, int x, int y, int vga)
* @param  hdc     : handle of device context�@
* @param  x       : �\���ʒu�i���j
* @param  y       : �\���ʒu�i���j
* @param  vga     : �摜���[�h( 0: QVGA , 1: VGA)
*/
void dsp_com_vga(HDC hdc, int x, int y, int vga)
{
	char* V_Type;

	SetTextColor(hdc, RGB(0, 0, 0));
	V_Type = "Type[QVGA]";
	if (vga) V_Type = "Type[VGA]";
	TextOut(hdc, x, y, V_Type, lstrlen(V_Type));
}

/** ����`�����x�@�\��
*  void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size)
* @param  hdc      : handle of device context�@
* @param  x        : �\���ʒu�i���j
* @param  y        : �\���ʒu�i���j
* @param  kbps     : ����`�����x�@�v���l
* @param  jpeg_size: JPEG data �̃T�C�Y
*/
void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size)
{
	static char MsgBuf[256];

	jpeg_size = jpeg_size / 100;
	int rq_speed = kbps / 100;
	int speed = TPJT_get_vspeed() / 100;
	//	wsprintf(MsgBuf,"����`���T�C�Y,%2d.%01d[Kbyte] �v���ʐM�ш�,%2d.%01d[Mbps] ���s�ʐM���x,%2d.%01d[Mbps]  ",
	//		jpeg_size/10, abs(jpeg_size%10), rq_speed/10, abs(rq_speed%10), speed/10, abs(speed%10));
	wsprintf(MsgBuf, "����`���T�C�Y  %02d.%01d[Kbyte]", jpeg_size / 10, abs(jpeg_size % 10));
	TextOut(hdc, x, y, MsgBuf, lstrlen(MsgBuf));
	wsprintf(MsgBuf, "�v���ʐM�ш�    %02d.%01d[Mbps]", rq_speed / 10, abs(rq_speed % 10));
	TextOut(hdc, x, y + 20, MsgBuf, lstrlen(MsgBuf));
	wsprintf(MsgBuf, "���s�ʐM���x    %02d.%01d[Mbps]", speed / 10, abs(speed % 10));
	TextOut(hdc, x, y + 40, MsgBuf, lstrlen(MsgBuf));
}
