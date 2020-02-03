//******************************************************************************
//	ファイル名	：	GameOver.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/28
//	機能		：	システム管理
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef GAMEOVERSYSTEM_H_
#define GAMEOVERSYSTEM_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace gameover
{
	class C_System : public C_Base
	{
		bool mFadeF;
		bool GetNowSelectF;
		int next;
		enum NEXT{NEXT_TITLE,NEXT_RETRY};

		explicit C_System(){}
		C_System(const C_System&){}
		C_System operator=(const C_System&){}
	public:
		explicit C_System(const char* fileName);
		virtual ~C_System();

		void Draw();
		void Create(){}
		bool Control();

		bool GetFadeF()const{return mFadeF;}
		void SetNowSelectF(bool f){GetNowSelectF = f;}
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
