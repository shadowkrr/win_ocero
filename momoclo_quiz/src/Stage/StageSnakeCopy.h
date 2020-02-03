//******************************************************************************
//	�t�@�C����	�F	StageSnakeCopy.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/05/01
//	�@�\		�F	�X�l�[�N�̃R�s�[
//------------------------------------------------------------------------------
//	�X�V����	�F	
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
//	�N���X��`
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
//�O���Ŏg�p����ϐ��A�\���̂�extern�錾
//==========================================================//
//==========================================================//
//�O���Ŏg�p����֐���extern�錾
//==========================================================//
