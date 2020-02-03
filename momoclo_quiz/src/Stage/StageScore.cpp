//******************************************************************************
//	ファイル名	：	StageScore.cpp
//	作成者		：	kageyama taiki
//	作成日		：	2012/04/01
//	機能		：	スコア
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//------------------------------------------------------------
//	呼ばなければいけないヘッダ
//------------------------------------------------------------
#include	"Stage.h"
#include	"StageScore.h"
//------------------------------------------------------------
//	呼びたいヘッダ
//------------------------------------------------------------
#include	"StageSnake.h"
#include	"StageSnakeCopy.h"
//------------------------------------------------------------
//	変数や宣言
//------------------------------------------------------------
using namespace stage;
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Score::C_Score(const char* fileName) : C_Base(fileName)
{
	scorecnt = 1;
	//mImage = C_BaseManager::SetLoadImage(mImageFileName.c_str());
	OutputDebugString("constructor:C_Score\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Score::~C_Score()
{
	OutputDebugString("destructor:C_Score\n");
}

//------------------------------------------------------------
//	描画
//------------------------------------------------------------
void C_Score::Draw()
{
	char str[32];

	C_Snake* snake = GET_OBJECT(C_Snake* ,C_Stage::OBJ_SNAKE);
	C_SnakeCopy* snakecopy = GET_OBJECT(C_SnakeCopy* ,C_Stage::OBJ_SNAKECOPY);

	sprintf(str,"score = %d",scorecnt);
	Draw_TextXY(400,WINH / 4,str,0xffff0000,1);
	sprintf(str,"moveValue = %f", snake->GetmoveValue());
	Draw_TextXY(400,WINH / 4 + 32,str,0xffff0000,1);
	sprintf(str,"posx = %f", snake->GetPosX());
	Draw_TextXY(400,WINH / 4 + 32 * 2,str,0xffff0000,1);
	sprintf(str,"posy = %f", snake->GetPosY());
	Draw_TextXY(400,WINH / 4 + 32 * 3,str,0xffff0000,1);
	sprintf(str,"addpx = %f", snake->GetAddPx());
	Draw_TextXY(400,WINH / 4 + 32 * 4,str,0xffff0000,1);
	sprintf(str,"addpy = %f", snake->GetAddPy());
	Draw_TextXY(400,WINH / 4 + 32 * 5,str,0xffff0000,1);
	sprintf(str,"dir = %d", snake->GetDir());
	Draw_TextXY(400,WINH / 4 + 32 * 6,str,0xffff0000,1);
	sprintf(str,"backdir = %d", snake->GetBufDir());
	Draw_TextXY(400,WINH / 4 + 32 * 7,str,0xffff0000,1);
	sprintf(str,"copynum = %d", snakecopy->GetCopyNum());
	Draw_TextXY(400,WINH / 4 + 32 * 8,str,0xffff0000,1);
	//GetDraw_TpBitmap();
}

//------------------------------------------------------------
//	制御
//------------------------------------------------------------
bool C_Score::Control()
{
	return false;
}
//------------------------------------------------------------
// スコアの加算
//------------------------------------------------------------
void C_Score::ScorePlus()
{
	scorecnt++;
}