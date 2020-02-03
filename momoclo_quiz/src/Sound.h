#pragma once

//==========================================================//
//	�񋓌^��`
//==========================================================//
//���y
enum BGM_NO
{
	BGM_TITLE,
	BGM_STAGE,

	BGM_MAX
};

//�G�t�F�N�g
enum SE_NO
{
	SE_BOMB,
	SE_COPY,
	SE_MOVE,
	
	SE_MAX
};

//==========================================================//
//	�N���X�^��`
//==========================================================//
//==========================================================//
//�O���Ŏg�p����ϐ��A�\���̂�extern�錾
//==========================================================//
extern DSHOWOBJ		BgmTable[BGM_MAX];
extern DSOUNDOBJ	SeTable[SE_MAX];
//==========================================================//
//�O���Ŏg�p����֐���extern�錾
//==========================================================//
extern void		Load_Bgm();						//BGB�̃��[�h
extern void		Load_Se();						//SE�̃��[�h
extern void		SetStart_DShow();				//BGM���X�^�[�g�ʒu�ɃZ�b�g
extern void		Init_Sound();					//BGM�ESE������
extern void		EndCheck_Sound();				//BGM�ESE�I���m�F
extern void		Delete_Sound();					//BGM�ESE�j��
extern void		SetPlay_Bgm(BGM_NO name);		//BGM�𗬂�
extern void		SetStop_Bgm(BGM_NO name);		//BGM���~�߂�
extern void		SetPlay_Se(SE_NO name);			//SE�𗬂�
extern void		SetStop_Se(SE_NO name);			//SE���~�߂�
extern void		AllBgmStop();					//BGM�����ׂĒ�~