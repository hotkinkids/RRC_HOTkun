
/**
* wavw file read/write program file
* @file wave_file.cpp
* @brief wave file read/write プログラム・ファイル
*
* @author Katayama
* @date 2010-04-15
* @version ver 1.00 2010/04/15
*
* Copyright (C) 2010 TPIP User Community All rights reserved.
* このファイルの著作権は、TPIPユーザーコミュニティの規約に従い
* 使用許諾をします。
*/
#include <windows.h>
#include <stdio.h>
#include "wave_file.h"
#include "TPSC_12dll.h"
#pragma  warning(disable:4996)	
#pragma  warning(disable:4244)

static FILE* wf_stream = NULL;		// 
static wvf_riff_t  wf_riff;			// RIFF header
static wvf_chunk_t wf_chunk;		// Chunk header
static chunk_fmt_t wf_fmt_chunk;	// Format chunk data
static int wf_data_size;// wave data size
static int smpl_rate;
static int save_sz;
static int load_sz;

/** get_riff_header
*  int get_riff_header()
* @retval == 0  : OK
* @retval != 0  : ERROR
*/
int get_riff_header()
{
	int rcnt;

	if (!wf_stream) return -1;
	rcnt = fread(&wf_riff, 1, sizeof(wf_riff), wf_stream);
	if (rcnt != sizeof(wf_riff)) return -2;
	if (strncmp("RIFF", &wf_riff.gp_id[0], 4) != 0) return -3;
	if (strncmp("WAVE", &wf_riff.id[0], 4) != 0)    return -4;
	return 0;
}

/** put_riff_header
*  int put_riff_header(int wv_size)
* @param  wv_size : wave data size
* @retval == 0  : OK
* @retval != 0  : ERROR
*/
int put_riff_header(int wv_size)
{
	int wcnt;

	if (!wf_stream) return -1;
	memcpy(&wf_riff.gp_id[0], "RIFF", 4);
	memcpy(&wf_riff.id[0], "WAVE", 4);
	wf_riff.dt_sz = wv_size + (sizeof(wf_chunk) * 2) + sizeof(wf_fmt_chunk);
	wcnt = fwrite(&wf_riff, 1, sizeof(wf_riff), wf_stream);
	if (wcnt != sizeof(wf_riff)) return -2;

	return 0;
}


/** get chunk data
*  int get_chunk()
* @retval > 0   : OK wave data size
* @retval <= 0  : ERROR
*/
int get_chunk()
{
	int rcnt;
	int loop;
	char nm[5];

	if (!wf_stream) return -1;
	loop = 1;
	while (loop) {
		rcnt = fread(&wf_chunk, 1, sizeof(wf_chunk), wf_stream);
		if (rcnt == sizeof(wf_chunk)) {
			if (strncmp("fmt ", wf_chunk.name, 4) == 0) {	// 'fmt ' chunk
				printf(" fmt chunk size=%d ", wf_chunk.size);
				if (wf_chunk.size == sizeof(wf_fmt_chunk)) {
					rcnt = fread(&wf_fmt_chunk, 1, sizeof(wf_fmt_chunk), wf_stream);
					printf(" sample:%d Hz ch:%d \n", wf_fmt_chunk.smpl_rate, wf_fmt_chunk.ch);
				}
				else {
					printf(" error \n");
					return -2; // 'fmt ' chunk size error
				}
			}
			else if (strncmp("data", wf_chunk.name, 4) == 0) {	// 'data' chunk
				printf(" data chunk size=%d \n", wf_chunk.size);
				wf_data_size = wf_chunk.size;
				loop = 0;
			}
			else { // other chunk
				memcpy(nm, wf_chunk.name, 4);
				nm[4] = 0;
				printf(" other(%s) chunk size=%d \n", nm, wf_chunk.size);
				fseek(wf_stream, wf_chunk.size, SEEK_CUR);		// Next chunk
			}
		}
	}
	return 0;
}

