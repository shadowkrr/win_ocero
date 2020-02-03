//******************************************************************************
//	�t�@�C����	�F	StageTimeCnt.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/11/1
//	�@�\		�F	�^�C���J�E���g
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageTimeCnt.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"StageStartCnt.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_TimeCnt::C_TimeCnt(const char* fileName) : C_Base(fileName)
{
	LoadTimeCnt("res/txt/timecnt.txt");
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_TimeCnt\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_TimeCnt::~C_TimeCnt()
{
	OutputDebugString("destructor:C_TimeCnt\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_TimeCnt::Draw()
{
	mPosX = 672;
	mPosY = 536;
	char msg[256];

	sprintf_s(msg,"%.2f�b",timecnt);
	Draw_FontDrawText(static_cast<int>(mPosX),static_cast<int>(mPosY),msg,ARGB(255,255,255,255),3,0.2f);

	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_TimeCnt::Control()
{
	C_StartCnt* startcnt = GET_OBJECT(C_StartCnt* ,C_Stage::OBJ_STARTCNT);

	//�Q�[���X�^�[�g���Ă���Γ���
	if(startcnt->GetCntF()){
		timecnt += (1.0f/60);	//60�t���[�� 
	}
	return false;
}
//------------------------------------------------------------
//	�^�C���J�E���g�t�@�C������ǂݍ���
//------------------------------------------------------------
bool C_TimeCnt::LoadTimeCnt(char* filename)
{
	FILE* fp;

	char Buffer[256];

	//fopen_s(&fp, filename, "r");
	fp = fopen(filename, "r");

	//�ǂݍ��ݎ��s
	if(fp == NULL)
	{
		MessageBox(hWnd, "�ǂݍ��ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	//�ǂݍ��ݐ���
	else
	{

			fgets(Buffer,256,fp);							//��s�Âǂ݂���
		/*	if(Buffer[0] == 10){
				continue;
			}*/
			sscanf(Buffer,"%f",					//�f�[�^��U�蕪����
				&timecnt);

		return true;
	}
	fclose(fp);	//�t�@�C�������
}
