//******************************************************************************
//	�t�@�C����	�F	StageSnake.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/05/01
//	�@�\		�F	�X�l�[�N�{��
//------------------------------------------------------------------------------
//	�X�V����	�F	
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
//	�N���X��`
//==========================================================//
namespace stage
{
	class C_Snake : public C_Base
	{
	public:
		enum SNAKE_DIR {DIR_UP,DIR_DOWN,DIR_RIGHT,DIR_LEFT};
	private:
		int mdir;	//�ړ��������
		int bufdir; //�O��̈ړ��������m��
		float moveY;
		float moveX;
		float moveValue;				//�ړ��l

		bool hitcheck;
		bool pause;		/* �ہ[�� */

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
//�O���Ŏg�p����ϐ��A�\���̂�extern�錾
//==========================================================//
//==========================================================//
//�O���Ŏg�p����֐���extern�錾
//==========================================================//
