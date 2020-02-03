///////////////////////////////////////////////////////////////////
//
//	ゲームプログラムⅢ
//	　　　　　授業用ライブラリ
//		 　　　　　　Gp3Lib
//								2011.2.6  Created by K.Koyanagi
//
//-----------------------------------------------------------------
//#define	RELEASE_PROG
//　デバッグ関数無効化
///////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_DEPRECATE				1
#pragma warning(disable : 4995)	
#pragma warning(disable : 4996) 

#pragma once
#pragma			comment(lib,"d3dxof.lib")
#pragma			comment(lib,"dxguid.lib")
#pragma			comment(lib,"d3d9.lib")
#pragma			comment(lib,"d3dx9.lib")
#pragma			comment(lib,"winmm")
#pragma			comment(lib,"dinput8.lib")
#pragma			comment(lib,"strmiids")
#pragma			comment(lib,"dsound.lib")
#pragma			comment(lib,"dxguid.lib")

#include	<d3dx9.h>
#include	<dxerr9.h>
#include	<dsound.h>
#include	<windows.h>
#include	<mmsystem.h>
#include	<stdio.h>
#include	<time.h>
#include	<dshow.h>
#include	<crtdbg.h>

#define		RELEASE(X)		{if(X!=NULL)	{X->Release();X = NULL;}}
#define		SAFE_DELETE(p)			{if(p!=NULL) {delete p; (p)=NULL;}}
#define		SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p);   (p)=NULL;}}


typedef LPDIRECT3DTEXTURE9	D3DTEX9;
#define ARGB				D3DCOLOR_ARGB

typedef int		IMGOBJ;
typedef int		DSOUNDOBJ;
typedef int		DSHOWOBJ;


//****************************************************************/
//外部変数
//****************************************************************/
extern HWND hWnd;						//ウインドウハンドル
extern const char USERNAME[256];		//タイトル名
extern const int 	WINW;				//WINDOW幅
extern const int	WINH;				//WINDOW高さ
extern BOOL			WindowMode;			//ウインドウモード
extern int			PLAYER_NUM;			//　プレイヤーの数


//****************************************************************/
//定数
//****************************************************************/
//キーチェック
#define		FREE_KEY	0
#define		PUSH_KEY	1
#define		HOLD_KEY	2
#define		PULL_KEY	3

//////////////////////////////////////////
//使用変数
enum{JOY1,JOY2};
enum{JOY_DOWN = 0x1000,JOY_LEFT = 0x1001,JOY_UP = 0x1002,JOY_RIGHT = 0x1004};

//キーに名前をつける
enum CODE_KEY
{
	KEY_NEUTRAL = -1,	//ニュートラル状態
	KEY_DOWN,			//ボタン0
	KEY_LEFT,			//ボタン1
	KEY_UP,				//ボタン2
	//KEY_RIGHT,			//ボタン3
	//KEY_SHIFT,		
	KEY_R_MOUSE,
	KEY_L_MOUSE,
	KEY_SPACE,		
	KEY_ENTER,			
	KEY_DEBUG1,			//デバッグ用
	KEY_DEBUG2,			//デバッグ用
	KEY_MAX				//最大数
};


//キー情報構造体（キーコンフィグがしやすくなります）
struct KEYINFO
{
	int code;	//ヴァーチャルキーコード
	int status;	//ステータス
	int frameHold;	//押されているフレーム数
	int frameFree;	//はなされているフレーム数
};

//-------------------------------------------------------------//
//基本関数関係
//#define RELEASE_PROG
//と記述することでデバック用の関数を無効化する。
//各R付きの関数はシステム用のもの
#ifndef RELEASE_PROG
	#define		TToM		Dbg_TilteToMessage
	#define		FToM		Dbg_FileOut
	#define		BToM		Dbg_BoxToMessage
	#define		SToM		Dbg_StringOutXY
	#define		DFPS		Draw_ShowFPS
	#define		YESNO		Dbg_YesNo
#else
	#define		TToM		//	
	#define		FToM		//
	#define		BToM		//
	#define		SToM		//
	#define		DFPS		//
	#define		YESNO		//
#endif

#define		TToMR		Dbg_TilteToMessage
#define		FToMR		Dbg_FileOut
#define		BToMR		Dbg_BoxToMessage
#define		SToMR		Dbg_StringOutXY
#define		DFPSR		Draw_ShowFPS
#define		YESNOR		Dbg_YesNo


