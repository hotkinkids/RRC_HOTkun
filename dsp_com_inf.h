/** 
 * @file dsp_com_inf.h
 * @brief 通信情報の表示( header file)
 *
 * @author Katayama
 * @date 2008-03-21
 *
 * Copyright (C) 2008 TPIP User Community All rights reserved.
 * このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
 * 使用許諾をします。
 */

extern void Dsp_Com_stat(HDC hDC, int x, int y, int req, int stat);
extern void dsp_com_vga(HDC hDC, int x, int y, int vga);
extern void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size);