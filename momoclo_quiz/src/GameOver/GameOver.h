//******************************************************************************
//	ファイル名	：	GameOver.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/09/23
//	機能		：	ゲームオーバー
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef GAMEOVER_H_
#define GAMEOVER_H_
//==============================================================
//==============================================================
#include	"../BaseScreen.h"
#include	"../BaseManager.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace gameover
{
	class C_GameOver : public C_BaseScreen, C_BaseManager
	{
		C_GameOver(const C_GameOver&){}
		C_GameOver operator=(const C_GameOver&){}
	public:
		enum ObjName{OBJ_BACK,OBJ_UNDER,OBJ_SYSTEM,OBJ_MAX};

		explicit C_GameOver();
		virtual ~C_GameOver();

		void Draw();
		void Control();
	};
}
//==============================================================
#endif
//==============================================================