/** 
 * @file dsp_com_inf.h
 * @brief �ʐM���̕\��( header file)
 *
 * @author Katayama
 * @date 2008-03-21
 *
 * Copyright (C) 2008 TPIP User Community All rights reserved.
 * ���̃t�@�C���̒��쌠�́ATPIP���[�U�[�R�~���j�e�B�̋K��ɏ]��
 * �g�p���������܂��B
 */

extern void Dsp_Com_stat(HDC hDC, int x, int y, int req, int stat);
extern void dsp_com_vga(HDC hDC, int x, int y, int vga);
extern void dsp_jpeg_speed(HDC hdc, int x, int y, int kbps, int jpeg_size);