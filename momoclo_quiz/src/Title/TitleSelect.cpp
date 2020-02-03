//******************************************************************************
//	ファイル名	：	TitleSelect.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/15
//	機能		：	項目選択
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Title.h"
#include	"TitleSelect.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"TitleSystem.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace title;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Select::C_Select(const char* fileName, SELECT selName) : C_Base(fileName)
{
	if(selName == SEL_START){
		mPlace = PLA_CENTER;
	}
	else if(selName == SEL_RANKING){
		mPlace = PLA_DOWN;
	}
	else if(selName == SEL_END){
		mPlace = PLA_UP;
	}

	TopPos = 270.0f;
	CenterPos = 330.0f;
	BottomPos = 390.0f;
	mSelectStageCnt = SEL_START;
	mUpF = mDownF = false;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());

	OutputDebugString("constructor:C_Select\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Select::~C_Select()
{
	OutputDebugString("destructor:C_Select\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_Select::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_Select::Control()
{
	IsNowSelect();
	return false;
}

//------------------------------------------------------------
//	選択中か判定
//------------------------------------------------------------
void C_Select::IsNowSelect()
{
	C_System* system = GET_OBJECT(C_System*,C_Title::OBJ_SYSTEM);

	if(!system->GetFadeF())
	{
		if(!mUpF && !mDownF){
			if(CheckPress(KEY_UP)){
		//		SetPlay_Se(SE_SELECT);
				mUpF = true;
				ChangeSelectStage(CNT_ADD,SEL_END,SEL_START);
				system->SetNowSelectF(true);
			}
			else if(CheckPress(KEY_DOWN)){
		//		SetPlay_Se(SE_SELECT);
				mDownF = true;
				ChangeSelectStage(CNT_SUB,SEL_START,SEL_END);
				system->SetNowSelectF(true);
			}
		}
		else{
			if(NowSelectControl()){
				system->SetNowSelectF(false);
			}
		}
	}
}

//------------------------------------------------------------
//	選択中の制御
//------------------------------------------------------------
bool C_Select::NowSelectControl()
{
	if(mPlace == PLA_UP){
		if(IsControlUp()){
			return true;
		}
	}
	else if(mPlace == PLA_CENTER){
		if(IsControlCenter()){
			return true;
		}
	}
	else if(mPlace == PLA_DOWN){
		if(IsControlDown()){
			return true;
		}
	}	
	return false;
}

//------------------------------------------------------------
//	中心の制御判定
//------------------------------------------------------------
bool C_Select::IsControlCenter()
{
	const float Scal = 0.6f;

	if(mUpF)
	{
		mPosY -= mAddPy;
		mScaX -= mAddScaX;
		mScaY -= mAddScaY;

		if(mPosY <= TopPos && mScaX <= Scal && mScaY <= Scal ){
			mPosY = TopPos;
			mScaX = mScaY = Scal;
			mPosZ = 0.3f;
			mPlace = PLA_UP;
			mUpF = false;
			return true;
		}
	}
	else if(mDownF)
	{
		mPosY += mAddPy;
		mScaX -= mAddScaX;
		mScaY -= mAddScaY;

		if(mPosY >= BottomPos && mScaX <= Scal && mScaY <= Scal ){
			mPosY = BottomPos;
			mScaX = mScaY = Scal;
			mPosZ = 0.3f;
			mPlace = PLA_DOWN;
			mDownF = false;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------
//	上の制御判定
//------------------------------------------------------------
bool C_Select::IsControlUp()
{
	if(mUpF)
	{
		mPosY += (mAddPy * 2.0f);

		if(mPosY >= BottomPos){
			mPosY = BottomPos;
			mPosZ = 0.3f;
			mPlace = PLA_DOWN;
			mUpF = false;
			return true;
		}
	}
	else if(mDownF)
	{
		mPosY += mAddPy;
		mScaX += mAddScaX;
		mScaY += mAddScaY;

		if(mPosY >= CenterPos && mScaX >= 1.0f && mScaY >= 1.0f ){
			mPosY = CenterPos;
			mScaX = mScaY = 1.0f;
			mPosZ = 0.5f;
			mPlace = PLA_CENTER;
			mDownF = false;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------
//	下の制御判定
//------------------------------------------------------------
bool C_Select::IsControlDown()
{
	if(mUpF)
	{
		mPosY -= mAddPy;
		mScaX += mAddScaX;
		mScaY += mAddScaY;

		if(mPosY <= CenterPos && mScaX >= 1.0f && mScaY >= 1.0f ){
			mPosY = CenterPos;
			mScaX = mScaY = 1.0f;
			mPosZ = 0.5f;
			mPlace = PLA_CENTER;
			mUpF = false;
			return true;
		}
	}
	else if(mDownF)
	{
		mPosY -= (mAddPy * 2.0f);

		if(mPosY <= TopPos){
			mPosY = TopPos;
			mPosZ = 0.3f;
			mPlace = PLA_UP;
			mDownF = false;
			return true;
		}
	}
	return false;
}