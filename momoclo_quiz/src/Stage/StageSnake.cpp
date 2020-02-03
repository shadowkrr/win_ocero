//******************************************************************************
//	ファイル名	：	StageSnake.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2012/05/01
//	機能		：	スネーク本体
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageSnake.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"StageMap.h"
#include	"StageSnakeCopy.h"
#include	"StageItem.h"
#include	"StageScore.h"
#include	"../Sound.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Snake::C_Snake(const char* fileName) : C_Base(fileName)
{
	SetPlay_Bgm(BGM_STAGE);

	mPosX = 1.0f;
	mPosY = 1.0f;
	moveY = 0.0f;
	moveX = 0.0f;
	mdir = DIR_DOWN;
	bufdir = mdir;
	moveValue = 0.8f;
	hitcheck = false;
	pause = false;
	mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Snake\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Snake::~C_Snake()
{
	OutputDebugString("destructor:C_Snake\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_Snake::Draw()
{
	const C_Map* map = GET_OBJECT(const C_Map* ,C_Stage::OBJ_MAP);
	
	if (hitcheck)	/* 衝突した */
	{
		//Draw_Box(0,0,640,480,0xfffeeewwwwwwwwwwer4f0000,0xffff0000,1,1,0.5f);
		Draw_TextXY(400 - 32*2,WINH / 2 + 200,"－－－－－　終了　－－－－－－",0xffff0000,1);
	}
	if (pause)	/* 衝突した */
	{
		//Draw_Box(0,0,640,480,0xfffeeewwwwwwwwwwer4f0000,0xffff0000,1,1,0.5f);
		Draw_TextXY(400 - 32*2,WINH / 2 + 200,"－－－－－　ぽーず　－－－－－－",0xffff0000,1);
	}

	Draw_Box(static_cast<int>(mPosX)  * map->GetBlockSize() + static_cast<int>(mAddPx), static_cast<int>(mPosY) * map->GetBlockSize() + static_cast<int>(mAddPy), static_cast<int>(mPosX) * map->GetBlockSize() + static_cast<int>(mAddPx) + map->GetBlockSize(), static_cast<int>(mPosY) * map->GetBlockSize() + static_cast<int>(mAddPy) + map->GetBlockSize(), 0xff000000,0xff000000,1,1,0.5f);
//	GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_Snake::Control()
{
	if (!hitcheck && !pause)
	{
		Key();
		Move();
		GetItem();
	}
	else if (hitcheck)
	{
		SetStop_Bgm(BGM_STAGE);
		if(CheckPress(KEY_ENTER))
		{
			SetStop_Bgm(BGM_STAGE);
			C_BaseScreen::SetScreenNo(C_BaseScreen::TITLE_SCREEN);
		//	C_BaseScreen::SetScreenNo(C_BaseScreen::STAGE_SCREEN);
		}
	}
	else if (pause)
	{
		if(CheckPush(KEY_ENTER))
		{
			pause = false;
		}
	}
	return false;
}
//------------------------------------------------------------
//	キー
//------------------------------------------------------------
void C_Snake::Key()
{
	if(moveX == 0.0f)
	{
		if(CheckPress(KEY_UP))
		{
			if (mdir != DIR_DOWN)
			{
				SetMoveInfo(DIR_UP, &moveY,-moveValue);
				moveX = 0.0f;
			}
		}
		if(CheckPress(KEY_DOWN))
		{
			if (mdir != DIR_UP)
			{
				SetMoveInfo(DIR_DOWN, &moveY,moveValue);
				moveX = 0.0f;
			}
		}
	}
	if(moveY == 0.0f)
	{
		if(CheckPress(KEY_LEFT))
		{
			if (mdir != DIR_RIGHT)
			{
				SetMoveInfo(DIR_LEFT, &moveX,-moveValue);
				moveY = 0.0f;
			}
		}
		/*if(CheckPress(KEY_RIGHT))
		{
			if (mdir != DIR_LEFT)
			{
				SetMoveInfo(DIR_RIGHT, &moveX,moveValue);
				moveY = 0.0f;	
			}
		}*/
	}


	if (!pause)
	{
		if(CheckPush(KEY_ENTER))
		{
			pause = true;
		}
	}
	 /*C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

	if(CheckPress(KEY_SPACE))
	{
		sne_copy->CreateSnake(mPosX, mPosY, mdir);
	}*/
}
//------------------------------------------------------------
//一マスづつ動く処理
//------------------------------------------------------------
void C_Snake::Move()
{
	const C_Map* map = GET_OBJECT(const C_Map* ,C_Stage::OBJ_MAP);

	if(moveY == 0.0f)
	{
		if (mdir == DIR_DOWN)
		{
			SetMoveInfo(DIR_DOWN, &moveY,moveValue);
		}
		else if (mdir == DIR_UP)
		{
			SetMoveInfo(DIR_UP, &moveY,-moveValue);
		}
	}
	if(moveX == 0.0f)
	{
		if (mdir == DIR_RIGHT)
		{
			SetMoveInfo(DIR_RIGHT, &moveX,moveValue);
		}
		else if (mdir == DIR_LEFT)
		{
			SetMoveInfo(DIR_LEFT, &moveX,-moveValue);
		}
	}









	if(moveY != 0.0f)
	{
		mAddPy += moveY;
		if(mAddPy >= map->GetBlockSize())
		{
			MoveFinishPy(1.0f);
		}
		else if(mAddPy <= -map->GetBlockSize())
		{
			MoveFinishPy(-1.0f);
		}
	}
	//キャラ移動 X軸
	if(moveX != 0.0f)
	{
		mAddPx += moveX;
		if(mAddPx >= map->GetBlockSize())
		{
			MoveFinishPx(1.0f);
		}
		else if(mAddPx <= -map->GetBlockSize())
		{
			MoveFinishPx(-1.0f);
		}
	}
}
//------------------------------------------------------------
//キーが押された後の移動情報をセット
//------------------------------------------------------------
void C_Snake::SetMoveInfo(SNAKE_DIR dir, float* set_ptr, float setValue,int checkpos)
{
	//ch_ptr->dir = dir;
	mdir = dir;
	mImgSize.top = dir*mImgSize.bottom;

	if(CheckMoveDirection(checkpos))
	{
		*set_ptr = setValue;
	}
	else
	{
		*set_ptr = 0.0f;	/* 壁があるときmoveを0.0f */
		//mAddPy = mAddPx = 0.0f;
		hitcheck = true;
	}
}
//------------------------------------------------------------
//向かっている方向は移動可能かどうか
//	true 行ける
//	false 行けない
//------------------------------------------------------------
bool C_Snake::CheckMoveDirection(int checkpos)
{
	C_Map* map = GET_OBJECT(C_Map* ,C_Stage::OBJ_MAP);

	bool ret = false;

	switch(mdir)
	{
		case DIR_DOWN:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)+1));	break;
		case DIR_UP:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX),   static_cast<int>(mPosY)-1));	break;
		case DIR_LEFT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)-1, static_cast<int>(mPosY)));		break;
		case DIR_RIGHT:	ret = CheckMove(map->GetMapArray(static_cast<int>(mPosX)+1, static_cast<int>(mPosY)));		break;
	}
	//ret = true;
	return ret;
}
//------------------------------------------------------------
//行こうとしている場所は移動可能かどうか
//	true 行ける
//	false 行けない
//------------------------------------------------------------
bool C_Snake::CheckMove(int chipNo)
{
	return chipNo == 1 ? false: true;
}
//------------------------------------------------------------
//移動完了処理ワープ回避で二つに分けて作成
//------------------------------------------------------------
void C_Snake::MoveFinishPx(float x)
{
	SetPlay_Se(SE_MOVE);
	mPosX += x;
	//ch_ptr->posY += y;
	moveX =/* ch_ptr->moveY =*/ 
	mAddPx =/* ch_ptr->addPy =*/ 0.0f;

	bufdir = mdir; /* 移動した方向を覚えさせておく */


	C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);
	//sne_copy->CreateSnake(mPosX - 1, mPosY, mdir);

	if (sne_copy->SnakeCheckPos(mPosX, mPosY))
	{
		hitcheck = true;
	}
}
//------------------------------------------------------------
//移動完了処理ワープ回避で二つに分けて作成
//------------------------------------------------------------
void C_Snake::MoveFinishPy(float y)
{
	SetPlay_Se(SE_MOVE);
	//ch_ptr->posX += x;
	mPosY += y;
	/*ch_ptr->moveX = */moveY = 
	/*ch_ptr->addPx = */mAddPy = 0.0f;

	bufdir = mdir; /* 移動した方向を覚えさせておく */

	C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);
	//sne_copy->CreateSnake(mPosX, mPosY - 1, mdir);

	if (sne_copy->SnakeCheckPos(mPosX, mPosY))
	{
		hitcheck = true;
	}
}
//------------------------------------------------------------
//移動完了した前回の方向を取得
//------------------------------------------------------------
int C_Snake::GetBackDir()
{
	return bufdir; /* 前回移動した方向を送る*/
}
//------------------------------------------------------------
//移動速度の取得
//------------------------------------------------------------
float C_Snake::GetMoveValue()
{
	return moveValue; /* 移動速度の取得 */
}
//------------------------------------------------------------
//終了フラグの取得
//------------------------------------------------------------
bool C_Snake::GetHitCheck()
{
	return hitcheck;
}
//------------------------------------------------------------
//	アイテム取得
//------------------------------------------------------------
void C_Snake::GetItem()
{
	C_Item* item = GET_OBJECT(C_Item* ,C_Stage::OBJ_ITEM);

	if (mPosX == item->GetPosX() && mPosY == item->GetPosY())
	{
		C_Score* score = GET_OBJECT( C_Score* ,C_Stage::OBJ_SCORE);

		score->ScorePlus();	/* スコアの加算 */
		/* 蛇を増やす */
		C_SnakeCopy* sne_copy = GET_OBJECT( C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

		if (mdir == DIR_UP)
		{
			sne_copy->CreateSnake(mPosX, mPosY + 1, mdir);
		}
		else if (mdir == DIR_DOWN)
		{
			sne_copy->CreateSnake(mPosX, mPosY - 1, mdir);
		}
		else if (mdir == DIR_RIGHT)
		{
			sne_copy->CreateSnake(mPosX - 1, mPosY, mdir);
		}
		else if (mdir == DIR_LEFT)
		{
			sne_copy->CreateSnake(mPosX + 1, mPosY, mdir);
		}

		moveValue += 0.2f;	/* 速度の加算 */
		item->item_reset();	/* アイテムの場所を変える */
	}
}