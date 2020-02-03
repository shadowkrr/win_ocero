//******************************************************************************
//	ファイル名	：	StageSnake.h
//	作成者		：	kageyama taiki
//	作成日		：	2012/05/01
//	機能		：	スネーク本体
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGESNAKE_H_
#define STAGESNAKE_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_Snake : public C_Base
	{
	public:
		enum SNAKE_DIR {DIR_UP,DIR_DOWN,DIR_RIGHT,DIR_LEFT};
	private:
		int mdir;	//移動する方向
		int bufdir; //前回の移動方向を確保
		float moveY;
		float moveX;
		float moveValue;				//移動値

		bool hitcheck;
		bool pause;		/* ぽーず */

		void GetItem();

		explicit C_Snake(){}
		C_Snake(const C_Snake&){}
		C_Snake operator=(const C_Snake&){}
	public:
		C_Snake(const char* fileName);
		virtual ~C_Snake();

		void Draw();
		void Create(){}
		bool Control();

		void Key();
		void Move();
		void SetMoveInfo(SNAKE_DIR dir, float* set_ptr, float setValue,int checkpos = 1);
		bool CheckMoveDirection(int checkpos);
		bool CheckMove(int chipNo);
		void MoveFinishPx(float x);
		void MoveFinishPy(float y);
		int GetBackDir();
		float GetMoveValue();
		bool GetHitCheck();
		float GetmoveValue(){return moveValue;};
		int GetDir(){return mdir;}
		int GetBufDir(){return bufdir;}
		bool GetPause(){return pause;};
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
