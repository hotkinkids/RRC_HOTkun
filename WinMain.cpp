/******************************************************
*	2015 Hotaka Hayashi Kindai Univ.
*******************************************************/

#define TITLETEXT "1号機"
#define IP "192.168.13.101"		//無線用	
//#define IP "192.168.0.200"			//有線用
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


//TPIP動画用変数
static int vga = 0;				// 0:QVGA 1:VGA
static int kbps = 4000;			// 動画伝送速度　要求値


								//描画のための基本変数
static HWND  hwnd;
static HDC hdc;
static PAINTSTRUCT ps;


//入出力用構造体
struct OUT_DT_STR out_dt		// 制御出力データ
	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
struct INP_DT_STR inp_dt;		// センサー入力データ
struct GC_STRUCT ps2;			//ゲームコントローラ入力格納


								//画面の大きさの指定
static int WIN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
static int WIN_HEIGHT = -40 + GetSystemMetrics(SM_CYSCREEN);


//システムタイマ用変数
static DWORD dwStart;		//タイマスタート時の時間を記録(msec)
static DWORD Count;			//タイマ（msec）


							//カメラ・モード用フラグ
int camNum = 0;				//モード(カメラ、操作)フラグ
int Ctrlmode = 0;
static bool tFlag = false;	//タイマ用フラグ


							//音声処理用関数
static int err;
static int sample = 16000;	// sampling rate 16000Hz指定
static int msec = 0;
static int stat;


//Thread用変数
DWORD dwExCode;
HANDLE hTh;
unsigned int *thID;


//画像用変数
static bool dsp_Flag = false;	//画像用フラグ
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


//プロトタイプ宣言
//unsigned int __stdcall wavThread(void*);					//スレッド用
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		//ウィンドウプロシージャ
HWND Create(HINSTANCE hInst);								//ウィンドウ生成

void dib_disp(int dxPos, int dyPos);								//画面描画用
int RGBToHSV(int r, int g, int b);								//画像用
COLORREF HSVToRGBColor(float hue, float saturation, float value);	//
void HueColor(int hue);											//hue値を色として出力
void time_disp(HDC hdc);
void dib_disp(HDC hdc, BYTE* pBits) {

}

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG          msg;

	//画面生成
	hwnd = Create(hInstance);

	//動画用bufferの確保
	buffer = new BYTE[buffer_sz];

	//bitmap画像用格納エリア
	hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, "hot-kun.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// サーフェス生成
	int ans = TPGM_create(hwnd, vga, hBitmap);

	//TPIP用関数の初期化
	TPGC_init(hInstance, hwnd);// ゲームコントローラ入力関数の初期化
	TPJT_init_ex(IP, hwnd, 0x03);		// リアルタイム動画伝送処理の初期化
	TPJT_set_com_req(0x03, 0);			// 制御ボード通信＋JPEG通信


										//描画更新用
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


//基本情報の表示用(座標指定)
void Disp_lib(int x, int y) {
	char msg[256];
	int com_rq, com_st;
	HBRUSH hBrush;

	com_rq = TPJT_get_com_req();					// 通信モード要求
	com_st = TPJT_get_com_stat();					// 通信モード結果
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
	sprintf(msg, "BATT残量　　   　%1d.%1d[V] ", inp_dt.batt / 100, (inp_dt.batt / 100) % 100);
	//(inp_dt.batt * 20) / 0xffff, abs((inp_dt.batt * 20 * 10) / 0xffff % 10));
	TextOut(hdc, x + 50, y + 20 + 80, msg, lstrlen(msg));		// 文字表示
	SetTextColor(hdc, RGB(0, 0, 0));
}

