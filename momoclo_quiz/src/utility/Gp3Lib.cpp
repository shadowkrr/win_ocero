///////////////////////////////////////////////////////////////////
//
//	�Q�[���v���O�����V
//	�@�@�@�@�@���Ɨp���C�u����
//		 �@�@�@�@�@�@Gp3Lib
//								2011.2.6  Created by K.Koyanagi
//
//-----------------------------------------------------------------
// �X�V����
//		2011.7.1	�o�O�C����
//			�L�[�p�b�h�������̃}�E�X�{�^����Ɨ�
//			CheckPress,CheckFree��interval�����Ȃ��Ȃ��Ă����s��̏C��
//			�����`���Z���w�莞�̕s��̏C��
//			�f�o�b�O�֐��Ăяo���I���t���O�̃R�����g�C��
//		2011.8.28
//			Draw_LoadObject�@format�̎w��������ɕύX
//			CreateDevice�@MULTITHREAD�t���O��ǉ�
//			�t���X�N���[�����̕s����C��
//			�p�b�h�𕡐��������Ƃ��̕s����C��
///////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_DEPRECATE				1
#pragma warning(disable : 4995)	

#include	<windows.h>

#include	"Gp3Lib.h"
//���\�[�X�e�[�u���̓ǂݍ��݂̂���
#include	"../define.h"
//===================================================
//���ʕϐ��錾
HWND	hWnd;

//===================================================
//�����ϐ��錾
//D3Draw �֘A
LPDIRECT3D9				pD3D;
D3DPRESENT_PARAMETERS	D3DPP;
LPDIRECT3DDEVICE9		pD3DDevice;
LPD3DXSPRITE			pSprite;
LPD3DXFONT				pD3DXFont;

//FONT�֘A
#define	DDFONTMAX	100
LPD3DXFONT			pFontData[DDFONTMAX];

//-------------------------------------------------------
//�t���[���^�C���v�Z�֘A
DWORD		old_time;
float		FrameTime;

//-------------------------------------------------------
//�ėp�ϐ�
char		Buffer[1024];

//-------------------------------------------------------
//���[�f�B���O�X���b�h�֘A
BOOL _LoadEnd;

//-------------------------------------------------------
//Graphics�֘A
#define		IMGTABLEMAX		100
static D3DTEX9		D3DTextureObj[IMGTABLEMAX];

//-------------------------------------------------------
//BGM�Đ��֘A
#define	DSHOWOBJ		int
#define DSHOWMAX		10
static struct tagDxShow{
	IGraphBuilder	*pBuilder;
	IMediaControl	*pMediaControl;
	IMediaEvent		*pMediaEvent;
	IMediaSeeking	*pMediaSeeking;
	int				use;
}DShowObj[DSHOWMAX];

//-------------------------------------------------------
//SE�Đ��֘A
// DirectSound8
LPDIRECTSOUND8          lpDS;   
// �v���C�}���T�E���h�o�b�t�@
LPDIRECTSOUNDBUFFER		lpPrimary;

#define	DSOUNDOBJ			int
//���������ł���ő吔	
#define					DSPLAYMAX	10
enum					{DSNONE,DSSTART,DSPLAY,DSSTOP,DSEND};
struct tagPlayBuffer{
	LPDIRECTSOUNDBUFFER     PlayBuffer;		// �v���C�}���T�E���h�o�b�t�@
	int						ID;				//�x�[�X�ɂȂ����o�b�t�@��ID
	WORD					BufferSize;		//�x�[�X�ɂȂ����o�b�t�@�̃T�C�Y
	int						State;			//�o�b�t�@�̏��
	BOOL					Loop;			//���[�v�t���O
};

//����ǂݍ��߂�ő吔
#define					DSFILEMAX	30
struct tagSoundBuffer{
	LPDIRECTSOUNDBUFFER     SoundBuffer;	//�Z�J���_���o�b�t�@
	WORD					BufferSize;		//�o�b�t�@�̃T�C�Y
	DWORD					Fre;
	struct tagPlayBuffer	PlayBuffer[DSPLAYMAX];
}SoundBuffer[DSFILEMAX];

//=======================================================================//
//
//						�֐���`��
//
//=======================================================================//

//********************************************************************//
//
//						���w�֘A
//
//********************************************************************//

