//******************************************************************************
//	�t�@�C����	�F	StageSnakeCopy.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/05/01
//	�@�\		�F	�X�l�[�N�̃R�s�[
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageSnakeCopy.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageSnake.h"
#include	"StageMap.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;

static const int arraymax = 104;
typedef struct copy_t
{
	float posX;
	float posY;
	float addPx;
	float addPy;
	float moveX;
	float moveY;

	int dir;
	int bufdir;
	bool show;
};
static copy_t copy[arraymax];
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_SnakeCopy::C_SnakeCopy(const char* fileName) : C_Base(fileName)
{

	openlimitno = 0;

	for (int i = 0; i < arraymax; i++)
	{
		copy[i].posX = 0.0f;
		copy[i].posY = 0.0f;
		copy[i].addPx = 0.0f;
		copy[i].addPy = 0.0f;
		copy[i].moveX = 0.0f;
		copy[i].moveY = 0.0f;
		copy[i].show = false;
		copy[i].dir = C_Snake::DIR_DOWN;
		copy[i].bufdir = copy[i].dir;
	}
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_SnakeCopy\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_SnakeCopy::~C_SnakeCopy()
{
	OutputDebugString("destructor:C_SnakeCopy\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_SnakeCopy::Draw()
{
	for (int i = 0; i < arraymax; i++)
	{
		if (copy[i].show)
		{
			Draw_Box(static_cast<int>(copy[i].posX * 32 + copy[i].addPx),      static_cast<int>(copy[i].posY * 32 + copy[i].addPy),
					 static_cast<int>(copy[i].posX * 32 + copy[i].addPx + 32), static_cast<int>(copy[i].posY * 32 + copy[i].addPy + 32),
					 0xff000000,0xff000000,1,1,0.5f);
		}
	}
	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_SnakeCopy::Control()
{
	C_Snake* snake = GET_OBJECT(C_Snake* ,C_Stage::OBJ_SNAKE);
	C_Map* map = GET_OBJECT(C_Map* ,C_Stage::OBJ_MAP);

	if (!snake->GetHitCheck() && !snake->GetPause())
	{
		for (int i = 0; i < arraymax; i++)
		{
			if (copy[i].show)
			{
				if (i == 0)
				{
					if (snake->GetBackDir() == C_Snake::DIR_UP)
					{
						SetMoveInfo(C_Snake::DIR_UP, &copy[i].moveY, i, -snake->GetMoveValue());
		//				SetMoveInfo(DIR_UP, &moveY,-moveValue);
					}
					if (snake->GetBackDir() == C_Snake::DIR_DOWN)
					{
						SetMoveInfo(C_Snake::DIR_DOWN, &copy[i].moveY, i, snake->GetMoveValue());
						//SetMoveInfo(DIR_DOWN, &moveY,moveValue);
					}
					if (snake->GetBackDir() == C_Snake::DIR_RIGHT)
					{
						SetMoveInfo(C_Snake::DIR_RIGHT, &copy[i].moveX, i, snake->GetMoveValue());
						//SetMoveInfo(DIR_RIGHT, &moveX,moveValue);
					}
					if (snake->GetBackDir() == C_Snake::DIR_LEFT)
					{
						SetMoveInfo(C_Snake::DIR_LEFT, &copy[i].moveX, i, -snake->GetMoveValue());
						//SetMoveInfo(DIR_LEFT, &moveX,-moveValue);
					}
				}
				else
				{
					if (copy[i - 1].bufdir == C_Snake::DIR_UP)
					{
						SetMoveInfo(C_Snake::DIR_UP, &copy[i].moveY, i, -snake->GetMoveValue());
					}
					if (copy[i - 1].bufdir == C_Snake::DIR_DOWN)
					{
						SetMoveInfo(C_Snake::DIR_DOWN, &copy[i].moveY, i, snake->GetMoveValue());
					}

					if (copy[i - 1].bufdir == C_Snake::DIR_RIGHT)
					{
						SetMoveInfo(C_Snake::DIR_RIGHT, &copy[i].moveX, i, snake->GetMoveValue());
					}

					if (copy[i - 1].bufdir == C_Snake::DIR_LEFT)
					{
						SetMoveInfo(C_Snake::DIR_LEFT, &copy[i].moveX, i, -snake->GetMoveValue());
					}
				}

			}
		}


		for (int i = 0; i < arraymax; i ++)
		{
			if (copy[i].show)
			{
				if(copy[i].moveY != 0.0f)
				{
					copy[i].addPy += copy[i].moveY;
					if(copy[i].addPy >= map->GetBlockSize())
					{
						MoveFinishPy(1.0f, i);
					}
					else if(copy[i].addPy <= -map->GetBlockSize())
					{
						MoveFinishPy(-1.0f, i);
					}
				}
				//�L�����ړ� X��
				if(copy[i].moveX != 0.0f)
				{
					copy[i].addPx += copy[i].moveX;
					if(copy[i].addPx >= map->GetBlockSize())
					{
						MoveFinishPx(1.0f, i);
					}
					else if(copy[i].addPx <= -map->GetBlockSize())
					{
						MoveFinishPx(-1.0f, i);
					}
				}
			}
		}
	}
	return false;
}
//------------------------------------------------------------
//	�R�s�[����
//------------------------------------------------------------
void C_SnakeCopy::CreateSnake(const float posx, const float posy, const int dir)
{
	SetPlay_Se(SE_COPY);
	//else
	{
		if (copy[openlimitno - 1].bufdir == C_Snake::DIR_UP)
		{
			copy[openlimitno].posX = copy[openlimitno - 1].posX;
			copy[openlimitno].posY = copy[openlimitno - 1].posY + 1;
		}
		if (copy[openlimitno - 1].bufdir == C_Snake::DIR_DOWN)
		{
			copy[openlimitno].posX = copy[openlimitno - 1].posX;
			copy[openlimitno].posY = copy[openlimitno - 1].posY - 1;
		}
		if (copy[openlimitno - 1].bufdir == C_Snake::DIR_RIGHT)
		{
			copy[openlimitno].posX = copy[openlimitno - 1].posX - 1;
			copy[openlimitno].posY = copy[openlimitno - 1].posY;
		}
		if (copy[openlimitno - 1].bufdir == C_Snake::DIR_LEFT)
		{
			copy[openlimitno].posX = copy[openlimitno - 1].posX + 1;
			copy[openlimitno].posY = copy[openlimitno - 1].posY;
		}
	}

	if (openlimitno == 0)
	{
		copy[openlimitno].posX = posx;
		copy[openlimitno].posY = posy;
	}

	copy[openlimitno].addPx = 0.0f;
	copy[openlimitno].addPy = 0.0f;
	copy[openlimitno].moveX = 0.0f;
	copy[openlimitno].moveY = 0.0f;
	copy[openlimitno].show = true;

	openlimitno++;
}
//------------------------------------------------------------
//�L�[�������ꂽ��̈ړ������Z�b�g
//------------------------------------------------------------
void C_SnakeCopy::SetMoveInfo(C_Snake::SNAKE_DIR dir, float* set_ptr, const int id, float setValue,int checkpos)
{
	//ch_ptr->dir = dir;
	copy[id].dir = dir;
	//mImgSize.top = dir*mImgSize.bottom;

	if(CheckMoveDirection(checkpos, id))
	{
		*set_ptr = setValue;
	}
	else
	{
		*set_ptr = 0.0f;	/* �ǂ�����Ƃ�move��0.0f */
		//mAddPy = mAddPx = 0.0f;
	}
}
//------------------------------------------------------------
//�������Ă�������͈ړ��\���ǂ���
//	true �s����
//	false �s���Ȃ�
//------------------------------------------------------------
bool C_SnakeCopy::CheckMoveDirection(int checkpos, const int id)
{
	//C_Map* map = GET_OBJECT(C_Map* ,C_Stage::OBJ_MAP);

	bool ret = false;

	/*switch(copy[id].dir)
	{
	case C_Snake::DIR_DOWN:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)+1));	break;
		case C_Snake::DIR_UP:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)-1));	break;
		case C_Snake::DIR_LEFT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)-1, static_cast<int>(mPosY)));		break;
		case C_Snake::DIR_RIGHT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)+1, static_cast<int>(mPosY)));		break;
	}*/

	ret = true;
	return ret;
}
//------------------------------------------------------------
//�s�����Ƃ��Ă���ꏊ�͈ړ��\���ǂ���
//	true �s����
//	false �s���Ȃ�
//------------------------------------------------------------
bool C_SnakeCopy::CheckMove(int chipNo)
{
	return chipNo == 1 ? false: true;
}
//------------------------------------------------------------
//�ړ������������[�v����œ�ɕ����č쐬
//------------------------------------------------------------
void C_SnakeCopy::MoveFinishPx(float x, const int id)
{
	copy[id].posX += x;
	//ch_ptr->posY += y;
	copy[id].moveX =/* ch_ptr->moveY =*/ 
	copy[id].addPx =/* ch_ptr->addPy =*/ 0.0f;

	copy[id].bufdir = copy[id].dir; /* �ړ������������o�������Ă��� */
}
//------------------------------------------------------------
//�ړ������������[�v����œ�ɕ����č쐬
//------------------------------------------------------------
void C_SnakeCopy::MoveFinishPy(float y, const int id)
{
	//ch_ptr->posX += x;
	copy[id].posY += y;
	/*ch_ptr->moveX = */copy[id].moveY = 
		/*ch_ptr->addPx = */copy[id].addPy = 0.0f;

	copy[id].bufdir = copy[id].dir; /* �ړ������������o�������Ă��� */
}
//------------------------------------------------------------
//�����蔻��
//------------------------------------------------------------
bool C_SnakeCopy::SnakeCheckPos(const float px, const float py)
{
	for (int i = 0; i < arraymax; i++)
	{
		if (px == copy[i].posX && py == copy[i].posY)
		{
			return true;
		}
	}
	return false;
}