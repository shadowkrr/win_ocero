#pragma once

//==========================================================//
//	列挙型定義
//==========================================================//
//音楽
enum BGM_NO
{
	BGM_TITLE,
	BGM_STAGE,

	BGM_MAX
};

//エフェクト
enum SE_NO
{
	SE_BOMB,
	SE_COPY,
	SE_MOVE,
	
	SE_MAX
};

//==========================================================//
//	クラス型定義
//==========================================================//
//==========================================================//
//外部で使用する変数、構造体のextern宣言
//==========================================================//
extern DSHOWOBJ		BgmTable[BGM_MAX];
extern DSOUNDOBJ	SeTable[SE_MAX];
//==========================================================//
//外部で使用する関数のextern宣言
//==========================================================//
extern void		Load_Bgm();						//BGBのロード
extern void		Load_Se();						//SEのロード
extern void		SetStart_DShow();				//BGMをスタート位置にセット
extern void		Init_Sound();					//BGM・SE初期化
extern void		EndCheck_Sound();				//BGM・SE終了確認
extern void		Delete_Sound();					//BGM・SE破棄
extern void		SetPlay_Bgm(BGM_NO name);		//BGMを流す
extern void		SetStop_Bgm(BGM_NO name);		//BGMを止める
extern void		SetPlay_Se(SE_NO name);			//SEを流す
extern void		SetStop_Se(SE_NO name);			//SEを止める
extern void		AllBgmStop();					//BGMをすべて停止