//-------------------------------------------------------------//
//��`���m�̏d�Ȃ�𒲂ׂ�
//-------------------------------------------------------------//
BOOL RectCheck(const RECT *rt1,const RECT *rt2)
{
	if(rt1->left < rt2->right && rt2->left < rt1->right && 
		rt1->top < rt2->bottom && rt2->top < rt1->bottom)
	{
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------//
//��X�y�э���Y�̒l����p�x�Ƃ����߁ACosSin�̒l�����߂�
//�߂�l�@���W�A��
//-------------------------------------------------------------//
// ��
//float rad = CalcXYToSinCos(30,30);
//-------------------------------------------------------------//
float Calc_XYToRad(int x,int y)
{
	return static_cast<float>(atan2((double)y,(double)x));
}

//-------------------------------------------------------------//
//��X�y�э���Y�̒l����p�x�Ƃ����߁ACosSin�̒l�����߂�
//�߂�l�@���W�A��
//-------------------------------------------------------------//
// ��
//float rad = CalcXYToSinCos(30,30,&AddPx,&AddPy);
//-------------------------------------------------------------//
float Calc_XYToSinCos(int x,int y,float *addx,float *addy)
{
	float rad = static_cast<float>(atan2((double)y,(double)x));
	*addx = static_cast<float>(cos(rad));
	*addy = static_cast<float>(-sin(rad));

	return rad;
}

//-------------------------------------------------------------//
//���W�A���l����p�x�ɕϊ�����
//�߂�l�@float �p�x
//-------------------------------------------------------------//
//float deg = RadToDegree(r);
//-------------------------------------------------------------//
float Calc_RadToDegree(float rad)
{
	return static_cast<float>((-rad / 2.0f / PAI * 360.0f));
}

//-------------------------------------------------------------//
//�p�x���烉�W�A���l�ɕϊ�����
//�߂�l�@float ���W�A��
//-------------------------------------------------------------//
//float rad = DegreeToRad(180);
//-------------------------------------------------------------//
float Calc_DegreeToRad(float degree)
{
	return static_cast<float>((degree * 2.0f * PAI / 360.0f));
}

//********************************************************************//
//
//				�L�[�@�֘A�֐�
//
//********************************************************************//


//�L�[���
static KEYINFO g_key[2][KEY_MAX];

static KEYINFO padbase[KEY_MAX] =
{
	{JOY_DOWN,	 FREE_KEY, 0}, //DOWN,
	{JOY_LEFT,	 FREE_KEY, 0}, //LEFT,
	{JOY_UP,	 FREE_KEY, 0}, //UP,
	{JOY_RIGHT,	 FREE_KEY, 0}, //RIGHT,
	{JOY_BUTTON1,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
	{JOY_BUTTON2,FREE_KEY, 0}, //KEY_MOUSE_RBTN,
	{JOY_BUTTON3,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
	{JOY_BUTTON4,FREE_KEY, 0}, //KEY_MOUSE_RBTN,
	{JOY_BUTTON5,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
};

static KEYINFO keybase[KEY_MAX] =
{
	{VK_DOWN   ,FREE_KEY, 0},
	{VK_LEFT   ,FREE_KEY, 0},
	{VK_UP     ,FREE_KEY, 0},
	{VK_RBUTTON  ,FREE_KEY, 0},
	{VK_LBUTTON  ,FREE_KEY, 0},
	{VK_SPACE  ,FREE_KEY, 0},
	{VK_RETURN ,FREE_KEY, 0},
	{'U'	   ,FREE_KEY, 0},
	{'I'       ,FREE_KEY, 0},
};

//--------------------------------------------------------------------------------------------
//�@�������p�@�p�b�h�̐��擾
//--------------------------------------------------------------------------------------------
JOYINFO		*JoyInfo;
int		padcount;
void InitPlayerInput()
{
	//�p�b�h�J�E���g
	for(int i = 0 ; i < PLAYER_NUM ; i ++){
		JOYINFO		JoyInfoTemp;
		if(joyGetPos(i,&JoyInfoTemp) == JOYERR_NOERROR){
			padcount ++;
		}
	}
	//�������m��
	JoyInfo = (JOYINFO*)malloc(sizeof(JOYINFO)*padcount);
	//�p�b�h����
	for(int i = 0 ; i < PLAYER_NUM ; i ++){
		memcpy(g_key[i],keybase,sizeof(KEYINFO)*10);
		////�p�b�h�D��
		//if(i < padcount){
		//	memcpy(g_key[i],padbase,sizeof(KEYINFO)*10);
		//}else{
		//	memcpy(g_key[i],keybase,sizeof(KEYINFO)*10);
		//}
	}
}

//--------------------------------------------------------------------------------------------
//	�������p�@�p�b�h�f�[�^����
//--------------------------------------------------------------------------------------------
void DeletePlayerInput()
{
	free(JoyInfo);		
}
//--------------------------------------------------------------------------------------------
//	�������p�@�p�b�h�f�[�^�擾
//--------------------------------------------------------------------------------------------
BOOL JoyGet(int JoyID)
{
	//�W���C�X�e�B�b�N��Ԏ擾
	if(joyGetPos(JoyID,&JoyInfo[JoyID]) != JOYERR_NOERROR){
		return 0;
	}
	return 1;
}
//-------------------------------------------------------------//
//�@�������p�@�W���C�X�e�B�b�N�̃{�^��������Ԕ���
//		�����F	Joy�@�W���C�X�e�B�b�NID�@JOY�P�@JOY2�̂ǂ��炩
//				Code �{�^���ԍ��@BUTTON1�@���珇�ɂQ�C�R�C�S
//				Flag�@��Ԃ��󂯎��|�C���^�ϐ�
//-------------------------------------------------------------//
void JoyPressButton(int Joy,UINT Code,int *Flag)
{
	if ((JoyInfo[Joy].wButtons	&	Code)){
		if (*Flag==FREE_KEY)	*Flag=PUSH_KEY;
		else					*Flag=HOLD_KEY;
	}
	else{
		if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
		else									*Flag=FREE_KEY;
	}
}

//-------------------------------------------------------------//
//�@�������p�@�W���C�X�e�B�b�N�̏\���L�[������Ԕ���
//		�����F	Joy�@�W���C�X�e�B�b�NID�@JOY�P�@JOY2�̂ǂ��炩
//				Direct �����@JOY_UP,JOY_DOWN,JOY_LEFT,JOY_RIGHT�̂ǂꂩ
//				Flag�@��Ԃ��󂯎��|�C���^�ϐ�
//-------------------------------------------------------------//
void JoyPressXY(int Joy,int Direct,int *Flag)
{
	switch(Direct){
		case JOY_DOWN:
			if(JoyInfo[Joy].wYpos > 40000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;

		case JOY_UP:
			if(JoyInfo[Joy].wYpos < 2000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
		case JOY_RIGHT:
			if(JoyInfo[Joy].wXpos > 40000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
		case JOY_LEFT:
			if(JoyInfo[Joy].wXpos < 2000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
	}


}

//--------------------------------------------------------------------------------------------
//�����ꂽ�^�C�~���O���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
static bool IsHoldTiming(int now, int prev)
{
	return now == PUSH_KEY ||					//�������ꂽ�@��������
		   (prev != PUSH_KEY && prev != now);	//1�t���[���O�ɉ�����ĂȂ��A1�t���[���O�ƃX�e�[�^�X���Ⴄ�Ƃ�
}

//--------------------------------------------------------------------------------------------
//�͂Ȃ��ꂽ�^�C�~���O���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
static bool IsPullTiming(int now, int prev)
{
	return now == PULL_KEY ||					//���͂Ȃ��ꂽ�@��������
		   (prev != PULL_KEY && prev != now);	//1�t���[���O�ɂ͂Ȃ���ĂȂ��A1�t���[���O�ƃX�e�[�^�X���Ⴄ�Ƃ�
}

//--------------------------------------------------------------------------------------------
//�t���[���J�n���ɌĂсA�Q�[���Ŏg���L�[�̏�Ԃ𒲂ׂ܂�
//���t���[���K���Ă΂Ȃ���΂Ȃ�܂���
//--------------------------------------------------------------------------------------------
void CheckKey()
{
	int i, prevStatus;

	for(int playernum = 0 ; playernum < PLAYER_NUM ; playernum++)
	{
		//if(playernum < padcount){
		//	for(i = 0; i < KEY_MAX; i++)
		//	{
		//		//�p�b�h�ł��}�E�X�̃{�^���͔F��������
		//		if( i < 8 ){
		//			//�t���[���J�E���^�`�F�b�N
		//			if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
		//			else						g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

		//			//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
		//			prevStatus = g_key[playernum][i].status;
		//			JoyGet(playernum);
		//			if(g_key[playernum][i].code == JOY_DOWN || g_key[playernum][i].code == JOY_UP ||
		//				g_key[playernum][i].code == JOY_RIGHT || g_key[playernum][i].code == JOY_LEFT){
		//				JoyPressXY(playernum,g_key[playernum][i].code,&g_key[playernum][i].status);
		//			}else{
		//				JoyPressButton(playernum,g_key[playernum][i].code, &g_key[playernum][i].status);
		//			}
		//			//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
		//			if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
		//			else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
		//		}else{
		//			//�t���[���J�E���^�`�F�b�N
		//			if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
		//			else						g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

		//			//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
		//			prevStatus = g_key[playernum][i].status;
		//			Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

		//			//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
		//			if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
		//			else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
		//		}
		//	}
		//}else{
			for(i = 0; i < KEY_MAX; i++)
			{
				//�t���[���J�E���^�`�F�b�N
				if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
				else			  g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

				//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
				prevStatus = g_key[playernum][i].status;
				Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

				//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
				if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
				else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
			//}
		}
	}
}

//--------------------------------------------------------------------------------------------
//�L�[���u������Ă���v�����`�F�b�N���܂�
//interval�ŗ^�����t���[���P�ʂ�true���������Ă��܂�
//�y��z
//interval��1�Ȃ疈�t���[��true
//interval��2�Ȃ�2�t���[���Ɉ��true
//interval��5�Ȃ�5�t���[���Ɉ��true
//--------------------------------------------------------------------------------------------
bool CheckPress(int keyno, int interval,int playernum)
{
	interval = interval > 0 ? interval: 1;	//0���Z�΍�

	//�L�[�X�e�[�^�X���u�����ꂽ�v�u������Ă���v�̂ǂ��炩��
	//�L�[�������ꂽ�t���[������]�ݒ�ƈ�v���Ă��邩�H
	if((g_key[playernum][keyno].status == HOLD_KEY ||
		g_key[playernum][keyno].status == PUSH_KEY) &&
		g_key[playernum][keyno].frameHold % interval == 0)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[���u�͂Ȃ���Ă���v�����`�F�b�N���܂�
//interval�ŗ^�����t���[���P�ʂ�true���������Ă��܂�
//�y��z
//interval��1�Ȃ疈�t���[��true
//interval��2�Ȃ�2�t���[���Ɉ��true
//interval��5�Ȃ�5�t���[���Ɉ��true
//--------------------------------------------------------------------------------------------
bool CheckFree(int keyno, int interval,int playernum)
{
	interval = interval > 0 ? interval: 1;	//0���Z�΍�

	//�L�[�X�e�[�^�X���u�͂Ȃ��ꂽ�v�u�͂Ȃ���Ă���v�̂ǂ��炩��
	//�L�[�������ꂽ�t���[������]�ݒ�ƈ�v���Ă��邩�H
	if((g_key[playernum][keyno].status == FREE_KEY ||
		g_key[playernum][keyno].status == PULL_KEY) &&
		g_key[playernum][keyno].frameFree % interval == 0)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�����̃t���[���Łu�����ꂽ�v�����`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool CheckPush(int keyno,int playernum)
{
	if(g_key[playernum][keyno].status == PUSH_KEY)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�����̃t���[���Łu�͂Ȃ��ꂽ�v�����`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool CheckPull(int keyno,int playernum)
{
	if(g_key[playernum][keyno].status == PULL_KEY)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃X�e�[�^�X��Ԃ��܂�
//--------------------------------------------------------------------------------------------
int GetKeyStatus(int keyno,int playernum)
{
	return g_key[playernum][keyno].status;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃t���[����Ԃ��܂�
//--------------------------------------------------------------------------------------------
int GetKeyFrameHold(int keyno,int playernum)
{
	return g_key[playernum][keyno].frameHold;
}
int GetKeyFrameFree(int keyno,int playernum)
{
	return g_key[playernum][keyno].frameFree;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃t���[�������������܂�
//--------------------------------------------------------------------------------------------
void InitKeyInfo(int playernum)
{
	for(int i = 0; i < KEY_MAX; i++)
	{
		g_key[playernum][i].status = FREE_KEY;
		g_key[playernum][i].frameHold  = 0;
		g_key[playernum][i].frameFree  = 0;
	}
}

//--------------------------------------------------------------------------------------------
//�L�[�R���t�B�O
//--------------------------------------------------------------------------------------------
void SetKeyFormat(CODE_KEY key, DWORD keycode,int playernum)
{
	g_key[playernum][key].code	  = keycode;
	g_key[playernum][key].status = FREE_KEY;
	g_key[playernum][key].frameHold  = 0;
	g_key[playernum][key].frameFree  = 0;
}

//--------------------------------------------------------------------------------------------
//�}�E�X�J�[�\���̍��W���擾(�߂�l)
//--------------------------------------------------------------------------------------------
POINT GetMousePotision()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	return pt;
}

//--------------------------------------------------------------------------------------------
//�}�E�X�J�[�\���̍��W���擾(����)
//--------------------------------------------------------------------------------------------
void GetMousePotision(POINT* pt_ptr)
{
	GetCursorPos(pt_ptr);
	ScreenToClient(hWnd, pt_ptr);
}

//--------------------------------------------------------------------------------------------
//�L�[��Ԏ擾
//Code�@�擾�������L�[�̉��z�L�[�R�[�h�A�`�r�b�h�h�R�[�h
//Flag�@�L�[�̏�Ԃ��擾�������o�b�t�@�ւ̃|�C���^
//--------------------------------------------------------------------------------------------
void Key_GetKey(unsigned int Code,int* Flag)
{
	if ((GetKeyState(Code)&0x80)==0x80){
		if (*Flag==FREE_KEY){
			*Flag=PUSH_KEY;
		}else{
			*Flag=HOLD_KEY;
		}
	}
	else{
		if(*Flag==PUSH_KEY||*Flag==HOLD_KEY){ 
			*Flag=PULL_KEY;
		}else{
			*Flag=FREE_KEY;
		}
	}
}


//********************************************************************//
//
//				�L�[�@�֘A�֐��@�����܂�
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//�������p�@
//�@�ĕ`����s��
//--------------------------------------------------------------------------------------------
void Draw_ReDraw()
{
	pSprite->End();
	pD3DDevice->EndScene();
	pD3DDevice->BeginScene();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
#define Draw_ReDrawCheck(x) if((x&0xff000000)!=0xff000000)Draw_ReDraw();

//********************************************************************//
//
//				FONT�@�֘A�֐�
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//�t�H���g�̍쐬
//	Num			�t�H���g�ԍ�
//	size		�t�H���g�T�C�Y
//	fontname	�t�H���g�̖��O
//--------------------------------------------------------------------------------------------
//�g�p��
// void OnCreate(){
//		DD_CreateFont(0,30);	//�w��Ȃ��͂l�r�S�V�b�N
// }
//�g�p��@0�Ԃɐݒ肵���T�C�Y30�̃t�H���g�ŕ`�揈�����s��
//	DD_FontDrawText(0,0,"MainStage",RGB(255,255,0),0);
//
//--------------------------------------------------------------------------------------------
void Draw_CreateFont(int Num,int size,LPCTSTR	fontname)
{
	if(pFontData[Num] == NULL){
		 // �t�H���g�̍쐬
		D3DXCreateFont(pD3DDevice,			//LPDIRECT3DDEVICE9	lpD3DDev,
			size,							//UINT				Height,
			0,								//UINT				Width,
			FW_NORMAL,						//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			SHIFTJIS_CHARSET,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			fontname,						//LPCTSTR			pFacename,
			&pFontData[Num]						//LPD3DXFONT		*ppFont		&pD3DXFont
		);
	}else{
		BToMR("��x�쐬�����t�H���g�͍ēx�쐬�ł��܂���B");
		return;
	}
}


//--------------------------------------------------------------------------------------------
//�����`�� ��`�w��@Z���w�肠��
//	r		�����`��ʒu��`�w��
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//	
//--------------------------------------------------------------------------------------------
//�g�p��
//	DD_FontDrawText(0,0,100,30,"MainStage",D3DCOLOR_XRGB(255,255,0));
//--------------------------------------------------------------------------------------------
int Draw_FontDrawText(RECT rt,LPCSTR string,D3DCOLOR color,int Num,float z)
{
	Draw_ReDrawCheck(color);
	if(pFontData[Num] == NULL)	return 0;

	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXMATRIX		matDraw;					
	D3DXMatrixTranslation(&matDraw,0,0,z);
	pSprite->SetTransform(&matDraw);

	//�`��
	int res = pFontData[Num]->DrawText(pSprite,string,-1,&rt,DT_LEFT | DT_WORDBREAK,color);

	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);

	return res;
}

//--------------------------------------------------------------------------------------------
//�����`�� Z���w�肠��
//	x,y		�����`��ʒu
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//	z		Z���ʒu
//--------------------------------------------------------------------------------------------
//�g�p��
//	DD_FontDrawText(0,0,"MainStage",D3DCOLOR_XRGB(255,255,0),1,0.3f);
//--------------------------------------------------------------------------------------------
int Draw_FontDrawText(int x,int y,LPCSTR string,D3DCOLOR color,int Num,float z)
{
	Draw_ReDrawCheck(color);
	if(pFontData[Num] == NULL)	return 0;
	//�\���e�L�X�g�T�C�Y�v�Z
	TEXTMETRIC  TextMetric;    
    pFontData[Num]->GetTextMetrics(&TextMetric);
	int w = x+(TextMetric.tmMaxCharWidth*static_cast<int>(strlen(string)));
	int h = y+TextMetric.tmHeight;
	//�\���pRECT�쐬
	RECT	rt = {x,y,w,h};

	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXMATRIX		matDraw;					
	D3DXMatrixTranslation(&matDraw,0,0,z);
	pSprite->SetTransform(&matDraw);

	//�`��
	int res = pFontData[Num]->DrawText(pSprite,string,-1,&rt,DT_LEFT | DT_WORDBREAK,color);

	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);

	return res;
}

//--------------------------------------------------------------------------------------------
//�t�H���g�폜�@�����g�p
//--------------------------------------------------------------------------------------------
void Draw_DeleteFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		if(pFontData[i] != NULL){
			RELEASE(pFontData[i]);
		}
	}
}

//--------------------------------------------------------------------------------------------
//�t�H���g�e�[�u���������@�����g�p
//--------------------------------------------------------------------------------------------
void Draw_InitFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		pFontData[i]= NULL;
	}
}

//--------------------------------------------------------------------------------------------
//�����`��@�ȈՔ�
//	x,y		�����`��ʒu
//	string	�����̕�
//	color	�����F
//	t = 0	�t�H���g�ԍ�
//--------------------------------------------------------------------------------------------
//�g�p��
//	Draw_TextXY(0,0,"MainStage",RGB(255,255,0));
//	
//�t�H���g�ԍ��w�莞��OnCreate�ɂăt�H���g��p�ӂ��邱��
//void OnCreate()
//{
//		Draw_CreateFont(0,15);
//		Draw_CreateFont(1,30,"HG�n�p�p�߯�ߑ�");
//}
//
//	Draw_TextXY(0,0,"MainStage",RGB(255,255,0),1);
//--------------------------------------------------------------------------------------------
void Draw_TextXY(int x,int y,char* string,D3DCOLOR color,int t)
{
	Draw_FontDrawText(x,y,string,color,t);
}

//********************************************************************//
//
//				FONT�@�֘A�֐� �����܂�
//
//********************************************************************//

//********************************************************************//
//
//				�摜�`��@�֘A�֐�
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��\������ ���ڎw��
//	����
//		X1,Y1			�\���J�n���W
//		hBmp			�`�悵����BMP�̖��O
//		TempX1,TempY1	BMP����̎Q�ƍ��W
//		TempX2,TempY2	BMP����̕��A����
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_BitmapDirect(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z)
{
	//�`���`�v�Z
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR3	center;
	center=D3DXVECTOR3(static_cast<float>(X1),static_cast<float>(Y1),z);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	//�`��
	pSprite->Draw(D3DTextureObj[hImg],&rt,NULL,&center,D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

}
//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��\������
//	����
//		X1,Y1			�\���J�n���W
//		hBmp			�`�悵����BMP�̖��O
//		TempX1,TempY1	BMP����̎Q�ƍ��W
//		TempX2,TempY2	BMP����̕��A����
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_Bitmap(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z)
{
	//�`���`�v�Z
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR3	center;
	center=D3DXVECTOR3(static_cast<float>(X1),static_cast<float>(Y1),z);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	//�`��
	pSprite->Draw(D3DTextureObj[ImgTable[hImg]],&rt,NULL,&center,D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

}
//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��ǂݍ��� �ǂݍ��݈ʒu���w�肷��
//--------------------------------------------------------------------------------------------
//�߂�l�@
//�@�ǂݍ��܂ꂽLPDIRECT3DTEXTURE9�̔ԍ�
//int ImgNo			//�ǂݍ��ݐ���w�肷��
//char *FileName	//�ǂݍ��ރt�@�C����
//int Transparent	//���ߐF�̐ݒ�
//
//--------------------------------------------------------------------------------------------
IMGOBJ Draw_LoadObject(int ImgNo,char *FileName,D3DCOLOR Transparent)
{
	for(int i = 0 ; i < IMGTABLEMAX ; i ++){
		if(D3DTextureObj[ImgNo] == NULL){
			if(FAILED( D3DXCreateTextureFromFileEx(pD3DDevice,(LPCSTR)(FileName),D3DX_DEFAULT ,D3DX_DEFAULT ,
									D3DX_DEFAULT ,
									0 ,
									D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED,
									D3DX_FILTER_NONE,
									D3DX_DEFAULT,
									Transparent,
									//0xff000000,	//������ύX���邱�Ƃœ��ߐF��ύX���邱�Ƃ��ł��܂��B
									NULL,NULL,
									&D3DTextureObj[ImgNo]))){
				GetErrorString("�e�N�X�`���𐶐��ł��܂���");
			}
			return ImgNo;
		}
	}
	BToMR("�w��̔ԍ��ɓǂݍ��߂܂���ł����B�ԍ����m�F���Ă��������B");
	return -1;
}
		 
//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��ǂݍ���
//--------------------------------------------------------------------------------------------
//�߂�l�@
//�@�ǂݍ��܂ꂽLPDIRECT3DTEXTURE9�̔ԍ�
//char *FileName	//�ǂݍ��ރt�@�C����
//int Transparent	//���ߐF�̐ݒ�
//
//--------------------------------------------------------------------------------------------
IMGOBJ Draw_LoadObject(char *FileName,D3DCOLOR Transparent)
{
	for(int i = 0 ; i < IMGTABLEMAX ; i ++){
		if(D3DTextureObj[i] == NULL){
			if(FAILED( D3DXCreateTextureFromFileEx(pD3DDevice,(LPCSTR)(FileName),D3DX_DEFAULT ,D3DX_DEFAULT ,
									D3DX_DEFAULT ,
									0 ,
									D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED,
									D3DX_FILTER_NONE,
									D3DX_DEFAULT,
									Transparent,
									//0xff000000,	//������ύX���邱�Ƃœ��ߐF��ύX���邱�Ƃ��ł��܂��B
									NULL,NULL,
									&D3DTextureObj[i]))){
				GetErrorString("�e�N�X�`���𐶐��ł��܂���");
			}
			return i;
		}
	}
	BToMR("�e�N�X�`�����[�h���̌��E�𒴂��܂����B�e�[�u���𑝂₵�Ă�������");
	return -1;
}

//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��\������B�g��������
//	����
//		X1,Y1			�\���J�n���W
//		hBmp			�`�悵����BMP�̖��O
//		TempX1,TempY1	BMP����̎Q�ƍ��W
//		TempX2,TempY2	BMP����̕��A����
//		deg				��]�p�x
//		*pCenter		��]�̒��S
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_TpBitmap(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
				int		deg,		//��]�p
				POINT	*pCenter,	//��]���S
				float	ScaleX,		//�{��
				float	ScaleY,		//�{��
				u_char	a,			//�A���t�@�[�`�����l��
				u_char	r,			//��
				u_char	g,			//��
				u_char	b,			//��
				float	z)
{

	//�`���`�v�Z
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR2		center;
	if(pCenter==NULL)		center=D3DXVECTOR2(0,0);
	else{
		center=D3DXVECTOR2(static_cast<float>(pCenter->x),static_cast<float>(pCenter->y));
	}

	if(a != 255)Draw_ReDraw();

	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 ptCenter(0.0f, 0.0f, z);	//	�`��̊�l�̐ݒ�
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);	//	�\������ʒu���w��
	D3DXVECTOR2 draw(static_cast<float>(X1),static_cast<float>(Y1));	//	�`�����W�i���Z�O�j
	D3DXVECTOR2 scale(ScaleX, ScaleY);	//	�X�P�[�����O�W���i�{�����w��j

	D3DXMatrixTransformation2D(&matDraw, &center, 0.0f, &scale, 
						&center, D3DXToRadian(-deg), &draw);
	pSprite->SetTransform(&matDraw);
	//�`��
	pSprite->Draw(D3DTextureObj[ImgTable[hImg]],&rt,NULL,&ptCenter,D3DCOLOR_ARGB(a,r,g,b));
	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
}

//--------------------------------------------------------------------------------------------
//�r�b�g�}�b�v��\������B�g�������� ���ڎw��
//	����
//		X1,Y1			�\���J�n���W
//		hBmp			�`�悵����BMP�̖��O
//		TempX1,TempY1	BMP����̎Q�ƍ��W
//		TempX2,TempY2	BMP����̕��A����
//		deg				��]�p�x
//		*pCenter		��]�̒��S
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_TpBitmapDirect(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
				int		deg,		//��]�p
				POINT	*pCenter,	//��]���S
				float	ScaleX,		//�{��
				float	ScaleY,		//�{��
				u_char	a,			//�A���t�@�[�`�����l��
				u_char	r,			//��
				u_char	g,			//��
				u_char	b,			//��
				float	z)
{

	//�`���`�v�Z
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR2		center;
	if(pCenter==NULL)		center=D3DXVECTOR2(0,0);
	else{
		center=D3DXVECTOR2(static_cast<float>(pCenter->x),static_cast<float>(pCenter->y));
	}

	if(a != 255)Draw_ReDraw();

	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 ptCenter(0.0f, 0.0f, z);	//	�`��̊�l�̐ݒ�
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);	//	�\������ʒu���w��
	D3DXVECTOR2 draw(static_cast<float>(X1),static_cast<float>(Y1));	//	�`�����W�i���Z�O�j
	D3DXVECTOR2 scale(ScaleX, ScaleY);	//	�X�P�[�����O�W���i�{�����w��j

	D3DXMatrixTransformation2D(&matDraw, &center, 0.0f, &scale, 
						&center, D3DXToRadian(-deg), &draw);
	pSprite->SetTransform(&matDraw);
	//�`��
	pSprite->Draw(D3DTextureObj[hImg],&rt,NULL,&ptCenter,D3DCOLOR_ARGB(a,r,g,b));
	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
}
//--------------------------------------------------------------------------------------------
//��n���Ăяo��
//--------------------------------------------------------------------------------------------
void Draw_DeleteObject(D3DTEX9	obj)
{
	RELEASE(obj);
}

void Draw_DeleteObject(IMGOBJ	obj)
{
	RELEASE(D3DTextureObj[obj]);
}

//--------------------------------------------------------------------------------------------
//�摜�f�[�^�T�C�Y�擾
//--------------------------------------------------------------------------------------------
D3DSURFACE_DESC Draw_GetImageSize(IMGOBJ	obj){
	D3DSURFACE_DESC TexData;
	D3DTextureObj[obj]->GetLevelDesc(0,&TexData);
	return TexData;
}
//--------------------------------------------------------------------------------------------
//���ꍇ���J�n
//--------------------------------------------------------------------------------------------
//�g�p��
//	Draw_SetRenderMode(ADD);
//	Draw_Bitmap(100,100,Test,0,0,32,32,0,NULL,1.0,1.0,100,255,255,255);
//	Draw__EndRenderMode();
//--------------------------------------------------------------------------------------------
void Draw_SetRenderMode(int Mode)
{
	//Clear���Ɉ�xBegin����Ă���̂ň��I��点��
	pSprite->End();
	//������x
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if(Mode == DEC){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //�A���t�@�u�����f�B���O�̗L����
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRC�̐ݒ�
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DEST�̐ݒ�
	}else if(Mode == ADD){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //�A���t�@�u�����f�B���O�̗L����
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);   //SRC�̐ݒ�
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  //DEST�̐ݒ�
	}else if(Mode == HALFADD){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //�A���t�@�u�����f�B���O�̗L����
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);   //SRC�̐ݒ�
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DEST�̐ݒ�
	}
}

//--------------------------------------------------------------------------------------------
//������ʏI��
//--------------------------------------------------------------------------------------------
//�g�p��
//	Draw_SetRenderMode�Q��
//--------------------------------------------------------------------------------------------
void Draw_EndRenderMode()
{
	Draw_SetRenderMode(DEC);
}
//********************************************************************//
//
//				�摜�`��@�֘A�֐��@�����܂�
//
//********************************************************************//

//********************************************************************//
//
//				�}�`�@�֘A�֐�
//
//********************************************************************//
void Draw_GetRGB(COLORREF color1,int *r,int *g,int *b)
{
	*b = ((color1 & 0x00ff0000) >> 16);
	*g = ((color1 & 0x0000ff00) >> 8);
	*r = ((color1 & 0x000000ff) >> 0);
}

//--------------------------------------------------------------------------------------------
//�����g�p�@�|���S���`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//--------------------------------------------------------------------------------------------
void Draw_2DPolygon(int X1,int Y1,int X2,int Y2,float z,D3DCOLOR Color)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};
	D3DCOLOR c = Color;
	
	CUSTOMVERTEX v[4] = {
		{static_cast<float>(X2), static_cast<float>(Y1), z, 1.0f, c}, 
		{static_cast<float>(X2), static_cast<float>(Y2), z, 1.0f, c}, 
		{static_cast<float>(X1), static_cast<float>(Y1), z, 1.0f, c}, 
		{static_cast<float>(X1), static_cast<float>(Y2), z, 1.0f, c}
	}; 
	pD3DDevice->SetTexture(0,NULL);
	pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v , sizeof (CUSTOMVERTEX));
}

//--------------------------------------------------------------------------------------------
//��`�`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//		Color2			�g�F
//		Size			���C����
//		Nakanuri		�h��Ԃ����H
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_Box(int X1,int Y1,int X2,int Y2,
				D3DCOLOR Color1,D3DCOLOR Color2,
				int Size,int Nakanuri,float z)
{
	Draw_ReDrawCheck(Color1);
	if(Nakanuri){
		Draw_2DPolygon(X1,Y1,X2,Y2,z,Color1);
	}
	D3DXVECTOR2 vec[] =
	{
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y1)),	// ���C���̎n�_
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y2)),	// ���C��
		D3DXVECTOR2(static_cast<float>(X2), static_cast<float>(Y2)),	// ���C��
		D3DXVECTOR2(static_cast<float>(X2), static_cast<float>(Y1)),	// ���C��
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y1)),	// ���C���̏I�_
	};

	Draw_ReDrawCheck(Color2);

	for(int i = 0 ; i < 4 ; i ++){
		Draw_Line(static_cast<int>(vec[i].x),static_cast<int>(vec[i].y),static_cast<int>(vec[i+1].x),static_cast<int>(vec[i+1].y),
					Color2,Size,z);
	}
}

