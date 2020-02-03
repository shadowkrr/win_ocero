//******************************************************************************
//	�t�@�C����	�F	StageMap.cpp
//	�쐬��		�F	�e�R�@���
//	�쐬��		�F	2011/10/23
//	�@�\		�F	�}�b�v
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageMap.h"
#include	<string>
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageScore.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_Map::C_Map(const char* fileName/*, const int stageCnt*/) : C_Base(fileName)
{

	BlockNum = 0;
	WhiteNum = 0;
	bingocnt = 0;
	InitMap();

	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Map\n");
}
//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_Map::~C_Map()
{
	OutputDebugString("destructor:C_Map\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_Map::Draw()
{

	BlockNum = 0;
	WhiteNum = 0;
	char str[32];

	for (int y = 0; y < mHeightSize; y++)
	{
		for (int x = 0; x < mWidthSize; x++)
		{
			//if ((mMap[((y ) * mWidthSize) + (x )]) == 0)
			{
				Draw_Box(x*blocksize,y*blocksize,x*blocksize+blocksize,y*blocksize+blocksize,0xff00ff00,0xff000000,1,1,0.5);
			}
			if ((mMap[((y ) * mWidthSize) + (x )]) == 1)
			{
				WhiteNum++;
				//Draw_Box(x*blocksize,y*blocksize,x*blocksize+blocksize,y*blocksize+blocksize,0xffffffff,0xff000000,1,1,0.5);
				Draw_Ellipse(x*blocksize,y*blocksize,x*blocksize+blocksize,y*blocksize+blocksize,0xffffffff,0xff000000,1,1,0.5);
			}
			if ((mMap[((y ) * mWidthSize) + (x )]) == 2)
			{
				BlockNum++;
//				Draw_Box(x*blocksize,y*blocksize,x*blocksize+blocksize,y*blocksize+blocksize,0xff000000,0xff000000,1,1,0.5);
				Draw_Ellipse(x*blocksize,y*blocksize,x*blocksize+blocksize,y*blocksize+blocksize,0xff000000,0xff000000,1,1,0.5);
			}
		}
	}

	sprintf(str,"White = %d",WhiteNum);
	Draw_TextXY(500,WINH/4 + 32 * 4,str,0xffff0000,1);
	sprintf(str,"Block = %d",BlockNum);
	Draw_TextXY(500,WINH/4 + 32 * 5,str,0xffff0000,1);
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_Map::Control()
{
	return false;
}

//------------------------------------------------------------
//	�}�b�v������
//------------------------------------------------------------
void C_Map::InitMap()
{
	std::string MapName[4] = 
	{
		"res/txt/stage/map1chip.txt","res/txt/stage/map2chip.txt","res/txt/stage/map3chip.txt","res/txt/stage/map4chip.txt"
	}; 
	
	std::string IsHitMapName[4] = 
	{
		"res/txt/stage/IsHitMap1chip.txt","res/txt/stage/IsHitMap2chip.txt","res/txt/stage/IsHitMap3chip.txt","res/txt/stage/IsHitMap4chip.txt"
	};
	std::string mapName = MapName[0];
	std::string ismapName = IsHitMapName[0];

	Load(mapName.c_str(),mMap);			//�}�b�v
	//Load(mapName.c_str(),mIsHitMap);	//�}�b�v�̓����蔻��vecter
	LoadIsHitMap(ismapName.c_str());	//�}�b�v�̓����蔻��񎟌��z��
}

//------------------------------------------------------------
//	�}�b�v�ǂݍ���
//------------------------------------------------------------
bool C_Map::Load(const char* fileName, std::vector<int>& map)
{
	FILE* fp = NULL;	
	int temp = 0;		

	fopen_s(&fp, fileName, "r" );
	
	//�ǂݍ��ݎ��s
	if( fp == NULL )
	{
		MessageBox( hWnd, "�ǂݍ��ݎ��s", __FUNCTION__, MB_OK );
		return false;
	}
	//�ǂݍ��ݐ���
	else
	{	
		//�}�b�v�T�C�Y
		fscanf_s( fp, "%d, %d", &mWidthSize, &mHeightSize);

		if( feof( fp ) )
		{
			fclose( fp );	
			return true;	
		}
		else
		{
			//�}�b�v�T�C�Y��
			for(int i = 0; i < mHeightSize * mWidthSize; ++i){
				fscanf_s( fp, "%d,", &temp );
				map.push_back(temp);
				mARGBMap.push_back(0xffaaaaaa);	/* �F�̏����� */
			}
			
			fclose( fp );	
		}
	}
	return true;
}
//------------------------------------------------------------
//�t�@�C������ǃf�[�^��ǂݍ���
//------------------------------------------------------------
bool C_Map::LoadIsHitMap(const char* filename/*,const int MAPY, const int MAPX*/)
{
	FILE* fp;

	fopen_s(&fp, filename, "r");
	//fp = fopen(fileName, "r");

	//�ǂݍ��ݎ��s
	if(fp == NULL)
	{
		MessageBox(hWnd, "�ǂݍ��ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	////�ǂݍ��ݐ���
	else
	{
		//�}�b�v�f�[�^���R�s�[
		for(int y = 0; y < BLOCKY; y++)
		{
			for(int x = 0; x < BLOCKX; x++)
			{
				fscanf_s(fp, "%d,", &IsHitmap[y][x]);	//�t�@�C������}�b�v�ϐ��֑��
			}
			
		}
		
	}
	fclose(fp);	//�t�@�C�������
	return true;
}
//------------------------------------------------------------
//�t�@�C������A�C�e���f�[�^��ǂݍ���
//------------------------------------------------------------
bool C_Map::LoadItem(const char* filename)
{
	FILE* fp;

	fopen_s(&fp, filename, "r");
	//fp = fopen(fileName, "r");

	//�ǂݍ��ݎ��s
	if(fp == NULL)
	{
		MessageBox(hWnd, "�ǂݍ��ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	////�ǂݍ��ݐ���
	else
	{
		//�}�b�v�f�[�^���R�s�[
		for(int y = 0; y < BLOCKY; y++)
		{
			for(int x = 0; x < BLOCKX; x++)
			{
				fscanf_s(fp, "%d,", &Item[y][x]);	//�t�@�C������A�C�e���ϐ��֑��
			}
			
		}
	}
	fclose(fp);	//�t�@�C�������
	return true;
}
//------------------------------------------------------------
//�}�b�v�̏����擾
//------------------------------------------------------------
int C_Map::GetMapArray(const int x, const int y)
{
	if (0 > x && 0 > y && x < mWidthSize && y < mHeightSize)
	{
		return -1;
	}

	return (mMap[((y ) * mWidthSize) + (x )]);
}
//------------------------------------------------------------
//�}�b�v�̏��Ƀf�[�^���Z�b�g
//------------------------------------------------------------
void C_Map::SetMapArray(const int x, const int y, const int date)
{
	if (0 > x && 0 > y && x < mWidthSize && y < mHeightSize)
	{
		return;
	}
	(mMap[((y ) * mWidthSize) + (x )]) = date;
}
//------------------------------------------------------------
//�}�b�v�̏���ARGB���擾
//------------------------------------------------------------
int C_Map::GetARGBArray(const int x, const int y)
{
	if (0 > x || 0 > y)
	{
		return 0;
	}
	return (mARGBMap[((y ) * mWidthSize) + (x )]);
}

//------------------------------------------------------------
//�}�b�v�̏���ARGB���Z�b�g
//------------------------------------------------------------
void C_Map::SetARGBArray(const int x, const int y, const int date)
{
	if (0 > x || 0 > y)
	{
		return;
	}
	(mARGBMap[((y ) * mWidthSize) + (x )]) = date;
}
//------------------------------------------------------------
// �}�b�v�̓����蔻��
//------------------------------------------------------------
int C_Map::GetIsHitMap(int y,int x)
{
	return IsHitmap[y][x];
}
//------------------------------------------------------------
//	���]�`�F�b�N
//------------------------------------------------------------
bool C_Map::ReverceCheck(int x, int y, int ocerono)
{

	int cnt = 0;

	if (DownCheck(x, y, ocerono, 0))
	{
		//return true;
		cnt++;
	}
	if (UpCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (RightCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (LeftCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (RightDownCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (RightUpCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (LeftDownCheck(x, y, ocerono, 0))
	{
		cnt++;
	}
	if (LeftUpCheck(x, y, ocerono, 0))
	{
		cnt++;
	}

	if(0 < cnt)
	{
		return true;
	}

	return false;
}
//------------------------------------------------------------
//	HeightCheck
//------------------------------------------------------------
bool C_Map::DownCheck(int x, int y, int ocerono,int cnt)
{
	if (y == 7)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x, y + 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x, y + 1) != ocerono && GetMapArray(x, y + 1) != 0)
	{
		cnt++;
		if (DownCheck(x, y + 1, ocerono, cnt))
		{
			SetMapArray(x, y + 1, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	HeightCheck
//------------------------------------------------------------
bool C_Map::UpCheck(int x, int y, int ocerono,int cnt)
{
	if (y == 0)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x, y - 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x, y - 1) != ocerono && GetMapArray(x, y - 1) != 0)
	{
		cnt++;
		if (UpCheck(x, y - 1, ocerono, cnt))
		{
			SetMapArray(x, y - 1, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	rightcheck
//------------------------------------------------------------
bool C_Map::RightCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 7)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x + 1, y) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x + 1, y) != ocerono && GetMapArray(x + 1, y) != 0)
	{
		cnt++;
		if (RightCheck(x + 1, y, ocerono, cnt))
		{
			SetMapArray(x + 1, y, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	leftcheck
//------------------------------------------------------------
bool C_Map::LeftCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 0)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x - 1, y) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x - 1, y) != ocerono && GetMapArray(x - 1, y) != 0)
	{
		cnt++;
		if (LeftCheck(x - 1, y, ocerono, cnt))
		{
			SetMapArray(x - 1, y, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	RightDownCheck
//------------------------------------------------------------
bool C_Map::RightDownCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 7 || y == 7)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x + 1, y + 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x + 1, y + 1) != ocerono && GetMapArray(x + 1, y + 1) != 0)
	{
		cnt++;
		if (RightDownCheck(x + 1, y + 1, ocerono, cnt))
		{
			SetMapArray(x + 1, y + 1, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	RightUpCheck
//------------------------------------------------------------
bool C_Map::RightUpCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 7 || y == 0)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x + 1, y - 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x + 1, y - 1) != ocerono && GetMapArray(x + 1, y - 1) != 0)
	{
		cnt++;
		if (RightUpCheck(x + 1, y - 1, ocerono, cnt))
		{
			SetMapArray(x + 1, y - 1, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	LeftDownCheck
//------------------------------------------------------------
bool C_Map::LeftDownCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 0 || y == 7)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x - 1, y + 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x - 1, y + 1) != ocerono && GetMapArray(x - 1, y + 1) != 0)
	{
		cnt++;
		if (LeftDownCheck(x - 1, y + 1, ocerono, cnt))
		{
			SetMapArray(x - 1, y + 1, ocerono);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
//	LeftUpCheck
//------------------------------------------------------------
bool C_Map::LeftUpCheck(int x, int y, int ocerono,int cnt)
{

	if (x == 0 || y == 0)
	{
		if (GetMapArray(x, y) == ocerono)
		{	if (cnt != 0)
			{
				return true;
			}	
		}
		return false;
	}

	if (GetMapArray(x - 1, y - 1) == ocerono)
	{	
		if (cnt != 0)
		{
			return true;
		}
	}

	if (GetMapArray(x - 1, y - 1) != ocerono && GetMapArray(x - 1, y - 1) != 0)
	{
		cnt++;
		if (LeftUpCheck(x - 1, y - 1, ocerono, cnt))
		{
			SetMapArray(x - 1, y - 1, ocerono);
			return true;
		}
	}

	return false;
}