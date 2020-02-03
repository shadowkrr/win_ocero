//******************************************************************************
//	�t�@�C����	�F	StageSnake.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/05/01
//	�@�\		�F	�X�l�[�N�{��
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageSnake.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageMap.h"
#include	"StageSnakeCopy.h"
#include	"StageItem.h"
#include	"StageScore.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Snake::C_Snake(const char* fileName) : C_Base(fileName)
{
	SetPlay_Bgm(BGM_STAGE);

	mPosX = 1.0f;
	mPosY = 1.0f;
	moveY = 0.0f;
	moveX = 0.0f;
	mdir = DIR_DOWN;
	bufdir = mdir;
	moveValue = 0.8f;
	hitcheck = false;
	pause = false;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Snake\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Snake::~C_Snake()
{
	OutputDebugString("destructor:C_Snake\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Snake::Draw()
{
	const C_Map* map = GET_OBJECT(const C_Map* ,C_Stage::OBJ_MAP);
	
	if (hitcheck)	/* �Փ˂��� */
	{
		//Draw_Box(0,0,640,480,0xfffeeewwwwwwwwwwer4f0000,0xffff0000,1,1,0.5f);
		Draw_TextXY(400 - 32*2,WINH / 2 + 200,"�|�|�|�|�|�@�I���@�|�|�|�|�|�|",0xffff0000,1);
	}
	if (pause)	/* �Փ˂��� */
	{
		//Draw_Box(0,0,640,480,0xfffeeewwwwwwwwwwer4f0000,0xffff0000,1,1,0.5f);
		Draw_TextXY(400 - 32*2,WINH / 2 + 200,"�|�|�|�|�|�@�ہ[���@�|�|�|�|�|�|",0xffff0000,1);
	}

	Draw_Box(static_cast<int>(mPosX)  * map->GetBlockSize() + static_cast<int>(mAddPx), static_cast<int>(mPosY) * map->GetBlockSize() + static_cast<int>(mAddPy), static_cast<int>(mPosX) * map->GetBlockSize() + static_cast<int>(mAddPx) + map->GetBlockSize(), static_cast<int>(mPosY) * map->GetBlockSize() + static_cast<int>(mAddPy) + map->GetBlockSize(), 0xff000000,0xff000000,1,1,0.5f);
//	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_Snake::Control()
{
	if (!hitcheck && !pause)
	{
		Key();
		Move();
		GetItem();
	}
	else if (hitcheck)
	{
		SetStop_Bgm(BGM_STAGE);
		if(CheckPress(KEY_ENTER))
		{
			SetStop_Bgm(BGM_STAGE);
			C_BaseScreen::SetScreenNo(C_BaseScreen::TITLE_SCREEN);
		//	C_BaseScreen::SetScreenNo(C_BaseScreen::STAGE_SCREEN);
		}
	}
	else if (pause)
	{
		if(CheckPush(KEY_ENTER))
		{
			pause = false;
		}
	}
	return false;
}
//------------------------------------------------------------
//	�L�[
//------------------------------------------------------------
void C_Snake::Key()
{
	if(moveX == 0.0f)
	{
		if(CheckPress(KEY_UP))
		{
			if (mdir != DIR_DOWN)
			{
				SetMoveInfo(DIR_UP, &moveY,-moveValue);
				moveX = 0.0f;
			}
		}
		if(CheckPress(KEY_DOWN))
		{
			if (mdir != DIR_UP)
			{
				SetMoveInfo(DIR_DOWN, &moveY,moveValue);
				moveX = 0.0f;
			}
		}
	}
	if(moveY == 0.0f)
	{
		if(CheckPress(KEY_LEFT))
		{
			if (mdir != DIR_RIGHT)
			{
				SetMoveInfo(DIR_LEFT, &moveX,-moveValue);
				moveY = 0.0f;
			}
		}
		/*if(CheckPress(KEY_RIGHT))
		{
			if (mdir != DIR_LEFT)
			{
				SetMoveInfo(DIR_RIGHT, &moveX,moveValue);
				moveY = 0.0f;	
			}
		}*/
	}


	if (!pause)
	{
		if(CheckPush(KEY_ENTER))
		{
			pause = true;
		}
	}
	 /*C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

	if(CheckPress(KEY_SPACE))
	{
		sne_copy->CreateSnake(mPosX, mPosY, mdir);
	}*/
}
//------------------------------------------------------------
//��}�X�Â�������
//------------------------------------------------------------
void C_Snake::Move()
{
	const C_Map* map = GET_OBJECT(const C_Map* ,C_Stage::OBJ_MAP);

	if(moveY == 0.0f)
	{
		if (mdir == DIR_DOWN)
		{
			SetMoveInfo(DIR_DOWN, &moveY,moveValue);
		}
		else if (mdir == DIR_UP)
		{
			SetMoveInfo(DIR_UP, &moveY,-moveValue);
		}
	}
	if(moveX == 0.0f)
	{
		if (mdir == DIR_RIGHT)
		{
			SetMoveInfo(DIR_RIGHT, &moveX,moveValue);
		}
		else if (mdir == DIR_LEFT)
		{
			SetMoveInfo(DIR_LEFT, &moveX,-moveValue);
		}
	}









	if(moveY != 0.0f)
	{
		mAddPy += moveY;
		if(mAddPy >= map->GetBlockSize())
		{
			MoveFinishPy(1.0f);
		}
		else if(mAddPy <= -map->GetBlockSize())
		{
			MoveFinishPy(-1.0f);
		}
	}
	//�L�����ړ� X��
	if(moveX != 0.0f)
	{
		mAddPx += moveX;
		if(mAddPx >= map->GetBlockSize())
		{
			MoveFinishPx(1.0f);
		}
		else if(mAddPx <= -map->GetBlockSize())
		{
			MoveFinishPx(-1.0f);
		}
	}
}
//------------------------------------------------------------
//�L�[�������ꂽ��̈ړ������Z�b�g
//------------------------------------------------------------
void C_Snake::SetMoveInfo(SNAKE_DIR dir, float* set_ptr, float setValue,int checkpos)
{
	//ch_ptr->dir = dir;
	mdir = dir;
	mImgSize.top = dir*mImgSize.bottom;

	if(CheckMoveDirection(checkpos))
	{
		*set_ptr = setValue;
	}
	else
	{
		*set_ptr = 0.0f;	/* �ǂ�����Ƃ�move��0.0f */
		//mAddPy = mAddPx = 0.0f;
		hitcheck = true;
	}
}
//------------------------------------------------------------
//�������Ă�������͈ړ��\���ǂ���
//	true �s����
//	false �s���Ȃ�
//------------------------------------------------------------
bool C_Snake::CheckMoveDirection(int checkpos)
{
	C_Map* map = GET_OBJECT(C_Map* ,C_Stage::OBJ_MAP);

	bool ret = false;

	switch(mdir)
	{
		case DIR_DOWN:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)+1));	break;
		case DIR_UP:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)-1));	break;
		case DIR_LEFT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)-1, static_cast<int>(mPosY)));		break;
		case DIR_RIGHT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)+1, static_cast<int>(mPosY)));		break;
	}
	//ret = true;
	return ret;
}
//------------------------------------------------------------
//�s�����Ƃ��Ă���ꏊ�͈ړ��\���ǂ���
//	true �s����
//	false �s���Ȃ�
//------------------------------------------------------------
bool C_Snake::CheckMove(int chipNo)
{
	return chipNo == 1 ? false: true;
}
//------------------------------------------------------------
//�ړ������������[�v����œ�ɕ����č쐬
//------------------------------------------------------------
void C_Snake::MoveFinishPx(float x)
{
	SetPlay_Se(SE_MOVE);
	mPosX += x;
	//ch_ptr->posY += y;
	moveX =/* ch_ptr->moveY =*/ 
	mAddPx =/* ch_ptr->addPy =*/ 0.0f;

	bufdir = mdir; /* �ړ������������o�������Ă��� */


	C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);
	//sne_copy->CreateSnake(mPosX - 1, mPosY, mdir);

	if (sne_copy->SnakeCheckPos(mPosX, mPosY))
	{
		hitcheck = true;
	}
}
//------------------------------------------------------------
//�ړ������������[�v����œ�ɕ����č쐬
//------------------------------------------------------------
void C_Snake::MoveFinishPy(float y)
{
	SetPlay_Se(SE_MOVE);
	//ch_ptr->posX += x;
	mPosY += y;
	/*ch_ptr->moveX = */moveY = 
	/*ch_ptr->addPx = */mAddPy = 0.0f;

	bufdir = mdir; /* �ړ������������o�������Ă��� */

	C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);
	//sne_copy->CreateSnake(mPosX, mPosY - 1, mdir);

	if (sne_copy->SnakeCheckPos(mPosX, mPosY))
	{
		hitcheck = true;
	}
}
//------------------------------------------------------------
//�ړ����������O��̕������擾
//------------------------------------------------------------
int C_Snake::GetBackDir()
{
	return bufdir; /* �O��ړ����������𑗂�*/
}
//------------------------------------------------------------
//�ړ����x�̎擾
//------------------------------------------------------------
float C_Snake::GetMoveValue()
{
	return moveValue; /* �ړ����x�̎擾 */
}
//------------------------------------------------------------
//�I���t���O�̎擾
//------------------------------------------------------------
bool C_Snake::GetHitCheck()
{
	return hitcheck;
}
//------------------------------------------------------------
//	�A�C�e���擾
//------------------------------------------------------------
void C_Snake::GetItem()
{
	C_Item* item = GET_OBJECT(C_Item* ,C_Stage::OBJ_ITEM);

	if (mPosX == item->GetPosX() && mPosY == item->GetPosY())
	{
		C_Score* score = GET_OBJECT( C_Score* ,C_Stage::OBJ_SCORE);

		score->ScorePlus();	/* �X�R�A�̉��Z */
		/* �ւ𑝂₷ */
		C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

		if (mdir == DIR_UP)
		{
			sne_copy->CreateSnake(mPosX, mPosY + 1, mdir);
		}
		else if (mdir == DIR_DOWN)
		{
			sne_copy->CreateSnake(mPosX, mPosY - 1, mdir);
		}
		else if (mdir == DIR_RIGHT)
		{
			sne_copy->CreateSnake(mPosX - 1, mPosY, mdir);
		}
		else if (mdir == DIR_LEFT)
		{
			sne_copy->CreateSnake(mPosX + 1, mPosY, mdir);
		}

		moveValue += 0.2f;	/* ���x�̉��Z */
		item->item_reset();	/* �A�C�e���̏ꏊ��ς��� */
	}
}