//******************************************************************************
//	�t�@�C����	�F	Base.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/09/23
//	�@�\		�F	�I�u�W�F�N�g�p�̊��N���X
//------------------------------------------------------------------------------
//	�X�V����	�F	
//
//******************************************************************************
//==============================================================================//
//���C�u�����ȊO�ŕK�v�ƂȂ�w�b�_�[�t�@�C���̃C���N���[�h
//�\���̒�`���K�v�ƂȂ�̂œ����̃w�b�_�[�t�@�C�����L�q���Ă����B
//==============================================================================//
#include	"Base.h"
#include	<fstream>
#include	<sstream>
using namespace std;
//==============================================================================//
//�����Ŏg�p����ϐ��A�\���̂̐錾
//�܂��O���ɂ���ϐ����g�p�����ꍇ��extern�Ő錾����
//==============================================================================//
C_Base::S_CommandFunc C_Base::mComFunc[COMMAND_MAX] =
{
	{"#POS", &C_Base::PosFunc},
	{"#ADDPOS", &C_Base::AddPosFunc},
	{"#IMAGE_FILENAME", &C_Base::ImageFileNameFunc},
	{"#IMAGE_SIZE", &C_Base::ImageSizeFunc},
	{"#ANGLE", &C_Base::AngleFunc},
	{"#ADDANGLE", &C_Base::AddAngleFunc},
	{"#POINT", &C_Base::PointFunc},
	{"#SCALE", &C_Base::ScaleFunc},
	{"#ADDSCALE", &C_Base::AddScaleFunc},
	{"#ALPHACOLOR", &C_Base::AlphaColorFunc},
	{"#TIME", &C_Base::TimeFunc},
	{"#FADE", &C_Base::FadeFunc}
};

//==============================================================================//
//	�N���X�֐�
//==============================================================================//
//--------------------------------------------------------------------------------
//	�R���X�g���N�^
//--------------------------------------------------------------------------------
C_Base::C_Base() :
mPosX(0.0f),mPosY(0.0f),mPosZ(0.5f),mAddPx(0.0f),mAddPy(0.0f),mImage(-1),
mAngle(0.0f),mAddAngle(0.0f),mScaX(1.0f),mScaY(1.0f),mAddScaX(0.0f),mAddScaY(0.0f),mAlpha(255),mCol_r(255),mCol_g(255),mCol_b(255),
mBufTime(0.0f),mAddTime(0.0f),mEndTime(0.0f),mAddFade(0.0f)
{
	mImgSize.left = mImgSize.top = mImgSize.right = mImgSize.bottom = 0;
	mPt.x = mPt.y = 0;
	OutputDebugString("constructor:C_Base\n");
}

//--------------------------------------------------------------------------------
//	�R���X�g���N�^
//--------------------------------------------------------------------------------
C_Base::C_Base(const char* fileName) : 
mPosX(0.0f),mPosY(0.0f),mPosZ(0.5f),mAddPx(0.0f),mAddPy(0.0f),mImage(-1),
mAngle(0.0f),mAddAngle(0.0f),mScaX(1.0f),mScaY(1.0f),mAddScaX(0.0f),mAddScaY(0.0f),mAlpha(255),mCol_r(255),mCol_g(255),mCol_b(255),
mBufTime(0.0f),mAddTime(0.0f),mEndTime(0.0f),mAddFade(0.0f)
{
	mImgSize.left = mImgSize.top = mImgSize.right = mImgSize.bottom = 0;
	mPt.x = mPt.y = 0;
	LoadValue(fileName);
	OutputDebugString("constructor:C_Base\n");
}

//--------------------------------------------------------------------------------
//	�f�X�g���N�^
//--------------------------------------------------------------------------------
C_Base::~C_Base()
{
	OutputDebugString("destructor:C_Base\n");
}

//--------------------------------------------------------------------------------
//	���W(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::PosFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mPosX >> mPosY >> mPosZ;	
}

