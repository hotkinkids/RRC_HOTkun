/** 
 * TPIP UDP communication class Driver 
 * @file TPUD_13dll.h
 * @brief UDP communication �̃N���X�^�h���C�o�[
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
// V 1.1o recv�֐��Ɏ�Mtimeout�w��t�֐���ǉ��B
//       APP�pHeader file�𐮗��i�]�v��#include���폜�j
// V 1.20 send_to�֐��Arecv_from�֐��̒ǉ�
// V 1.30 addr��񂩂�IP addres(ASCII)����o���֐���ǉ�
// V 1.31 �R�����g�C��

/** 
 * ���̃��W���[����UDP�ŒʐM����N���X�֐��ł��B
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
	w32udp(char* sv_cl);		// �R���X�g���N�^ sv_cl= "UDP_C":client "UDP_S":server
	~w32udp();					// �f�X�g���N�^
	int open(char* host_adr, int port_no, int rcv_tm); //  open & �������֐�
	int retry(void);			// �������Ď��s�֐�
	void close(void);			// �I���֐�
	int send( void* send_buf, unsigned int send_size);	//	���M�֐�
	int recv( void* rcv_buf , unsigned int r_cnt);		// ��M�֐�
	int recv_rt(void* rcv_buf ,unsigned int r_cnt, int rv_tm);	// ��M�֐�(timer�w��)
	int send_to( void* send_buf, unsigned int send_size, void* addr, int addr_sz);	//	���M�֐�
	int recv_from( void* rcv_buf , unsigned int r_cnt, void* addr, int* addr_sz);		// ��M�֐�
	int get_ip_address(void* addr, int addr_sz, char* ip_adr);	// IP�A�h���X�̎�o��
protected:
	char ip_adr[16];
	int  sv_flg;
	int  port_no;
	int  rcv_tm;
	int  init_flg;				//	�����������t���O(0:�����A1:����)
	int  data1;
	int  data2[4];
	int  data3[100];
};

#endif /* ifndef _TPUD_13DLL_H__ */
