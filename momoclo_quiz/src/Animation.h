#pragma once
#include	"utility\\Gp3Lib.h"

const int ANIME_MAX = 30;
const int ANIME_MOTION_MAX = 4;

enum ANIM_KINDS
{
	ANIM_NO,
	ANIM_DAM_YAZIRUSHI,
	ANIM_ICON_SET,
	ANIM_EFFECT_PLAYER,
	ANIM_EFFECT_ENEMY,
	ANIM_EXPLOSION,
	ANIM_KINDS_MAX,
};
//==========================================================//
//構造体の型定義
//==========================================================//
struct Animation
{
	int no;		//アニメ
	int time;	//切り替え時間
};

class CAnim
{
	ANIM_KINDS kind;				//使うアニメ
	int posX, posY;					//座標
	float posZ;						//座標
	int shiftPosX, shiftPosY;		//ずらし座標
	RECT imgSize;					//画像サイズ
	int motion;						//モーション
	int animNo;						//現在のアニメ
	int animCntBuf;					//アニメカウンタの溜め
	bool mControlF;					//制御フラグ
public:
	CAnim():posX(0),posY(0),posZ(0.5f),shiftPosX(0),shiftPosY(0),kind(ANIM_NO),motion(0),animNo(0),animCntBuf(0),mControlF(true){
		imgSize.left = imgSize.top = imgSize.right = imgSize.bottom = 0;
	}
	virtual ~CAnim(){}
	//初期化
	void SetAnim(int posX, int posY, int shiftPosX, int shiftPosy, int sX, int sY, int eX, int eY, ANIM_KINDS kind, int motion, float posZ = 0.5f);
	//描画
	void DispAnim(IMGOBJ img);
	//制御
	bool ControlAnim();
	//座標セット
	void SetPosXY(int posX, int posY){this->posX = posX, this->posY = posY;}
	//制御させるか
	void SetControlF(bool set){mControlF = set;};
};

//==========================================================//
//外部で使用する変数、構造体のextern宣言
//==========================================================//
extern Animation anim[ANIM_KINDS_MAX][ANIME_MOTION_MAX][ANIME_MAX];
//==========================================================//
//外部で使用する関数のextern宣言
//==========================================================//
