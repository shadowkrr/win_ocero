//******************************************************************************
//	ファイル名	：	StageSystem.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/27
//	機能		：	システム管理
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------

//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_System::C_System(const char* fileName/*,int stageno*/) : C_Base(fileName)
 {
	SetPlay_Bgm(BGM_STAGE);
	//mstageno = stageno;
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
	if(!mFadeF){
		if(ClearCheck()){
			mFadeF = true;
//			SetPlay_Se(SE_SELECT2);
		}
		if(EndCheck()){
			if(IsFade(C_Base::FADE_IN)){
				SetStop_Bgm(BGM_STAGE);
				C_BaseScreen::SetScreenNo(C_BaseScreen::GAMEOVER_SCREEN);
			}
		}
	}
	if(mFadeF){ 
		if(IsFade(C_Base::FADE_IN)){
			SetStop_Bgm(BGM_STAGE);
			C_BaseScreen::SetScreenNo(C_BaseScreen::TITLE_SCREEN);
		}
	}
	
	return false;
}
//------------------------------------------------------------
//	エンドチェック
//	プレイヤの体力が0でゲームオーバー
//------------------------------------------------------------
bool C_System::EndCheck()
{
	return false;
}
//------------------------------------------------------------
//	クリアチェック
//	エネミーをすべて倒したらゲームクリア
//------------------------------------------------------------
bool C_System::ClearCheck()
{
	if(CheckPress(KEY_SPACE))
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------
//	ファイルに書き込み
//------------------------------------------------------------
bool C_System::SaveStage(char* filename)
{
	FILE* fp;

	//char Buffer[256];
	//fopen_sで読み込んで上書きしようとするとエラーがでる
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//書き込み失敗
	if(&fp == NULL)
	{
		MessageBox(hWnd, "書き込み失敗", __FUNCTION__, MB_OK);
		return false;
	}
	//書き込み成功
	else
	{
			
	//	fprintf(fp,"%d %d \n",mcharacterno,mstageno);
		fclose(fp);
		return true;
	}
}
//------------------------------------------------------------
//	タイムカウントをファイルに書き込み
//------------------------------------------------------------
bool C_System::SaveTimeCnt(char* filename)
{
	//C_TimeCnt* timecnt = GET_OBJECT(C_TimeCnt* ,C_Stage::OBJ_TIMECNT);

	FILE* fp;

	//char Buffer[256];
	//fopen_sで読み込んで上書きしようとするとエラーがでる
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//書き込み失敗
	if(&fp == NULL)
	{
		MessageBox(hWnd, "書き込み失敗", __FUNCTION__, MB_OK);
		return false;
	}
	//書き込み成功
	else
	{
			
	//	fprintf(fp,"%f\n",timecnt->GetTimeCnt());
		fclose(fp);
		return true;
	}
}
//------------------------------------------------------------
//	キャラクターステータスの初期化
//------------------------------------------------------------
bool C_System::SaveStates(char* filename)
{
	//C_Player* player = GET_OBJECT(C_Player* ,C_Stage::OBJ_PLAYER);

	FILE* fp;

	//char Buffer[256];
	//fopen_sで読み込んで上書きしようとするとエラーがでる
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//書き込み失敗
	if(&fp == NULL)
	{
		MessageBox(hWnd, "書き込み失敗", __FUNCTION__, MB_OK);
		return false;
	}
	//書き込み成功
	else
	{
		//fprintf(fp,"%f %f %f %f \n",player->GetHpMax(),player->GetBlastMax(),player->GetBlastMax(),player->GetMoveValue());
		fclose(fp);
		return true;
	}
}