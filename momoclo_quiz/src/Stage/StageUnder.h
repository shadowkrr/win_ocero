//******************************************************************************
//	�t�@�C����	�F	StageUnder.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/11/09
//	�@�\		�F	�������
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//==============================================================
#ifndef STAGEUNDERUNDER_H_
#define STAGEUNDERUNDER_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	�N���X��`
//==========================================================//
namespace stage
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
//�O���Ŏg�p����ϐ��A�\���̂�extern�錾
//==========================================================//
//==========================================================//
//�O���Ŏg�p����֐���extern�錾
//==========================================================//