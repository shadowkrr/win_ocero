//******************************************************************************
//	ファイル名	：	Title.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2011/09/23
//	機能		：	タイトル
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Title.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"TitleBack.h"
#include	"TitleSelect.h"
#include	"TitleSystem.h"
#include	"TitleUnder.h"

//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace title;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Title::C_Title()
{
	mObj.resize(OBJ_MAX);	//最初に使用するだけ生成数を設定しておく

	//以下に最初に生成しておきたいオブジェクトを入れる
	C_Base*  b = new C_Back("res/txt/title/back.txt");
	SET_OBJECT(b,OBJ_BACK);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_START);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_RANKING);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Select("res/txt/title/back.txt",C_Select::SEL_END);
	SET_OBJECT(b,OBJ_SELECT);
	b = new C_Under("res/txt/title/back.txt");
	SET_OBJECT(b,OBJ_UNDER);
	b = new C_System("res/txt/title/system.txt");
	SET_OBJECT(b,OBJ_SYSTEM);
	//以上

	OutputDebugString("constructor:C_Title\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Title::~C_Title()
{
	OutputDebugString("destructor:C_Title\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_Title::Draw()
{
	//Draw_FontDrawText(scriptdata.TextArea,"aaweweat",ARGB(255,255,255,255),scriptdata.TextType,0.4f);
	GetDraw();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
void C_Title::Control()
{
	GetControl();
}
