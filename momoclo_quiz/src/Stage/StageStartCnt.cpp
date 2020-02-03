//******************************************************************************
//	ファイル名	：	StageStartCnt.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/25
//	機能		：	スタートカウント
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageStartCnt.h"
#include	"../sound.h"
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
C_StartCnt::C_StartCnt(const char* fileName) : C_Base(fileName)
{
	cnt = 0;
	cntF = false;
	degF = false;

	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_StartCnt\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_StartCnt::~C_StartCnt()
{
	OutputDebugString("destructor:C_StartCnt\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_StartCnt::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_StartCnt::Control()
{
	if(!cntF){
		if(mAngle <= -360){
			degF = true;
			mAngle = 0;
		}
		if(!degF){
			mAngle += mAddAngle;
		}
		if(degF){
		static float stopcnt;
		stopcnt += (1.0f/60);			//60フレーム
			if(stopcnt >= 0.2f){
				mImgSize.left -= mImgSize.right;
				degF = false;
				stopcnt = 0;

//				if(mImgSize.left >= 0)
//				SetPlay_Se(SE_STAGECNT);
			}
		}
		if(mImgSize.left < 0){ 

			cntF = true;
//			SetPlay_Se(SE_STAGESTART);
		}
		else if (mImgSize.left < -mImgSize.right*2) return true;//必要がなくなった時消す
	}
	return false;
}