/** put fmt chunk header
*  int put_chunk_header(int sample)
* @param  sample  : sampling rate
* @param  wv_size : wave data size
* @retval == 0    : OK
* @retval != 0    : ERROR
*/
int put_chunk_header(int sample, int wv_size)
{
	int wcnt;

	if (!wf_stream) return -1;
	memcpy(&wf_chunk.name[0], "fmt ", 4);
	wf_chunk.size = sizeof(wf_fmt_chunk);
	wcnt = fwrite(&wf_chunk, 1, sizeof(wf_chunk), wf_stream);
	if (wcnt != sizeof(wf_chunk)) return -3;

	wf_fmt_chunk.d_type = 0x0001;		// PCM
	wf_fmt_chunk.ch = 0x0002;		// stereo
	wf_fmt_chunk.smpl_rate = sample;		// sample rate
	wf_fmt_chunk.s_c = 4;				// block size (16bit stereo = 2 * 2 = 4 byte) 
	wf_fmt_chunk.byte_sec = sample * wf_fmt_chunk.s_c;		// data speed (byte/sec)
	wf_fmt_chunk.byte_smpl = 16;			// bit / sample (16 bit)
	wcnt = fwrite(&wf_fmt_chunk, 1, sizeof(wf_fmt_chunk), wf_stream);
	if (wcnt != sizeof(wf_fmt_chunk)) return -4;

	memcpy(&wf_chunk.name[0], "data", 4);
	wf_chunk.size = wv_size;
	wcnt = fwrite(&wf_chunk, 1, sizeof(wf_chunk), wf_stream);
	if (wcnt != sizeof(wf_chunk)) return -5;
	return 0;
}


/** load Wave data
*  int load_wave_data(void)
*  @retval == 0  : OK
*  @retval != 0  : error
*/
int load_wave_data(void)
{
	int cnt;
	char* wf_data;

	wf_data = TPSC_get_soundout_bf(wf_data_size);
	if (wf_data == NULL) return -1;
	cnt = fread(wf_data, 1, wf_data_size, wf_stream);
	TPSC_ent_sndout_bf(cnt);
	return 0;
}

/** Wave file load
*  int wf_load(char* fl_name)
* @param  fl_name : file name
* @retval == 0   : OK
* @retval == -2  : open error
* @retval == -3  : file size error
* @retval == -4  : sampling rate error
*/
int wf_load(char* fl_name)
{
	int err;
	int res;

	if ((wf_stream = fopen(fl_name, "r+b")) == NULL) {	// open error
		return -2;
	}
	res = 0;
	if (err = get_riff_header()) res = -3;	// RIFF format error

	if (!res) {
		if (err = get_chunk()) res = -4;	// RIFF format error
	}
	if (!res) {
		smpl_rate = TPSC_get_sample();
		if (wf_fmt_chunk.smpl_rate != smpl_rate) res = -5; // sampling rate error
	}

	if (!res) {
		if (err = load_wave_data()) res = -6;
	}

	if (wf_stream) {
		fclose(wf_stream);
		wf_stream = NULL;
	}
	return res;
}


/** Wave file save
*  int wf_save(char* fl_name)
* @param  fl_name : file name
* @param  bf      : buffer pointer
* @param  time_ms : Record time(msec)
* @retval > 0   : OK wave data size
* @retval == -2 : open error
* @retval == -3 : RIFF header write error
* @retval == -4 : 'fmt ' chunk header write error
*/
int wf_save(char* fl_name, int time_ms)
{
	int err;
	int res;
	int wcnt;
	short* wv_data;
	int dt_size;
	int data_sz;

	data_sz = (TPSC_get_sample() * time_ms * 2 * 2) / 1000;	// sampling rate * time * 2ch * 2byte
	if (data_sz < 2) return -1;
	wv_data = (short*)malloc(data_sz);
	if (wv_data == NULL) return -1;

	res = 0;
	dt_size = (TPSC_get_data(wv_data, data_sz / 2) * 2);
	printf(" wave data size = %d \n", dt_size);
	if ((wf_stream = fopen(fl_name, "w+b")) == NULL) res = -2;	// open error

	if (!res) {
		if (err = put_riff_header(data_sz)) res = -3;					// write RIFF header
	}

	if (!res) {
		if (err = put_chunk_header(TPSC_get_sample(), dt_size)) res = -4;	// write 'fmt '& 'data' chunk header
	}

	if (!res) {
		wcnt = fwrite((char*)wv_data, 1, dt_size, wf_stream);			// write wave data (PCM)
		if (wcnt != dt_size) res = -5;
		else res = wcnt;
	}

	if (wf_stream) {
		fclose(wf_stream);
		wf_stream = NULL;
	}
	if (wv_data) {
		free(wv_data);
		wv_data = NULL;
	}
	return res;
}

/** wave file save size
*  int wf_save_sz(void)
* @retval   : save size
*/
int wf_save_sz(void)
{
	return save_sz;
}

/** wave file load size
*  int wf_load_sz(void)
* @retval   : load size
*/
int wf_load_sz(void)
{
	return load_sz;
}

/** wave file data size
*  int wf_data_sz(void)
* @retval   : wave file size
*/
int wf_data_sz(void)
{
	return wf_data_size;
}