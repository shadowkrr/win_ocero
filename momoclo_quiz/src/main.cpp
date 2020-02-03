//**********************************************************//
//			Main.cpp　
//			プログラムはここから始まり派生する。
//**********************************************************//
//基本システムライブラリ
#include	"utility/Gp3Lib.h"

//宣言、定義等のインクルード
//ライブラリのインクルードはこのファイルに行われている。
#include "define.h"
#include "Base.h"
#include "BaseScreen.h"

//------------------------------------------------------------
//ゲーム内で必要な変数　
//TITLENAME　WINW　WINHは必要に応じて変更すること
const char USERNAME[256] =	"OH! セロ！！！！！！！ｗｗｗｗｗｗｗｗｗ";
const int	WINW	=	640;
const int	WINH	=	480;
enum		{FULL,WIN};
BOOL WindowMode = WIN;			

//------------------------------------------------------------
//　プレイヤーの数
//------------------------------------------------------------
int			PLAYER_NUM = 1;

//*************************************************************************
//　シングルトンメインクラス(ここだけしか使わないのでヘッダには定義しない)
//*************************************************************************
class C_Main
{
	C_Main();
	C_Main(const C_Main&){}
	C_Main operator=(const C_Main&){}

	C_BaseScreen* mScreen;
	C_BaseScreen::SCREEN_NAME mIsScreenNo;

	void SetScreenNo(C_BaseScreen::SCREEN_NAME name);
public:
	~C_Main();

	static C_Main& GetInstance(){
		static 	C_Main main;
		return main;
	}

	C_BaseScreen* GetScreen()const{return mScreen;}

	void IsChangeScreen();
};
//------------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------------
C_Main::C_Main() : mScreen(NULL)
{
	//最初はロゴから始める
	/*GetInstance().mScreen = new opening::C_Opening;
	SetScreenNo(C_BaseScreen::OPENING_SCREEN);*/
	GetInstance().mScreen = new stage::C_Stage;	
	SetScreenNo(C_BaseScreen::STAGE_SCREEN);
	/*GetInstance().mScreen = new logo::C_Logo;	
	SetScreenNo(C_BaseScreen::LOGO_SCREEN);*/

	OutputDebugString("constructor:C_Main\n");
}

//------------------------------------------------------------
//	デストラクタ
//------------------------------------------------------------
C_Main::~C_Main()
{
	SAFE_DELETE(C_Main::GetInstance().mScreen);
	OutputDebugString("destructor:C_Main\n");
	//_CrtDumpMemoryLeaks();										//この関数を呼んだ時点で開放されていないメモリの情報を表示する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//プログラムが終了したときに自動的に_CrtDumpMemoryLeaks()を呼んでくれる
}

//------------------------------------------------------------
//	スクリーン変更の判定
//------------------------------------------------------------
void C_Main::IsChangeScreen()
{
	if(GetInstance().mScreen->GetScreenNo() != mIsScreenNo){

		for(int i = 0; i < C_BaseScreen::MAX_SCREEN; ++i)
		{
			if(GetInstance().mScreen->GetScreenNo() == C_BaseScreen::SCREEN_NAME(i)){

				SAFE_DELETE(C_Main::GetInstance().mScreen);

				if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::LOGO_SCREEN){
					GetInstance().mScreen = new logo::C_Logo;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::CLEAR_SCREEN){
					GetInstance().mScreen = new clear::C_Clear;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::STAGE_SCREEN){
					GetInstance().mScreen = new stage::C_Stage;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::TITLE_SCREEN){
					GetInstance().mScreen = new title::C_Title;
				}
				else if(C_Main::GetInstance().mScreen->GetScreenNo() == C_BaseScreen::GAMEOVER_SCREEN){
					GetInstance().mScreen = new gameover::C_GameOver;
				}
				SetScreenNo(C_BaseScreen::SCREEN_NAME(i));
				return;
			}
		}
	}
}

//------------------------------------------------------------
//	スクリーンナンバーの格納
//------------------------------------------------------------
void C_Main::SetScreenNo(C_BaseScreen::SCREEN_NAME name)
{
	GetInstance().mScreen->SetScreenNo(name);		
	GetInstance().mIsScreenNo = name;
}

//------------------------------------------------------------
//リソースデータ宣言
//------------------------------------------------------------
IMGOBJ		ImgLoad;
IMGOBJ		ImgTable[IMG_MAX];
DSOUNDOBJ	SeTable[SE_MAX];
DSHOWOBJ	BgmTable[BGM_MAX];
//DMOVIEOBJ	MovieTable[MOVIE_MAX];
//------------------------------------------------------------
//ゲームループ 
//現在のStageNoに応じて処理が派生する。
//------------------------------------------------------------
void GameLoop()
{
	Sys_Clear();
	C_Main::GetInstance().GetScreen()->Draw();
//	Draw_ShowFPS();
  	Sys_Refresh();
	C_Main::GetInstance().GetScreen()->Control();
	C_Main::GetInstance().IsChangeScreen();
	CheckKey();
}

//------------------------------------------------------------
//別スレッドによる画像のローディング処理
//画像のロードなど起動時に一回だけ行う処理はここで行う
//------------------------------------------------------------
void OnLoading()
{
	ImgLoad = Draw_LoadObject("res/gra/Loading.png");
	Load_Bgm();
	Load_Se();
	//Load_Movie();
	//MNo = DShowMovie_Load("res/movie/Wildlife.wmv");
	//Load_Img();	
	Sleep(1000);
}
//------------------------------------------------------------
//プログラム全体の初期化
//フォントの作成、ローディング画面で使用する画像の読み込みなど
//起動時に一回だけ行う処理はここで行う
//------------------------------------------------------------
void OnCreate()
{
	srand( ( unsigned int )time( NULL ) );
	Create_Font();
	
}

//------------------------------------------------------------
//プログラム全体の後始末
//終了時に一回だけ行う処理はここで行う
//------------------------------------------------------------
void OnDestroy()
{
	//Delete_Sound();
	Draw_DeleteObject(ImgLoad);
	//Delete_Img();
}

//----------------------------------------------------
//メインループ前に呼び出されるロード専用のスクリーン
// Loading ... が表示されるスクリーン
//----------------------------------------------------
void DrawLoading()
{
	static float cnt;
	char msg[] = "Loading...";
	char temp[20] = "";
	Sys_Clear();
	strncpy(temp, msg, 7+static_cast<int>(cnt));
	Draw_Bitmap( 0, 0, ImgLoad,0 ,0 ,WINW, WINH );
	Draw_TextXY(WINW-200,WINH-50,temp,ARGB(255,255,0,0),1);
	Sys_Refresh();
	cnt += FrameTime * 2;
	if(cnt >= 4){
		cnt = 0;
	}
}

//**********************************************************//
//WINDOW　メイン
//**********************************************************//
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
	return DoWindow(hInstance,hPreInst,lpszCmdLine,nCmdShow);
}