//フロートキャスト
#define		F(X)		static_cast<float>(X)
//-------------------------------------------------------------//
//汎用マクロ
#define		END			Sys_QuitAPI()		//プログラム停止

//半透明描画 減算合成　加算合成　半加算合成
enum{DEC,ADD,HALFADD};
//ダイアログ用
enum DLG_ERROR
{
	DLG_ERROR_INVALIDBUFFER,	//getのポインタがNULLになっている
	DLG_ERROR_CANCEL,			//キャンセルが押された
	DLG_OK
};

//フレーム計算
extern float		FrameTime;

//三角関数関係
#define		PAI		3.14159265358979323846
#define ROUND_X(Angle,Length,Center)    static_cast<float>(cos((Angle)*PAI*2/360)*(Length)+(Center))
#define ROUND_Y(Angle,Length,Center)	static_cast<float>(-sin((Angle)*PAI*2/360)*(Length)+(Center))

//****************************************************************/
//外部関数宣言
//****************************************************************/
//システム関連関数 - 内部利用
extern	void		GameLoop();
extern	void		Sys_Clear();
extern	void		Sys_Refresh();
extern	BOOL		Sys_LoopControl(int FPS);
extern	void		OnCreate();
extern	void		OnDestroy();
extern	BOOL		DoWindow(HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow);
LRESULT	CALLBACK	WndProc(HWND,UINT,WPARAM,LPARAM);
extern	void		InitDxLib();
static 	void		InitDx();
extern	void		Draw_ShowFPS();
extern	void		GetErrorString(char* text);
extern	void		Sys_SetScreenMode();
extern  void		Sys_QuitAPI();


//数学関連
extern BOOL			RectCheck(const RECT *rt1,const RECT *rt2);
extern float		Calc_XYToRad(int x,int y);		//座標XYへの角度を求める
extern float		Calc_XYToSinCos(int x,int y,float *addx,float *addy);	//座標XYへの移動量XYを求める
extern float		Calc_RadToDegree(float rad);		//ラジアンから角度へ
extern float		Calc_DegreeToRad(float degree);		//角度からラジアンへ

//フォント関連関数
extern	int			Draw_FontDrawText(int x,int y,LPCSTR string,D3DCOLOR color,int Num,float z = 0.5);	//フォント描画
extern	int			Draw_FontDrawText(RECT rt,LPCSTR string,D3DCOLOR color,int Num,float z = 0.5);
extern	void		Draw_DeleteFont();	//フォント削除
extern	void		Draw_InitFont();	//フォント初期化
extern	void		Draw_TextXY(int x,int y,char *s,D3DCOLOR Textcolor,int t = 0);	//フォント描画簡易版
extern	void		Draw_CreateFont(int Num,int size,LPCTSTR	fontname = "ＭＳ ゴシック");	//フォント作成

//図形描画関連関数
extern	void		Draw_Box(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,D3DCOLOR Color2,int Size,int Nakanuri,float z = 0.5);	//矩形の描画
extern	void		Draw_Ellipse(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,D3DCOLOR Color2,int Size,int Nakanuri,float z = 0.5);	//円の描画
extern	void		Draw_Line(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,int Size,float z = 0.5);	//直線の描画
//画像描画関連関数
extern	IMGOBJ		Draw_LoadObject(int ImgNo,char *FileName,D3DCOLOR Transparent = ARGB(255,0,0,0));		//画像のロード
extern	IMGOBJ		Draw_LoadObject(char *FileName,D3DCOLOR Transparent = ARGB(255,0,0,0));		//画像のロード
extern	void		Draw_Bitmap(int X1,int Y1,IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z = 0.5);	//画像描画簡易版
extern	void		Draw_TpBitmap(int X1,int Y1,			//画像描画
					IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
					int		deg = 0,			//回転角
					POINT	*pCenter = NULL,	//回転中心
					float	ScaleX = 1.0f,		//倍率
					float	ScaleY = 1.0f,		//倍率
					u_char	a = 0xff,			//アルファーチャンネル
					u_char	r = 0xff,			//赤
					u_char	g = 0xff,			//緑
					u_char	b = 0xff,			//青
					float z = 0.5);				//Z値
