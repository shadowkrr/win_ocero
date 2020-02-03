//**********************************************************//
//			Main.cpp�@
//			�v���O�����͂�������n�܂�h������B
//**********************************************************//
//��{�V�X�e�����C�u����
#include	"utility/Gp3Lib.h"

//�錾�A��`���̃C���N���[�h
//���C�u�����̃C���N���[�h�͂��̃t�@�C���ɍs���Ă���B
#include "define.h"
#include "Base.h"
#include "BaseScreen.h"

//------------------------------------------------------------
//�Q�[�����ŕK�v�ȕϐ��@
//TITLENAME�@WINW�@WINH�͕K�v�ɉ����ĕύX���邱��
const char USERNAME[256] =	"OH! �Z���I�I�I�I�I�I�I������������������";
const int	WINW	=	640;
const int	WINH	=	480;
enum		{FULL,WIN};
BOOL WindowMode = WIN;			

//------------------------------------------------------------
//�@�v���C���[�̐�
//------------------------------------------------------------
int			PLAYER_NUM = 1;

//*************************************************************************
//�@�V���O���g�����C���N���X(�������������g��Ȃ��̂Ńw�b�_�ɂ͒�`���Ȃ�)
//*************************************************************************
class C_Main
{
	C_Main();
	C_Main(const C_Main&){}
	C_Main operator=(const C_Main&){}

	C_BaseScreen* mScreen;
	C_BaseScreen::SCREEN_NAME mIsScreenNo;

	void SetScreenNo(C_BaseScreen::SCREEN_NAME name);
public:
	~C_Main();

	static C_Main& GetInstance(){
		static 	C_Main main;
		return main;
	}

	C_BaseScreen* GetScreen()const{return mScreen;}

	void IsChangeScreen();
};
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Main::C_Main() : mScreen(NULL)
{
	//�ŏ��̓��S����n�߂�
	/*GetInstance().mScreen = new opening::C_Opening;
	SetScreenNo(C_BaseScreen::OPENING_SCREEN);*/
	GetInstance().mScreen = new stage::C_Stage;	
	SetScreenNo(C_BaseScreen::STAGE_SCREEN);
	/*GetInstance().mScreen = new logo::C_Logo;	
	SetScreenNo(C_BaseScreen::LOGO_SCREEN);*/

	OutputDebugString("constructor:C_Main\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Main::~C_Main()
{
	SAFE_DELETE(C_Main::GetInstance().mScreen);
	OutputDebugString("destructor:C_Main\n");
	//_CrtDumpMemoryLeaks();										//���̊֐����Ă񂾎��_�ŊJ������Ă��Ȃ��������̏���\������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//�v���O�������I�������Ƃ��Ɏ����I��_CrtDumpMemoryLeaks()���Ă�ł����
}

//------------------------------------------------------------
//	�X�N���[���ύX�̔���
//------------------------------------------------------------
void C_Main::IsChangeScreen()
{
	if(GetInstance().mScreen->GetScreenNo() != mIsScreenNo){

		for(int i = 0; i < C_BaseScreen::MAX_SCREEN; ++i)
		{
			if(GetInstance().mScreen->GetScreenNo() == C_BaseScreen::SCREEN_NAME(i)){

				SAFE_DELETE(C_Main::GetInstance().mScreen);

				if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::LOGO_SCREEN){
					GetInstance().mScreen = new logo::C_Logo;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::CLEAR_SCREEN){
					GetInstance().mScreen = new clear::C_Clear;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::STAGE_SCREEN){
					GetInstance().mScreen = new stage::C_Stage;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::TITLE_SCREEN){
					GetInstance().mScreen = new title::C_Title;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::GAMEOVER_SCREEN){
					GetInstance().mScreen = new gameover::C_GameOver;
				}
				SetScreenNo(C_BaseScreen::SCREEN_NAME(i));
				return;
			}
		}
	}
}

//------------------------------------------------------------
//	�X�N���[���i���o�[�̊i�[
//------------------------------------------------------------
void C_Main::SetScreenNo(C_BaseScreen::SCREEN_NAME name)
{
	GetInstance().mScreen->SetScreenNo(name);		
	GetInstance().mIsScreenNo = name;
}

//------------------------------------------------------------
//���\�[�X�f�[�^�錾
//------------------------------------------------------------
IMGOBJ		ImgLoad;
IMGOBJ		ImgTable[IMG_MAX];
DSOUNDOBJ	SeTable[SE_MAX];
DSHOWOBJ	BgmTable[BGM_MAX];
//DMOVIEOBJ	MovieTable[MOVIE_MAX];
//------------------------------------------------------------
//�Q�[�����[�v 
//���݂�StageNo�ɉ����ď������h������B
//------------------------------------------------------------
void GameLoop()
{
	Sys_Clear();
	C_Main::GetInstance().GetScreen()->Draw();
//	Draw_ShowFPS();
  	Sys_Refresh();
	C_Main::GetInstance().GetScreen()->Control();
	C_Main::GetInstance().IsChangeScreen();
	CheckKey();
}

//------------------------------------------------------------
//�ʃX���b�h�ɂ��摜�̃��[�f�B���O����
//�摜�̃��[�h�ȂǋN�����Ɉ�񂾂��s�������͂����ōs��
//------------------------------------------------------------
void OnLoading()
{
	ImgLoad = Draw_LoadObject("res/gra/Loading.png");
	Load_Bgm();
	Load_Se();
	//Load_Movie();
	//MNo = DShowMovie_Load("res/movie/Wildlife.wmv");
	//Load_Img();	
	Sleep(1000);
}
//------------------------------------------------------------
//�v���O�����S�̂̏�����
//�t�H���g�̍쐬�A���[�f�B���O��ʂŎg�p����摜�̓ǂݍ��݂Ȃ�
//�N�����Ɉ�񂾂��s�������͂����ōs��
//------------------------------------------------------------
void OnCreate()
{
	srand( ( unsigned int )time( NULL ) );
	Create_Font();
	
}

//------------------------------------------------------------
//�v���O�����S�̂̌�n��
//�I�����Ɉ�񂾂��s�������͂����ōs��
//------------------------------------------------------------
void OnDestroy()
{
	//Delete_Sound();
	Draw_DeleteObject(ImgLoad);
	//Delete_Img();
}

//----------------------------------------------------
//���C�����[�v�O�ɌĂяo����郍�[�h��p�̃X�N���[��
// Loading ... ���\�������X�N���[��
//----------------------------------------------------
void DrawLoading()
{
	static float cnt;
	char msg[] = "Loading...";
	char temp[20] = "";
	Sys_Clear();
	strncpy(temp, msg, 7+static_cast<int>(cnt));
	Draw_Bitmap( 0, 0, ImgLoad,0 ,0 ,WINW, WINH );
	Draw_TextXY(WINW-200,WINH-50,temp,ARGB(255,255,0,0),1);
	Sys_Refresh();
	cnt += FrameTime * 2;
	if(cnt >= 4){
		cnt = 0;
	}
}

//**********************************************************//
//WINDOW�@���C��
//**********************************************************//
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
	return DoWindow(hInstance,hPreInst,lpszCmdLine,nCmdShow);
}
