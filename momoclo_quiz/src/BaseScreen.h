//******************************************************************************
//	ファイル名	：	BaseScreen.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/09/15
//	機能		：	画面切り替え用の基底クラス(stage,gameover,clearなどの継承に使用)
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef BASE_SCREEN_H_
#define BASE_SCREEN_H_
//==============================================================
#include	<windows.h>	//OutputDebugString()を使用する為インクルード
//==============================================================
// クラス宣言
//==============================================================
class C_BaseScreen{
public:
	enum SCREEN_NAME{LOGO_SCREEN,TITLE_SCREEN,
			STAGE_SCREEN,CLEAR_SCREEN,GAMEOVER_SCREEN,MAX_SCREEN};
private:
	static SCREEN_NAME	mScreenNo;

	C_BaseScreen(const C_BaseScreen&){}
	C_BaseScreen operator=(const C_BaseScreen&){}
public:
	explicit C_BaseScreen(){
		OutputDebugString("constructor:C_BaseScreen\n");
	}
	virtual ~C_BaseScreen(){
		OutputDebugString("destructor:C_BaseScreen\n");
	}

	virtual void Draw() = 0;
	virtual void Control() = 0;

	static void SetScreenNo(SCREEN_NAME screenNo){mScreenNo = screenNo;}
	int  GetScreenNo()const{return mScreenNo;}
};

//==============================================================
#endif
//==============================================================