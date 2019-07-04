/******************************************************
*	2015 Hotaka Hayashi Kindai Univ.
*******************************************************/

#define TITLETEXT "1���@"
#define IP "192.168.13.101"		//�����p	
//#define IP "192.168.0.200"			//�L���p
//#define IP "192.168.13.103"
#define ROBOT ""
int RNo = 03;

#define tSet (8*(60)*1000+500)

#define _CRT_SECURE_NO_WARNINGS

//include files
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include "TPIP.h"
#include "dsp_com_inf.h"
#include "ctrl_PWM.h"
#include "wave_file.h"
#include "disp_io.h"
#include "TPSC_12dll.h"
#include "TPSC_10dll.h"

#pragma  warning(disable:4996)	
#pragma  warning(disable:4244)



// Bitmap Load area
static void *jpeg_data;
static int jpeg_size;
HBITMAP	hBitmap;


//TPIP����p�ϐ�
static int vga = 0;				// 0:QVGA 1:VGA
static int kbps = 4000;			// ����`�����x�@�v���l


								//�`��̂��߂̊�{�ϐ�
static HWND  hwnd;
static HDC hdc;
static PAINTSTRUCT ps;


//���o�͗p�\����
struct OUT_DT_STR out_dt		// ����o�̓f�[�^
	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
struct INP_DT_STR inp_dt;		// �Z���T�[���̓f�[�^
struct GC_STRUCT ps2;			//�Q�[���R���g���[�����͊i�[


								//��ʂ̑傫���̎w��
static int WIN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
static int WIN_HEIGHT = -40 + GetSystemMetrics(SM_CYSCREEN);


//�V�X�e���^�C�}�p�ϐ�
static DWORD dwStart;		//�^�C�}�X�^�[�g���̎��Ԃ��L�^(msec)
static DWORD Count;			//�^�C�}�imsec�j


							//�J�����E���[�h�p�t���O
int camNum = 0;				//���[�h(�J�����A����)�t���O
int Ctrlmode = 0;
static bool tFlag = false;	//�^�C�}�p�t���O


							//���������p�֐�
static int err;
static int sample = 16000;	// sampling rate 16000Hz�w��
static int msec = 0;
static int stat;


//Thread�p�ϐ�
DWORD dwExCode;
HANDLE hTh;
unsigned int *thID;


//�摜�p�ϐ�
static bool dsp_Flag = false;	//�摜�p�t���O
static 	int bmp_sz;
static int dsp_cnt = 0;
static BYTE* buffer = NULL;
static unsigned int buffer_sz = sizeof(BITMAPINFOHEADER) + (640 * 3 + 4) * 480;
BITMAPINFOHEADER* dib;
BITMAPINFO* pbmi;
static POINT pos;
static POINT epos;
static bool pFlag = false;
static bool tesF = false;


//�v���g�^�C�v�錾
//unsigned int __stdcall wavThread(void*);					//�X���b�h�p
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		//�E�B���h�E�v���V�[�W��
HWND Create(HINSTANCE hInst);								//�E�B���h�E����

void dib_disp(int dxPos, int dyPos);								//��ʕ`��p
int RGBToHSV(int r, int g, int b);								//�摜�p
COLORREF HSVToRGBColor(float hue, float saturation, float value);	//
void HueColor(int hue);											//hue�l��F�Ƃ��ďo��
void time_disp(HDC hdc);
void dib_disp(HDC hdc, BYTE* pBits) {

}

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG          msg;

	//��ʐ���
	hwnd = Create(hInstance);

	//����pbuffer�̊m��
	buffer = new BYTE[buffer_sz];

	//bitmap�摜�p�i�[�G���A
	hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, "hot-kun.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// �T�[�t�F�X����
	int ans = TPGM_create(hwnd, vga, hBitmap);

	//TPIP�p�֐��̏�����
	TPGC_init(hInstance, hwnd);// �Q�[���R���g���[�����͊֐��̏�����
	TPJT_init_ex(IP, hwnd, 0x03);		// ���A���^�C������`�������̏�����
	TPJT_set_com_req(0x03, 0);			// ����{�[�h�ʐM�{JPEG�ʐM


										//�`��X�V�p
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


