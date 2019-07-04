/** 
 * TPIP Sound communication Header file 
 * @file TPSC_10dll.h
 * @brief TPIP �����f�[�^�ʐM �w�b�_�E�t�@�C��
 *
 * @author Katayama
 * @date 2010-04-19
 * @version ver 1.00 2010/04/19
 *
 * Copyright (C) 2010 TPIP User Community All rights reserved.
 * ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
 * �g�p���������܂��B
 */
#ifndef  __TPSC_10DLL_H__
#define  __TPSC_10DLL_H__

extern int  __stdcall TPSC_open(char* ip_adr, int sample);
extern int  __stdcall TPSC_close(void);
extern int  __stdcall TPSC_get_data(short* bf, int rq_size);
extern int  __stdcall TPSC_get_sample(void);
extern int  __stdcall TPSC_get_soundin_time(void);
extern void __stdcall TPSC_clear_soundin_data(void);
extern int  __stdcall TPSC_get_status(void);

extern char* __stdcall TPSC_get_soundout_bf(int dt_sz);
extern void  __stdcall TPSC_ent_sndout_bf(int dt_sz);

#endif	// #ifndef  __TPSC_10DLL_H__
