/** 
 * Serial I/O class Driver 
 * @file TPSI_10dll.h
 * @brief COM port Serial I/O �̃N���X�^�h���C�o�[
 *
 * @author Katayama (Sanritz Automation Co., Ltd.)
 * @date 2007-09-04
 * @version $Id: TPSI_10.dll ,v 1.0 2007/09/04 00:00:00 katayama $
 *
 * Copyright (C) 2007 Sanritz Automation Co.,Ltd. All rights reserved.
 */

/** 
 * ���̃��W���[���� �V���A���|�[�g�ւ̃A�N�Z�X����N���X�֐��ł��B
 */
#ifndef _TPSI_10DLL_H__
#define _TPSI_10DLL_H__
#include <windows.h>

#define X_ON  1
#define X_OFF 0

class __declspec(dllimport) w32sio
{
public:
	w32sio(char* Port_Name);					/// �R���X�g���N�^
	~w32sio();					/// �f�X�g���N�^
	int open(int Baudrate, int x_ctrl, int rcv_tm); /// COM port open & �������֐�
	int retry(void);			/// �������Ď��s�֐�
	void close(void);			/// �I���֐�
	int send( void* send_buf, unsigned int send_size);	///	���M�֐�
	int recv( void* rcv_buf , unsigned int r_cnt);		/// ��M�֐�
	void dtr(int on);			/// DTR �Z�b�g�֐�
	void rts(int on);			/// RTS �Z�b�g�֐�
protected:
	char port[16];
	int  speed;
	int  xonoff;
	int  rcv_tm;
	int  init_flg;				//	�����������t���O(0:�����A1:����)
	HANDLE	fHandle;
};
#endif