//--------------------------------------------------------------------------------------------
//�����`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color2			�F
//		Size			���C����
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_Line(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,int Size,float z)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};

	D3DCOLOR c = Color1;
	for(int i = 0; i < Size ; i++){
		int tx,ty;
		if(abs(X1-X2) > abs(Y1-Y2)){
			tx = 0;
			ty = i-Size/2;
		}else{
			tx = i-Size/2;
			ty = 0;
		}
		CUSTOMVERTEX v[2] = {
			{static_cast<float>(X1)+tx, static_cast<float>(Y1)+ty, z, 1.0f, c}, 
			{static_cast<float>(X2)+tx, static_cast<float>(Y2)+ty, z, 1.0f, c}, 
		}; 
		pD3DDevice->SetTexture(0,NULL);
		pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v , sizeof (CUSTOMVERTEX));
	}
}

//--------------------------------------------------------------------------------------------
//�~�`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//		Color2			�g�F
//		Size			���C����
//		Nakanuri		�h��Ԃ����H
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//--------------------------------------------------------------------------------------------
void Draw_Ellipse(int X1,int Y1,int X2,int Y2,
				D3DCOLOR Color1,D3DCOLOR Color2,
				int Size,int Nakanuri,float z)
{
	Draw_ReDrawCheck(Color1);
	//�F�쐬
	D3DCOLOR c = Color1;

	D3DXVECTOR2 vec[181];
	int rx = (X2-X1)/2;
	int ry = (Y2-Y1)/2;
	float x,y;
	for(int angle = 0 ; angle < 180 + 1; angle++)
	{
		x = static_cast<float>(cos(angle*2*3.1415*2/360)*rx+X1+rx);
		y = static_cast<float>(sin(angle*2*3.1415*2/360)*ry+Y1+ry);
		vec[angle] = D3DXVECTOR2(static_cast<float>(x),static_cast<float>(y));	// ���C���̎n�_
	}
	//�h��`��
	if(Nakanuri == 1){
		struct CUSTOMVERTEX {
				FLOAT x, y, z;
				FLOAT rhw;
				DWORD color;
		};

		CUSTOMVERTEX v[182];
		v[0].x = static_cast<float>(X1+rx);
		v[0].y = static_cast<float>(Y1+ry);
		v[0].z = z;
		v[0].rhw = 1.0f;
		v[0].color = c;
		for(int i = 1 ; i < 181+1  ; i++)
		{
			v[i].x = vec[i-1].x;
			v[i].y = vec[i-1].y;
			v[i].z = z;
			v[i].rhw = 1.0f;
			v[i].color = c;
		}
		pD3DDevice->SetTexture(0,NULL);
		pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 180, v , sizeof (CUSTOMVERTEX));
	}
	
	Draw_ReDrawCheck(Color2);
	//�~���`��
	for(int i = 1 ; i < 180; i++)
	{
		Draw_Line(static_cast<int>(vec[i].x),static_cast<int>(vec[i].y),static_cast<int>(vec[i+1].x),static_cast<int>(vec[i+1].y),Color2,Size,z);
	}	
	Draw_Line(static_cast<int>(vec[179].x),static_cast<int>(vec[179].y),static_cast<int>(vec[1].x),static_cast<int>(vec[1].y),Color2,Size,z);
}
//********************************************************************//
//
//				�}�`�@�֘A�֐��@�����܂�
//
//********************************************************************//

