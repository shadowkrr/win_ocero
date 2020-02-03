//******************************************************************************
//	�t�@�C����	�F	StageScore.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/04/01
//	�@�\		�F	�X�R�A
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageScore.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageSnake.h"
#include	"StageSnakeCopy.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Score::C_Score(const char* fileName) : C_Base(fileName)
{
	scorecnt = 1;
	//mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Score\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Score::~C_Score()
{
	OutputDebugString("destructor:C_Score\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Score::Draw()
{
	char str[32];

	C_Snake* snake = GET_OBJECT(C_Snake* ,C_Stage::OBJ_SNAKE);
	C_SnakeCopy* snakecopy = GET_OBJECT(C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

	sprintf(str,"score = %d",scorecnt);
	Draw_TextXY(400,WINH / 4,str,0xffff0000,1);
	sprintf(str,"moveValue = %f", snake->GetmoveValue());
	Draw_TextXY(400,WINH / 4 + 32,str,0xffff0000,1);
	sprintf(str,"posx = %f", snake->GetPosX());
	Draw_TextXY(400,WINH / 4 + 32 * 2,str,0xffff0000,1);
	sprintf(str,"posy = %f", snake->GetPosY());
	Draw_TextXY(400,WINH / 4 + 32 * 3,str,0xffff0000,1);
	sprintf(str,"addpx = %f", snake->GetAddPx());
	Draw_TextXY(400,WINH / 4 + 32 * 4,str,0xffff0000,1);
	sprintf(str,"addpy = %f", snake->GetAddPy());
	Draw_TextXY(400,WINH / 4 + 32 * 5,str,0xffff0000,1);
	sprintf(str,"dir = %d", snake->GetDir());
	Draw_TextXY(400,WINH / 4 + 32 * 6,str,0xffff0000,1);
	sprintf(str,"backdir = %d", snake->GetBufDir());
	Draw_TextXY(400,WINH / 4 + 32 * 7,str,0xffff0000,1);
	sprintf(str,"copynum = %d", snakecopy->GetCopyNum());
	Draw_TextXY(400,WINH / 4 + 32 * 8,str,0xffff0000,1);
	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_Score::Control()
{
	return false;
}
//------------------------------------------------------------
// �X�R�A�̉��Z
//------------------------------------------------------------
void C_Score::ScorePlus()
{
	scorecnt++;
}