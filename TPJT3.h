/** 
 * @file  TPJT3.h
 * @brief TPIP3 リアルタイム動画伝送関数 ヘッダーファイル
 *
 * @version $Id: TPJT_10.dll   ,v 1.0  2008/03/20 00:00:00 katayama $
 * @version $Id: TPJT_11.dll   ,v 1.1  2008/05/04 00:00:00 katayama $
 * @version $Id: TPJT_11.dll   ,v 1.1a 2008/05/15 00:00:00 katayama $
 * @version $Id: TPJT_11.dll   ,v 1.1b 2008/07/24 00:00:00 katayama $
 * @version $Id: TPJT16_11.dll ,v 1.1c 2008/09/21 00:00:00 katayama $
 * @version $Id: TPJT216_11.dll ,v 2.00  2008/10/08 00:00:00 katayama $
 * @version $Id: TPJT216_12.dll ,v 2.10  2009/03/31 00:00:00 katayama $
 * @version $Id: TPJT216_13.dll ,v 2.20  2009/06/08 00:00:00 katayama $
 * @version $Id: TPJT216_13.dll ,v 2.21  2009/06/29 00:00:00 katayama $
 * @version $Id: TPJT216_14.dll ,v 2.30  2009/08/04 00:00:00 katayama $
 * @version $Id: TPJT216_15.dll ,v 2.40  2011/10/11 00:00:00 katayama $
 * @version $Id: TPJT216wr_dll.dll, v2.50 2012/11/19 00:00:00 hoshino $
 *
 * Copyright (C) 2014 Sanritz Automation
 */

#ifndef __TPJT3_H__
#define __TPJT3_H__

#pragma pack(1)

typedef struct OUT_DT_STR {
	unsigned short	 d_out;		
	short         	 nouse1;		
	short         	 PWM[4];		
	short         	 PWM2[16];	
}out_dt_t;

typedef struct INP_DT_STR {
	unsigned short	 b_ptn;	
	unsigned short	 AI[8];	
	short         	 PI[4];	
	unsigned short	 batt;	
	short         	 PI1;		
	short         	 PI2;		
	unsigned short	 DI;		
}in_dt_t;

// PI　データ 
typedef struct INP_PI_DT_STR{
    unsigned long   PI01_z;    //PI ch1 z相
    long            PI01_ab;   //PI ch1 ab相
    unsigned long   PI02_z;    //PI ch2 z相
    long            PI02_ab;   //PI ch2 ab相
    unsigned long   PI03_z;    //PI ch3 z相
    long            PI03_ab;   //PI ch3 ab相
    unsigned long   PI04_z;    //PI ch4 z相
    long            PI04_ab;   //PI ch4 ab相
} inp_pi_dt_t;

//
// CAN data 
typedef struct mctrl_can_str{	
	unsigned char	flg;			// send/receive flag
	unsigned char	RTR;			// RTR
	unsigned char	sz;				// data size
	unsigned char	stat;			// status
	unsigned short	STD_ID;			// standard ID
	unsigned char	data[8];		// data
} mctrl_can_t ;

// I2C　データ 
typedef struct mctrl_i2c_str{
	unsigned char	slave_id;		// スレーブID
	unsigned char 	sz;				// data size
	unsigned char 	data[32];		// data
} mctrl_i2c_t ;

// GPS データフレーム定義
typedef struct {
	unsigned char 	 YY;			// UTC 日付(YY)
	unsigned char 	 MM;			// UTC 日付(MM)
	unsigned char 	 DD;			// UTC 日付(DD)
	unsigned char 	 reserv1;
	unsigned char 	 hh;			// UTC 時刻(hh)
	unsigned char 	 mm;			// UTC 時刻(mm)
	unsigned char 	 ss;			// UTC 時刻(ss)
	unsigned char 	 reserv2;
	int           	 la_deg;		// 緯度(latitude) [度] x1000000
	int           	 lo_deg;		// 経度(longitude)[度] x1000000
	unsigned char 	 GPS_qlty;		// GPS Quality <0:非測位 1:GSP測位 2:DGPS測位>
	unsigned char 	 sat_cnt;		// Satellite cont 衛星数
	unsigned short	 HDOP;			// HDOP(horizontal dilution of precision) x100
	unsigned short	 speed;			// 速度 x10
	unsigned short	 course;		// 進行方位 x10
} mctrl_gps_t;