//********************************************************************//
//
//				�V�X�e���^�f�o�b�O�@�֘A�֐�
//
//********************************************************************//
//-------------------------------------------------------------//
//�t���X�N���[���؂�ւ�
//-------------------------------------------------------------//
//	void OnCreate(){
//		Sys_SetScreenMode();
//		InitDx();
//	}
//-------------------------------------------------------------//
void Sys_SetScreenMode()
{
	if(YESNO("�t���X�N���[���Ŏ��s���܂����H") == IDNO){
		WindowMode = TRUE;
	}else{
		WindowMode = FALSE;
		SetWindowLong(hWnd,GWL_STYLE,WS_POPUP | WS_VISIBLE);
	}
}

//--------------------------------------------------------------------------------------------
//�G���[������\��
//�@MB�ɂĒ��O�ɔ��������G���[���o��
//--------------------------------------------------------------------------------------------
//�g�p��
//	char Buff[256];
//	GetErrorString(Buff);
//--------------------------------------------------------------------------------------------
void GetErrorString(char* text)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	BToMR("%s\n%s",text,lpMsgBuf);
}

//--------------------------------------------------------------------------------------------
//DirectDraw������
//--------------------------------------------------------------------------------------------
void InitDx()
{
	//DX MAIN INIT
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(pD3D == NULL){
		BToMR("D3D Create Error");
		return;
	}

	//WINDOW MODE INIT
	ZeroMemory(&D3DPP,sizeof(D3DPP));
	if(WindowMode == TRUE){
		D3DPP.Windowed			= TRUE;							//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select
		D3DPP.BackBufferFormat	= D3DFMT_UNKNOWN;				//FORMAT AUTO Select
		D3DPP.BackBufferCount	= 1;
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}else{
		D3DPP.BackBufferWidth	= WINW;
		D3DPP.BackBufferHeight	= WINH;
		D3DPP.BackBufferCount	= 1;
		D3DPP.BackBufferFormat	= D3DFMT_X8R8G8B8;				//FORMAT Select
		D3DPP.Windowed			= FALSE;						//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.FullScreen_RefreshRateInHz = 60;
	}
	//Create Device
	HRESULT hr;
	//HAL(pure vp)
	if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
	{
		//HAL(soft vp)
		if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
		{
			//REF
			if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
			{
				BToMR("CreateDevice Error");
				return ;
//				END;
			}
		}
	}
	
	// ���ʕ`�惂�[�h�̎w��
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h 	
    pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//--------------------------------------
	// �e�N�X�`���X�e�[�W�̐ݒ�
	//--------------------------------------
	// �e�N�X�`���X�e�[�W�̐ݒ� 
	pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	// �e�N�X�`���t�B���^�[�̐ݒ�
	pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	D3DXCreateSprite(pD3DDevice,&pSprite);

}

