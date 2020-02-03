//******************************************************************************
//	ファイル名	：	StageSnakeCopy.h
//	作成者		：	kageyama taiki
//	作成日		：	2012/05/01
//	機能		：	スネークのコピー
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGESNAKECOPY_H_
#define STAGESNAKECOPY_H_
//==============================================================
//==============================================================
#include	"../Base.h"
#include	"StageSnake.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_SnakeCopy : public C_Base
	{
	private:
		int openlimitno;

		void SetMoveInfo(C_Snake::SNAKE_DIR dir, float* set_ptr, const int id, float setValue,int checkpos = 1);
		bool CheckMoveDirection(int checkpos, const int id);
		bool CheckMove(int chipNo);
		void MoveFinishPx(float x, const int id);
		void MoveFinishPy(float y, const int id);

		explicit C_SnakeCopy(){}
		C_SnakeCopy(const C_SnakeCopy&){}
		C_SnakeCopy operator=(const C_SnakeCopy&){}
	public:
		C_SnakeCopy(const char* fileName);
		virtual ~C_SnakeCopy();

		void Draw();
		void Create(){}
		bool Control();
		void CreateSnake(const float posx, const float posy, const int dir);
		bool SnakeCheckPos(const float px, const float py);

		int GetCopyNum(){return openlimitno;};
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
