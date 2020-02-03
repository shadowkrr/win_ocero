//******************************************************************************
//	�t�@�C����	�F	StageSystem.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/27
//	�@�\		�F	�V�X�e���Ǘ�
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------

//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_System::C_System(const char* fileName/*,int stageno*/) : C_Base(fileName)
 {
	SetPlay_Bgm(BGM_STAGE);
	//mstageno = stageno;
	mFadeF = false;
	GetNowSelectF = false;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());

	OutputDebugString("constructor:C_System\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_System::~C_System()
{
	OutputDebugString("destructor:C_System\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_System::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_System::Control()
{
	if(!mFadeF){
		if(ClearCheck()){
			mFadeF = true;
//			SetPlay_Se(SE_SELECT2);
		}
		if(EndCheck()){
			if(IsFade(C_Base::FADE_IN)){
				SetStop_Bgm(BGM_STAGE);
				C_BaseScreen::SetScreenNo(C_BaseScreen::GAMEOVER_SCREEN);
			}
		}
	}
	if(mFadeF){ 
		if(IsFade(C_Base::FADE_IN)){
			SetStop_Bgm(BGM_STAGE);
			C_BaseScreen::SetScreenNo(C_BaseScreen::TITLE_SCREEN);
		}
	}
	
	return false;
}
//------------------------------------------------------------
//	�G���h�`�F�b�N
//	�v���C���̗̑͂�0�ŃQ�[���I�[�o�[
//------------------------------------------------------------
bool C_System::EndCheck()
{
	return false;
}
//------------------------------------------------------------
//	�N���A�`�F�b�N
//	�G�l�~�[�����ׂē|������Q�[���N���A
//------------------------------------------------------------
bool C_System::ClearCheck()
{
	if(CheckPress(KEY_SPACE))
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------
//	�t�@�C���ɏ�������
//------------------------------------------------------------
bool C_System::SaveStage(char* filename)
{
	FILE* fp;

	//char Buffer[256];
	//fopen_s�œǂݍ���ŏ㏑�����悤�Ƃ���ƃG���[���ł�
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//�������ݎ��s
	if(&fp == NULL)
	{
		MessageBox(hWnd, "�������ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	//�������ݐ���
	else
	{
			
	//	fprintf(fp,"%d %d \n",mcharacterno,mstageno);
		fclose(fp);
		return true;
	}
}
//------------------------------------------------------------
//	�^�C���J�E���g���t�@�C���ɏ�������
//------------------------------------------------------------
bool C_System::SaveTimeCnt(char* filename)
{
	//C_TimeCnt* timecnt = GET_OBJECT(C_TimeCnt* ,C_Stage::OBJ_TIMECNT);

	FILE* fp;

	//char Buffer[256];
	//fopen_s�œǂݍ���ŏ㏑�����悤�Ƃ���ƃG���[���ł�
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//�������ݎ��s
	if(&fp == NULL)
	{
		MessageBox(hWnd, "�������ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	//�������ݐ���
	else
	{
			
	//	fprintf(fp,"%f\n",timecnt->GetTimeCnt());
		fclose(fp);
		return true;
	}
}
//------------------------------------------------------------
//	�L�����N�^�[�X�e�[�^�X�̏�����
//------------------------------------------------------------
bool C_System::SaveStates(char* filename)
{
	//C_Player* player = GET_OBJECT(C_Player* ,C_Stage::OBJ_PLAYER);

	FILE* fp;

	//char Buffer[256];
	//fopen_s�œǂݍ���ŏ㏑�����悤�Ƃ���ƃG���[���ł�
	//fopen_s(&fp, filename, "w");
	fp = fopen(filename, "w");
	
	//�������ݎ��s
	if(&fp == NULL)
	{
		MessageBox(hWnd, "�������ݎ��s", __FUNCTION__, MB_OK);
		return false;
	}
	//�������ݐ���
	else
	{
		//fprintf(fp,"%f %f %f %f \n",player->GetHpMax(),player->GetBlastMax(),player->GetBlastMax(),player->GetMoveValue());
		fclose(fp);
		return true;
	}
}