//--------------------------------------------------------------------------------------------
//Dx���C�u�����̏������@
//--------------------------------------------------------------------------------------------
//	�v���O�����N�����ɌĂяo��
//--------------------------------------------------------------------------------------------
void InitDxLib()
{
	InitDx();
	//���[�U�[�t�H���g�w��
	Draw_InitFont();
	//�V�X�e���t�H���g�ݒ�

	for(int i = 0 ; i < IMGTABLEMAX;i++){
		D3DTextureObj[i] = NULL;
	}
}

//-------------------------------------------------------------//
//DirectDraw�I��
//-------------------------------------------------------------//
//�@�v���O�����I�����ɌĂяo������
//-------------------------------------------------------------//
void DelDxLib()
{
	for(int i = 0 ; i < IMGTABLEMAX;i++){
		if(D3DTextureObj[i] != NULL)
		{
			RELEASE(D3DTextureObj[i]);
		}
	}

	Draw_DeleteFont();
	RELEASE(pSprite);
	RELEASE(pD3DDevice);
	RELEASE(pD3D);
}


//--------------------------------------------------------------------------------------------
// FPS�̕\��
//--------------------------------------------------------------------------------------------
//�@DD_Clear();
//	DD_ShowFPS();
//	DDDFPS();		�ł���
//�@DD_Refresh();
//--------------------------------------------------------------------------------------------
void Draw_ShowFPS()
{
	//�T���v�����O��2�b�ɍL����
	static LONG FpsTime=(timeGetTime()+1000L);
	static int FpsData = 10;
	static int FpsCnt = 1;
	if (timeGetTime()>=(unsigned)FpsTime){
		FpsData=FpsCnt;
		FpsTime=(timeGetTime()+1000L);
		FpsCnt=1;
	}
	//FPS�J�E���g
	FpsCnt++;
	char Buffer[1024];	
	sprintf_s(Buffer,"fps %3d / FrameTime %0.3f",FpsData,FrameTime);
	Draw_TextXY(0,0,Buffer,ARGB(255,255,200,200),0);
}

//--------------------------------------------------------------------------------------------
//�v���O�����I��
//--------------------------------------------------------------------------------------------
void Sys_QuitAPI(){
	DShow_AllStop();	//���ׂĂ�BGM���I��
	SendMessage(hWnd,WM_CLOSE,0,0);

}

//--------------------------------------------------------------------------------------------
//�T�[�t�F�X�]��
//--------------------------------------------------------------------------------------------
void Sys_Refresh()
{
	if(pSprite != NULL)
		pSprite->End();
	//Draw End
	if(pD3DDevice != NULL)
		pD3DDevice->EndScene();
	//ReDraw
	HRESULT hr;
	if(pD3DDevice != NULL)
		hr = pD3DDevice->Present(NULL,NULL,NULL,NULL);

	if(hr == D3DERR_DEVICELOST){
		//��ԃ`�F�b�N
		if(pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			pSprite->OnLostDevice();
			//���Z�b�g
			hr = pD3DDevice->Reset(&D3DPP);
			if(hr != D3D_OK){
				return;
			}
			//�Ď擾
			pSprite->OnResetDevice();
		}
	
	}
}
//--------------------------------------------------------------------------------------------
//�I�t�X�N���[���̓��e�����ŏ���������
//--------------------------------------------------------------------------------------------
void Sys_Clear()
{
	//Scene Clear Black
	if(pD3DDevice != NULL)
		pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,64),1.0f,0);
	//Draw Start
	if(pD3DDevice != NULL)
		pD3DDevice->BeginScene();
	if(pSprite != NULL)
		pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}


//--------------------------------------------------------------------------------------------
//���[�v����
//--------------------------------------------------------------------------------------------
//�������p
//--------------------------------------------------------------------------------------------
BOOL Sys_LoopControl(int FPS){
	static int _Time = timeGetTime();	//�������Ԋi�[
	static float _FPS;					//FPS�v�Z�p�o�b�t�@
	if( FPS != 0){
		_FPS = 1000.0f / float(FPS);		//�^����ꂽFPS����FrameTime���v�Z	
		if(timeGetTime() - _Time > (unsigned)_FPS){		//FrameTime���o�߂���܂ő҂�
			_Time = timeGetTime();
			return 1;
		}
		return 0;
	}else{
		return 1;
	}
}

//--------------------------------------------------------------------------------------------
//���[�h�X���b�h
//--------------------------------------------------------------------------------------------
void Sys_LoadThread(HWND hWnd)
{
	OnLoading();
	InterlockedExchange((volatile LONG*)&_LoadEnd,1);
//	_LoadEnd = 1;
}

//--------------------------------------------------------------------------------------------
//���[�h�X���b�h�J�n
//--------------------------------------------------------------------------------------------
void Sys_CreateLoadThread()
{
	HANDLE newThread;
	InterlockedExchange((volatile LONG*)&_LoadEnd,0);
//	_LoadEnd = 0;
	newThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Sys_LoadThread,hWnd,0,NULL);
}

//--------------------------------------------------------------------------------------------
//WINDOW�@�v���V�[�W��
//--------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HRESULT res;
	static float tempFrame;
	switch (msg) {
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)		END;
			break;
		case WM_CREATE:
			res = timeBeginPeriod(1);
			break;
		case WM_PAINT:
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
			break;
        case WM_DESTROY:
			timeEndPeriod(1);
			PostQuitMessage(0); 
			break;
		default: return(DefWindowProc(hWnd, msg, wParam, lParam)); 
    }
    return (0L);
}