//��{���̕\���p(���W�w��)
void Disp_lib(int x, int y) {
	char msg[256];
	int com_rq, com_st;
	HBRUSH hBrush;

	com_rq = TPJT_get_com_req();					// �ʐM���[�h�v��
	com_st = TPJT_get_com_stat();					// �ʐM���[�h����
	hBrush = CreateSolidBrush(RGB(255, 255, 255));


	if (com_st & 0x01)
	{

		SetTextColor(hdc, RGB(0, 255, 0));
		sprintf(msg, ">>connected<<");

	}
	else
	{

		SetTextColor(hdc, RGB(255, 0, 0));
		sprintf(msg, "<<disconnect>>");

	}


	TextOut(hdc, x + 150, y + 20, msg, lstrlen(msg));
	SetTextColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + 48, y + 40);
	DeleteObject(hBrush);


	Dsp_Com_stat(hdc, x + 2, y + 2, com_rq, com_st);		//sio,jpeg,user		
	dsp_com_vga(hdc, x + 50, y + 20, vga);					//qvga,vga
	dsp_jpeg_speed(hdc, x + 50, y + 40, kbps, jpeg_size);	//jpegspeeds


	sprintf(msg, "Link:%02d", TPJT_Get_Wlink());
	TextOut(hdc, x + 54, y, msg, lstrlen(msg));
	sprintf(msg, "IP:%s", IP);
	TextOut(hdc, x + 110, y, msg, lstrlen(msg));


	if (((inp_dt.batt * 20) / 0xffff) <= 9) {
		SetTextColor(hdc, RGB(168, 0, 47));
	}
	sprintf(msg, "BATT�c�ʁ@�@   �@%1d.%1d[V] ", inp_dt.batt / 100, (inp_dt.batt / 100) % 100);
	//(inp_dt.batt * 20) / 0xffff, abs((inp_dt.batt * 20 * 10) / 0xffff % 10));
	TextOut(hdc, x + 50, y + 20 + 80, msg, lstrlen(msg));		// �����\��
	SetTextColor(hdc, RGB(0, 0, 0));
}

