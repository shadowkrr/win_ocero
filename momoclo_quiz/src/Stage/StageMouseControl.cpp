//******************************************************************************
//	ファイル名	：	StageMouseControl.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2012/05/05
//	機能		：	マウスコントロール
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageMouseControl.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"StageMap.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_MouseControl::C_MouseControl(const char* fileName) : C_Base(fileName)
{
	seluser = 1;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_MouseControl\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_MouseControl::~C_MouseControl()
{
	OutputDebugString("destructor:C_MouseControl\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_MouseControl::Draw()
{
	char str[32];
	int argb = 0xffffffff;

	C_Map* map = GET_OBJECT( C_Map* ,C_Stage::OBJ_MAP);


	if (seluser == 1) argb = 0xffffffff;
	if (seluser == 2) argb = 0xff000000;

	Draw_Ellipse(GetMousePotision().x - map->GetBlockSize()/2,GetMousePotision().y - map->GetBlockSize()/2,GetMousePotision().x + map->GetBlockSize() - map->GetBlockSize()/2,GetMousePotision().y + map->GetBlockSize() - map->GetBlockSize()/2,argb,0xff000000,1,1,0.2f);
	sprintf(str,"MPosX = %d",GetMousePotision().x / map->GetBlockSize());
	Draw_TextXY(500,WINH / 4,str,0xffff0000,1);
	sprintf(str,"MPosY = %d",GetMousePotision().y / map->GetBlockSize());
	Draw_TextXY(500,WINH / 4 + 32,str,0xffff0000,1);

	Draw_TextXY(500,WINH / 4 + 32 * 2,"lclick = put",0xffff0000,1);
	Draw_TextXY(500,WINH / 4 + 32 * 3,"rclick = pass",0xffff0000,1);

}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_MouseControl::Control()
{
	C_Map* map = GET_OBJECT( C_Map* ,C_Stage::OBJ_MAP);

	if (CheckPush(KEY_L_MOUSE) && (GetMousePotision().x <= map->GetBlockSize() * 8) && map->GetMapArray(GetMousePotision().x / map->GetBlockSize(), GetMousePotision().y / map->GetBlockSize()) == 0)
	{
		if (map->ReverceCheck(GetMousePotision().x / map->GetBlockSize(), GetMousePotision().y / map->GetBlockSize(), seluser))
		{
			SetPlay_Se(SE_BOMB);
			map->SetMapArray(GetMousePotision().x / map->GetBlockSize(), GetMousePotision().y / map->GetBlockSize(), seluser);
			seluser++;
			if (seluser > 2)
			{
				seluser = 1;
			}
		}
	}
	/* パス */
	if (CheckPush(KEY_R_MOUSE))
	{
		seluser++;
		if (seluser > 2)
		{
			seluser = 1;
		}
	}
	return false;
}
