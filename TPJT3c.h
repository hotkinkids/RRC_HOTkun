/** 
* TPIP3 JTP communication(16bit class) Header file
 * @file TPJT216c_11dll.h
 * @brief JTP communication �v���O�����@DLL �w�b�_�[�t�@�C��
 *
 * @author Katayama
 * @date 2011-06-29
 *
 * @version 1.10 katayama
 * Copyright (C) 2011 TPIP User Community All rights reserved.
 * ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
 * �g�p���������܂��B
 */
// ver1.10 :GetComMode(void) �ǉ�

/** \mainpage
 * ���̃��W���[���͓���`���̐���v���g�R�������ł��B
 */
#ifndef __TPJT3C__H__
#define __TPJT3C__H__

#pragma pack(1)
#include<windows.h>

typedef struct OUT_DT_STR {
	unsigned short d_out;	//!< [ver 2.21]DO bit3 - 0���L��
	short          nouse1;
	short          PWM[4];
	short          PWM2[16];
} out_dt_t;

typedef struct INP_DT_STR {
	unsigned short b_ptn;	// [ver 2.20]
	unsigned short AI[8];	// [ver 2.10]
	short          PI[4];
	unsigned short batt;	// [ver 2.20]
	short          PI1;		//!< PI[0]�Ɠ���
	short          PI2;		//!< PI[1]�Ɠ���
	unsigned short DI;
} inp_dt_t;

// PI �f�[�^
typedef struct INP_PI_DT_STR{
    unsigned long   PI01_z;    //PI ch1 z��
    long            PI01_ab;   //PI ch1 ab��
    unsigned long   PI02_z;    //PI ch2 z��
    long            PI02_ab;   //PI ch2 ab��
    unsigned long   PI03_z;    //PI ch3 z��
    long            PI03_ab;   //PI ch3 ab��
    unsigned long   PI04_z;    //PI ch4 z��
    long            PI04_ab;   //PI ch4 ab��
} inp_pi_dt_t;

// CAN data 
typedef struct mctrl_can_str{	// Ver 2.00a
	unsigned char  flg;		// send/receive flag
	unsigned char  RTR;		// RTR
	unsigned char  sz;		// data size
	unsigned char  stat;	// status
	unsigned short STD_ID;	// standard ID
	unsigned char  data[8];	// data
} mctrl_can_t ;

// GPS �f�[�^�t���[����`
typedef struct {
	unsigned char  YY;			// UTC ���t(YY)
	unsigned char  MM;			// UTC ���t(MM)
	unsigned char  DD;			// UTC ���t(DD)
	unsigned char  reserv1;
	unsigned char  hh;			// UTC ����(hh)
	unsigned char  mm;			// UTC ����(mm)
	unsigned char  ss;			// UTC ����(ss)
	unsigned char  reserv2;
	int            la_deg;		// �ܓx(latitude) [�x] x1000000
	int            lo_deg;		// �o�x(longitude)[�x] x1000000
	unsigned char  GPS_qlty;	// GPS Quality <0:�񑪈� 1:GSP���� 2:DGPS����>
	unsigned char  sat_cnt;		// Satellite cont �q����
	unsigned short HDOP;		// HDOP(horizontal dilution of precision) x100
	unsigned short speed;		// ���x x10
	unsigned short course;		// �i�s���� x10
} mctrl_gps_t;

// I2C�@�f�[�^ 
typedef struct mctrl_i2c_str{
	unsigned char  slave_id;	// �X���[�uID
	unsigned char  nouse;		// no use
	unsigned int   sz;			// data size
	unsigned char  data[1024];	// data
} mctrl_i2c_t ;

// JTPC �{�f�B�� �w�b�_�[��`
typedef struct mctrl_hd_str{
	u_char	ver;	// Header Version & F/W version
	u_char	msg_no;	// Message No
	u_char	d_id;	// Data ID
	u_char	stat;	// no use & F/W status
} mctrl_hd_t ;


// General�@�f�[�^�t���[����`
typedef struct {
	mctrl_hd_t  hd;
	char dt[36];
} mctrl_dt_t;

#pragma pack(8)
#ifndef ___TPJT_TEST__
class __declspec(dllimport) TPJTclass
#else
class  TPJTclass
#endif	// #ifndef ___TPJT_TEST__
{
public:
	TPJTclass(void);
	~TPJTclass();
	void Init(char* host_ip, HWND hwnd, int req);
	void Close(void);
	char* GetIPaddr(void);
	int  SetComReq(int req, int w_time);
	int  GetComReq(void);
	int  GetComStat(void);
	void SetCtrlOn(int on);
	void SetCtrlData(out_dt_t* buf, int sz);
	void GetSensData(inp_dt_t* buf, int buf_sz, int wait_tm = 0);
	void ChangeCamera(int no);
	int  GetWlink(void);
	void SetSync(HWND hwnd);
	int  GetComm(void);
	int  GetVersion(void);

	int  SendCMDdata(int no,mctrl_dt_t* buf, int sz);
	int  RecvRESdata(int no,mctrl_dt_t* buf, int wait_tm = 0);

	void SetVideoType(int type);
	int  GetVideoType(void);
	void SetVSpeed(int speed);
	int  GetVSpeed(void);

	void* GetJpegFile(int wait_flg, int wait_time, int* dwBytesRead);
	void FreeJpegFile(void);
	int  GetJpegData(void* buf, int buf_sz);				// Get JPEG data
	int  GetComMode(void);									// get_com_mode
	int  GetCamNo(void);    								//#1725
	unsigned long GetJpegTimeStamp(void); 					//2013/08/22
	int  GetDtime(void);									// 2013/08/22
	int  GetStaytime(void);									// 2013/08/22
	int  GetBoardStat(int no);								// 2013/08/22
	void SetAutoCtrl(int val);      						// 2013/08/27
	int  RecvGPSData(mctrl_gps_t* buf);						// 2013/08/28
	int  SendCANData(int no,mctrl_can_t* buf,int sz);		// 2013/08/28
	int  RecvCANData(int no,mctrl_can_t *buf);				// 2013/08/28
	int  SetrSIOPara(int UDP_port,int SIOspeed); 			// 2013/08/28
	int  SendI2CData(int no,char* buf,int slave_id,int sz);	// 2014/03/11
	int  RecvI2CData(int no,char* buf,int *slave_id,int *sz);	// 2014/03/11
	int  ReqRecvI2CData(int no,int slave_id,int sz);        // 2014/03/19
	int  ReqRecvPIData(int no);                         // 2014/06/10
	int  ReqResetPIData(int no);                        // 2014/06/10
	int  RecvPIData(int no , inp_pi_dt_t *buf);         // 2014/06/10
	void  SetVideoFps(int fps);                         // 2014/04/01
protected:
	long dumy[16];
};


#endif /* ifndef __TPJT3C__H__ */
