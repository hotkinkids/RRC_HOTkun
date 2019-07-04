/** 
 * wavw file read/write program Header file 
 * @file wave_file.h
 * @brief wave file read/write ヘッダ・ファイル
 *
 * @author Katayama
 * @date 2010-04-15
 * @version ver 1.00 2010/04/15
 *
 * Copyright (C) 2010 TPIP User Community All rights reserved.
 * このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
 * 使用許諾をします。
 */
#ifndef __WAVE_FILE_H__
#define __WAVE_FILE_H__

#pragma pack(1)
// wave file header format
// RIFF WAVE chunk format
typedef struct {
	char  gp_id[4];		// mark 'RIFF'
	int   dt_sz;		// data size ( file size - 8 )
	char  id[4];		// ID 'WAVE'
} wvf_riff_t;

// chunk Header format
typedef struct {		// chunk structural
	char  name[4];		// chunk name
	int   size;			// chunk size
} wvf_chunk_t;

// 'fmt ' chunk (size = 16 byte <PCM>)
typedef struct {
	short d_type;		// FormatTag  (PCM = 0x0001)
	short ch;			// Channels   (monaural = 0x0001, stereo = 0x0002)
	int   smpl_rate;	// sampling rate (Hz)
	int   byte_sec;		// data speed (byte/sec)<smpl_rate * s_c>
	short s_c;			// block size (16bit stereo = 2 * 2 = 4 byte) 
	short byte_smpl;	// bit / sample (16 bit)
} chunk_fmt_t;
#pragma pack(8)

extern int wf_load(char* fl_name);
extern int wf_load_sz(void);
extern int wf_data_sz(void);
extern int wf_save(char* fl_name, int data_sz);
extern int wf_save_sz(void);
#endif // #ifndef __WAVE_FILE_H__