//--------------------------------------------------------------------------------
//	�ړ���(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::AddPosFunc(const std::string str, const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mAddPx >> mAddPy;
}

//--------------------------------------------------------------------------------
//	�摜�t�@�C����(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::ImageFileNameFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));
	sstr >> mImageFileName;	
}

//--------------------------------------------------------------------------------
//	�摜�T�C�Y(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::ImageSizeFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mImgSize.left >> mImgSize.top >> mImgSize.right >> mImgSize.bottom;	
}

//--------------------------------------------------------------------------------
//	�p�x(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::AngleFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mAngle;	
}

//--------------------------------------------------------------------------------
//	�p�x����(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::AddAngleFunc(const std::string str, const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));
	sstr >> mAddAngle;
}

//--------------------------------------------------------------------------------
//	��(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::PointFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mPt.x >> mPt.y;	
}

//--------------------------------------------------------------------------------
//	�{��(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::ScaleFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mScaX >> mScaY;	
}

//--------------------------------------------------------------------------------
//	�{������(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::AddScaleFunc(const std::string str, const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));	
	sstr >> mAddScaX >> mAddScaY;
}

//--------------------------------------------------------------------------------
//	alpha�ƐF(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::AlphaColorFunc(const string str,const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));
	sstr >> mAlpha >> mCol_r >> mCol_g >> mCol_b;
}

//--------------------------------------------------------------------------------
//	�^�C�}�[(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::TimeFunc(const std::string str, const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));
	sstr >> mBufTime >> mAddTime >> mEndTime;
}

//--------------------------------------------------------------------------------
//	�t�F�[�h��(�֐��|�C���^)
//--------------------------------------------------------------------------------
void C_Base::FadeFunc(const std::string str, const size_t t)
{
	stringstream sstr;	
	sstr.str(str.substr(t));
	sstr >> mAddFade;
}

//--------------------------------------------------------------------------------
//	�S�̂̃��[�h
//--------------------------------------------------------------------------------
bool C_Base::LoadValue(const char* fileName)
{
	fstream file;
	string str;
	size_t t = -1;

	file.open(fileName, ios::in);

	if(!file.is_open()){
		MessageBox( hWnd, "�ǂݍ��ݎ��s", __FILE__, MB_OK );
		return false;
	}
	else
	{
		while(!file.eof())
		{		
			getline(file, str);

			for(int i = 0; i < COMMAND_MAX; ++i)
			{
				string::size_type pos = str.find(mComFunc[i].command);

				if(pos != string::npos)
				{
					t = strlen(mComFunc[i].command.c_str()) + 1;
					(this->*mComFunc[i].func)(str,t);
					break;
				}
			}
		}
		file.close();
	}
	return true;
}

//--------------------------------------------------------------------------------
//	�摜�̎󂯎��(Draw_Bitmap())
//--------------------------------------------------------------------------------
void C_Base::GetDraw_Bitmap()
{
	Draw_Bitmap(static_cast<int>(mPosX), static_cast<int>(mPosY),
				  mImage, 
				  mImgSize.left, mImgSize.top,
				  mImgSize.right, mImgSize.bottom,
				  mPosZ);	
}

//--------------------------------------------------------------------------------
//	�摜�̎󂯎��(Draw_TpBitmap())
//--------------------------------------------------------------------------------
void C_Base::GetDraw_TpBitmap()
{
	Draw_TpBitmap(static_cast<int>(mPosX), static_cast<int>(mPosY),
				  mImage, 
				  mImgSize.left, mImgSize.top,
				  mImgSize.right, mImgSize.bottom,
				  static_cast<int>(mAngle), &mPt,
				  mScaX, mScaY,
				  static_cast<int>(mAlpha), static_cast<int>(mCol_r), static_cast<int>(mCol_g), static_cast<int>(mCol_b),
				  mPosZ);
}

