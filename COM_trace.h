/**
* @file  RS_trace.h
* @brief RS232�ʐM�g���[�X�E�v���O���� Header file
*
* @author Katayama
* @date 2009-01-26
* @version 1.00 : 2009/01/26 katayama
*
* Copyright (C) 2009 TPIP User Community All rights reserved.
* ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
* �g�p���������܂��B
*/

#ifndef __COM_TRACE_H__
#define __COM_TRACE_H__

typedef short TRACE_DT;

class COM_trace {
public:
	COM_trace(void);
	~COM_trace(void);
	void init(int sz);
	void put_data(unsigned char atr, unsigned char dt);
	int getm_data(TRACE_DT* ptr, int n);
	int get_index(void) { return (indx); };
protected:
	int	buf_sz;
	int indx;
	int cnt;
	TRACE_DT* tr_buf;
};

#endif /* ifndef __COM_TRACE_H__ */
