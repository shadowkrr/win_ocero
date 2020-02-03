///////////////////////////////////////////////////////////////////
//
//	�Q�[���v���O�����V
//	�@�@�@�@�@���Ɨp���C�u����
//		 �@�@�@�@�@�@Gp3Lib
//								2011.2.6  Created by K.Koyanagi
//
//-----------------------------------------------------------------
//#define	RELEASE_PROG
//�@�f�o�b�O�֐�������
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
//�O���ϐ�
//****************************************************************/
extern HWND hWnd;						//�E�C���h�E�n���h��
extern const char USERNAME[256];		//�^�C�g����
extern const int 	WINW;				//WINDOW��
extern const int	WINH;				//WINDOW����
extern BOOL			WindowMode;			//�E�C���h�E���[�h
extern int			PLAYER_NUM;			//�@�v���C���[�̐�


//****************************************************************/
//�萔
//****************************************************************/
//�L�[�`�F�b�N
#define		FREE_KEY	0
#define		PUSH_KEY	1
#define		HOLD_KEY	2
#define		PULL_KEY	3

//////////////////////////////////////////
//�g�p�ϐ�
enum{JOY1,JOY2};
enum{JOY_DOWN = 0x1000,JOY_LEFT = 0x1001,JOY_UP = 0x1002,JOY_RIGHT = 0x1004};

//�L�[�ɖ��O������
enum CODE_KEY
{
	KEY_NEUTRAL = -1,	//�j���[�g�������
	KEY_DOWN,			//�{�^��0
	KEY_LEFT,			//�{�^��1
	KEY_UP,				//�{�^��2
	//KEY_RIGHT,			//�{�^��3
	//KEY_SHIFT,		
	KEY_R_MOUSE,
	KEY_L_MOUSE,
	KEY_SPACE,		
	KEY_ENTER,			
	KEY_DEBUG1,			//�f�o�b�O�p
	KEY_DEBUG2,			//�f�o�b�O�p
	KEY_MAX				//�ő吔
};


//�L�[���\���́i�L�[�R���t�B�O�����₷���Ȃ�܂��j
struct KEYINFO
{
	int code;	//���@�[�`�����L�[�R�[�h
	int status;	//�X�e�[�^�X
	int frameHold;	//������Ă���t���[����
	int frameFree;	//�͂Ȃ���Ă���t���[����
};

//-------------------------------------------------------------//
//��{�֐��֌W
//#define RELEASE_PROG
//�ƋL�q���邱�ƂŃf�o�b�N�p�̊֐��𖳌�������B
//�eR�t���̊֐��̓V�X�e���p�̂���
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


//�t���[�g�L���X�g
#define		F(X)		static_cast<float>(X)
//-------------------------------------------------------------//
//�ėp�}�N��
#define		END			Sys_QuitAPI()		//�v���O������~

//�������`�� ���Z�����@���Z�����@�����Z����
enum{DEC,ADD,HALFADD};
//�_�C�A���O�p
enum DLG_ERROR
{
	DLG_ERROR_INVALIDBUFFER,	//get�̃|�C���^��NULL�ɂȂ��Ă���
	DLG_ERROR_CANCEL,			//�L�����Z���������ꂽ
	DLG_OK
};

//�t���[���v�Z
extern float		FrameTime;

//�O�p�֐��֌W
#define		PAI		3.14159265358979323846
#define ROUND_X(Angle,Length,Center)    static_cast<float>(cos((Angle)*PAI*2/360)*(Length)+(Center))
#define ROUND_Y(Angle,Length,Center)	static_cast<float>(-sin((Angle)*PAI*2/360)*(Length)+(Center))

//****************************************************************/
//�O���֐��錾
//****************************************************************/
//�V�X�e���֘A�֐� - �������p
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


//���w�֘A
extern BOOL			RectCheck(const RECT *rt1,const RECT *rt2);
extern float		Calc_XYToRad(int x,int y);		//���WXY�ւ̊p�x�����߂�
extern float		Calc_XYToSinCos(int x,int y,float *addx,float *addy);	//���WXY�ւ̈ړ���XY�����߂�
extern float		Calc_RadToDegree(float rad);		//���W�A������p�x��
extern float		Calc_DegreeToRad(float degree);		//�p�x���烉�W�A����

//�t�H���g�֘A�֐�
extern	int			Draw_FontDrawText(int x,int y,LPCSTR string,D3DCOLOR color,int Num,float z = 0.5);	//�t�H���g�`��
extern	int			Draw_FontDrawText(RECT rt,LPCSTR string,D3DCOLOR color,int Num,float z = 0.5);
extern	void		Draw_DeleteFont();	//�t�H���g�폜
extern	void		Draw_InitFont();	//�t�H���g������
extern	void		Draw_TextXY(int x,int y,char *s,D3DCOLOR Textcolor,int t = 0);	//�t�H���g�`��ȈՔ�
extern	void		Draw_CreateFont(int Num,int size,LPCTSTR	fontname = "�l�r �S�V�b�N");	//�t�H���g�쐬

