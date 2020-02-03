//******************************************************************************
//	ファイル名	：	Stage.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2012/04/01
//	機能		：	ステージ
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"StageBack.h"
//#include	"StageSelect.h"
#include	"StageSystem.h"
#include	"StageItem.h"
#include	"StageMap.h"
#include	"StageBlock.h"
#include	"StageScore.h"
#include	"StageSnake.h"
#include	"StageSnakeCopy.h"
//#include	"StageUnder.h"


#include	"StageMouseControl.h"

//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Stage::C_Stage()
{
	mObj.resize(OBJ_MAX);	//最初に使用するだけ生成数を設定しておく

	//以下に最初に生成しておきたいオブジェクトを入れる
	C_Base*  b = new C_Back("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_BACK);
	b = new C_Map("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_MAP);
	b = new C_MouseControl("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_MOUSECONTROL);
	/*b = new C_Item("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_ITEM);*/
	/*b = new C_Block("res/txt/Stage/back.txt", 0, 0, C_Block::BLOCK_1);
	SET_OBJECT(b,OBJ_BLOCK1);
	b = new C_Score("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SCORE);*/
	/*b = new C_Score("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SCORE);
	b = new C_Snake("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SNAKE);
	b = new C_SnakeCopy("res/txt/Stage/back.txt");
	SET_OBJECT(b,OBJ_SNAKECOPY);*/
	b = new C_System("res/txt/Stage/system.txt");
	SET_OBJECT(b,OBJ_SYSTEM);
	//以上

	OutputDebugString("constructor:C_Stage\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Stage::~C_Stage()
{
	OutputDebugString("destructor:C_Stage\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_Stage::Draw()
{
	//Draw_FontDrawText(scriptdata.TextArea,"aaweweat",ARGB(255,255,255,255),scriptdata.TextType,0.4f);
	GetDraw();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
void C_Stage::Control()
{
	GetControl();
}
