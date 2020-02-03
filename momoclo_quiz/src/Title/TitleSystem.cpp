//******************************************************************************
//	�t�@�C����	�F	TitleSystem.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/15
//	�@�\		�F	�V�X�e���Ǘ�
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Title.h"
#include	"TitleSystem.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	�Ăт����w�b�_
//------------------------------------------------------------
#include	"TitleSelect.h"
//------------------------------------------------------------
//	�ϐ���錾
//------------------------------------------------------------
using namespace title;
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_System::C_System(const char* fileName) : C_Base(fileName)
 {
	//SetPlay_Bgm(BGM_TITLE);
	mFadeF = false;
	GetNowSelectF = false;

	//LoadTimeCnt("res/txt/timecnt.txt");
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
	const C_Select* sel = GET_OBJECT(const C_Select* ,C_Title::OBJ_SELECT);

	//if(!GetNowSelectF && CheckPush(KEY_SPACE) && !mFadeF)
	{
		mFadeF = true;
		//SetPlay_Se(SE_SELECT2);
	}

	if(mFadeF){
		if(IsFade(C_Base::FADE_IN)){

			SetStop_Bgm(BGM_TITLE);

			switch(sel->GetSelectStage())
			{
				case sel->SEL_START:	C_BaseScreen::SetScreenNo(C_BaseScreen::STAGE_SCREEN);		break;
				case sel->SEL_RANKING:	Sys_QuitAPI();//C_BaseScreen::SetScreenNo(C_BaseScreen::RANKING_SCREEN);		break;
				case sel->SEL_END:		Sys_QuitAPI();													break;
			}
		}
	}	
	return false;
}
//------------------------------------------------------------
//	�^�C���J�E���g�t�@�C������ǂݍ���
//  �^�C����0���ƃR���e�j���[�ł��Ȃ��悤�ɂ���
//------------------------------------------------------------
bool C_System::LoadTimeCnt(char* filename)
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
