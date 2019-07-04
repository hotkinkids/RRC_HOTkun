/** 
 * TPIP Sound communication Header file 
 * @file TPSC_12dll.h
 * @brief TPIP 音声データ通信 ヘッダ・ファイル
 *
 * @author Katayama
 * @date 2010-04-19
 * @version ver 1.00 2010/04/19
 * @version ver 1.20 2011/01/09
 *
 * Copyright (C) 2010 TPIP User Community All rights reserved.
 * このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
 * 使用許諾をします。
 */
#ifndef  __TPSC_12DLL_H__
#define  __TPSC_12DLL_H__

extern int  __stdcall TPSC_open(char* ip_adr, int sample = 8000, int ch = 2, int buf_sz = 10);
extern int  __stdcall TPSC_close(void);
extern int  __stdcall TPSC_get_data(short* bf, int rq_size);
extern int  __stdcall TPSC_put_data(short* bf, int rq_size);
extern int  __stdcall TPSC_get_sample(void);
extern int  __stdcall TPSC_get_ch(void);
extern int  __stdcall TPSC_get_status(void);
extern int  __stdcall TPSC_cnv_b2tm(int sz);
extern int  __stdcall TPSC_cnv_tm2b(int tm);

extern void __stdcall TPSC_clear_sndin_bf(void);
extern int  __stdcall TPSC_get_sndin_size(void);
extern int  __stdcall TPSC_wait_sndin_size(int wait_size, int wait_tm);
extern int  __stdcall TPSC_set_sndin_amp(int amp);
extern int  __stdcall TPSC_lock_sndin(void);
extern void __stdcall TPSC_release_sndin(void);

extern void __stdcall TPSC_clear_sndout_bf(void);
extern int  __stdcall TPSC_get_sndout_rsize(void);
extern int  __stdcall TPSC_wait_sndout_rsize(int wait_rsize, int wait_tm);
extern int  __stdcall TPSC_set_sndout_amp(int amp);
extern int  __stdcall TPSC_lock_sndout(void);
extern void __stdcall TPSC_release_sndout(void);
//音声関係いらないようであれば以下のものを消す
extern int  __stdcall TPSC_get_soundin_time(void);
extern void __stdcall TPSC_clear_soundin_data(void);

extern char* __stdcall TPSC_get_soundout_bf(int dt_sz);
extern void  __stdcall TPSC_ent_sndout_bf(int dt_sz);

#endif	// #ifndef  __TPSC_12DLL_H__
