/** 
 * TPIP Graphic Managment Header file
 * @file TPGM_12dll.h
 * @brief JTP communication �v���O�����@DLL �w�b�_�[�t�@�C��
 *
 * @author Katayama
 * @date 2003-03-20
 * @version $Id: TPGM_10.dll   ,v 1.0  2008/03/20 00:00:00 katayama $
 * @version $Id: TPGM_10.dll   ,v 1.01 2009/01/26 00:00:00 katayama $
 * @version $Id: TPGM_11.dll   ,v 1.10 2009/01/26 00:00:00 katayama $
 * @version $Id: TPGM_12.dll   ,v 1.20 2009/07/15 00:00:00 katayama $
 * @version $Id: TPGM_12.dll   ,v 1.21 2010/01/27 00:00:00 katayama $
 *
 * Copyright (C) 2008 Sanritz Automation
 */

// ver 1.21 Video memory �� system memory��ɂ���ꍇ�̍����]����

#ifndef __JDRAW_DDL_H__
#define __JDRAW_DDL_H__


extern BOOL	__stdcall TPGM_create(HWND,int,HBITMAP);	// �T�[�t�F�X����
extern BOOL	__stdcall TPGM_create_ex(HWND,int,HBITMAP, HBRUSH);	// �T�[�t�F�X����
extern void	__stdcall TPGM_delete	(void);				// �T�[�t�F�X�j��
extern BOOL	__stdcall TPGM_decode(void* lpSrc, DWORD dwSize);	// JPEG �L��
extern void	__stdcall TPGM_copy(int,int);						// JPEG�摜�R�s�[
extern HDC	__stdcall TPGM_getDC(void);                    // DC�擾
extern void	__stdcall TPGM_releaseDC(void);                // DC���
extern void	__stdcall TPGM_screen(void);                     // �X�N���[���`��
extern int	__stdcall TPGM_sprite(RECT*,int dst_x,int dst_y);	// �X�v���C�g�`��
extern int	__stdcall TPGM_sprite2(RECT*,int dst_x,int dst_y);	// �X�v���C�g�`��
extern int __declspec(dllimport) TPGM_JPEGfile2DIB(LPCSTR lpszPathName, BITMAPINFOHEADER* dib, unsigned int dib_sz);
extern int __declspec(dllimport) TPGM_JPEGbuffer2DIB(void* lpSrc, DWORD dwSize, BITMAPINFOHEADER* dib, unsigned int dib_sz);

#ifdef __CV_H__		// V1.10 append
extern int __declspec(dllimport) TPGM_JPEGbuffer2CV(void* lpSrc, DWORD dwSize, IplImage *image);
#endif

extern void	__stdcall TPGM_copy2(RECT*,RECT*);				// JPEG�摜�R�s�[�i�͈͎w��j

#endif // __JDRAW_DDL_H__