//--------------------------------------------------------------------------------------------
//WINDOW�@�ݒ�֐�
//--------------------------------------------------------------------------------------------
BOOL DoWindow(HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{

	MSG			lpMsg;
	WNDCLASS	myProg;
	LPCSTR		Name = "Window_Class_Name";
	if (!hPreInst) {                                        
		myProg.style            = CS_HREDRAW | CS_VREDRAW;
   	 	myProg.lpfnWndProc      = WndProc;
   	 	myProg.cbClsExtra       = 0;
   	 	myProg.cbWndExtra       = 0;
    	myProg.hInstance        = hInstance;
		myProg.hIcon            = LoadIcon(hInstance,"MAIN");
    	myProg.hCursor          = LoadCursor(NULL, IDC_ARROW);
    	myProg.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    	myProg.lpszMenuName     = NULL;
    	myProg.lpszClassName    = (Name);
    	if (!RegisterClass(&myProg)) return FALSE;
	}

	hWnd=CreateWindow(
		(Name)	, 
		(USERNAME),
		/*WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU	,*/
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_THICKFRAME | WS_TABSTOP | WS_MINIMIZEBOX | WS_POPUPWINDOW,
		0			,
		0			,

		WINW
		+GetSystemMetrics(SM_CXFRAME)*2			,
		WINH
		+GetSystemMetrics(SM_CYCAPTION)
		+GetSystemMetrics(SM_CYFRAME)*2			,

		NULL					,
		NULL					,
		hInstance				,
		NULL
	);
	//�E�B���h�E���[�h�̌���
	Sys_SetScreenMode();

	InitDxLib();			//Dx�֘A������
	DSound_Init();			//DSOUND������
	DShow_Init();			//DSHOW������
	OnCreate();				//�������֐�
	Sys_CreateLoadThread();	//���[�h�p�X���b�h�N��
	InitPlayerInput();		//���[�U�[���͏�����
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	while(true){
		if(PeekMessage(&lpMsg,NULL,0,0,PM_REMOVE)){			//���b�Z�[�W����
			if(lpMsg.message == WM_QUIT)	break;		
			TranslateMessage(&lpMsg);
			DispatchMessage(&lpMsg);
		}else if(Sys_LoopControl(60)){						//�Q�[�����[�v
			FrameTime = (timeGetTime()-old_time)/F(1000);	//FrameTime�v�Z
			old_time = timeGetTime();
			if(_LoadEnd){									//���C�����[�v�Ăяo��
				GameLoop();
				DSound_EndCheck();							//DSOUND�I���`�F�b�N
				DShow_EndCheck();							//DSHOW�I���`�F�b�N
			}else{											//���[�h��ʌĂяo��
				DrawLoading();
			}
		}
		Sleep(1);
	}

	/*while(true){
		if(PeekMessage(&lpMsg,NULL,0,0,PM_REMOVE)){
			if(lpMsg.message == WM_QUIT)	break;		
			TranslateMessage(&lpMsg);
			DispatchMessage(&lpMsg);
		}else Sys_MainThread(hwnd);
		Sleep(1);
	}*/
	OnDestroy();			//�Q�[����n������
	DSound_Del();			//DSOUND�j��
	DShow_Del();			//DSHOW�j��
	DeletePlayerInput();	//���[�U�[���͔j��
	DelDxLib();				//Dx�֘A�j��
	return(static_cast<int>(lpMsg.wParam));
}

//********************************************************************//
//
//				�V�X�e���@�֘A�֐� �����܂�
//
//********************************************************************//

//********************************************************************//
//
//				�f�o�b�O�@�֘A�֐�
//
//********************************************************************//
//-------------------------------------------------------------//
//���b�Z�[�W�{�b�N�X�ł�YesNo�m�F
//-------------------------------------------------------------//
//	�g�p��
/*
	int res = Dbg_BoxToMessage("�t���X�N���[���ɂ��܂����H");
	if(res == IDYES){
		
	}else{

	}
*/
//-------------------------------------------------------------//
int Dbg_YesNo(LPCSTR str,...)
{
	char buf[256];
//	wsprintf(buf,str,(char*)(&str + 1));
	vsprintf(buf,str,(char*)(&str+1));
	return MessageBox(hWnd,buf,"Message",MB_YESNO);
}

//--------------------------------------------------------------------------------------------
//�t�@�C��OUT�֐�
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//--------------------------------------------------------------------------------------------
void Dbg_FileOut(char* str,...){

	FILE*	fp;
	char buff[128];
	vsprintf(buff,(char*)str,(char*)(&str+1));
//	strcat(buff,"\n");
	if ((fp=fopen("debug.txt","at"))!=NULL){
		fprintf(fp,"%s",buff);
		fclose(fp);
	}
}
//-------------------------------------------------------------//
//Dbg_FileOut�ɂ��쐬�����Debug.txt�̍폜
//-------------------------------------------------------------//
void Dbg_FileDel()
{
	remove("debug.txt");
}

//--------------------------------------------------------------------------------------------
//�^�C�g���փ��b�Z�[�W�̕\��
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//--------------------------------------------------------------------------------------------
void Dbg_TilteToMessage(char* str,...){

	char buf[256];
	vsprintf(buf,(char*)str,(char*)(&str+1));
	SetWindowText(hWnd,(LPCSTR)buf);
}

//--------------------------------------------------------------------------------------------
//���b�Z�[�W�{�b�N�X�ւ̕\��
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//--------------------------------------------------------------------------------------------
void Dbg_BoxToMessage(char *str,...){

	char buf[256];
	vsprintf(buf,(char*)str,(char*)(&str+1));
	MessageBox(hWnd,(LPCSTR)buf,"Message",MB_OK);
}

//--------------------------------------------------------------------------------------------
//�f�o�b�O���\���֐��@�E�C���h�E���̎w��ʒu
//	����
//		x,y			�`����W
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//--------------------------------------------------------------------------------------------
//	�g�p��
//		Dbg_StringOutXY(10,10,"%d��",a); 
//--------------------------------------------------------------------------------------------
void		Dbg_StringOutXY(int x,int y,char* str,...)
{
	char buff[128];
	vsprintf(buff,str,(char*)(&str+1));
	Draw_TextXY(x,y,buff,ARGB(255,255,0,255),0);
}

//--------------------------------------------------------------------------------------------
//	���͊֐�
//--------------------------------------------------------------------------------------------
#define IDD_DIALOG1                     101
#define ID_DLG_INPUT                    101
#define ID_EDIT                         1001
#define ID_LABEL                        1002

struct DLG_DATA
{
	char title[MAX_PATH];
	char label[MAX_PATH];
	char get[MAX_PATH];
	int size;
};

//--------------------------------------------------------------------------------------------
//�_�C�A���O�v���V�[�W��
//	�O���t�@�C������Ăяo���o���܂���
//--------------------------------------------------------------------------------------------
static int CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static DLG_DATA* d_ptr = NULL;

	switch(msg)
	{
		//�_�C�A���O����������
		case WM_INITDIALOG:
			d_ptr = (DLG_DATA*)lParam;	//�Ăяo��������f�[�^�ւ̃|�C���^���󂯎��
			if(d_ptr->title[0] != '\0') SetWindowText(hDlg, d_ptr->title);	//�^�C�g���ύX
			if(d_ptr->label[0] != '\0') SetWindowText(GetDlgItem(hDlg, ID_LABEL), d_ptr->label);//���x���ύX
			SetFocus(hDlg);
			break;
		//�{�^������
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				//�uok�v�{�^���������ꂽ
				case IDOK:
					GetDlgItemText(hDlg, ID_EDIT, d_ptr->get, MAX_PATH);
					EndDialog(hDlg, 1);
					break;
				//�u�L�����Z���v�{�^���������́ux�v�{�^���������ꂽ
				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;
				default:
					return 0;	//Windows�ɔC����̂�0��Ԃ�
			}
			break;
		//����̏����FWindows�ɔC����
		default:
			return 0;	//Windows�ɔC����̂�0��Ԃ�
	}
	return 1;		//�����Ń��b�Z�[�W�����������̂�1��Ԃ�
}

//--------------------------------------------------------------------------------------------
//��������͊֐�
//	����
//		get			�擾���������̊i�[��
//		size		get�̃T�C�Y
//		title		�^�C�g���̕�����
//		label		�G�f�B�b�g�{�b�N�X�ɕ\�����镶����
//--------------------------------------------------------------------------------------------
//	�g�p��
//		char str[100];
//		Dbg_InputDialog(str, 100, "�m�F", "���O����͂��Ă�������");
//--------------------------------------------------------------------------------------------
DLG_ERROR Dbg_InputDialog(char* get, int size, const char* title, const char* label)
{
	DLG_DATA data;

	if(get == NULL) return DLG_ERROR_INVALIDBUFFER;
	//�^�C�g���R�s�[
	if(title != NULL)
	{
		memcpy(&data.title, title, MAX_PATH);
	}
	else
	{
		data.title[0] = '\0';
	}
	//���x���R�s�[
	if(label != NULL)
	{
		memcpy(&data.label, label, MAX_PATH);
	}
	else
	{
		data.label[0] = '\0';
	}
	//�_�C�A���O�\��
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DLG_INPUT),
					  hWnd, (DLGPROC)DlgProc, (LPARAM)&data) == 0)
	{
		//�u�L�����Z���v�{�^���������́ux�v�{�^���������ꂽ
		return DLG_ERROR_CANCEL;
	}
	//�uok�v�������ꂽ�i���͂���Ă��Ȃ��Ă��R�R�ɂ���j
	memcpy(get, data.get, size - 1);
	get[size-1] = '\0';
	return DLG_OK;
}

//********************************************************************//
//
//				�f�o�b�O�@�֘A�֐� �����܂�
//
//********************************************************************//
//********************************************************************//
//
//				BGM�Đ��@�֘A�֐� 
//
//********************************************************************//
//�֐��ꗗ
/*
BOOL		DShow_Init()						DShow�̏�����
void		DShow_Del();						DShow_�̌�n��
DSHOWOBJ	DShow_LoadFile(LPCWSTR)				���y�t�@�C���̓ǂݍ���
BOOL		DShow_Play();						�Đ�
void		DShow_RateControl(DSHOWOBJ,float)	�Đ����x�ύX
void		DShow_EndCheck()					�I���`�F�b�N
void		DShow_Stop(DSHOWOBJ)				�Đ��X�g�b�v
void		DShow_AllStop()						�S��~
LONGLONG	DShow_GetCurrentPos(int index)		�Đ��ʒu�擾
void		DShow_SetStartPos(int index)		�X�^�[�g�ʒu�ɐݒ�
*/
//-------------------------------------------------------
/*
�g����

	�O���[�o���Ƃ��Đ錾
	DSHOWOBJ		test;

	�Đ�����
	DShow_Play(test);		//�ʏ�Đ�-���ׂă��[�v�Đ�

	��~����
	DShow_Stop(test);		//�������������ׂĒ�~
	DShow_AllStop();		//�Đ����̂��ׂẲ������~

	�G�t�F�N�g
	DShow_RateControl(test,1.0);	//test�̍Đ����x���w�肵���{���ɕύX����B

*/
//--------------------------------------------------------------------------------------------
// DirectShow�̏�����
//--------------------------------------------------------------------------------------------
DSHOWOBJ	DShow_LoadFile(char* filename)
{
	wchar_t name[256];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename, -1, name, 256);

	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 0){
			if(FAILED(DShowObj[i].pBuilder->RenderFile(name,NULL))){
				BToM("DShow\n�t�@�C���ǂݍ��݂Ɏ��s���܂���\n�t�@�C�����A�R�[�f�b�N�̊m�F�����肢���܂��B");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaControl,
						(void**)&DShowObj[i].pMediaControl))){
				BToM("DShow\nMediaControl�̐ݒ�Ɏ��s���܂���");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaEvent,
						(void**)&DShowObj[i].pMediaEvent))){
					BToM("DShow\nMediaEvent�̐ݒ�Ɏ��s���܂���");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaSeeking,
						(void**)&DShowObj[i].pMediaSeeking))){
					BToM("DShow\nMediaSeeking�̐ݒ�Ɏ��s���܂���");
			}
			GUID format = TIME_FORMAT_FRAME;
			DShowObj[i].pMediaSeeking->SetTimeFormat(&format);
			DShowObj[i].use = 1;
			return i;
		}
	}
	BToM("DShow\n�ǂݍ��݌��E���𒴂��܂���");
	return -1;
}

