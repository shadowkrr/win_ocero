//******************************************************************************
//	�t�@�C����	�F	StageBack.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/19
//	�@�\		�F	�w�i
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//==============================================================
#ifndef STAGEBACK_H_
#define STAGEBACK_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	�N���X��`
//==========================================================//
namespace stage
{
	class C_Back : public C_Base
	{
		explicit C_Back(){}
		C_Back(const C_Back&){}
		C_Back operator=(const C_Back&){}
	public:
		C_Back(const char* fileName);
		virtual ~C_Back();

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