extern	void		Draw_BitmapDirect(int X1,int Y1,IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z = 0.5);	//画像描画簡易版
extern	void		Draw_TpBitmapDirect(int X1,int Y1,			//画像描画
					IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
					int		deg = 0,			//回転角
					POINT	*pCenter = NULL,	//回転中心
					float	ScaleX = 1.0f,		//倍率
					float	ScaleY = 1.0f,		//倍率
					u_char	a = 0xff,			//アルファーチャンネル
					u_char	r = 0xff,			//赤
					u_char	g = 0xff,			//緑
					u_char	b = 0xff,			//青
					float z = 0.5);				//Z値

extern	void		Draw_SetRenderMode(int Mode);		//合成モード変更開始
extern	void		Draw_EndRenderMode();				//合成モード変更終了
extern	void		Draw_DeleteObject(IMGOBJ obj);		//ロードした画像の削除
extern	void		Draw_DeleteObject(IMGOBJ	obj);	//ロードした画像の削除
extern	D3DSURFACE_DESC Draw_GetImageSize(IMGOBJ	obj);	//画像サイズの取得

//デバッグ関連関数
extern	void		Dbg_BoxToMessage(char* str,...);		//メッセージボックスでデバッグメッセージ
extern	void		Dbg_TilteToMessage(char* str,...);		//タイトルバーにデバッグメッセージ
extern	void		Dbg_FileOut(char* str,...);				//テキストファイルにデバッグメッセージ
extern	void		Dbg_FileDel();							//デバッグ用のテキストファイル削除
extern	void		Dbg_StringOutXY(int x,int y,char* str,...);		//指定位置にデバッグメッセージ
extern	DLG_ERROR	Dbg_InputDialog(char* get, int size, const char* title, const char* label);		//入力ダイアログ表示
extern	int			Dbg_YesNo(LPCSTR str,...);			//YESNOのダイアログボックス表示

//キーチェック関数//キー関連
extern void			CheckKey();					//毎フレーム必ず呼び出すこと
extern bool			CheckPress(int keyno, int interval = 1,int player = 0);	//押されているかチェック
extern bool			CheckFree(int keyno, int interval = 1,int player = 0);		//はなされているかチェック
extern bool			CheckPush(int keyno,int player = 0);		//押したかをチェック
extern bool			CheckPull(int keyno,int player = 0);		//はなしたかをチェック
extern int			GetKeyStatus(int keyno,int player = 0);	//キーの状態をチェック
extern int			GetKeyFrameHold(int keyno,int player = 0);	//押されているフレーム数を取得
extern int			GetKeyFrameFree(int keyno,int player = 0);	//はなされているフレーム数を取得
extern void			InitKeyInfo(int player = 0);				//ステータスとフレームを初期化
extern void			Key_GetKey(unsigned int Code,int* Flag);
//マウス関連
extern POINT		GetMousePotision();				//マウスカーソルの座標を取得
extern void			GetMousePotision(POINT* pt_ptr);	//マウスカーソルの座標を取得

//スレッド処理 - 内部利用
extern void			Sys_CreateLoadThread();
extern void			Sys_LoadThread(HWND hWnd);
extern void			OnLoading();
extern void			DrawLoading();

//BGM再生関数
extern BOOL			DShow_Init();
extern void			DShow_Del();
extern DSHOWOBJ		DShow_LoadFile(char* filename);
extern DSHOWOBJ		DShow_LoadFile(int BgmNo,char* filename);
extern BOOL			DShow_Play(DSHOWOBJ index);
extern void			DShow_RateControl(DSHOWOBJ index,float rate);
extern void			DShow_EndCheck();
extern void			DShow_Stop(DSHOWOBJ index);
extern void			DShow_AllStop();
extern LONGLONG		DShow_GetCurrentPos(int index);
extern void			DShow_SetStartPos(int index);

//SE再生関数
extern void			DSound_Init();
extern BOOL			DSound_Create();
extern void			DSound_CreateSecondaryBuffer();
extern BOOL			DSound_CreatePrimaryBuffer();
extern BOOL			DSound_Del();
extern void			DSound_Play(int SoundNo);
extern void			DSound_PlayLoop(int SoundNo);
extern BOOL			DSound_Del();
extern int			DSound_LoadFile( char* file );
extern int			DSound_LoadFile(int SoundNo,char* file );
extern void			DSound_AllStop();
extern void			DSound_Stop(int SoundNo);
extern void			DSound_EndCheck();
extern void			DSound_SetFrequency(int SoundNo,int Fre);