//ウィンドウ描画用
void WM_Paint(HWND hwnd)
{
	int ans;
	static int s_vga;
	RECT rec = { WIN_WIDTH / 2 - 160, 20, WIN_WIDTH / 2 + 160, 80 }; //タイマ用
	static LOGFONT logfont, f0;
	HFONT hFont, hF0;
	HPEN hPen;
	HBRUSH hBrush;
	char msg[256];

	//******************ディスプレイ位置****************//

	RECT bmp = { 0, 4, 100, 104 };

	RECT rect1 = { 0, 0, 640, 480 };
	RECT rect1q = { 0, 0, 320, 240 };
	RECT rect2 = { 30, 600, 30 + 320, 600 + 240 };
	RECT rect2q = { 330, 90, 100 + 320 * 4, 90 + 240 * 2.6 };

	RECT vga1 = { 0,0,320,480 };
	RECT vga2 = { 319,0,640,480 };

	RECT vga1a = { 2000,25,2000 + 720,975 };
	RECT vga2a = { 2000 + 720 + 0,25, 2000 + 720 + 720 + 0, 975 };

	RECT vga1b = { 400,90,1300,900 };   //aとbは固定




										//***************************************************//
	BYTE* pBits;
	bool err = false;
	int dxPos, dyPos;
	int cxDib, cyDib;

	ZeroMemory(&f0, sizeof(f0));			// フォント情報構造体初期化
	f0.lfCharSet = DEFAULT_CHARSET;			// システムのデフォルト文字セットを使う
	f0.lfHeight = 16;
	f0.lfQuality = ANTIALIASED_QUALITY;

	ZeroMemory(&logfont, sizeof(logfont));
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfHeight = 16;
	logfont.lfQuality = ANTIALIASED_QUALITY;

	// 上書画像用変数

	// JPEG	データの取出し	
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
	TPJT_free_jpeg_file();	// JPEG file 解放

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

	// 動画像の転送（動画　→　作業画面） (640,480)
	//TPGM_copy((WIN_WIDTH / 2) - 310, 90);//(WIN_HEIGHT/2)-240

	if (vga == 1) {
		TPGM_copy2(&vga1, &vga1a);	//左目(VRモニタ用)
		TPGM_copy2(&vga2, &vga2a);	//右目(VRモニタ用)

		TPGM_copy2(&vga1, &vga1b);	//本画面用

		TPGM_copy2(&rect1, &rect2);	//テスト用
	}
	else {	//QVGA
		TPGM_copy2(&rect1q, &rect2q);
	}

	//	if (hBitmap) TPGM_sprite(&bmp,0,0);		//.bmpこの関数はDC解放時のみしか描画してくれない


	// 上書画像の転送（上書画像　→　作業画面）
	hdc = TPGM_getDC();

	//ステータス表示
	SetBkColor(hdc, RGB(255, 255, 255));

	SetTextColor(hdc, RGB(0, 0, 0));
	logfont.lfHeight = 16;
	hFont = CreateFontIndirect(&logfont);		// 論理フォントを作成
	SelectObject(hdc, hFont);

	sprintf(msg, ROBOT);
	TextOut(hdc, 5, 20, msg, lstrlen(msg));
	DeleteObject(hFont);

	hF0 = CreateFontIndirect(&f0);			// 論理フォントを作成
	SelectObject(hdc, hF0);

	if (bmp_sz) {
		pbmi = (BITMAPINFO*)dib;
		pBits = (BYTE *)dib + sizeof(BITMAPINFOHEADER);
		cxDib = pbmi->bmiHeader.biWidth;
		cyDib = abs(pbmi->bmiHeader.biHeight);	// 必ず高さの絶対値を入れる
		dxPos = 20;
		if (cxDib == 640) dxPos = 20 + 320;	// VGAの時の描画位置を指定
		dyPos = 90;
		SetDIBitsToDevice(hdc,	// Window上に、Bitmap(DIB)データを描画する関数
			dxPos, dyPos,		// Window上の描画位置(x,y)
			cxDib, cyDib,		// Bitmapの幅、高さ
			0, 0,				// Bitmapの取出位置(x,y)
			0, cyDib,			// 描画する最初の走査線、走査線数
			pBits,				// DIB ピクセルビットを示すポインタ
			pbmi,				// DIB 情報を示すポインタ
			DIB_RGB_COLORS);	// カラー情報フラグ

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


															   // 通信状態の表示
	Disp_lib(30, 100);

	//mode　番号
	mode_number(hdc, 1880, 250, RNo);

	//コントローラ入力表示
	disp_io(hdc, WIN_WIDTH - 500, 180, RNo);

	//モーター番号表示
	disp_number(hdc, 1200, 600, RNo);

	//センサ表示
	//disp_sensor(hdc, 140, 400, RNo);



	//音声取得関連
	GetExitCodeThread(hTh, &dwExCode);
	if (dwExCode == STILL_ACTIVE) {
		sprintf(msg, "音声取り込み中");
	}
	else {
		sprintf(msg, "準備中");
		if (stat)
			sprintf(msg, "準備完了");
		if (msec>10000) {
			sprintf(msg, "取り込み完了");
		}
	}
	TextOut(hdc, 5, 500, msg, lstrlen(msg));
	sprintf(msg, "%d", stat);
	TextOut(hdc, 50, 480, msg, lstrlen(msg));
	sprintf(msg, "%d", msec);
	TextOut(hdc, 100, 480, msg, lstrlen(msg));


	//タイマここから
	SetTextColor(hdc, RGB(0, 0, 255));
	logfont.lfHeight = 18;
	hFont = CreateFontIndirect(&logfont);   // 論理フォントを作成
	SelectObject(hdc, hFont);


	if (tFlag == true) {
		Count = GetTickCount();
		if ((tSet - (Count - dwStart)) <= 60000) {
			SetTextColor(hdc, RGB(255, 0, 0));
		}


		//	wsprintf(msg,"%02d",(Count-dwStart)/1000);//起動してからの秒
		wsprintf(msg, "残り時間　 %d:%02d", (tSet - (Count - dwStart)) / 1000 / 60, (tSet - (Count - dwStart)) / 1000 % 60);
	}
	else if (tSet - (Count - dwStart)<1000) {
		wsprintf(msg, "      終了        ");
	}
	else {
		wsprintf(msg, "残り時間　  -:-- ");

	}
	DrawText(hdc, msg, -1, &rec, DT_BOTTOM);
	//DeleteObject(hFont);   // 作成した論理フォントを削除する
	if (tSet - (Count - dwStart)<10000) {
		//	Count=dwStart;
		tFlag = false;
	}
	DeleteObject(hFont);
	hF0 = CreateFontIndirect(&f0);   // 論理フォントを作成
	SelectObject(hdc, hF0);
	SetTextColor(hdc, RGB(0, 0, 0));
	time_disp(hdc);
	//タイマここまで

	TPGM_screen();
	DeleteObject(hFont);   // 作成した論理フォントを削除する
}

//画像処理用構造体
struct HSVcolors {
	int Hue;
	float Saturation;
	float Value;
}HSVC;


//音声処理系(TPSC)

static FILE* wf_stream = NULL;		// 
static wvf_riff_t  wf_riff;			// RIFF header
static wvf_chunk_t wf_chunk;		// Chunk header
static chunk_fmt_t wf_fmt_chunk;	// Format chunk data
static int wf_data_size;			// wave data size

									//thread置き場


unsigned int __stdcall wavThread(void*) {


	TPSC_open(IP, 16000, 2, 10);// 音声通信スタート 


	msec = 0;
	while ((stat = TPSC_get_status()) == 0) {// 音声通信のレディ待ち
		Sleep(10);

	}

	err = wf_load("test.wav");			// 音声出力ファイルのロード
	TPSC_clear_soundin_data();			// 音声入力データの初期化
	while (msec < 10000) {				// 秒分の音声入力待ち(最大10秒(ミリ秒))
										//msec = TPSC_get_soundin_time();		// 現在の音声入力データ量取出し
										//Sleep(100);
		int soundbyte = TPSC_wait_sndin_size(600000, 100);
		msec += TPSC_cnv_b2tm(soundbyte);
	}
	err = wf_save("sound.wav", 15000);//秒分の音声入力データを	WAVEファイルに書込み


	msec += 10000;

	//_endthreadex(0);


	return 0;

}


//キー入力処理
void WM_KeyDown(HWND hwnd, UINT wParam)
{

	switch (wParam) {

	case 'V':		// QVGA / VGA 切替
		TPGM_delete();
		vga = (vga + 1) % 2;
		TPJT_set_video_inf(vga);
		TPGM_create(hwnd, vga, hBitmap);

		return;
		/*
		case VK_UP:		// 動画伝送速度UP
		kbps += 100;
		TPJT_set_vspeed(kbps);
		return;
		case VK_DOWN:	// 動画伝送速度DOWNF
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
		/*	case 'C'://jpeg画像保存処理()
		if(save_jpeg( jpeg_data, jpeg_size, "jpeg_save.jpeg")==0)
		MessageBox( hwnd, _T("成功"), _T("保存状態"), MB_OK);
		else
		MessageBox( hwnd, _T("失敗"), _T("保存状態"), MB_OK);
		return;
		*/

	case VK_ESCAPE:
	case VK_F12:
		DestroyWindow(hwnd);		// ウインドウを閉じる
		return;

		//画像キャプチャ処理
	case 'X':
		dsp_Flag = true;

		return;

		//音声用スタート処理
	case 'S':
		if (MessageBox(hwnd, _T("wav取得します"), _T("確認"), MB_OKCANCEL) == IDCANCEL) {
			return;
		}

		hTh = (HANDLE)_beginthreadex(NULL, 0, wavThread, NULL, 0, thID);
		return;

	case 'Z'://タイマスタート処理
		if (MessageBox(hwnd, _T("カウントダウンします"), _T("確認"), MB_OKCANCEL) == IDCANCEL) {
			return;
		}
		tFlag = true;
		dwStart = GetTickCount();
		return;
	}

}


//タイマ関連
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

	//画面下部のバー	ダサいので書きなおしてくれると嬉しい
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

	//ビットマップ表示用
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

//ウィンドウの作成
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

	// ウィンドウクラスを登録する
	if (RegisterClass(&wndclass) == 0) { return NULL; }

	hwnd = CreateWindow(szAppName,					// ウィンドウクラス名
		TEXT(TITLETEXT),							// ウィンドウバーの文字列
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,		// ウィンドウスタイル
		150,										// ウィンドウの表示位置(x座標)
		0,											// ウィンドウの表示位置(y座標)
		1920 * 2,// WIN_WIDTH,						// ウィンドウ幅(x長)
		WIN_HEIGHT + 40,							// ウィンドウ高(y長)
		NULL,										// 親ウィンドウのハンドル
		NULL,										// メニューハンドル
		hInstance,									// インスタンスハンドル
		NULL);										// その他作成データ

	return hwnd;
}