//�E�B���h�E�`��p
void WM_Paint(HWND hwnd)
{
	int ans;
	static int s_vga;
	RECT rec = { WIN_WIDTH / 2 - 160, 20, WIN_WIDTH / 2 + 160, 80 }; //�^�C�}�p
	static LOGFONT logfont, f0;
	HFONT hFont, hF0;
	HPEN hPen;
	HBRUSH hBrush;
	char msg[256];

	//******************�f�B�X�v���C�ʒu****************//

	RECT bmp = { 0, 4, 100, 104 };

	RECT rect1 = { 0, 0, 640, 480 };
	RECT rect1q = { 0, 0, 320, 240 };
	RECT rect2 = { 30, 600, 30 + 320, 600 + 240 };
	RECT rect2q = { 330, 90, 100 + 320 * 4, 90 + 240 * 2.6 };

	RECT vga1 = { 0,0,320,480 };
	RECT vga2 = { 319,0,640,480 };

	RECT vga1a = { 2000,25,2000 + 720,975 };
	RECT vga2a = { 2000 + 720 + 0,25, 2000 + 720 + 720 + 0, 975 };

	RECT vga1b = { 400,90,1300,900 };   //a��b�͌Œ�




										//***************************************************//
	BYTE* pBits;
	bool err = false;
	int dxPos, dyPos;
	int cxDib, cyDib;

	ZeroMemory(&f0, sizeof(f0));			// �t�H���g���\���̏�����
	f0.lfCharSet = DEFAULT_CHARSET;			// �V�X�e���̃f�t�H���g�����Z�b�g���g��
	f0.lfHeight = 16;
	f0.lfQuality = ANTIALIASED_QUALITY;

	ZeroMemory(&logfont, sizeof(logfont));
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfHeight = 16;
	logfont.lfQuality = ANTIALIASED_QUALITY;

	// �㏑�摜�p�ϐ�

	// JPEG	�f�[�^�̎�o��	
	jpeg_data = TPJT_get_jpeg_file(0, 0, &jpeg_size);
	if ((jpeg_data) && (jpeg_size > 0)) {
		if (dsp_Flag) {
			bmp_sz = 0;
			dib = NULL;
			dib = (BITMAPINFOHEADER*)buffer;
			bmp_sz = TPGM_JPEGbuffer2DIB(jpeg_data, jpeg_size, dib, buffer_sz);
			dsp_Flag = false;
		}
		ans = TPGM_decode(jpeg_data, jpeg_size);
	}
	TPJT_free_jpeg_file();	// JPEG file ���

	hdc = TPGM_getDC();

	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	hPen = CreatePen(PS_INSIDEFRAME, 10, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, 0, 0, 4000/*WIN_WIDTH*/, WIN_HEIGHT + 100);//330
	DeleteObject(hPen);
	Rectangle(hdc, WIN_WIDTH / 2 - 324, 86, WIN_WIDTH / 2 + 324, 88 + 486);
	DeleteObject(hBrush);
	TPGM_releaseDC();

	if (s_vga != vga) {
		TPGM_delete();
		TPGM_create(hwnd, vga, hBitmap);
		s_vga = vga;
	}

	// ���摜�̓]���i����@���@��Ɖ�ʁj (640,480)
	//TPGM_copy((WIN_WIDTH / 2) - 310, 90);//(WIN_HEIGHT/2)-240

	if (vga == 1) {
		TPGM_copy2(&vga1, &vga1a);	//����(VR���j�^�p)
		TPGM_copy2(&vga2, &vga2a);	//�E��(VR���j�^�p)

		TPGM_copy2(&vga1, &vga1b);	//�{��ʗp

		TPGM_copy2(&rect1, &rect2);	//�e�X�g�p
	}
	else {	//QVGA
		TPGM_copy2(&rect1q, &rect2q);
	}

	//	if (hBitmap) TPGM_sprite(&bmp,0,0);		//.bmp���̊֐���DC������݂̂����`�悵�Ă���Ȃ�


	// �㏑�摜�̓]���i�㏑�摜�@���@��Ɖ�ʁj
	hdc = TPGM_getDC();

	//�X�e�[�^�X�\��
	SetBkColor(hdc, RGB(255, 255, 255));

	SetTextColor(hdc, RGB(0, 0, 0));
	logfont.lfHeight = 16;
	hFont = CreateFontIndirect(&logfont);		// �_���t�H���g���쐬
	SelectObject(hdc, hFont);

	sprintf(msg, ROBOT);
	TextOut(hdc, 5, 20, msg, lstrlen(msg));
	DeleteObject(hFont);

	hF0 = CreateFontIndirect(&f0);			// �_���t�H���g���쐬
	SelectObject(hdc, hF0);

	if (bmp_sz) {
		pbmi = (BITMAPINFO*)dib;
		pBits = (BYTE *)dib + sizeof(BITMAPINFOHEADER);
		cxDib = pbmi->bmiHeader.biWidth;
		cyDib = abs(pbmi->bmiHeader.biHeight);	// �K�������̐�Βl������
		dxPos = 20;
		if (cxDib == 640) dxPos = 20 + 320;	// VGA�̎��̕`��ʒu���w��
		dyPos = 90;
		SetDIBitsToDevice(hdc,	// Window��ɁABitmap(DIB)�f�[�^��`�悷��֐�
			dxPos, dyPos,		// Window��̕`��ʒu(x,y)
			cxDib, cyDib,		// Bitmap�̕��A����
			0, 0,				// Bitmap�̎�o�ʒu(x,y)
			0, cyDib,			// �`�悷��ŏ��̑������A��������
			pBits,				// DIB �s�N�Z���r�b�g�������|�C���^
			pbmi,				// DIB ���������|�C���^
			DIB_RGB_COLORS);	// �J���[���t���O

		dsp_cnt++;

		if (pFlag) {
			dib_disp(hdc, pBits);
		}
	}
	else
	{
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 10, 90, 330, 330);
		DeleteObject(hBrush);
	}

	sprintf(msg, "CamNo.%d", camNum + 1);
	TextOut(hdc, (WIN_WIDTH / 2) - 400, 70, msg, lstrlen(msg));//TextOut(hdc, (WIN_WIDTH / 2) - 400, 90, msg, lstrlen(msg));


															   // �ʐM��Ԃ̕\��
	Disp_lib(30, 100);

	//mode�@�ԍ�
	mode_number(hdc, 1880, 250, RNo);

	//�R���g���[�����͕\��
	disp_io(hdc, WIN_WIDTH - 500, 180, RNo);

	//���[�^�[�ԍ��\��
	disp_number(hdc, 1200, 600, RNo);

	//�Z���T�\��
	//disp_sensor(hdc, 140, 400, RNo);



	//�����擾�֘A
	GetExitCodeThread(hTh, &dwExCode);
	if (dwExCode == STILL_ACTIVE) {
		sprintf(msg, "������荞�ݒ�");
	}
	else {
		sprintf(msg, "������");
		if (stat)
			sprintf(msg, "��������");
		if (msec>10000) {
			sprintf(msg, "��荞�݊���");
		}
	}
	TextOut(hdc, 5, 500, msg, lstrlen(msg));
	sprintf(msg, "%d", stat);
	TextOut(hdc, 50, 480, msg, lstrlen(msg));
	sprintf(msg, "%d", msec);
	TextOut(hdc, 100, 480, msg, lstrlen(msg));


	//�^�C�}��������
	SetTextColor(hdc, RGB(0, 0, 255));
	logfont.lfHeight = 18;
	hFont = CreateFontIndirect(&logfont);   // �_���t�H���g���쐬
	SelectObject(hdc, hFont);


	if (tFlag == true) {
		Count = GetTickCount();
		if ((tSet - (Count - dwStart)) <= 60000) {
			SetTextColor(hdc, RGB(255, 0, 0));
		}


		//	wsprintf(msg,"%02d",(Count-dwStart)/1000);//�N�����Ă���̕b
		wsprintf(msg, "�c�莞�ԁ@ %d:%02d", (tSet - (Count - dwStart)) / 1000 / 60, (tSet - (Count - dwStart)) / 1000 % 60);
	}
	else if (tSet - (Count - dwStart)<1000) {
		wsprintf(msg, "      �I��        ");
	}
	else {
		wsprintf(msg, "�c�莞�ԁ@  -:-- ");

	}
	DrawText(hdc, msg, -1, &rec, DT_BOTTOM);
	//DeleteObject(hFont);   // �쐬�����_���t�H���g���폜����
	if (tSet - (Count - dwStart)<10000) {
		//	Count=dwStart;
		tFlag = false;
	}
	DeleteObject(hFont);
	hF0 = CreateFontIndirect(&f0);   // �_���t�H���g���쐬
	SelectObject(hdc, hF0);
	SetTextColor(hdc, RGB(0, 0, 0));
	time_disp(hdc);
	//�^�C�}�����܂�

	TPGM_screen();
	DeleteObject(hFont);   // �쐬�����_���t�H���g���폜����
}

