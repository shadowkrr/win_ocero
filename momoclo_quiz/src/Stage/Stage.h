//******************************************************************************
//	ファイル名	：	Stage.h
//	作成者		：	kageyama taiki
//	作成日		：	2012/04/01
//	機能		：	ステージ
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGE_H_
#define STAGE_H_
//==============================================================
//==============================================================
#include	"../BaseScreen.h"
#include	"../BaseManager.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_Stage : public C_BaseScreen, public C_BaseManager
	{
		C_Stage(const C_Stage&){}
		C_Stage operator=(const C_Stage&){}
	public:
		enum ObjName{OBJ_BACK,OBJ_SELECT,OBJ_MOUSECONTROL,OBJ_ITEM,OBJ_MAP,OBJ_SYSTEM,OBJ_UNDER,OBJ_SNAKE,OBJ_SNAKECOPY,OBJ_BLOCK1,OBJ_BLOCK2,OBJ_SCORE,OBJ_STARTCNT,OBJ_MAX};

		explicit C_Stage();
		virtual ~C_Stage();

		void Draw();
		void Control();
	};
}

//==============================================================
#endif
//==============================================================