//******************************************************************************
//	�t�@�C����	�F	GameOver.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/28
//	�@�\		�F	�V�X�e���Ǘ�
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"GameOver.h"
#include	"GameOverSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
//#include	"TitleSelect.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace gameover;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_System::C_System(const char* fileName) : C_Base(fileName)
 {
	//SetPlay_Se(SE_GAMEOVER);

	next = 0;

	mFadeF = false;
	GetNowSelectF = false;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());

	OutputDebugString("constructor:C_System\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_System::~C_System()
{

	OutputDebugString("destructor:C_System\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_System::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_System::Control()
{
	if(!GetNowSelectF && CheckPush(KEY_SPACE) && !mFadeF){
		mFadeF = true;
	//	SetPlay_Se(SE_SELECT2);
		next = NEXT_TITLE;
	}
	if(!GetNowSelectF && CheckPush(KEY_ENTER) && !mFadeF){
		mFadeF = true;
	//	SetPlay_Se(SE_SELECT2);
		next = NEXT_RETRY;
	}

	if(mFadeF){
		if(IsFade(C_Base::FADE_IN)){
	//		SetStop_Se(SE_GAMEOVER);
			if(next == NEXT_TITLE)
			C_BaseScreen::SetScreenNo(C_BaseScreen::TITLE_SCREEN);
			if(next == NEXT_RETRY)
			C_BaseScreen::SetScreenNo(C_BaseScreen::STAGE_SCREEN);
				
		}	
	}
	return false;
}