//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 50, NULL);	// 50ms毎にタイマー・イベントを発生
		return 0;

	case WM_TIMER:								// タイマー・イベント処理
		TPJT_get_sens(&inp_dt, sizeof(inp_dt));	// センサー入力データ取込
		TPJT_set_ctrl(&out_dt, sizeof(out_dt));	// 制御出力データセット
		TPGC_get(&ps2);							//ゲームコントローラの状態取得
		ctrl_PWM();								// 制御処理

		InvalidateRect(hwnd, NULL, FALSE);		//表示の更新
		return 0;

	case WM_KEYDOWN:							// キーが押された
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


		/*	case WM_LBUTTONDOWN:						// 配置用関数

		// マウスの右ボタンが押されたときマウスカーソルの位置（クライアント座標系）を取り出す
		pFlag=false;
		pos.x = LOWORD( lParam );
		pos.y = HIWORD( lParam );
		if(!(pos.x>=10&&pos.x<330)||!(pos.y>=90&&pos.y<330)){
		pos.x=0;
		pos.y=0;
		}

		//		_stprintf( str, _T("X:%d  Y:%d"), pos.x, pos.y );
		//		MessageBox( hwnd, str, _T("座標情報"), MB_OK );
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
		TPGC_close();	// ゲーム・コントローラ入力終了処理
		if (hBitmap) DeleteObject(hBitmap);
		TPGM_delete();	// サーフェス破棄
		TPSC_close();		// 音声通信終了

		PostQuitMessage(0);
		delete buffer;

		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
