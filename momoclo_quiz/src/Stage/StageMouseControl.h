//******************************************************************************
//	ファイル名	：	StageMouseCntrol.h
//	作成者		：	kageyama taiki
//	作成日		：	2012/05/05
//	機能		：	マウスコントロール
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGEMOUSECNTROL_H_
#define STAGEMOUSECNTROL_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_MouseControl : public C_Base
	{
	private:
		int seluser;
		explicit C_MouseControl(){}
		C_MouseControl(const C_MouseControl&){}
		C_MouseControl operator=(const C_MouseControl&){}
	public:
		C_MouseControl(const char* fileName);
		virtual ~C_MouseControl();

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
