//******************************************************************************
//	ファイル名	：	TitleSelect.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/15
//	機能		：	項目選択
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef TITLESELECT_H_
#define TITLESELECT_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace title
{
	class C_Select : public C_Base
	{
	public:
		enum PLACE{PLA_UP,PLA_CENTER,PLA_DOWN};
		enum SELECT{SEL_START,SEL_RANKING,SEL_END};
	private:
		enum IS_COL_CNT{CNT_ADD,CNT_SUB};

		float TopPos;
		float CenterPos;
		float BottomPos;

		bool mDownF, mUpF;
		PLACE mPlace;
		int mSelectStageCnt;

		void IsNowSelect();
		bool NowSelectControl();
		bool IsControlCenter();
		bool IsControlUp();
		bool IsControlDown();

		explicit C_Select(){}
		C_Select(const C_Select&){}
		C_Select operator=(const C_Select&){}
	public:
		C_Select(const char* fileName, SELECT selName);
		virtual ~C_Select();

		void Draw();
		void Create(){}
		bool Control();

		void ChangeSelectStage(const IS_COL_CNT c, const int limitCnt, const int resetCnt){
			if(c == CNT_ADD){
				if(++mSelectStageCnt > limitCnt){mSelectStageCnt = resetCnt;}
			}
			else if(c == CNT_SUB){
				if(--mSelectStageCnt < limitCnt){mSelectStageCnt = resetCnt;}
			}
		}

		int GetSelectStage()const{return mSelectStageCnt;}
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
