/** 
 * Serial I/O class Driver 
 * @file TPSI_10dll.h
 * @brief COM port Serial I/O のクラス型ドライバー
 *
 * @author Katayama (Sanritz Automation Co., Ltd.)
 * @date 2007-09-04
 * @version $Id: TPSI_10.dll ,v 1.0 2007/09/04 00:00:00 katayama $
 *
 * Copyright (C) 2007 Sanritz Automation Co.,Ltd. All rights reserved.
 */

/** 
 * このモジュールは シリアルポートへのアクセスするクラス関数です。
 */
#ifndef _TPSI_10DLL_H__
#define _TPSI_10DLL_H__
#include <windows.h>

#define X_ON  1
#define X_OFF 0

class __declspec(dllimport) w32sio
{
public:
	w32sio(char* Port_Name);					/// コンストラクタ
	~w32sio();					/// デストラクタ
	int open(int Baudrate, int x_ctrl, int rcv_tm); /// COM port open & 初期化関数
	int retry(void);			/// 初期化再試行関数
	void close(void);			/// 終了関数
	int send( void* send_buf, unsigned int send_size);	///	送信関数
	int recv( void* rcv_buf , unsigned int r_cnt);		/// 受信関数
	void dtr(int on);			/// DTR セット関数
	void rts(int on);			/// RTS セット関数
protected:
	char port[16];
	int  speed;
	int  xonoff;
	int  rcv_tm;
	int  init_flg;				//	初期化完了フラグ(0:未完、1:完了)
	HANDLE	fHandle;
};
#endif

