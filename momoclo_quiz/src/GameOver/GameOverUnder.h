//******************************************************************************
//	ファイル名	：	GameOverUnder.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/11/11
//	機能		：	操作説明
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef GAMEOVERUNDER_H_
#define GAMEOVERUNDER_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace gameover
{
	class C_Under : public C_Base
	{
		explicit C_Under(){}
		C_Under(const C_Under&){}
		C_Under operator=(const C_Under&){}
	public:
		explicit C_Under(const char* fileName);
		virtual ~C_Under();

		void Draw();
		void Create(){}
		bool Control();
	};
}
//==============================================================
#endif
//==============================================================
//==========================================================//
//外部で使用する変数、構造体のextern宣言
//==========================================================//
//==========================================================//
//外部で使用する関数のextern宣言
//==========================================================//
