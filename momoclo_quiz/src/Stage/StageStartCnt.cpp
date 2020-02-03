//******************************************************************************
//	�t�@�C����	�F	StageStartCnt.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/25
//	�@�\		�F	�X�^�[�g�J�E���g
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//------------------------------------------------------------
//	�Ă΂Ȃ���΂����Ȃ��w�b�_
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageStartCnt.h"
#include	"../sound.h"
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
C_StartCnt::C_StartCnt(const char* fileName) : C_Base(fileName)
{
	cnt = 0;
	cntF = false;
	degF = false;

	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_StartCnt\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_StartCnt::~C_StartCnt()
{
	OutputDebugString("destructor:C_StartCnt\n");
}

//------------------------------------------------------------
//	�`��
//------------------------------------------------------------
void C_StartCnt::Draw()
{
	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	����
//------------------------------------------------------------
bool C_StartCnt::Control()
{
	if(!cntF){
		if(mAngle <= -360){
			degF = true;
			mAngle = 0;
		}
		if(!degF){
			mAngle += mAddAngle;
		}
		if(degF){
		static float stopcnt;
		stopcnt += (1.0f/60);			//60�t���[��
			if(stopcnt >= 0.2f){
				mImgSize.left -= mImgSize.right;
				degF = false;
				stopcnt = 0;

//				if(mImgSize.left >= 0)
//				SetPlay_Se(SE_STAGECNT);
			}
		}
		if(mImgSize.left < 0){ 

			cntF = true;
//			SetPlay_Se(SE_STAGESTART);
		}
		else if (mImgSize.left < -mImgSize.right*2) return true;//�K�v���Ȃ��Ȃ���������
	}
	return false;
}