// JTPC ボディ部 ヘッダー定義
typedef struct mctrl_hd_str{
	u_char			ver;			// Header Version & F/W version
	u_char			msg_no;			// Message No
	u_char			d_id;			// Data ID
	u_char			stat;			// no use & F/W status
} mctrl_hd_t ;


// General　データフレーム定義
typedef struct {
	mctrl_hd_t  hd;
	char dt[36];
} mctrl_dt_t;

#pragma pack(8)

extern void				__stdcall TPJT_init( char* host_ip, HWND hwnd );
extern void				__stdcall TPJT_close( void );
extern void				__stdcall TPJT_set_ctrl( void* buf, int sz );
extern void				__stdcall TPJT_get_sens( void* buf, int buf_sz );
extern void				__stdcall TPJT_chg_camera( int no );
extern unsigned long 	__stdcall TPJT_JF_get_stamp( void );
extern int  			__stdcall TPJT_Get_Wlink( void );
extern int  			__stdcall TPJT_Get_stat( int no );
extern void 			__stdcall TPJT_init_ex( char* host_ip, HWND hwnd, int req );


extern int  			 __stdcall TPJT_get_jpeg( void* buf, int buf_sz );
extern int  			 __stdcall TPJT_get_vspeed( void );

extern int  			 __stdcall TPJT_set_com_req( int req, int w_time );
extern int  			 __stdcall TPJT_get_com_req( void );
extern int  			 __stdcall TPJT_get_com_stat( void );
extern int  			 __stdcall TPJT_get_com_ver( void );
extern void 			 __stdcall TPJT_set_video_inf( int type );
extern void 			 __stdcall TPJT_set_vspeed( int speed );

extern void*			 __stdcall TPJT_get_jpeg_file( int wait_flg, int wait_time, int* dwBytesRead );
extern void 			 __stdcall TPJT_free_jpeg_file( void );

extern int  			 __stdcall TPJT_Get_dtime( void );
extern int  			 __stdcall TPJT_Get_Staytime( void );

extern int  			 __stdcall TPJT_get_com_mode( void );
extern int  			 __stdcall TPJT_get_cam_no( void );

extern int  			 __stdcall TPJT_Send_CANdata( int no, mctrl_can_t* buf, int sz );
extern int  			 __stdcall TPJT_Recv_CANdata( int no, mctrl_can_t* buf );

extern int  			 __stdcall TPJT_Send_I2Cdata( int no, char *buf, int slave_id, int sz );
extern int  			 __stdcall TPJT_Req_Recv_I2Cdata( int no, int slave_id, int sz );
extern int  			 __stdcall TPJT_Recv_I2Cdata( int no, char *buf, int *slave_id, int *sz );

extern int   __stdcall TPJT_Req_Recv_PIdata(int no );	                    //
extern int   __stdcall TPJT_Req_Reset_PIdata(int no );	                    //
extern int   __stdcall TPJT_Recv_PIdata(int no, inp_pi_dt_t* buf);			//

extern void 			 __stdcall TPJT_set_video_fps( int fps );
extern int  			 __stdcall TPJT_Send_CMDdata( int no, mctrl_dt_t* buf, int sz );
extern int  			 __stdcall TPJT_Recv_RESdata( int no, mctrl_dt_t* buf );

#endif /* ifndef __TPJT3_H__ */
/*
//TPIP3では削除
extern int   __stdcall TPJT_set_rSIO_para(int UDP_port, int SIO_speed);	// [Ver 1.1] append

extern void  __stdcall TPJT_Set_sync(HWND hwnd);						// [Ver 1.1b] append

extern int   __stdcall TPJT_Recv_GPSdata(mctrl_gps_t* buf);			// [Ver 2.01] append

extern void __stdcall TPJT_Set_auto(int val);						// [Ver 1.1c] append
*/

