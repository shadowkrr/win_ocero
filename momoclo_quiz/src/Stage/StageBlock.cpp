//******************************************************************************
//	�t�@�C����	�F	StageBlock.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2012/04/01
//	�@�\		�F	�u���b�N
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageBlock.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageMap.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;

static int randomARGB[6] = {0xffff0000,0xff00ff00,0xff0000ff,0xffff00ff,0xffffff00,0xff00ffff};
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Block::C_Block(const char* fileName, float posX, float posY, int blockno) : C_Base(fileName)
{
	/* �u���b�N�o���ʒu�̃Z�b�g */
	BposX = posX;
	BposY = posY;
	mPosX = 5 + posX;
	mPosY = 0 + posY;
	mAddPx = 0;
	mAddPy = 0;
	timercnt = 0;
	mblockno = blockno;
	keyflag = true;

	r_ARGB = rand() % 6;

	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Block\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Block::~C_Block()
{
	OutputDebugString("destructor:C_Block\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Block::Draw()
{
	Draw_Box(static_cast<int>(mPosX) * 32, static_cast<int>(mPosY) * 32,
		static_cast<int>(mPosX) * 32 + 32, static_cast<int>(mPosY) * 32 + 32,
		randomARGB[r_ARGB], 0xff000000,1,1,0.5);


	if (mblockno == BLOCK_1)
	{
		Draw_TextXY(mPosX*32,mPosY*32,"1",0xffff0000,1);
	}
	else
	{
		Draw_TextXY(mPosX*32,mPosY*32,"2",0xffff0000,1);
	}
	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_Block::Control()
{
	 C_Map* map = GET_OBJECT( C_Map* ,C_Stage::OBJ_MAP);

	if(keyflag)
	{
		//if(CheckPush(KEY_RIGHT))
		//{
		//	/* �E���ǂ���Ȃ��� */
		//	if (map->GetMapArray(static_cast<int>(mPosX + 1), static_cast<int>(mPosY)) == 0)
		//	{
		//		mPosX++;
		//	}
		//}
		if(CheckPush(KEY_LEFT))
		{
			/* �����ǂ���Ȃ��� */
			if (map->GetMapArray(static_cast<int>(mPosX - 1), static_cast<int>(mPosY)) == 0)
			{
				mPosX--;
			}
		}
	}
	/* �u���b�N�𗎂Ƃ� */
	if(CheckPress(KEY_DOWN) || timercnt++ >= 20)
	{
		timercnt = 0;
		/* �����ǂ���Ȃ��� */
		if (map->GetMapArray(static_cast<int>(mPosX), static_cast<int>(mPosY + 1)) == 0)
		{
			mPosY++;
		}
	}
	/* �������܂��Ă��鎞 */
	if ((map->GetMapArray(static_cast<int>(mPosX), static_cast<int>(mPosY + 1)) == 1) || (map->GetMapArray(static_cast<int>(mPosX), static_cast<int>(mPosY + 1)) == 2))
	{
		if (mPosY == 0)
		{
			return false;
		}
		if (keyflag)
		{
			map->SetMapArray(static_cast<int>(mPosX),static_cast<int>(mPosY),2);
			map->SetARGBArray(static_cast<int>(mPosX),static_cast<int>(mPosY),randomARGB[r_ARGB]);
			keyflag = false; /* �L�[��~ */
		}
	}
	/* �o�����Ă���u���b�N�����ׂĎ~�܂����� */
	if (!keyflag)
	{
		C_Base*  b = new C_Block("res/txt/Stage/back.txt", 0, 0, BLOCK_1);
		SET_OBJECT(b,C_Stage::OBJ_BLOCK1);

		return true;
	}

	return false;
}

//------------------------------------------------------------
//	�����(�A�s��)�擾
//------------------------------------------------------------
int C_Block::GetKeyFlag()
{
	return keyflag;
}