//�摜�����p�\����
struct HSVcolors {
	int Hue;
	float Saturation;
	float Value;
}HSVC;


//���������n(TPSC)

static FILE* wf_stream = NULL;		// 
static wvf_riff_t  wf_riff;			// RIFF header
static wvf_chunk_t wf_chunk;		// Chunk header
static chunk_fmt_t wf_fmt_chunk;	// Format chunk data
static int wf_data_size;			// wave data size

									//thread�u����


unsigned int __stdcall wavThread(void*) {


	TPSC_open(IP, 16000, 2, 10);// �����ʐM�X�^�[�g 


	msec = 0;
	while ((stat = TPSC_get_status()) == 0) {// �����ʐM�̃��f�B�҂�
		Sleep(10);

	}

	err = wf_load("test.wav");			// �����o�̓t�@�C���̃��[�h
	TPSC_clear_soundin_data();			// �������̓f�[�^�̏�����
	while (msec < 10000) {				// �b���̉������͑҂�(�ő�10�b(�~���b))
										//msec = TPSC_get_soundin_time();		// ���݂̉������̓f�[�^�ʎ�o��
										//Sleep(100);
		int soundbyte = TPSC_wait_sndin_size(600000, 100);
		msec += TPSC_cnv_b2tm(soundbyte);
	}
	err = wf_save("sound.wav", 15000);//�b���̉������̓f�[�^��	WAVE�t�@�C���ɏ�����


	msec += 10000;

	//_endthreadex(0);


	return 0;

}


