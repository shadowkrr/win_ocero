//******************************************************************************
//	�t�@�C����	�F	Base.h
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/09/23
//	�@�\		�F	�I�u�W�F�N�g�p�̊��N���X()
//------------------------------------------------------------------------------
//******************************************************************************
//==============================================================
#ifndef BASE_H_
#define BASE_H_
//==============================================================
#include	"utility/Gp3Lib.h"
#include	<string>

class C_Base{
public:
	enum{TIME_RESET = 0};
	enum{ALPHA_MIN = 0, ALPHA_MAX = 255};
	enum{ANGLE_ZERO = 0, ANGLE_HALF = 180, ANGLE_MAX = 360};
	enum POSHIT{HIT_POSX,HIT_POSY};
	enum FADE{FADE_IN, FADE_OUT};
private:
	struct S_CommandFunc
	{
		std::string command;
		void (C_Base::*func)(const std::string str, const size_t t);
	};
	static const int COMMAND_MAX = 12;
	static C_Base::S_CommandFunc mComFunc[COMMAND_MAX];

	void PosFunc(const std::string str, const size_t t);
	void AddPosFunc(const std::string str, const size_t t);
	void ImageFileNameFunc(const std::string str, const size_t t);
	void ImageSizeFunc(const std::string str, const size_t t);
	void AngleFunc(const std::string str, const size_t t);
	void AddAngleFunc(const std::string str, const size_t t);
	void PointFunc(const std::string str, const size_t t);
	void ScaleFunc(const std::string str, const size_t t);
	void AddScaleFunc(const std::string str, const size_t t);
	void AlphaColorFunc(const std::string str, const size_t t);
	void TimeFunc(const std::string str, const size_t t);
	void FadeFunc(const std::string str, const size_t t);

protected:
	float mPosX, mPosY, mPosZ;				//���W
	float mAddPx, mAddPy;					//�ړ���
	IMGOBJ	mImage;							//�摜
	std::string mImageFileName;				//�摜�t�@�C�����O
	RECT mImgSize;							//�摜�T�C�Y
	float mAngle;							//�p�x
	float mAddAngle;						//�p�x����
	POINT mPt;								//��]��
	float mScaX, mScaY;						//�{��
	float mAddScaX, mAddScaY;				//�{������
	float mAlpha, mCol_r, mCol_g, mCol_b;	//(����,��,��,��)
	float mBufTime, mAddTime, mEndTime;		//�^�C�}�[
	float mAddFade;							//�t�F�[�h��
	
	void GetDraw_Bitmap();
	void GetDraw_TpBitmap();
	virtual void Move();
	virtual bool IsTime();
	virtual bool IsFade(const C_Base::FADE fade);
	bool IsInCollision(const C_Base::POSHIT hit);
public:
	explicit C_Base();
	C_Base(const char* fileName);
	virtual ~C_Base();

	virtual void Draw() = 0;
	virtual void Create() = 0;
	virtual bool Control() = 0;

	const float GetPosX()const{return mPosX;}
	const float GetPosY()const{return mPosY;}
	const float GetAddPx()const{return mAddPx;}
	const float GetAddPy()const{return mAddPy;}
	const float GetAngle()const{return mAngle;}
	const RECT GetImgSize()const{return mImgSize;}

	virtual bool LoadValue(const char* fileName);

	bool IsCollision(const C_Base& c)const;
//********************************************************************************
//	�ȉ��f�o�b�O�p
//********************************************************************************
#ifndef	NDEBUG	
	virtual void DrawOnceValueDebug(const std::string name, int value, const int& posX, const int& posY);
	virtual void DrawValueDebug(const int& posX = 0, const int& posY = 0)const;
	virtual void DrawHitBox();
#endif
//********************************************************************************
//�ȏ�f�o�b�O�p
//********************************************************************************
};

//********************************************************************************
//�ȉ�for_each�p
//********************************************************************************
extern void FE_Draw(C_Base* c);
extern void FE_Control(C_Base* c);
extern void FE_Delete(C_Base* c);
extern void FE_Debug(C_Base* c);

// for_each �̈������ȗ�������e���v���[�g�֐�
template <typename T_container, typename T_function>
T_function for_each(T_container& rcontainer, T_function function) {
    return for_each(rcontainer.begin(), rcontainer.end(), function);
}

//********************************************************************************
//�ȏ�for_each�p
//********************************************************************************
//==============================================================
#endif
//==============================================================