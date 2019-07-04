/**
* Subroutine Module Header File
* @file sub_module.h
* @brief ��{�֐��̃v���b�g�^�C�v�錾
*
* @author Katayama
* @date 2009-01-26
* @version 1.00 : 2009/01/26 katayama
*
* Copyright (C) 2009 TPIP User Community All rights reserved.
* ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
* �g�p���������܂��B
*/

#ifndef _SUB_MODULE_H_
#define _SUB_MODULE_H_

#include <windows.h>

extern int Limit(int num, int min, int max);	// �㉺���J�b�g�֐�
extern int Cyclic(int num, int min, int max);	// �㉺���͈̓T�C�N���b�N�֐�
extern char* str_split(char *str);			// �����񕪊� �֐�
extern int mov_avr(int val);				// �ړ�����(Moving Average)
extern int Chk_File(char* file_name);		// �t�@�C���̃`�F�b�N


extern int mov_avr2(int val); // �ړ�����(Moving Average)
extern void str_cut(char* rd_msg, char* scan_word);
extern int rang_chg(int x, int x_min, int x_max, int y_min, int y_max);
extern int rang_chg_dzone(int x, int x_min, int x_max, int y_min, int y_max, int dzone);

extern void Font_init(void);
extern void Font_del(void);
extern HFONT Font_set(int Font_ID);
extern COLORREF chg_BkColor(HDC hDC, unsigned char id);


#endif /* ifndef _SUB_MODULE_H_ */