//�L�[���͏���
void WM_KeyDown(HWND hwnd, UINT wParam)
{

	switch (wParam) {

	case 'V':		// QVGA / VGA �ؑ�
		TPGM_delete();
		vga = (vga + 1) % 2;
		TPJT_set_video_inf(vga);
		TPGM_create(hwnd, vga, hBitmap);

		return;
		/*
		case VK_UP:		// ����`�����xUP
		kbps += 100;
		TPJT_set_vspeed(kbps);
		return;
		case VK_DOWN:	// ����`�����xDOWNF
		kbps -= 100;
		TPJT_set_vspeed(kbps);
		return;
		case VK_RIGHT:
		camNum = (camNum + 1) % 3;
		TPJT_chg_camera(camNum);
		return;
		case VK_LEFT:
		if (camNum == 0)camNum = 3;
		camNum = (camNum - 1) % 3;
		TPJT_chg_camera(camNum);
		return;
		*/
		/*	case 'C'://jpeg�摜�ۑ�����()
		if(save_jpeg( jpeg_data, jpeg_size, "jpeg_save.jpeg")==0)
		MessageBox( hwnd, _T("����"), _T("�ۑ����"), MB_OK);
		else
		MessageBox( hwnd, _T("���s"), _T("�ۑ����"), MB_OK);
		return;
		*/

	case VK_ESCAPE:
	case VK_F12:
		DestroyWindow(hwnd);		// �E�C���h�E�����
		return;

		//�摜�L���v�`������
	case 'X':
		dsp_Flag = true;

		return;

		//�����p�X�^�[�g����
	case 'S':
		if (MessageBox(hwnd, _T("wav�擾���܂�"), _T("�m�F"), MB_OKCANCEL) == IDCANCEL) {
			return;
		}

		hTh = (HANDLE)_beginthreadex(NULL, 0, wavThread, NULL, 0, thID);
		return;

	case 'Z'://�^�C�}�X�^�[�g����
		if (MessageBox(hwnd, _T("�J�E���g�_�E�����܂�"), _T("�m�F"), MB_OKCANCEL) == IDCANCEL) {
			return;
		}
		tFlag = true;
		dwStart = GetTickCount();
		return;
	}

}


//�^�C�}�֘A
void time_disp(HDC hdc)
{
	static int line, div = 100;
	static int xd, lx;
	char msg[10];
	RECT bmp = { 0, 4, 100, 104 };
	static int i, j = 0;
	int x1 = 5 * div / 10,
		x2 = 81,//*div/10,
		x3 = 10 * div / 10;
	HBRUSH hBrush;

	//��ʉ����̃o�[	�_�T���̂ŏ����Ȃ����Ă����Ɗ�����
	xd = tSet / div;
	line = WIN_WIDTH / div;
	lx = (WIN_WIDTH - (line*div)) / 2;

	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, lx + line * 0, WIN_HEIGHT - 40, lx + line * x3, WIN_HEIGHT - 2);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, lx + line * 0, WIN_HEIGHT - 20, lx + line * x1, WIN_HEIGHT - 2);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, lx + line * x1, WIN_HEIGHT - 20, lx + line * x2, WIN_HEIGHT - 2);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, lx + line * x2, WIN_HEIGHT - 20, lx + line * x3, WIN_HEIGHT - 2);
	DeleteObject(hBrush);



	if (tFlag) {
		j = (Count - dwStart) / xd;

		hBrush = CreateSolidBrush(RGB(52, 38, 89));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, lx, WIN_HEIGHT - 40, lx + j * line, WIN_HEIGHT - 20);
		DeleteObject(hBrush);

	}
	for (i = 0; i<div; i++) {
		MoveToEx(hdc, lx + i * line, WIN_HEIGHT - 40, NULL);
		if (i == 50) {
			MoveToEx(hdc, lx + i * line, WIN_HEIGHT - 50, NULL);
			wsprintf(msg, "4:00");
			TextOut(hdc, lx + i * line - 10, WIN_HEIGHT - 2, msg, lstrlen(msg));
		}
		if (i == 81) {
			MoveToEx(hdc, lx + i * line, WIN_HEIGHT - 50, NULL);
			wsprintf(msg, "1:30");
			TextOut(hdc, lx + i * line - 10, WIN_HEIGHT - 2, msg, lstrlen(msg));
		}
		LineTo(hdc, lx + i * line, WIN_HEIGHT - 2);
	}

	if (tSet - (Count - dwStart)<1000) {
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, lx, WIN_HEIGHT - 40, lx + div * line, WIN_HEIGHT - 20);
		DeleteObject(hBrush);
	}

	TPGM_releaseDC();

	//�r�b�g�}�b�v�\���p
	if ((line*j + line / 2 - 50)<0) {
		if (hBitmap) TPGM_sprite(&bmp, lx, WIN_HEIGHT - 130);		//.bmp
	}
	else if (2 * lx + line * j + line / 2 + 50 >= WIN_WIDTH) {
		if (hBitmap) TPGM_sprite(&bmp, WIN_WIDTH - (lx + 100), WIN_HEIGHT - 130);		//.bmp
	}
	else {
		if (hBitmap) TPGM_sprite(&bmp, lx + line * j + line / 2 - 50, WIN_HEIGHT - 130);		//.bmp
	}
}

