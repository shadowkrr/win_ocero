//******************************************************************************
//	ファイル名	：	StageTimeCnt.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/11/1
//	機能		：	タイムカウント
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageTimeCnt.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"StageStartCnt.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_TimeCnt::C_TimeCnt(const char* fileName) : C_Base(fileName)
{
	LoadTimeCnt("res/txt/timecnt.txt");
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_TimeCnt\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_TimeCnt::~C_TimeCnt()
{
	OutputDebugString("destructor:C_TimeCnt\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_TimeCnt::Draw()
{
	mPosX = 672;
	mPosY = 536;
	char msg[256];

	sprintf_s(msg,"%.2f秒",timecnt);
	Draw_FontDrawText(static_cast<int>(mPosX),static_cast<int>(mPosY),msg,ARGB(255,255,255,255),3,0.2f);

	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_TimeCnt::Control()
{
	C_StartCnt* startcnt = GET_OBJECT(C_StartCnt* ,C_Stage::OBJ_STARTCNT);

	//ゲームスタートしていれば入る
	if(startcnt->GetCntF()){
		timecnt += (1.0f/60);	//60フレーム 
	}
	return false;
}
//------------------------------------------------------------
//	タイムカウントファイルから読み込み
//------------------------------------------------------------
bool C_TimeCnt::LoadTimeCnt(char* filename)
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