//�}�`�`��֘A�֐�
extern	void		Draw_Box(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,D3DCOLOR Color2,int Size,int Nakanuri,float z = 0.5);	//��`�̕`��
extern	void		Draw_Ellipse(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,D3DCOLOR Color2,int Size,int Nakanuri,float z = 0.5);	//�~�̕`��
extern	void		Draw_Line(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,int Size,float z = 0.5);	//�����̕`��
//�摜�`��֘A�֐�
extern	IMGOBJ		Draw_LoadObject(int ImgNo,char *FileName,D3DCOLOR Transparent = ARGB(255,0,0,0));		//�摜�̃��[�h
extern	IMGOBJ		Draw_LoadObject(char *FileName,D3DCOLOR Transparent = ARGB(255,0,0,0));		//�摜�̃��[�h
extern	void		Draw_Bitmap(int X1,int Y1,IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z = 0.5);	//�摜�`��ȈՔ�
extern	void		Draw_TpBitmap(int X1,int Y1,			//�摜�`��
					IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
					int		deg = 0,			//��]�p
					POINT	*pCenter = NULL,	//��]���S
					float	ScaleX = 1.0f,		//�{��
					float	ScaleY = 1.0f,		//�{��
					u_char	a = 0xff,			//�A���t�@�[�`�����l��
					u_char	r = 0xff,			//��
					u_char	g = 0xff,			//��
					u_char	b = 0xff,			//��
					float z = 0.5);				//Z�l
extern	void		Draw_BitmapDirect(int X1,int Y1,IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z = 0.5);	//�摜�`��ȈՔ�
extern	void		Draw_TpBitmapDirect(int X1,int Y1,			//�摜�`��
					IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
					int		deg = 0,			//��]�p
					POINT	*pCenter = NULL,	//��]���S
					float	ScaleX = 1.0f,		//�{��
					float	ScaleY = 1.0f,		//�{��
					u_char	a = 0xff,			//�A���t�@�[�`�����l��
					u_char	r = 0xff,			//��
					u_char	g = 0xff,			//��
					u_char	b = 0xff,			//��
					float z = 0.5);				//Z�l

extern	void		Draw_SetRenderMode(int Mode);		//�������[�h�ύX�J�n
extern	void		Draw_EndRenderMode();				//�������[�h�ύX�I��
extern	void		Draw_DeleteObject(IMGOBJ obj);		//���[�h�����摜�̍폜
extern	void		Draw_DeleteObject(IMGOBJ	obj);	//���[�h�����摜�̍폜
extern	D3DSURFACE_DESC Draw_GetImageSize(IMGOBJ	obj);	//�摜�T�C�Y�̎擾

//�f�o�b�O�֘A�֐�
extern	void		Dbg_BoxToMessage(char* str,...);		//���b�Z�[�W�{�b�N�X�Ńf�o�b�O���b�Z�[�W
extern	void		Dbg_TilteToMessage(char* str,...);		//�^�C�g���o�[�Ƀf�o�b�O���b�Z�[�W
extern	void		Dbg_FileOut(char* str,...);				//�e�L�X�g�t�@�C���Ƀf�o�b�O���b�Z�[�W
extern	void		Dbg_FileDel();							//�f�o�b�O�p�̃e�L�X�g�t�@�C���폜
extern	void		Dbg_StringOutXY(int x,int y,char* str,...);		//�w��ʒu�Ƀf�o�b�O���b�Z�[�W
extern	DLG_ERROR	Dbg_InputDialog(char* get, int size, const char* title, const char* label);		//���̓_�C�A���O�\��
extern	int			Dbg_YesNo(LPCSTR str,...);			//YESNO�̃_�C�A���O�{�b�N�X�\��

//�L�[�`�F�b�N�֐�//�L�[�֘A
extern void			CheckKey();					//���t���[���K���Ăяo������
extern bool			CheckPress(int keyno, int interval = 1,int player = 0);	//������Ă��邩�`�F�b�N
extern bool			CheckFree(int keyno, int interval = 1,int player = 0);		//�͂Ȃ���Ă��邩�`�F�b�N
extern bool			CheckPush(int keyno,int player = 0);		//�����������`�F�b�N
extern bool			CheckPull(int keyno,int player = 0);		//�͂Ȃ��������`�F�b�N
extern int			GetKeyStatus(int keyno,int player = 0);	//�L�[�̏�Ԃ��`�F�b�N
extern int			GetKeyFrameHold(int keyno,int player = 0);	//������Ă���t���[�������擾
extern int			GetKeyFrameFree(int keyno,int player = 0);	//�͂Ȃ���Ă���t���[�������擾
extern void			InitKeyInfo(int player = 0);				//�X�e�[�^�X�ƃt���[����������
extern void			Key_GetKey(unsigned int Code,int* Flag);
//�}�E�X�֘A
extern POINT		GetMousePotision();				//�}�E�X�J�[�\���̍��W���擾
extern void			GetMousePotision(POINT* pt_ptr);	//�}�E�X�J�[�\���̍��W���擾

//�X���b�h���� - �������p
extern void			Sys_CreateLoadThread();
extern void			Sys_LoadThread(HWND hWnd);
extern void			OnLoading();
extern void			DrawLoading();

//BGM�Đ��֐�
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

//SE�Đ��֐�
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




