//******************************************************************************
//	ファイル名	：	StageTimeCnt.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/11/1
//	機能		：	タイムカウント
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGETIMECNT_H_
#define STAGETIMECNT_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_TimeCnt : public C_Base
	{
	public:
	private:
		float timecnt;

		bool LoadTimeCnt(char* filename);

		explicit C_TimeCnt(){}
		C_TimeCnt(const C_TimeCnt&){}
		C_TimeCnt operator=(const C_TimeCnt&){}
	public:
		C_TimeCnt(const char* fileName);
		virtual ~C_TimeCnt();

		void Draw();
		void Create(){}
		bool Control();
		float GetTimeCnt(){return timecnt;}
		float SetTimeCnt(float n){return timecnt = n;}
	};
}

//==============================================================
#endif
//==============================================================
//==========================================================//
//外部で使用する変数、構造体のextern宣言
//==========================================================//
//==========================================================//
//外部で使用する関数のextern宣言
//==========================================================//