//--------------------------------------------------------------------------------------------
// DirectShow�̏�����
//--------------------------------------------------------------------------------------------
DSHOWOBJ	DShow_LoadFile(int BgmNo,char* filename)
{
	wchar_t name[256];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename, -1, name, 256);

	if(DShowObj[BgmNo].use == 0){
		if(FAILED(DShowObj[BgmNo].pBuilder->RenderFile(name,NULL))){
			BToM("DShow\n�t�@�C���ǂݍ��݂Ɏ��s���܂���\n�t�@�C�����A�R�[�f�b�N�̊m�F�����肢���܂��B");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaControl,
					(void**)&DShowObj[BgmNo].pMediaControl))){
			BToM("DShow\nMediaControl�̐ݒ�Ɏ��s���܂���");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaEvent,
					(void**)&DShowObj[BgmNo].pMediaEvent))){
				BToM("DShow\nMediaEvent�̐ݒ�Ɏ��s���܂���");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaSeeking,
					(void**)&DShowObj[BgmNo].pMediaSeeking))){
				BToM("DShow\nMediaSeeking�̐ݒ�Ɏ��s���܂���");
		}
		GUID format = TIME_FORMAT_FRAME;
		DShowObj[BgmNo].pMediaSeeking->SetTimeFormat(&format);
		DShowObj[BgmNo].use = 1;
		return BgmNo;
	}
	return -1;
}

//--------------------------------------------------------------------------------------------
// DirectShow�̌�n��
//--------------------------------------------------------------------------------------------
void	DShow_Del()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			DShowObj[i].pMediaControl->Release();
			DShowObj[i].pMediaEvent->Release();
			DShowObj[i].pMediaSeeking->Release();
			DShowObj[i].pBuilder->Release();
		}
	}
	CoUninitialize();
}

//--------------------------------------------------------------------------------------------
// GraphBuilder�̐���
//--------------------------------------------------------------------------------------------
BOOL	DShow_Init()
{
	CoInitialize(NULL);

	HRESULT	hr;
	for(int i = 0 ; i < DSHOWMAX ; i++){
		hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
							IID_IGraphBuilder,(void **)&DShowObj[i].pBuilder);
		if(FAILED(hr)){
			BToM("DShow:GraphBuilder�I�u�W�F�N�g�̐����Ɏ��s���܂���%d",i);
			return 0;
		}
		DShowObj[i].use = 0;
	}
	return 1;
}