//�E�B���h�E�̍쐬
HWND Create(HINSTANCE hInstance)
{
	static TCHAR szAppName[] = TEXT("TPIP3");


	WNDCLASS     wndclass;
	ZeroMemory(&wndclass, sizeof(wndclass));
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// �E�B���h�E�N���X��o�^����
	if (RegisterClass(&wndclass) == 0) { return NULL; }

	hwnd = CreateWindow(szAppName,					// �E�B���h�E�N���X��
		TEXT(TITLETEXT),							// �E�B���h�E�o�[�̕�����
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,		// �E�B���h�E�X�^�C��
		150,										// �E�B���h�E�̕\���ʒu(x���W)
		0,											// �E�B���h�E�̕\���ʒu(y���W)
		1920 * 2,// WIN_WIDTH,						// �E�B���h�E��(x��)
		WIN_HEIGHT + 40,							// �E�B���h�E��(y��)
		NULL,										// �e�E�B���h�E�̃n���h��
		NULL,										// ���j���[�n���h��
		hInstance,									// �C���X�^���X�n���h��
		NULL);										// ���̑��쐬�f�[�^

	return hwnd;
}


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 50, NULL);	// 50ms���Ƀ^�C�}�[�E�C�x���g�𔭐�
		return 0;

	case WM_TIMER:								// �^�C�}�[�E�C�x���g����
		TPJT_get_sens(&inp_dt, sizeof(inp_dt));	// �Z���T�[���̓f�[�^�捞
		TPJT_set_ctrl(&out_dt, sizeof(out_dt));	// ����o�̓f�[�^�Z�b�g
		TPGC_get(&ps2);							//�Q�[���R���g���[���̏�Ԏ擾
		ctrl_PWM();								// ���䏈��

		InvalidateRect(hwnd, NULL, FALSE);		//�\���̍X�V
		return 0;

	case WM_KEYDOWN:							// �L�[�������ꂽ
		hdc = TPGM_getDC();
		WM_KeyDown(hwnd, wParam);
		TPGM_releaseDC();
		return 0;

	case WM_PAINT:

		BeginPaint(hwnd, &ps);
		WM_Paint(hwnd);
		ValidateRect(hwnd, NULL);

		EndPaint(hwnd, &ps);
		return 0;


		/*	case WM_LBUTTONDOWN:						// �z�u�p�֐�

		// �}�E�X�̉E�{�^���������ꂽ�Ƃ��}�E�X�J�[�\���̈ʒu�i�N���C�A���g���W�n�j�����o��
		pFlag=false;
		pos.x = LOWORD( lParam );
		pos.y = HIWORD( lParam );
		if(!(pos.x>=10&&pos.x<330)||!(pos.y>=90&&pos.y<330)){
		pos.x=0;
		pos.y=0;
		}

		//		_stprintf( str, _T("X:%d  Y:%d"), pos.x, pos.y );
		//		MessageBox( hwnd, str, _T("���W���"), MB_OK );
		return 0;


		case WM_LBUTTONUP:
		epos.x= LOWORD(lParam);
		epos.y= HIWORD(lParam);
		if(!(epos.x>=10&&epos.x<330)||!(epos.y>=90&&epos.y<330)){
		epos.x=0;
		epos.y=0;
		}
		if(pos.x+pos.y!=0&&epos.x+epos.y!=0)
		pFlag=true;
		tesF=true;


		return 0;
		*/
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		TPJT_close();	// 
		TPGC_close();	// �Q�[���E�R���g���[�����͏I������
		if (hBitmap) DeleteObject(hBitmap);
		TPGM_delete();	// �T�[�t�F�X�j��
		TPSC_close();		// �����ʐM�I��

		PostQuitMessage(0);
		delete buffer;

		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
