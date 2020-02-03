//******************************************************************************
//	ファイル名	：	GameOver.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/28
//	機能		：	システム管理
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"GameOver.h"
#include	"GameOverSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
//#include	"TitleSelect.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace gameover;
//------------------------------------------------------------
//	コンストラクタ
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
//	デストラクタ
//------------------------------------------------------------
C_System::~C_System()
{

	OutputDebugString("destructor:C_System\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_System::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
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