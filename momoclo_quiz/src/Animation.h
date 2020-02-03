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
//�\���̂̌^��`
//==========================================================//
struct Animation
{
	int no;		//�A�j��
	int time;	//�؂�ւ�����
};

class CAnim
{
	ANIM_KINDS kind;				//�g���A�j��
	int posX, posY;					//���W
	float posZ;						//���W
	int shiftPosX, shiftPosY;		//���炵���W
	RECT imgSize;					//�摜�T�C�Y
	int motion;						//���[�V����
	int animNo;						//���݂̃A�j��
	int animCntBuf;					//�A�j���J�E���^�̗���
	bool mControlF;					//����t���O
public:
	CAnim():posX(0),posY(0),posZ(0.5f),shiftPosX(0),shiftPosY(0),kind(ANIM_NO),motion(0),animNo(0),animCntBuf(0),mControlF(true){
		imgSize.left = imgSize.top = imgSize.right = imgSize.bottom = 0;
	}
	virtual ~CAnim(){}
	//������
	void SetAnim(int posX, int posY, int shiftPosX, int shiftPosy, int sX, int sY, int eX, int eY, ANIM_KINDS kind, int motion, float posZ = 0.5f);
	//�`��
	void DispAnim(IMGOBJ img);
	//����
	bool ControlAnim();
	//���W�Z�b�g
	void SetPosXY(int posX, int posY){this->posX = posX, this->posY = posY;}
	//���䂳���邩
	void SetControlF(bool set){mControlF = set;};
};

//==========================================================//
//�O���Ŏg�p����ϐ��A�\���̂�extern�錾
//==========================================================//
extern Animation anim[ANIM_KINDS_MAX][ANIME_MOTION_MAX][ANIME_MAX];
//==========================================================//
//�O���Ŏg�p����֐���extern�錾
//==========================================================//