//--------------------------------------------------------------------------------
//	�����̓����蔻��
//--------------------------------------------------------------------------------
//	hit : �q�b�g����
//--------------------------------------------------------------------------------
bool C_Base::IsInCollision(const C_Base::POSHIT hit)
{
	if(hit == C_Base::HIT_POSX){
		return (static_cast<int>(mPosX) <= 0 || static_cast<int>(mPosX) + static_cast<int>(mImgSize.right) >= WINW);
	}
	else if(hit == C_Base::HIT_POSY){
		return (static_cast<int>(mPosY) <= 0 || static_cast<int>(mPosY) + static_cast<int>(mImgSize.bottom) >= WINH);
	}
	return false;
}

//--------------------------------------------------------------------------------
//	�����蔻��
//--------------------------------------------------------------------------------
// c : ����̒l
//--------------------------------------------------------------------------------
bool C_Base::IsCollision(const C_Base& c)const
{
	return (static_cast<int>(mPosX)   < static_cast<int>(c.mPosX) + c.mImgSize.right  &&
			static_cast<int>(c.mPosX) < static_cast<int>(mPosX)   + mImgSize.right	  &&
			static_cast<int>(mPosY)   < static_cast<int>(c.mPosY) + c.mImgSize.bottom &&
			static_cast<int>(c.mPosY) < static_cast<int>(mPosY)   + mImgSize.bottom);
}

//--------------------------------------------------------------------------------
//	�ړ�
//--------------------------------------------------------------------------------
void C_Base::Move()
{
	mPosX += mAddPx;
	mPosY += mAddPy;
}

//--------------------------------------------------------------------------------
//	���Ԕ���
//--------------------------------------------------------------------------------
bool C_Base::IsTime()
{
	mBufTime += mAddTime;

	return (mBufTime >= mEndTime) ? true : false;
}

//--------------------------------------------------------------------------------
//	�t�F�[�h����
//--------------------------------------------------------------------------------
//	fade : �t�F�[�h���@
//--------------------------------------------------------------------------------
bool C_Base::IsFade(const C_Base::FADE fade)
{
	if(fade == C_Base::FADE_IN){
		mAlpha += mAddFade;
		if(mAlpha >= C_Base::ALPHA_MAX){
			mAlpha = C_Base::ALPHA_MAX;
			return true;
		}
	}
	else if(fade == C_Base::FADE_OUT){
		mAlpha -= mAddFade;
		if(mAlpha <= C_Base::ALPHA_MIN){
			mAlpha = C_Base::ALPHA_MIN;
			return true;
		}
	}
	return false;
}

//********************************************************************************
//	�ȉ��f�o�b�O�p
//********************************************************************************
#ifndef	NDEBUG
//--------------------------------------------------------------------------------
//	�l�̕`��(�f�o�b�O)
//--------------------------------------------------------------------------------
//	name		: �ϐ���
//	value		: �l
//	posX, posY	: �`����W
//--------------------------------------------------------------------------------
void C_Base::DrawOnceValueDebug(const std::string name, int value, const int& posX, const int& posY)
{
	std::string msg;
	stringstream ss;
	ss << value;

	msg += "�ϐ��� : ";
	msg += name;
	msg += " ";
	msg += "�l : ";
	msg +=  ss.str(); 
	//Draw_FontDrawText(posX,posY,const_cast<char*>(msg.c_str()),0xffffff00,0,0.1f);
	Draw_TextXY(posX,posY,const_cast<char*>(msg.c_str()),0xffffff00);
}

