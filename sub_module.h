/**
* Subroutine Module Header File
* @file sub_module.h
* @brief 基本関数のプロットタイプ宣言
*
* @author Katayama
* @date 2009-01-26
* @version 1.00 : 2009/01/26 katayama
*
* Copyright (C) 2009 TPIP User Community All rights reserved.
* このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
* 使用許諾をします。
*/

#ifndef _SUB_MODULE_H_
#define _SUB_MODULE_H_

#include <windows.h>

extern int Limit(int num, int min, int max);	// 上下限カット関数
extern int Cyclic(int num, int min, int max);	// 上下限範囲サイクリック関数
extern char* str_split(char *str);			// 文字列分割 関数
extern int mov_avr(int val);				// 移動平均(Moving Average)
extern int Chk_File(char* file_name);		// ファイルのチェック


extern int mov_avr2(int val); // 移動平均(Moving Average)
extern void str_cut(char* rd_msg, char* scan_word);
extern int rang_chg(int x, int x_min, int x_max, int y_min, int y_max);
extern int rang_chg_dzone(int x, int x_min, int x_max, int y_min, int y_max, int dzone);

extern void Font_init(void);
extern void Font_del(void);
extern HFONT Font_set(int Font_ID);
extern COLORREF chg_BkColor(HDC hDC, unsigned char id);


#endif /* ifndef _SUB_MODULE_H_ */
