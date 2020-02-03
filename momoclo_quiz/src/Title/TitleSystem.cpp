//******************************************************************************
//	ファイル名	：	TitleSystem.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/15
//	機能		：	システム管理
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Title.h"
#include	"TitleSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"TitleSelect.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace title;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_System::C_System(const char* fileName) : C_Base(fileName)
 {
	//SetPlay_Bgm(BGM_TITLE);
	mFadeF = false;
	GetNowSelectF = false;

	//LoadTimeCnt("res/txt/timecnt.txt");
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
	const C_Select* sel = GET_OBJECT(const C_Select* ,C_Title::OBJ_SELECT);

	//if(!GetNowSelectF && CheckPush(KEY_SPACE) && !mFadeF)
	{
		mFadeF = true;
		//SetPlay_Se(SE_SELECT2);
	}

	if(mFadeF){
		if(IsFade(C_Base::FADE_IN)){

			SetStop_Bgm(BGM_TITLE);

			switch(sel->GetSelectStage())
			{
				case sel->SEL_START:	C_BaseScreen::SetScreenNo(C_BaseScreen::STAGE_SCREEN);		break;
				case sel->SEL_RANKING:	Sys_QuitAPI();//C_BaseScreen::SetScreenNo(C_BaseScreen::RANKING_SCREEN);		break;
				case sel->SEL_END:		Sys_QuitAPI();													break;
			}
		}
	}	
	return false;
}
//------------------------------------------------------------
//	タイムカウントファイルから読み込み
//  タイムが0だとコンテニューできないようにする
//------------------------------------------------------------
bool C_System::LoadTimeCnt(char* filename)
{
	FILE* fp;

	char Buffer[256];

	//fopen_s(&fp, filename, "r");
	fp = fopen(filename, "r");

	//読み込み失敗
	if(fp == NULL)
	{
		MessageBox(hWnd, "読み込み失敗", __FUNCTION__, MB_OK);
		return false;
	}
	//読み込み成功
	else
	{

			fgets(Buffer,256,fp);							//一行づつ読みこみ
		/*	if(Buffer[0] == 10){
				continue;
			}*/
			sscanf(Buffer,"%f",					//データを振り分ける
				&timecnt);

		return true;
	}
	fclose(fp);	//ファイルを閉じる
}
