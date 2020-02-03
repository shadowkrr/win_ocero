//******************************************************************************
//	�t�@�C����	�F	Stage.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/04/01
//	�@�\		�F	�X�e�[�W
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageBack.h"
//#include	"StageSelect.h"
#include	"StageSystem.h"
#include	"StageItem.h"
#include	"StageMap.h"
#include	"StageBlock.h"
#include	"StageScore.h"
#include	"StageSnake.h"
#include	"StageSnakeCopy.h"
//#include	"StageUnder.h"


#include	"StageMouseControl.h"

//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Stage::C_Stage()
{
	mObj.resize(OBJ_MAX);	//�ŏ��Ɏg�p���邾����������ݒ肵�Ă���

	//�ȉ��ɍŏ��ɐ������Ă��������I�u�W�F�N�g������
	C_Base*  b = new C_Back("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_BACK);
	b = new C_Map("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_MAP);
	b = new C_MouseControl("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_MOUSECONTROL);
	/*b = new C_Item("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_ITEM);*/
	/*b = new C_Block("res/txt/Stage/back.txt", 0, 0, C_Block::BLOCK_1);
	SET_OBJECT(b,OBJ_BLOCK1);
	b = new C_Score("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SCORE);*/
	/*b = new C_Score("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SCORE);
	b = new C_Snake("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SNAKE);
	b = new C_SnakeCopy("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SNAKECOPY);*/
	b = new C_System("res/txt/Stage/system.txt");
	SET_OBJECT(b,OBJ_SYSTEM);
	//�ȏ�

	OutputDebugString("constructor:C_Stage\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Stage::~C_Stage()
{
	OutputDebugString("destructor:C_Stage\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Stage::Draw()
{
	//Draw_FontDrawText(scriptdata.TextArea,"aaweweat",ARGB(255,255,255,255),scriptdata.TextType,0.4f);
	GetDraw();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
void C_Stage::Control()
{
	GetControl();
}