//--------------------------------------------------------------------------------------------
// �t�@�C���̍Đ�
//--------------------------------------------------------------------------------------------
BOOL	DShow_Play(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		HRESULT	hr = DShowObj[BgmTable[index]].pMediaControl->Run();
		if(FAILED(hr)){
			BToM("DShow:�Đ��Ɏ��s���܂���1");
			return 0;
		}
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// �Đ����x�̍Đ�
//--------------------------------------------------------------------------------------------
void DShow_RateControl(DSHOWOBJ index,float rate)
{
	if(rate < 0)	return;
	DShowObj[BgmTable[index]].pMediaSeeking->SetRate(rate);
}

//--------------------------------------------------------------------------------------------
// ��~
//--------------------------------------------------------------------------------------------
void DShow_Stop(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		LONGLONG	start = 0;
		DShowObj[BgmTable[index]].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
				NULL,AM_SEEKING_NoPositioning);
		DShowObj[BgmTable[index]].pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// �ꎞ��~
//--------------------------------------------------------------------------------------------
void DShow_Pause(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		DShowObj[BgmTable[index]].pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// �S��~
//--------------------------------------------------------------------------------------------
void DShow_AllStop()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			DShowObj[i].pMediaControl->Stop();
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���m�F
//--------------------------------------------------------------------------------------------
void DShow_EndCheck()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			long code;
			//�Đ���Ԏ擾
			DShowObj[i].pMediaEvent->WaitForCompletion(0,&code);
			//�Đ��I�����̓��[�v���s���B
			if(code == EC_COMPLETE){
				LONGLONG	start = 0;
				DShowObj[i].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
					NULL,AM_SEEKING_NoPositioning);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu�擾
//--------------------------------------------------------------------------------------------
LONGLONG DShow_GetCurrentPos(int index)
{
	LONGLONG	Current;
	DShowObj[BgmTable[index]].pMediaSeeking->GetCurrentPosition(&Current);
	return Current;
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu������
//--------------------------------------------------------------------------------------------
void DShow_SetStartPos(int index)
{
	LONGLONG	start = 0;
	DShowObj[BgmTable[index]].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
			NULL,AM_SEEKING_NoPositioning);
}


//********************************************************************//
//
//				BGM�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//

//********************************************************************//
//
//				SE�Đ��@�֘A�֐�
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
// DirectSound�̏�����
//--------------------------------------------------------------------------------------------
void DSound_Init()
{
	if(DSound_Create()){
		if(!DSound_CreatePrimaryBuffer()){
			BToM("�v���C�}���o�b�t�@�̐����Ɏ��s���܂���");
		}
		DSound_CreateSecondaryBuffer();
	}else{
		BToM("DSound�̐����Ɏ��s���܂���");
	}

}

//--------------------------------------------------------------------------------------------
// DirectSound�̍쐬
//--------------------------------------------------------------------------------------------
BOOL DSound_Create( void )
{
    HRESULT ret;

    // DirectSound8���쐬
    ret = DirectSoundCreate8( NULL,&lpDS,NULL );
    if( FAILED(ret) ) {
        BToM( "�T�E���h�I�u�W�F�N�g�쐬���s\n","" );
        return FALSE;
    }

    // �������[�h
    ret = lpDS->SetCooperativeLevel( hWnd,DSSCL_EXCLUSIVE|DSSCL_PRIORITY );
    if( FAILED(ret) ) {
        BToM( "�������x���ݒ莸�s\n","" );
        return FALSE;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------
// �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
void DSound_CreateSecondaryBuffer()
{
	int i ;
	for( i = 0 ; i < DSFILEMAX ; i ++){
		SoundBuffer[i].SoundBuffer = NULL;
		SoundBuffer[i].BufferSize = 0;
	}

}

//--------------------------------------------------------------------------------------------
// �T�E���h�Đ�
//--------------------------------------------------------------------------------------------
void DSound_Play(DSOUNDOBJ SoundNo)
{
	int i;

	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){		//�ǂݍ��܂�Ă���Ȃ�Ζ��Ȃ�
		//�󂢂Ă���o�b�t�@���o
		for(i = 0 ; i < DSPLAYMAX ; i ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State == DSNONE){
				//�Đ��J�n
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].ID = SoundNo;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State = DSPLAY;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].PlayBuffer->Play(0,0,0);
				//PlayBuffer[i].BufferSize = SoundBuffer[SoundNo].BufferSize;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].Loop = 0;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h���[�v�Đ�
//--------------------------------------------------------------------------------------------
void DSound_PlayLoop(DSOUNDOBJ SoundNo)
{
	int i;

	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){		//�ǂݍ��܂�Ă���Ȃ�Ζ��Ȃ�
		//�󂢂Ă���o�b�t�@���o
		for(i = 0 ; i < DSPLAYMAX ; i ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State == DSNONE){
				//�Đ��J�n
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].ID = SoundNo;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State = DSPLAY;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].PlayBuffer->Play(0,0,0);
				//SoundBuffer[SoundNo].PlayBuffer[i].BufferSize = SoundBuffer[SoundNo].BufferSize;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].Loop = 1;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h��~
//	�������ʉ������ׂĒ�~����
//--------------------------------------------------------------------------------------------
void DSound_Stop(DSOUNDOBJ SoundNo)
{
	int j;
	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){
		for(j = 0 ; j < DSPLAYMAX ; j ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].State == DSPLAY){
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].PlayBuffer->Stop();
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].State = DSNONE;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// ���ׂẴT�E���h��~
//--------------------------------------------------------------------------------------------
void DSound_AllStop()
{
	int i,j;
	for(i = 0 ; i < DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){
			for(j = 0 ; j < DSPLAYMAX ; j ++){
				if(SoundBuffer[i].PlayBuffer[j].State == DSPLAY){
					SoundBuffer[i].PlayBuffer[j].PlayBuffer->Stop();
					SoundBuffer[i].PlayBuffer[j].State = DSNONE;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���`�F�b�N
//--------------------------------------------------------------------------------------------
void DSound_EndCheck()
{
	int i,j;
	DWORD Status;
	int a = 0;
	for(i = 0 ; i< DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){		//���ݓǂݍ��ݒ��̃o�b�t�@		
			for(j = 0 ; j < DSPLAYMAX ; j++){
				if(SoundBuffer[i].PlayBuffer[j].State == DSPLAY){		//���ݍĐ����̕����o�b�t�@
					//�I���ƔF������
					SoundBuffer[i].PlayBuffer[j].PlayBuffer->GetStatus(&Status);
					if( (Status & DSBSTATUS_PLAYING) != 1){
						if(SoundBuffer[i].PlayBuffer[j].Loop){
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->SetCurrentPosition(0);
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->Play(0,0,0);
						}else{
							SoundBuffer[i].PlayBuffer[j].State = DSNONE;
							SoundBuffer[i].PlayBuffer[j].BufferSize = 0 ;
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->Stop();
						}
					}else{
					}
					a++;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// �v���C�}���T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
BOOL DSound_CreatePrimaryBuffer( void )
{
    HRESULT ret;
    WAVEFORMATEX wf;

    // �v���C�}���T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
    dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsdesc.dwBufferBytes = 0;
    dsdesc.lpwfxFormat = NULL;
    ret = lpDS->CreateSoundBuffer( &dsdesc,&lpPrimary,NULL );
    if( FAILED(ret) ) {
        BToM( "�v���C�}���T�E���h�o�b�t�@�쐬���s\n","" );
        return FALSE;
    }

    // �v���C�}���o�b�t�@�̃X�e�[�^�X������
    wf.cbSize = sizeof( WAVEFORMATEX );
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.nSamplesPerSec = 44100;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    ret = lpPrimary->SetFormat( &wf );
    if( FAILED(ret) ) {
        BToM( "�v���C�}���o�b�t�@�̃X�e�[�^�X���s\n","" );
        return FALSE;
    }
    return TRUE;
}

//--------------------------------------------------------------------------------------------
//DSound�̌�n��
//--------------------------------------------------------------------------------------------
BOOL DSound_Del( void )
{
	int i;
	for( i = 0 ; i < DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){	
			SoundBuffer[i].SoundBuffer->Release();
		}
	}
    if( lpPrimary ) {
        lpPrimary->Release();
        lpPrimary = NULL;
    }
    if( lpDS ) {
        lpDS->Release();
        lpDS = NULL;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------
// �T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
int DSound_LoadFile(char* file )
{
    HRESULT ret;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;
	int SoundNo;
	for(SoundNo = 0; SoundNo < DSFILEMAX ; SoundNo ++ ){
		if(SoundBuffer[SoundNo].BufferSize == 0){
			break;
		}
	}
	if(SoundNo == DSFILEMAX)
		BToM("�o�b�t�@�̐�������܂���");
	int ID = SoundNo;

    // WAV�t�@�C�������[�h
    HMMIO hSrc;
    hSrc = mmioOpen( (LPSTR)file,NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
    if( !hSrc ) {
        BToM( "WAV�t�@�C�����[�h�G���[\n","" );
        return -1;
    }

    // 'WAVE'�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
    ret = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
        BToM( "WAVE�`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // 'fmt '�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
    ret = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
        BToM( "fmt �`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // �w�b�_�T�C�Y�̌v�Z
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // �w�b�_�������m��
    wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
    if( !wf ) {
        BToM( "�������m�ۃG���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVE�t�H�[�}�b�g�̃��[�h
    ret = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
    if( FAILED(ret) ) {
        BToM( "WAVE�t�H�[�}�b�g���[�h�G���[\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // fmt�`�����N�ɖ߂�
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // data�`�����N��T��
    while(1) {
        // ����
        ret = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(ret) ) {
            BToM( "data�`�����N��������Ȃ�\n","" );
            free( wf );
            mmioClose( hSrc,0 );
            return -1;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC("data",0) )
            break;
        // ���̃`�����N��
        ret = mmioAscend( hSrc,&mSrcWaveData,0 );
    }


    // �T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY;
    dsdesc.dwBufferBytes = mSrcWaveData.cksize;
    dsdesc.lpwfxFormat = wf;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer( &dsdesc,&SoundBuffer[ID].SoundBuffer,NULL );
	SoundBuffer[ID].BufferSize = (WORD)dsdesc.dwBufferBytes;
    if( FAILED(ret) ) {
        BToM( "�T�E���h�o�b�t�@�̍쐬�G���[\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ���b�N�J�n
    LPVOID pMem1,pMem2;
    DWORD dwSize1,dwSize2;
    ret = (SoundBuffer[ID].SoundBuffer)->Lock( 0,mSrcWaveData.cksize,&pMem1,&dwSize1,&pMem2,&dwSize2,0 );
	SoundBuffer[ID].SoundBuffer->GetFrequency(&SoundBuffer[ID].Fre);
    if( FAILED(ret) ) {
        BToM( "���b�N���s\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // �f�[�^��������
    mmioRead( hSrc,(char*)pMem1,dwSize1 );
    mmioRead( hSrc,(char*)pMem2,dwSize2 );

	// ���b�N����
    (SoundBuffer[ID].SoundBuffer)->Unlock( pMem1,dwSize1,pMem2,dwSize2 );
    // �w�b�_�p���������J��
    free( wf );
    // WAV�����
	mmioClose( hSrc,0 );

	//�����J�n
	for(int t = 0 ; t < 10 ; t++){
		if(lpDS->DuplicateSoundBuffer(SoundBuffer[SoundNo].SoundBuffer,
								&(SoundBuffer[SoundNo].PlayBuffer[t].PlayBuffer))
			!= DS_OK){
			BToM( "�Đ��p�o�b�t�@�쐬���s\n","" );
			break;
		}else{
			SoundBuffer[SoundNo].PlayBuffer[t].ID = SoundNo;
			SoundBuffer[SoundNo].PlayBuffer[t].State = DSNONE;
			SoundBuffer[SoundNo].PlayBuffer[t].BufferSize = SoundBuffer[SoundNo].BufferSize;
			SoundBuffer[SoundNo].PlayBuffer[t].Loop = 0;
		}
	}
    return SoundNo;
}

//--------------------------------------------------------------------------------------------
// �T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
int DSound_LoadFile(int SoundNo,char* file )
{
    HRESULT ret;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;

	int ID = SoundNo;

    // WAV�t�@�C�������[�h
    HMMIO hSrc;
    hSrc = mmioOpen( (LPSTR)file,NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
    if( !hSrc ) {
        BToM( "WAV�t�@�C�����[�h�G���[\n","" );
        return -1;
    }

    // 'WAVE'�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
    ret = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
        BToM( "WAVE�`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // 'fmt '�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
    ret = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
        BToM( "fmt �`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // �w�b�_�T�C�Y�̌v�Z
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // �w�b�_�������m��
    wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
    if( !wf ) {
        BToM( "�������m�ۃG���[\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVE�t�H�[�}�b�g�̃��[�h
    ret = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
    if( FAILED(ret) ) {
        BToM( "WAVE�t�H�[�}�b�g���[�h�G���[\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // fmt�`�����N�ɖ߂�
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // data�`�����N��T��
    while(1) {
        // ����
        ret = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(ret) ) {
            BToM( "data�`�����N��������Ȃ�\n","" );
            free( wf );
            mmioClose( hSrc,0 );
            return -1;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC("data",0) )
            break;
        // ���̃`�����N��
        ret = mmioAscend( hSrc,&mSrcWaveData,0 );
    }


    // �T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY;
    dsdesc.dwBufferBytes = mSrcWaveData.cksize;
    dsdesc.lpwfxFormat = wf;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer( &dsdesc,&SoundBuffer[ID].SoundBuffer,NULL );
	SoundBuffer[ID].BufferSize = (WORD)dsdesc.dwBufferBytes;
    if( FAILED(ret) ) {
        BToM( "�T�E���h�o�b�t�@�̍쐬�G���[\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ���b�N�J�n
    LPVOID pMem1,pMem2;
    DWORD dwSize1,dwSize2;
    ret = (SoundBuffer[ID].SoundBuffer)->Lock( 0,mSrcWaveData.cksize,&pMem1,&dwSize1,&pMem2,&dwSize2,0 );
	SoundBuffer[ID].SoundBuffer->GetFrequency(&SoundBuffer[ID].Fre);
    if( FAILED(ret) ) {
        BToM( "���b�N���s\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // �f�[�^��������
    mmioRead( hSrc,(char*)pMem1,dwSize1 );
    mmioRead( hSrc,(char*)pMem2,dwSize2 );

	// ���b�N����
    (SoundBuffer[ID].SoundBuffer)->Unlock( pMem1,dwSize1,pMem2,dwSize2 );
    // �w�b�_�p���������J��
    free( wf );
    // WAV�����
	mmioClose( hSrc,0 );

	//�����J�n
	for(int t = 0 ; t < 10 ; t++){
		if(lpDS->DuplicateSoundBuffer(SoundBuffer[SoundNo].SoundBuffer,
								&(SoundBuffer[SoundNo].PlayBuffer[t].PlayBuffer))
			!= DS_OK){
			BToM( "�Đ��p�o�b�t�@�쐬���s\n","" );
			break;
		}else{
			SoundBuffer[SoundNo].PlayBuffer[t].ID = SoundNo;
			SoundBuffer[SoundNo].PlayBuffer[t].State = DSNONE;
			SoundBuffer[SoundNo].PlayBuffer[t].BufferSize = SoundBuffer[SoundNo].BufferSize;
			SoundBuffer[SoundNo].PlayBuffer[t].Loop = 0;
		}
	}
    return SoundNo;
}


//--------------------------------------------------------------------------------------------
// ���g���ύX �Đ����x���ς��
//--------------------------------------------------------------------------------------------
void DSound_SetFrequency(DSOUNDOBJ SoundNo,int Fre)
{
	SoundBuffer[SeTable[SoundNo]].SoundBuffer->SetFrequency(SoundBuffer[SoundNo].Fre+Fre);

}

//********************************************************************//
//
//				SE�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//


//********************************************************************//
//
//				End Of File
//
//********************************************************************//