//--------------------------------------------------------------------------------
//	�l�̕`��(�f�o�b�O)
//--------------------------------------------------------------------------------
//	posX, posY : �`����W
//--------------------------------------------------------------------------------
void C_Base::DrawValueDebug(const int& posX, const int& posY)const
{
	const int MAIN_MSG_MAX = 27;
	const char msg[MAIN_MSG_MAX][50] = {
		"posX","posY","posZ","addPx","addPy","imageFileName","image","imgSize.left","imgSize.top","imgSize.right","imgSize.bottom",
		"angle","addAngle","pt.x","pt.y","scaX","scaY","addScaX","addScaY","alpha","col_r","col_g","col_b","timeBuffer","timeAdd","timeEnd","fadeAdd"};
	const float value[MAIN_MSG_MAX] = {
		mPosX,mPosY,mPosZ,mAddPx,mAddPy,static_cast<float>(mImage),static_cast<float>(mImage),
		static_cast<float>(mImgSize.left),static_cast<float>(mImgSize.top),static_cast<float>(mImgSize.right),static_cast<float>(mImgSize.bottom),
		mAngle,mAddAngle,static_cast<float>(mPt.x),static_cast<float>(mPt.y),mScaX,mScaY,mAddScaX,mAddScaY,
		mAlpha,mCol_r,mCol_g,mCol_b,mBufTime,mAddTime,mEndTime,mAddFade
	};
	
	const int offSetY = 12;
	const char c[4] = " = ";
	const string KEY_MSG[2] = {"U�L�[ : ���̊�{�I�u�W�F�N�g","I�L�[ : ��{�I�u�W�F�N�g�̎��̃I�u�W�F�N�g"};

	int i;

	//�I�u�W�F�N�g�̑S���̒l
	for(i = 0; i < MAIN_MSG_MAX; ++i)
	{
		std::ostringstream os;

		if(i != 5)
		{
			os << msg[i] << c << value[i];
		}
		else{
			os << msg[i] << c << mImageFileName;
		}

		Draw_TextXY(posX, posY + (i * offSetY), const_cast<char*>(os.str().c_str()), 0xffff0000, 0);
		//Draw_FontDrawText(posX, posY + (i * offSetY), const_cast<char*>(os.str().c_str()), 0xffff0000,0,0.1f);

	}

	//�L�[����}�j���A��
	for(int n = 0; n < 2; ++n)
	{
		Draw_TextXY(posX, (WINH - 30) - (n * 40) , const_cast<char*>(KEY_MSG[n].c_str()), 0xffffff00, 0);
		//Draw_FontDrawText(posX, (WINH - 30) - (n * 40) , const_cast<char*>(KEY_MSG[n].c_str()), 0xffffff00,0,0.1f);
	}
}

void C_Base::DrawHitBox()
{
	Draw_Box(static_cast<int>(mPosX), static_cast<int>(mPosY),
		static_cast<int>(mPosX) + static_cast<int>(mImgSize.right), static_cast<int>(mPosY) + static_cast<int>(mImgSize.bottom),
		0x00000000, 0xffff0000,4,0,0.1f);
}
#endif
//********************************************************************************
//�ȏ�f�o�b�O�p
//********************************************************************************
//********************************************************************************
//�ȉ�for_each�p
//********************************************************************************
//--------------------------------------------------------------------------------
//	�`��(for_each)
//--------------------------------------------------------------------------------
void FE_Draw(C_Base* c)
{
	c->Draw();
}

//--------------------------------------------------------------------------------
//	����(for_each)
//--------------------------------------------------------------------------------
void FE_Control(C_Base* c)
{
	c->Control();
}

//--------------------------------------------------------------------------------
//	�폜(for_each)
//--------------------------------------------------------------------------------
void FE_Delete(C_Base* c)
{
	if(c != NULL){
		delete c;
		c = NULL;
	}
}

//--------------------------------------------------------------------------------
//	�f�o�b�O(for_each)
//--------------------------------------------------------------------------------
void FE_Debug(C_Base* c)
{
	#ifndef	NDEBUG	
	c->DrawValueDebug();
	c->DrawHitBox();
	#endif
}

//********************************************************************************
//�ȏ�for_each�p
//********************************************************************************