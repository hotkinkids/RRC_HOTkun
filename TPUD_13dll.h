/** 
 * TPIP UDP communication class Driver 
 * @file TPUD_13dll.h
 * @brief UDP communication のクラス型ドライバー
 *
 * @author Katayama
 * @date 2003-03-20
 * @version $Id: TPUD_10.dll ,v 1.00 2008/03/20 00:00:00 katayama $
 * @version $Id: TPUD_11.dll ,v 1.10 2008/05/03 00:00:00 katayama $
 * @version $Id: TPUD_12.dll ,v 1.20 2008/10/01 00:00:00 katayama $
 * @version $Id: TPUD_13.dll ,v 1.30 2008/12/10 00:00:00 katayama $
 * @version $Id: TPUD_13.dll ,v 1.31 2009/03/04 00:00:00 katayama $
 *
 * Copyright (C) Sanritz Autiomation
 */
// V 1.1o recv関数に受信timeout指定付関数を追加。
//       APP用Header fileを整理（余計な#includeを削除）
// V 1.20 send_to関数、recv_from関数の追加
// V 1.30 addr情報からIP addres(ASCII)を取出す関数を追加
// V 1.31 コメント修正

/** 
 * このモジュールはUDPで通信するクラス関数です。
 */
#ifndef _TPUD_13DLL_H__
#define _TPUD_13DLL_H__


struct S_ADR {
	short family;
	unsigned short port;
	unsigned int   ip_adr;
	char           zero[8];
};

class __declspec(dllimport) w32udp
{
public:
	w32udp(char* sv_cl);		// コンストラクタ sv_cl= "UDP_C":client "UDP_S":server
	~w32udp();					// デストラクタ
	int open(char* host_adr, int port_no, int rcv_tm); //  open & 初期化関数
	int retry(void);			// 初期化再試行関数
	void close(void);			// 終了関数
	int send( void* send_buf, unsigned int send_size);	//	送信関数
	int recv( void* rcv_buf , unsigned int r_cnt);		// 受信関数
	int recv_rt(void* rcv_buf ,unsigned int r_cnt, int rv_tm);	// 受信関数(timer指定)
	int send_to( void* send_buf, unsigned int send_size, void* addr, int addr_sz);	//	送信関数
	int recv_from( void* rcv_buf , unsigned int r_cnt, void* addr, int* addr_sz);		// 受信関数
	int get_ip_address(void* addr, int addr_sz, char* ip_adr);	// IPアドレスの取出し
protected:
	char ip_adr[16];
	int  sv_flg;
	int  port_no;
	int  rcv_tm;
	int  init_flg;				//	初期化完了フラグ(0:未完、1:完了)
	int  data1;
	int  data2[4];
	int  data3[100];
};

#endif /* ifndef _TPUD_13DLL_H__ */
