/** 
 * TPIP GameController Header file
 * @file TPGC_11dll.h
 * @brief ゲーム　コントローラ入力関数
 *
 * @author Katayama
 * @date 2008-02-15
 * @version $Id: TPGC ,v 1.0  2008/02/15 00:00:00 katayama $
 * @version $Id: TPGC ,v 1.1  2008/10/12 00:00:00 katayama $
 * @version $Id: TPGC ,v 1.11 2009/01/26 00:00:00 katayama $
 *
 * Copyright (C) 2008 Sanritz Automation
 */

/** \mainpage
 * このファイルはゲーム　コントローラ入力関数(TPGC)のヘッダファイルです。
 */

#ifndef _TPGC_DLL_H__
#define _TPGC_DLL_H__
#include <windows.h>

//
//	Game controller 構造体定義
//
struct GC_STRUCT {	int	axis[8] ;	// [0]=lX,[1]=lY,[2]=lZ,[3]=lRx,[4]=lRy,[5]=lRz,[6]=Sl[0],[7]=Sl[1]
					int POV [4] ;	// [0]=POV[0],[1]=POV[1],[2]=POV[2],[3]=POV[3],
					int _BTN_bit;	// 前回BTN_bit
					int BTN_bit ;	// ボタンON/OFF ビットパターン
					int BTN_xor ;	// BTN_bit のXOR
					int POV_bit ;	// POV のビットパターン
					unsigned char Button[20]; // 各ボタンの状態( OFF: 0x00 / ON : 0x80 )
} ;


extern bool __stdcall TPGC_init(HINSTANCE, HWND);
extern bool __stdcall TPGC_close(void);
extern bool	__stdcall TPGC_get(GC_STRUCT*);
extern unsigned int __stdcall TPGC_get_type(void);
extern bool __stdcall TPGC_get_name(char* d_name);

#endif // _TPGC_DLL_H__
