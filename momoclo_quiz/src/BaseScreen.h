//******************************************************************************
//	�t�@�C����	�F	BaseScreen.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/09/15
//	�@�\		�F	��ʐ؂�ւ��p�̊��N���X(stage,gameover,clear�Ȃǂ̌p���Ɏg�p)
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//==============================================================
#ifndef BASE_SCREEN_H_
#define BASE_SCREEN_H_
//==============================================================
#include	<windows.h>	//OutputDebugString()���g�p����׃C���N���[�h
//==============================================================
// �N���X�錾
//==============================================================
class C_BaseScreen{
public:
	enum SCREEN_NAME{LOGO_SCREEN,TITLE_SCREEN,
			STAGE_SCREEN,CLEAR_SCREEN,GAMEOVER_SCREEN,MAX_SCREEN};
private:
	static SCREEN_NAME	mScreenNo;

	C_BaseScreen(const C_BaseScreen&){}
	C_BaseScreen operator=(const C_BaseScreen&){}
public:
	explicit C_BaseScreen(){
		OutputDebugString("constructor:C_BaseScreen\n");
	}
	virtual ~C_BaseScreen(){
		OutputDebugString("destructor:C_BaseScreen\n");
	}

	virtual void Draw() = 0;
	virtual void Control() = 0;

	static void SetScreenNo(SCREEN_NAME screenNo){mScreenNo = screenNo;}
	int  GetScreenNo()const{return mScreenNo;}
};

//==============================================================
#endif
//==============================================================