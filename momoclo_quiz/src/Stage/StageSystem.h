//******************************************************************************
//	ファイル名	：	StageSystem.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/27
//	機能		：	システム管理
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGESYSTEM_H_
#define STAGESYSTEM_H_
//==============================================================
//==============================================================
#include	"../Base.h"
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//
namespace stage
{
	class C_System : public C_Base
	{
	public:
		enum CHARKIND{CHARA_FIRST,CHARA_SECOND,CHARA_THIRD,CHARA_FOR,CHARA_MAX};
		enum SYSSTAGE{STG_FIRST,STG_SECOND,STG_THIRD,STG_FOR,STG_MAX};
	private:
		bool mFadeF;
		bool GetNowSelectF;
		bool EndCheck();
		int mstageno;

		bool SaveStage(char* filename);
		bool SaveTimeCnt(char* filename);
		bool SaveStates(char* filename);

		explicit C_System(){}
		C_System(const C_System&){}
		C_System operator=(const C_System&){}
	public:
		explicit C_System(const char* fileName/*,int stageno*/);
		virtual ~C_System();

		CHARKIND mCharaNo;
		SYSSTAGE mstsstageNo;


		void Draw();
		void Create(){}
		bool Control();
		bool ClearCheck();

		bool GetFadeF()const{return mFadeF;}
		void SetNowSelectF(bool f){GetNowSelectF = f;}
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
