//==========================================================//
//ライブラリ以外で必要となるヘッダーファイルのインクルード
//構造体定義が必要となるので同名のヘッダーファイルを記述しておく。
//==========================================================//
#include	"utility/Gp3Lib.h"
#include	"Sound.h"
//==========================================================//
//内部で使用する変数、構造体の宣言
//また外部にある変数を使用した場合はexternで宣言する
//==========================================================//
//------------------------------------------------------------
//	ファイル名
//------------------------------------------------------------
//音楽関連
char* BgmFileTable[] = 
{
	"res/snd/bgm/title_bgm.wav",
	"res/snd/bgm/stage_bgm.wav",
	"",
};

//エフェクト関連
char* SeFileTable[] = 
{
	"res/snd/se/bomb.wav",
	"res/snd/se/copy.wav",
	"res/snd/se/move.wav",
	"",
};

//==========================================================//
//関数プロトタイプ宣言
//==========================================================//
//==========================================================//
//	クラス関数
//==========================================================//
//==========================================================//
//	関数定義
//==========================================================//
//------------------------------------------------------------------
//	BGMのロード
//------------------------------------------------------------------
void		Load_Bgm()
{
	for(int i = 0 ; i < BGM_MAX ;  ++i)
	{
		BgmTable[i] = DShow_LoadFile(BgmFileTable[i]);
	}
}

//------------------------------------------------------------------
//	SEのロード
//------------------------------------------------------------------
void		Load_Se()
{	
	for(int i = 0 ; i < SE_MAX ;  ++i)
	{
		SeTable[i] = DSound_LoadFile(SeFileTable[i]);
	}
}

//------------------------------------------------------------------
//	BGMをスタート位置にセット
//------------------------------------------------------------------
void	SetStart_DShow()
{
	for(int i = 0 ; i < BGM_MAX ;  ++i)
	{
		DShow_SetStartPos(i);
	}
	DShow_AllStop();
}

//------------------------------------------------------------------
//	BGM・SEの初期化
//------------------------------------------------------------------
void		Init_Sound()
{
	DShow_Init();
	DSound_Init();
}

//------------------------------------------------------------------
//	BGM・SEの終了確認
//------------------------------------------------------------------
void		EndCheck_Sound()
{
	DShow_EndCheck();		
	DSound_EndCheck();
}

//------------------------------------------------------------------
//	BGM・SEの破棄
//------------------------------------------------------------------
void		Delete_Sound()
{
	DShow_Del();
	DSound_Del();
}

//------------------------------------------------------------------
//	BGMを流す
//------------------------------------------------------------------
void	SetPlay_Bgm(BGM_NO name)
{
	DShow_Play(BgmTable[name]);
}

//------------------------------------------------------------------
//	BGMを止める
//------------------------------------------------------------------
void	SetStop_Bgm(BGM_NO name)
{
	DShow_Stop(BgmTable[name]);	
}

//------------------------------------------------------------------
//	SEを流す
//------------------------------------------------------------------
void	SetPlay_Se(SE_NO name)
{
	DSound_Play(SeTable[name]);
}

//------------------------------------------------------------------
//	SEを止める
//------------------------------------------------------------------
void	SetStop_Se(SE_NO name)
{
	DSound_Stop(SeTable[name]);
}
//--------------------------------------------------------------
//BGMをすべて停止
//--------------------------------------------------------------
void	AllBgmStop()
{
	for(int i = 0; i < BGM_MAX;	i++/*最大数*/)
	{
		DShow_Stop(i);
	}
}
