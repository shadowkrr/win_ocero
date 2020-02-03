//==========================================================//
//ライブラリ以外で必要となるヘッダーファイルのインクルード
//構造体定義が必要となるので同名のヘッダーファイルを記述しておく。
//==========================================================//
#include	"utility/Gp3Lib.h"
#include	"Image.h"
using namespace std;
//==========================================================//
//内部で使用する変数、構造体の宣言
//また外部にある変数を使用した場合はexternで宣言する
//==========================================================//
//==========================================================//
//関数プロトタイプ宣言
//==========================================================//
//==========================================================//
//	クラス関数
//==========================================================//
//------------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------------
C_Image::C_Image()
{
	OutputDebugString("constructor:C_Image\n");
}

//------------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------------
C_Image::~C_Image()
{
	Delete();
	OutputDebugString("destructor:C_Image\n");
}

//------------------------------------------------------------------
//	削除
//------------------------------------------------------------------
void C_Image::Delete()
{
	while(!mImageNo.empty()){
		Draw_DeleteObject(mImageNo.back());	//後ろから順に削除する画像番号を渡す
		mImageNo.pop_back();				//削除済みの番号排出
	}	
	mImageFileName.clear();
}

//------------------------------------------------------------------
//	画像ロード
//------------------------------------------------------------------
IMGOBJ C_Image::IsLoadImage(const char* fileName)
{
	int sizeMax = mImageNo.size();

	for(int i = 0; i < sizeMax; ++i)
	{
		if(mImageFileName[i] == fileName)
		{
			return mImageNo[i];	//読み込み済みの画像を渡す
		}
	}
	
	IMGOBJ no = Draw_LoadObject(const_cast<char*>(fileName));	//番号受け取り
	ImgTable[no] = no;
	mImageNo.push_back(no);
	mImageFileName.push_back(fileName);
	return no;
}
