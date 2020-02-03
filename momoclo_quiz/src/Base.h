//******************************************************************************
//	ファイル名	：	Base.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/09/23
//	機能		：	オブジェクト用の基底クラス()
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
	float mPosX, mPosY, mPosZ;				//座標
	float mAddPx, mAddPy;					//移動量
	IMGOBJ	mImage;							//画像
	std::string mImageFileName;				//画像ファイル名前
	RECT mImgSize;							//画像サイズ
	float mAngle;							//角度
	float mAddAngle;						//角度増減
	POINT mPt;								//回転軸
	float mScaX, mScaY;						//倍率
	float mAddScaX, mAddScaY;				//倍率増減
	float mAlpha, mCol_r, mCol_g, mCol_b;	//(透過,赤,緑,青)
	float mBufTime, mAddTime, mEndTime;		//タイマー
	float mAddFade;							//フェード量
	
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
//	以下デバッグ用
//********************************************************************************
#ifndef	NDEBUG	
	virtual void DrawOnceValueDebug(const std::string name, int value, const int& posX, const int& posY);
	virtual void DrawValueDebug(const int& posX = 0, const int& posY = 0)const;
	virtual void DrawHitBox();
#endif
//********************************************************************************
//以上デバッグ用
//********************************************************************************
};

//********************************************************************************
//以下for_each用
//********************************************************************************
extern void FE_Draw(C_Base* c);
extern void FE_Control(C_Base* c);
extern void FE_Delete(C_Base* c);
extern void FE_Debug(C_Base* c);

// for_each の引数を簡略化するテンプレート関数
template <typename T_container, typename T_function>
T_function for_each(T_container& rcontainer, T_function function) {
    return for_each(rcontainer.begin(), rcontainer.end(), function);
}

//********************************************************************************
//以上for_each用
//********************************************************************************
//==============================================================
#endif
//==============================================================