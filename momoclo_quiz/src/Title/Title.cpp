//******************************************************************************
//	�t�@�C����	�F	Title.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/09/23
//	�@�\		�F	�^�C�g��
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Title.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"TitleBack.h"
#include	"TitleSelect.h"
#include	"TitleSystem.h"
#include	"TitleUnder.h"

//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace title;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Title::C_Title()
{
	mObj.resize(OBJ_MAX);	//�ŏ��Ɏg�p���邾����������ݒ肵�Ă���

	//�ȉ��ɍŏ��ɐ������Ă��������I�u�W�F�N�g������
	C_Base*  b = new C_Back("res/txt/title/back.txt");
	SET_OBJECT(b,OBJ_BACK);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_START);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_RANKING);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_END);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Under("res/txt/title/back.txt");
	SET_OBJECT(b,OBJ_UNDER);
	b = new C_System("res/txt/title/system.txt");
	SET_OBJECT(b,OBJ_SYSTEM);
	//�ȏ�

	OutputDebugString("constructor:C_Title\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Title::~C_Title()
{
	OutputDebugString("destructor:C_Title\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Title::Draw()
{
	//Draw_FontDrawText(scriptdata.TextArea,"aaweweat",ARGB(255,255,255,255),scriptdata.TextType,0.4f);
	GetDraw();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
void C_Title::Control()
{
	GetControl();
}
