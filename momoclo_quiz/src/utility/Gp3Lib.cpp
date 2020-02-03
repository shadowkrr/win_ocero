///////////////////////////////////////////////////////////////////
//
//	ゲームプログラムⅢ
//	　　　　　授業用ライブラリ
//		 　　　　　　Gp3Lib
//								2011.2.6  Created by K.Koyanagi
//
//-----------------------------------------------------------------
// 更新履歴
//		2011.7.1	バグ修正版
//			キーパッド処理時のマウスボタンを独立
//			CheckPress,CheckFreeのintervalが取れなくなっていた不具合の修正
//			文字描画内Z軸指定時の不具合の修正
//			デバッグ関数呼び出し終了フラグのコメント修正
//		2011.8.28
//			Draw_LoadObject　formatの指定を自動に変更
//			CreateDevice　MULTITHREADフラグを追加
//			フルスクリーン時の不具合を修正
//			パッドを複数さしたときの不具合を修正
///////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_DEPRECATE				1
#pragma warning(disable : 4995)	

#include	<windows.h>

#include	"Gp3Lib.h"
//リソーステーブルの読み込みのため
#include	"../define.h"
//===================================================
//共通変数宣言
HWND	hWnd;

//===================================================
//内部変数宣言
//D3Draw 関連
LPDIRECT3D9				pD3D;
D3DPRESENT_PARAMETERS	D3DPP;
LPDIRECT3DDEVICE9		pD3DDevice;
LPD3DXSPRITE			pSprite;
LPD3DXFONT				pD3DXFont;

//FONT関連
#define	DDFONTMAX	100
LPD3DXFONT			pFontData[DDFONTMAX];

//-------------------------------------------------------
//フレームタイム計算関連
DWORD		old_time;
float		FrameTime;

//-------------------------------------------------------
//汎用変数
char		Buffer[1024];

//-------------------------------------------------------
//ローディングスレッド関連
BOOL _LoadEnd;

//-------------------------------------------------------
//Graphics関連
#define		IMGTABLEMAX		100
static D3DTEX9		D3DTextureObj[IMGTABLEMAX];

//-------------------------------------------------------
//BGM再生関連
#define	DSHOWOBJ		int
#define DSHOWMAX		10
static struct tagDxShow{
	IGraphBuilder	*pBuilder;
	IMediaControl	*pMediaControl;
	IMediaEvent		*pMediaEvent;
	IMediaSeeking	*pMediaSeeking;
	int				use;
}DShowObj[DSHOWMAX];

//-------------------------------------------------------
//SE再生関連
// DirectSound8
LPDIRECTSOUND8          lpDS;   
// プライマリサウンドバッファ
LPDIRECTSOUNDBUFFER		lpPrimary;

#define	DSOUNDOBJ			int
//同時発音できる最大数	
#define					DSPLAYMAX	10
enum					{DSNONE,DSSTART,DSPLAY,DSSTOP,DSEND};
struct tagPlayBuffer{
	LPDIRECTSOUNDBUFFER     PlayBuffer;		// プライマリサウンドバッファ
	int						ID;				//ベースになったバッファのID
	WORD					BufferSize;		//ベースになったバッファのサイズ
	int						State;			//バッファの状態
	BOOL					Loop;			//ループフラグ
};

//音を読み込める最大数
#define					DSFILEMAX	30
struct tagSoundBuffer{
	LPDIRECTSOUNDBUFFER     SoundBuffer;	//セカンダリバッファ
	WORD					BufferSize;		//バッファのサイズ
	DWORD					Fre;
	struct tagPlayBuffer	PlayBuffer[DSPLAYMAX];
}SoundBuffer[DSFILEMAX];

//=======================================================================//
//
//						関数定義部
//
//=======================================================================//

//********************************************************************//
//
//						数学関連
//
//********************************************************************//

//-------------------------------------------------------------//
//矩形同士の重なりを調べる
//-------------------------------------------------------------//
BOOL RectCheck(const RECT *rt1,const RECT *rt2)
{
	if(rt1->left < rt2->right && rt2->left < rt1->right && 
		rt1->top < rt2->bottom && rt2->top < rt1->bottom)
	{
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------//
//幅X及び高さYの値から角度θを求め、CosSinの値を求める
//戻り値　ラジアン
//-------------------------------------------------------------//
// 例
//float rad = CalcXYToSinCos(30,30);
//-------------------------------------------------------------//
float Calc_XYToRad(int x,int y)
{
	return static_cast<float>(atan2((double)y,(double)x));
}

//-------------------------------------------------------------//
//幅X及び高さYの値から角度θを求め、CosSinの値を求める
//戻り値　ラジアン
//-------------------------------------------------------------//
// 例
//float rad = CalcXYToSinCos(30,30,&AddPx,&AddPy);
//-------------------------------------------------------------//
float Calc_XYToSinCos(int x,int y,float *addx,float *addy)
{
	float rad = static_cast<float>(atan2((double)y,(double)x));
	*addx = static_cast<float>(cos(rad));
	*addy = static_cast<float>(-sin(rad));

	return rad;
}

//-------------------------------------------------------------//
//ラジアン値から角度に変換する
//戻り値　float 角度
//-------------------------------------------------------------//
//float deg = RadToDegree(r);
//-------------------------------------------------------------//
float Calc_RadToDegree(float rad)
{
	return static_cast<float>((-rad / 2.0f / PAI * 360.0f));
}

//-------------------------------------------------------------//
//角度からラジアン値に変換する
//戻り値　float ラジアン
//-------------------------------------------------------------//
//float rad = DegreeToRad(180);
//-------------------------------------------------------------//
float Calc_DegreeToRad(float degree)
{
	return static_cast<float>((degree * 2.0f * PAI / 360.0f));
}

//********************************************************************//
//
//				キー　関連関数
//
//********************************************************************//


//キー情報
static KEYINFO g_key[2][KEY_MAX];

static KEYINFO padbase[KEY_MAX] =
{
	{JOY_DOWN,	 FREE_KEY, 0}, //DOWN,
	{JOY_LEFT,	 FREE_KEY, 0}, //LEFT,
	{JOY_UP,	 FREE_KEY, 0}, //UP,
	{JOY_RIGHT,	 FREE_KEY, 0}, //RIGHT,
	{JOY_BUTTON1,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
	{JOY_BUTTON2,FREE_KEY, 0}, //KEY_MOUSE_RBTN,
	{JOY_BUTTON3,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
	{JOY_BUTTON4,FREE_KEY, 0}, //KEY_MOUSE_RBTN,
	{JOY_BUTTON5,FREE_KEY, 0}, //KEY_MOUSE_LBTN,
};

static KEYINFO keybase[KEY_MAX] =
{
	{VK_DOWN   ,FREE_KEY, 0},
	{VK_LEFT   ,FREE_KEY, 0},
	{VK_UP     ,FREE_KEY, 0},
	{VK_RBUTTON  ,FREE_KEY, 0},
	{VK_LBUTTON  ,FREE_KEY, 0},
	{VK_SPACE  ,FREE_KEY, 0},
	{VK_RETURN ,FREE_KEY, 0},
	{'U'	   ,FREE_KEY, 0},
	{'I'       ,FREE_KEY, 0},
};

//--------------------------------------------------------------------------------------------
//　内部利用　パッドの数取得
//--------------------------------------------------------------------------------------------
JOYINFO		*JoyInfo;
int		padcount;
void InitPlayerInput()
{
	//パッドカウント
	for(int i = 0 ; i < PLAYER_NUM ; i ++){
		JOYINFO		JoyInfoTemp;
		if(joyGetPos(i,&JoyInfoTemp) == JOYERR_NOERROR){
			padcount ++;
		}
	}
	//メモリ確保
	JoyInfo = (JOYINFO*)malloc(sizeof(JOYINFO)*padcount);
	//パッド生成
	for(int i = 0 ; i < PLAYER_NUM ; i ++){
		memcpy(g_key[i],keybase,sizeof(KEYINFO)*10);
		////パッド優先
		//if(i < padcount){
		//	memcpy(g_key[i],padbase,sizeof(KEYINFO)*10);
		//}else{
		//	memcpy(g_key[i],keybase,sizeof(KEYINFO)*10);
		//}
	}
}

//--------------------------------------------------------------------------------------------
//	内部利用　パッドデータ消去
//--------------------------------------------------------------------------------------------
void DeletePlayerInput()
{
	free(JoyInfo);		
}
//--------------------------------------------------------------------------------------------
//	内部利用　パッドデータ取得
//--------------------------------------------------------------------------------------------
BOOL JoyGet(int JoyID)
{
	//ジョイスティック状態取得
	if(joyGetPos(JoyID,&JoyInfo[JoyID]) != JOYERR_NOERROR){
		return 0;
	}
	return 1;
}
//-------------------------------------------------------------//
//　内部利用　ジョイスティックのボタン押下状態判定
//		引数：	Joy　ジョイスティックID　JOY１　JOY2のどちらか
//				Code ボタン番号　BUTTON1　から順に２，３，４
//				Flag　状態を受け取るポインタ変数
//-------------------------------------------------------------//
void JoyPressButton(int Joy,UINT Code,int *Flag)
{
	if ((JoyInfo[Joy].wButtons	&	Code)){
		if (*Flag==FREE_KEY)	*Flag=PUSH_KEY;
		else					*Flag=HOLD_KEY;
	}
	else{
		if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
		else									*Flag=FREE_KEY;
	}
}

//-------------------------------------------------------------//
//　内部利用　ジョイスティックの十字キー押下状態判定
//		引数：	Joy　ジョイスティックID　JOY１　JOY2のどちらか
//				Direct 方向　JOY_UP,JOY_DOWN,JOY_LEFT,JOY_RIGHTのどれか
//				Flag　状態を受け取るポインタ変数
//-------------------------------------------------------------//
void JoyPressXY(int Joy,int Direct,int *Flag)
{
	switch(Direct){
		case JOY_DOWN:
			if(JoyInfo[Joy].wYpos > 40000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;

		case JOY_UP:
			if(JoyInfo[Joy].wYpos < 2000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
		case JOY_RIGHT:
			if(JoyInfo[Joy].wXpos > 40000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
		case JOY_LEFT:
			if(JoyInfo[Joy].wXpos < 2000 ){
				if (*Flag==FREE_KEY)		*Flag=PUSH_KEY;
				else						*Flag=HOLD_KEY;
			}
			else{
				if (*Flag==PUSH_KEY || *Flag==HOLD_KEY)	*Flag=PULL_KEY;
				else									*Flag=FREE_KEY;
			}
			break;
	}


}

//--------------------------------------------------------------------------------------------
//押されたタイミングをチェックします
//--------------------------------------------------------------------------------------------
static bool IsHoldTiming(int now, int prev)
{
	return now == PUSH_KEY ||					//今押された　もしくは
		   (prev != PUSH_KEY && prev != now);	//1フレーム前に押されてなく、1フレーム前とステータスが違うとき
}

//--------------------------------------------------------------------------------------------
//はなされたタイミングをチェックします
//--------------------------------------------------------------------------------------------
static bool IsPullTiming(int now, int prev)
{
	return now == PULL_KEY ||					//今はなされた　もしくは
		   (prev != PULL_KEY && prev != now);	//1フレーム前にはなされてなく、1フレーム前とステータスが違うとき
}

//--------------------------------------------------------------------------------------------
//フレーム開始時に呼び、ゲームで使うキーの状態を調べます
//毎フレーム必ず呼ばなければなりません
//--------------------------------------------------------------------------------------------
void CheckKey()
{
	int i, prevStatus;

	for(int playernum = 0 ; playernum < PLAYER_NUM ; playernum++)
	{
		//if(playernum < padcount){
		//	for(i = 0; i < KEY_MAX; i++)
		//	{
		//		//パッドでもマウスのボタンは認識させる
		//		if( i < 8 ){
		//			//フレームカウンタチェック
		//			if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//押されているフレームをカウント
		//			else						g_key[playernum][i].frameFree++;	//はなされているフレームをカウント

		//			//前回のキーを保持し、次に押されたキーと判別する
		//			prevStatus = g_key[playernum][i].status;
		//			JoyGet(playernum);
		//			if(g_key[playernum][i].code == JOY_DOWN || g_key[playernum][i].code == JOY_UP ||
		//				g_key[playernum][i].code == JOY_RIGHT || g_key[playernum][i].code == JOY_LEFT){
		//				JoyPressXY(playernum,g_key[playernum][i].code,&g_key[playernum][i].status);
		//			}else{
		//				JoyPressButton(playernum,g_key[playernum][i].code, &g_key[playernum][i].status);
		//			}
		//			//押されたタイミングとはなされたタイミングでカウンタを0クリアする
		//			if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//ステータスが違うのでフレームを0に戻す
		//			else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//ステータスが違うのでフレームを0に戻す
		//		}else{
		//			//フレームカウンタチェック
		//			if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//押されているフレームをカウント
		//			else						g_key[playernum][i].frameFree++;	//はなされているフレームをカウント

		//			//前回のキーを保持し、次に押されたキーと判別する
		//			prevStatus = g_key[playernum][i].status;
		//			Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

		//			//押されたタイミングとはなされたタイミングでカウンタを0クリアする
		//			if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//ステータスが違うのでフレームを0に戻す
		//			else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//ステータスが違うのでフレームを0に戻す
		//		}
		//	}
		//}else{
			for(i = 0; i < KEY_MAX; i++)
			{
				//フレームカウンタチェック
				if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//押されているフレームをカウント
				else			  g_key[playernum][i].frameFree++;	//はなされているフレームをカウント

				//前回のキーを保持し、次に押されたキーと判別する
				prevStatus = g_key[playernum][i].status;
				Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

				//押されたタイミングとはなされたタイミングでカウンタを0クリアする
				if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//ステータスが違うのでフレームを0に戻す
				else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//ステータスが違うのでフレームを0に戻す
			//}
		}
	}
}

//--------------------------------------------------------------------------------------------
//キーが「おされている」かをチェックします
//intervalで与えたフレーム単位でtrueがかえってきます
//【例】
//intervalが1なら毎フレームtrue
//intervalが2なら2フレームに一回true
//intervalが5なら5フレームに一回true
//--------------------------------------------------------------------------------------------
bool CheckPress(int keyno, int interval,int playernum)
{
	interval = interval > 0 ? interval: 1;	//0除算対策

	//キーステータスが「おされた」「おされている」のどちらかで
	//キーが押されたフレームが希望設定と一致しているか？
	if((g_key[playernum][keyno].status == HOLD_KEY ||
		g_key[playernum][keyno].status == PUSH_KEY) &&
		g_key[playernum][keyno].frameHold % interval == 0)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//キーが「はなされている」かをチェックします
//intervalで与えたフレーム単位でtrueがかえってきます
//【例】
//intervalが1なら毎フレームtrue
//intervalが2なら2フレームに一回true
//intervalが5なら5フレームに一回true
//--------------------------------------------------------------------------------------------
bool CheckFree(int keyno, int interval,int playernum)
{
	interval = interval > 0 ? interval: 1;	//0除算対策

	//キーステータスが「はなされた」「はなされている」のどちらかで
	//キーが押されたフレームが希望設定と一致しているか？
	if((g_key[playernum][keyno].status == FREE_KEY ||
		g_key[playernum][keyno].status == PULL_KEY) &&
		g_key[playernum][keyno].frameFree % interval == 0)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//キーがこのフレームで「おされた」かをチェックします
//--------------------------------------------------------------------------------------------
bool CheckPush(int keyno,int playernum)
{
	if(g_key[playernum][keyno].status == PUSH_KEY)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//キーがこのフレームで「はなされた」かをチェックします
//--------------------------------------------------------------------------------------------
bool CheckPull(int keyno,int playernum)
{
	if(g_key[playernum][keyno].status == PULL_KEY)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//キーのステータスを返します
//--------------------------------------------------------------------------------------------
int GetKeyStatus(int keyno,int playernum)
{
	return g_key[playernum][keyno].status;
}

//--------------------------------------------------------------------------------------------
//キーのフレームを返します
//--------------------------------------------------------------------------------------------
int GetKeyFrameHold(int keyno,int playernum)
{
	return g_key[playernum][keyno].frameHold;
}
int GetKeyFrameFree(int keyno,int playernum)
{
	return g_key[playernum][keyno].frameFree;
}

//--------------------------------------------------------------------------------------------
//キーのフレームを初期化します
//--------------------------------------------------------------------------------------------
void InitKeyInfo(int playernum)
{
	for(int i = 0; i < KEY_MAX; i++)
	{
		g_key[playernum][i].status = FREE_KEY;
		g_key[playernum][i].frameHold  = 0;
		g_key[playernum][i].frameFree  = 0;
	}
}

//--------------------------------------------------------------------------------------------
//キーコンフィグ
//--------------------------------------------------------------------------------------------
void SetKeyFormat(CODE_KEY key, DWORD keycode,int playernum)
{
	g_key[playernum][key].code	  = keycode;
	g_key[playernum][key].status = FREE_KEY;
	g_key[playernum][key].frameHold  = 0;
	g_key[playernum][key].frameFree  = 0;
}

//--------------------------------------------------------------------------------------------
//マウスカーソルの座標を取得(戻り値)
//--------------------------------------------------------------------------------------------
POINT GetMousePotision()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	return pt;
}

//--------------------------------------------------------------------------------------------
//マウスカーソルの座標を取得(引数)
//--------------------------------------------------------------------------------------------
void GetMousePotision(POINT* pt_ptr)
{
	GetCursorPos(pt_ptr);
	ScreenToClient(hWnd, pt_ptr);
}

//--------------------------------------------------------------------------------------------
//キー状態取得
//Code　取得したいキーの仮想キーコード、ＡＳＣＩＩコード
//Flag　キーの状態を取得したいバッファへのポインタ
//--------------------------------------------------------------------------------------------
void Key_GetKey(unsigned int Code,int* Flag)
{
	if ((GetKeyState(Code)&0x80)==0x80){
		if (*Flag==FREE_KEY){
			*Flag=PUSH_KEY;
		}else{
			*Flag=HOLD_KEY;
		}
	}
	else{
		if(*Flag==PUSH_KEY||*Flag==HOLD_KEY){ 
			*Flag=PULL_KEY;
		}else{
			*Flag=FREE_KEY;
		}
	}
}


//********************************************************************//
//
//				キー　関連関数　ここまで
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//内部利用　
//　再描画を行う
//--------------------------------------------------------------------------------------------
void Draw_ReDraw()
{
	pSprite->End();
	pD3DDevice->EndScene();
	pD3DDevice->BeginScene();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
#define Draw_ReDrawCheck(x) if((x&0xff000000)!=0xff000000)Draw_ReDraw();

//********************************************************************//
//
//				FONT　関連関数
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//フォントの作成
//	Num			フォント番号
//	size		フォントサイズ
//	fontname	フォントの名前
//--------------------------------------------------------------------------------------------
//使用例
// void OnCreate(){
//		DD_CreateFont(0,30);	//指定なしはＭＳゴシック
// }
//使用例　0番に設定したサイズ30のフォントで描画処理を行う
//	DD_FontDrawText(0,0,"MainStage",RGB(255,255,0),0);
//
//--------------------------------------------------------------------------------------------
void Draw_CreateFont(int Num,int size,LPCTSTR	fontname)
{
	if(pFontData[Num] == NULL){
		 // フォントの作成
		D3DXCreateFont(pD3DDevice,			//LPDIRECT3DDEVICE9	lpD3DDev,
			size,							//UINT				Height,
			0,								//UINT				Width,
			FW_NORMAL,						//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			SHIFTJIS_CHARSET,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			fontname,						//LPCTSTR			pFacename,
			&pFontData[Num]						//LPD3DXFONT		*ppFont		&pD3DXFont
		);
	}else{
		BToMR("一度作成したフォントは再度作成できません。");
		return;
	}
}


//--------------------------------------------------------------------------------------------
//文字描画 矩形指定　Z軸指定あり
//	r		文字描画位置矩形指定
//	string	文字の幅
//	color	文字色
//	Num		使用フォント番号
//	
//--------------------------------------------------------------------------------------------
//使用例
//	DD_FontDrawText(0,0,100,30,"MainStage",D3DCOLOR_XRGB(255,255,0));
//--------------------------------------------------------------------------------------------
int Draw_FontDrawText(RECT rt,LPCSTR string,D3DCOLOR color,int Num,float z)
{
	Draw_ReDrawCheck(color);
	if(pFontData[Num] == NULL)	return 0;

	//	座標変換マトリックスの格納先
	D3DXMATRIX		matDraw;					
	D3DXMatrixTranslation(&matDraw,0,0,z);
	pSprite->SetTransform(&matDraw);

	//描画
	int res = pFontData[Num]->DrawText(pSprite,string,-1,&rt,DT_LEFT | DT_WORDBREAK,color);

	//ワールドのリセット
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);

	return res;
}

//--------------------------------------------------------------------------------------------
//文字描画 Z軸指定あり
//	x,y		文字描画位置
//	string	文字の幅
//	color	文字色
//	Num		使用フォント番号
//	z		Z軸位置
//--------------------------------------------------------------------------------------------
//使用例
//	DD_FontDrawText(0,0,"MainStage",D3DCOLOR_XRGB(255,255,0),1,0.3f);
//--------------------------------------------------------------------------------------------
int Draw_FontDrawText(int x,int y,LPCSTR string,D3DCOLOR color,int Num,float z)
{
	Draw_ReDrawCheck(color);
	if(pFontData[Num] == NULL)	return 0;
	//表示テキストサイズ計算
	TEXTMETRIC  TextMetric;    
    pFontData[Num]->GetTextMetrics(&TextMetric);
	int w = x+(TextMetric.tmMaxCharWidth*static_cast<int>(strlen(string)));
	int h = y+TextMetric.tmHeight;
	//表示用RECT作成
	RECT	rt = {x,y,w,h};

	//	座標変換マトリックスの格納先
	D3DXMATRIX		matDraw;					
	D3DXMatrixTranslation(&matDraw,0,0,z);
	pSprite->SetTransform(&matDraw);

	//描画
	int res = pFontData[Num]->DrawText(pSprite,string,-1,&rt,DT_LEFT | DT_WORDBREAK,color);

	//ワールドのリセット
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);

	return res;
}

//--------------------------------------------------------------------------------------------
//フォント削除　内部使用
//--------------------------------------------------------------------------------------------
void Draw_DeleteFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		if(pFontData[i] != NULL){
			RELEASE(pFontData[i]);
		}
	}
}

//--------------------------------------------------------------------------------------------
//フォントテーブル初期化　内部使用
//--------------------------------------------------------------------------------------------
void Draw_InitFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		pFontData[i]= NULL;
	}
}

//--------------------------------------------------------------------------------------------
//文字描画　簡易版
//	x,y		文字描画位置
//	string	文字の幅
//	color	文字色
//	t = 0	フォント番号
//--------------------------------------------------------------------------------------------
//使用例
//	Draw_TextXY(0,0,"MainStage",RGB(255,255,0));
//	
//フォント番号指定時はOnCreateにてフォントを用意すること
//void OnCreate()
//{
//		Draw_CreateFont(0,15);
//		Draw_CreateFont(1,30,"HG創英角ﾎﾟｯﾌﾟ体");
//}
//
//	Draw_TextXY(0,0,"MainStage",RGB(255,255,0),1);
//--------------------------------------------------------------------------------------------
void Draw_TextXY(int x,int y,char* string,D3DCOLOR color,int t)
{
	Draw_FontDrawText(x,y,string,color,t);
}

//********************************************************************//
//
//				FONT　関連関数 ここまで
//
//********************************************************************//

//********************************************************************//
//
//				画像描画　関連関数
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
//ビットマップを表示する 直接指定
//	引数
//		X1,Y1			表示開始座標
//		hBmp			描画したいBMPの名前
//		TempX1,TempY1	BMPからの参照座標
//		TempX2,TempY2	BMPからの幅、高さ
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_BitmapDirect(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z)
{
	//描画矩形計算
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR3	center;
	center=D3DXVECTOR3(static_cast<float>(X1),static_cast<float>(Y1),z);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	//描画
	pSprite->Draw(D3DTextureObj[hImg],&rt,NULL,&center,D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

}
//--------------------------------------------------------------------------------------------
//ビットマップを表示する
//	引数
//		X1,Y1			表示開始座標
//		hBmp			描画したいBMPの名前
//		TempX1,TempY1	BMPからの参照座標
//		TempX2,TempY2	BMPからの幅、高さ
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_Bitmap(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,float z)
{
	//描画矩形計算
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR3	center;
	center=D3DXVECTOR3(static_cast<float>(X1),static_cast<float>(Y1),z);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	//描画
	pSprite->Draw(D3DTextureObj[ImgTable[hImg]],&rt,NULL,&center,D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

}
//--------------------------------------------------------------------------------------------
//ビットマップを読み込む 読み込み位置を指定する
//--------------------------------------------------------------------------------------------
//戻り値　
//　読み込まれたLPDIRECT3DTEXTURE9の番号
//int ImgNo			//読み込み先を指定する
//char *FileName	//読み込むファイル名
//int Transparent	//透過色の設定
//
//--------------------------------------------------------------------------------------------
IMGOBJ Draw_LoadObject(int ImgNo,char *FileName,D3DCOLOR Transparent)
{
	for(int i = 0 ; i < IMGTABLEMAX ; i ++){
		if(D3DTextureObj[ImgNo] == NULL){
			if(FAILED( D3DXCreateTextureFromFileEx(pD3DDevice,(LPCSTR)(FileName),D3DX_DEFAULT ,D3DX_DEFAULT ,
									D3DX_DEFAULT ,
									0 ,
									D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED,
									D3DX_FILTER_NONE,
									D3DX_DEFAULT,
									Transparent,
									//0xff000000,	//ここを変更することで透過色を変更することができます。
									NULL,NULL,
									&D3DTextureObj[ImgNo]))){
				GetErrorString("テクスチャを生成できません");
			}
			return ImgNo;
		}
	}
	BToMR("指定の番号に読み込めませんでした。番号を確認してください。");
	return -1;
}
		 
//--------------------------------------------------------------------------------------------
//ビットマップを読み込む
//--------------------------------------------------------------------------------------------
//戻り値　
//　読み込まれたLPDIRECT3DTEXTURE9の番号
//char *FileName	//読み込むファイル名
//int Transparent	//透過色の設定
//
//--------------------------------------------------------------------------------------------
IMGOBJ Draw_LoadObject(char *FileName,D3DCOLOR Transparent)
{
	for(int i = 0 ; i < IMGTABLEMAX ; i ++){
		if(D3DTextureObj[i] == NULL){
			if(FAILED( D3DXCreateTextureFromFileEx(pD3DDevice,(LPCSTR)(FileName),D3DX_DEFAULT ,D3DX_DEFAULT ,
									D3DX_DEFAULT ,
									0 ,
									D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED,
									D3DX_FILTER_NONE,
									D3DX_DEFAULT,
									Transparent,
									//0xff000000,	//ここを変更することで透過色を変更することができます。
									NULL,NULL,
									&D3DTextureObj[i]))){
				GetErrorString("テクスチャを生成できません");
			}
			return i;
		}
	}
	BToMR("テクスチャロード数の限界を超えました。テーブルを増やしてください");
	return -1;
}

//--------------------------------------------------------------------------------------------
//ビットマップを表示する。拡張処理つき
//	引数
//		X1,Y1			表示開始座標
//		hBmp			描画したいBMPの名前
//		TempX1,TempY1	BMPからの参照座標
//		TempX2,TempY2	BMPからの幅、高さ
//		deg				回転角度
//		*pCenter		回転の中心
//		ScaleX			X軸の倍率 1.0が等倍
//		ScaleY			X軸の倍率 1.0が等倍
//		a				αチャンネル
//		r				赤の強さ
//		g				緑の強さ
//		b				青の強さ
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_TpBitmap(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
				int		deg,		//回転角
				POINT	*pCenter,	//回転中心
				float	ScaleX,		//倍率
				float	ScaleY,		//倍率
				u_char	a,			//アルファーチャンネル
				u_char	r,			//赤
				u_char	g,			//緑
				u_char	b,			//青
				float	z)
{

	//描画矩形計算
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR2		center;
	if(pCenter==NULL)		center=D3DXVECTOR2(0,0);
	else{
		center=D3DXVECTOR2(static_cast<float>(pCenter->x),static_cast<float>(pCenter->y));
	}

	if(a != 255)Draw_ReDraw();

	D3DXMATRIX matDraw;	//	座標変換マトリックスの格納先
	D3DXVECTOR3 ptCenter(0.0f, 0.0f, z);	//	描画の基準値の設定
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);	//	表示する位置を指定
	D3DXVECTOR2 draw(static_cast<float>(X1),static_cast<float>(Y1));	//	描画先座標（演算前）
	D3DXVECTOR2 scale(ScaleX, ScaleY);	//	スケーリング係数（倍率を指定）

	D3DXMatrixTransformation2D(&matDraw, &center, 0.0f, &scale, 
						&center, D3DXToRadian(-deg), &draw);
	pSprite->SetTransform(&matDraw);
	//描画
	pSprite->Draw(D3DTextureObj[ImgTable[hImg]],&rt,NULL,&ptCenter,D3DCOLOR_ARGB(a,r,g,b));
	//ワールドのリセット
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
}

//--------------------------------------------------------------------------------------------
//ビットマップを表示する。拡張処理つき 直接指定
//	引数
//		X1,Y1			表示開始座標
//		hBmp			描画したいBMPの名前
//		TempX1,TempY1	BMPからの参照座標
//		TempX2,TempY2	BMPからの幅、高さ
//		deg				回転角度
//		*pCenter		回転の中心
//		ScaleX			X軸の倍率 1.0が等倍
//		ScaleY			X軸の倍率 1.0が等倍
//		a				αチャンネル
//		r				赤の強さ
//		g				緑の強さ
//		b				青の強さ
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_TpBitmapDirect(int X1,int Y1,
				 IMGOBJ hImg,int TempX1,int TempY1,int TempX2,int TempY2,
				int		deg,		//回転角
				POINT	*pCenter,	//回転中心
				float	ScaleX,		//倍率
				float	ScaleY,		//倍率
				u_char	a,			//アルファーチャンネル
				u_char	r,			//赤
				u_char	g,			//緑
				u_char	b,			//青
				float	z)
{

	//描画矩形計算
	RECT	rt;
	SetRect(&rt,TempX1,TempY1,TempX1+TempX2,TempY1+TempY2);
	D3DXVECTOR2		center;
	if(pCenter==NULL)		center=D3DXVECTOR2(0,0);
	else{
		center=D3DXVECTOR2(static_cast<float>(pCenter->x),static_cast<float>(pCenter->y));
	}

	if(a != 255)Draw_ReDraw();

	D3DXMATRIX matDraw;	//	座標変換マトリックスの格納先
	D3DXVECTOR3 ptCenter(0.0f, 0.0f, z);	//	描画の基準値の設定
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);	//	表示する位置を指定
	D3DXVECTOR2 draw(static_cast<float>(X1),static_cast<float>(Y1));	//	描画先座標（演算前）
	D3DXVECTOR2 scale(ScaleX, ScaleY);	//	スケーリング係数（倍率を指定）

	D3DXMatrixTransformation2D(&matDraw, &center, 0.0f, &scale, 
						&center, D3DXToRadian(-deg), &draw);
	pSprite->SetTransform(&matDraw);
	//描画
	pSprite->Draw(D3DTextureObj[hImg],&rt,NULL,&ptCenter,D3DCOLOR_ARGB(a,r,g,b));
	//ワールドのリセット
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
}
//--------------------------------------------------------------------------------------------
//後始末呼び出し
//--------------------------------------------------------------------------------------------
void Draw_DeleteObject(D3DTEX9	obj)
{
	RELEASE(obj);
}

void Draw_DeleteObject(IMGOBJ	obj)
{
	RELEASE(D3DTextureObj[obj]);
}

//--------------------------------------------------------------------------------------------
//画像データサイズ取得
//--------------------------------------------------------------------------------------------
D3DSURFACE_DESC Draw_GetImageSize(IMGOBJ	obj){
	D3DSURFACE_DESC TexData;
	D3DTextureObj[obj]->GetLevelDesc(0,&TexData);
	return TexData;
}
//--------------------------------------------------------------------------------------------
//特殊合成開始
//--------------------------------------------------------------------------------------------
//使用例
//	Draw_SetRenderMode(ADD);
//	Draw_Bitmap(100,100,Test,0,0,32,32,0,NULL,1.0,1.0,100,255,255,255);
//	Draw__EndRenderMode();
//--------------------------------------------------------------------------------------------
void Draw_SetRenderMode(int Mode)
{
	//Clear時に一度Beginやっているので一回終わらせる
	pSprite->End();
	//もう一度
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if(Mode == DEC){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //アルファブレンディングの有効化
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DESTの設定
	}else if(Mode == ADD){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //アルファブレンディングの有効化
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);   //SRCの設定
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  //DESTの設定
	}else if(Mode == HALFADD){
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);    //アルファブレンディングの有効化
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);   //SRCの設定
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DESTの設定
	}
}

//--------------------------------------------------------------------------------------------
//特殊効果終了
//--------------------------------------------------------------------------------------------
//使用例
//	Draw_SetRenderMode参照
//--------------------------------------------------------------------------------------------
void Draw_EndRenderMode()
{
	Draw_SetRenderMode(DEC);
}
//********************************************************************//
//
//				画像描画　関連関数　ここまで
//
//********************************************************************//

//********************************************************************//
//
//				図形　関連関数
//
//********************************************************************//
void Draw_GetRGB(COLORREF color1,int *r,int *g,int *b)
{
	*b = ((color1 & 0x00ff0000) >> 16);
	*g = ((color1 & 0x0000ff00) >> 8);
	*r = ((color1 & 0x000000ff) >> 0);
}

//--------------------------------------------------------------------------------------------
//内部使用　ポリゴン描画
//	引数
//		X1,Y1			矩形の開始座標
//		X2,Y2			矩形の幅高さ
//		Color1			塗り色
//--------------------------------------------------------------------------------------------
void Draw_2DPolygon(int X1,int Y1,int X2,int Y2,float z,D3DCOLOR Color)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};
	D3DCOLOR c = Color;
	
	CUSTOMVERTEX v[4] = {
		{static_cast<float>(X2), static_cast<float>(Y1), z, 1.0f, c}, 
		{static_cast<float>(X2), static_cast<float>(Y2), z, 1.0f, c}, 
		{static_cast<float>(X1), static_cast<float>(Y1), z, 1.0f, c}, 
		{static_cast<float>(X1), static_cast<float>(Y2), z, 1.0f, c}
	}; 
	pD3DDevice->SetTexture(0,NULL);
	pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v , sizeof (CUSTOMVERTEX));
}

//--------------------------------------------------------------------------------------------
//矩形描画
//	引数
//		X1,Y1			矩形の開始座標
//		X2,Y2			矩形の幅高さ
//		Color1			塗り色
//		Color2			枠色
//		Size			ライン幅
//		Nakanuri		塗りつぶすか？
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_Box(int X1,int Y1,int X2,int Y2,
				D3DCOLOR Color1,D3DCOLOR Color2,
				int Size,int Nakanuri,float z)
{
	Draw_ReDrawCheck(Color1);
	if(Nakanuri){
		Draw_2DPolygon(X1,Y1,X2,Y2,z,Color1);
	}
	D3DXVECTOR2 vec[] =
	{
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y1)),	// ラインの始点
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y2)),	// ライン
		D3DXVECTOR2(static_cast<float>(X2), static_cast<float>(Y2)),	// ライン
		D3DXVECTOR2(static_cast<float>(X2), static_cast<float>(Y1)),	// ライン
		D3DXVECTOR2(static_cast<float>(X1), static_cast<float>(Y1)),	// ラインの終点
	};

	Draw_ReDrawCheck(Color2);

	for(int i = 0 ; i < 4 ; i ++){
		Draw_Line(static_cast<int>(vec[i].x),static_cast<int>(vec[i].y),static_cast<int>(vec[i+1].x),static_cast<int>(vec[i+1].y),
					Color2,Size,z);
	}
}

//--------------------------------------------------------------------------------------------
//直線描画
//	引数
//		X1,Y1			矩形の開始座標
//		X2,Y2			矩形の幅高さ
//		Color2			色
//		Size			ライン幅
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_Line(int X1,int Y1,int X2,int Y2,D3DCOLOR Color1,int Size,float z)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};

	D3DCOLOR c = Color1;
	for(int i = 0; i < Size ; i++){
		int tx,ty;
		if(abs(X1-X2) > abs(Y1-Y2)){
			tx = 0;
			ty = i-Size/2;
		}else{
			tx = i-Size/2;
			ty = 0;
		}
		CUSTOMVERTEX v[2] = {
			{static_cast<float>(X1)+tx, static_cast<float>(Y1)+ty, z, 1.0f, c}, 
			{static_cast<float>(X2)+tx, static_cast<float>(Y2)+ty, z, 1.0f, c}, 
		}; 
		pD3DDevice->SetTexture(0,NULL);
		pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v , sizeof (CUSTOMVERTEX));
	}
}

//--------------------------------------------------------------------------------------------
//円描画
//	引数
//		X1,Y1			矩形の開始座標
//		X2,Y2			矩形の幅高さ
//		Color1			塗り色
//		Color2			枠色
//		Size			ライン幅
//		Nakanuri		塗りつぶすか？
//		z				表示Z軸座標0.0 から1.0の間で指定　0.0に近いほど手前になる
//--------------------------------------------------------------------------------------------
void Draw_Ellipse(int X1,int Y1,int X2,int Y2,
				D3DCOLOR Color1,D3DCOLOR Color2,
				int Size,int Nakanuri,float z)
{
	Draw_ReDrawCheck(Color1);
	//色作成
	D3DCOLOR c = Color1;

	D3DXVECTOR2 vec[181];
	int rx = (X2-X1)/2;
	int ry = (Y2-Y1)/2;
	float x,y;
	for(int angle = 0 ; angle < 180 + 1; angle++)
	{
		x = static_cast<float>(cos(angle*2*3.1415*2/360)*rx+X1+rx);
		y = static_cast<float>(sin(angle*2*3.1415*2/360)*ry+Y1+ry);
		vec[angle] = D3DXVECTOR2(static_cast<float>(x),static_cast<float>(y));	// ラインの始点
	}
	//塗り描画
	if(Nakanuri == 1){
		struct CUSTOMVERTEX {
				FLOAT x, y, z;
				FLOAT rhw;
				DWORD color;
		};

		CUSTOMVERTEX v[182];
		v[0].x = static_cast<float>(X1+rx);
		v[0].y = static_cast<float>(Y1+ry);
		v[0].z = z;
		v[0].rhw = 1.0f;
		v[0].color = c;
		for(int i = 1 ; i < 181+1  ; i++)
		{
			v[i].x = vec[i-1].x;
			v[i].y = vec[i-1].y;
			v[i].z = z;
			v[i].rhw = 1.0f;
			v[i].color = c;
		}
		pD3DDevice->SetTexture(0,NULL);
		pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 180, v , sizeof (CUSTOMVERTEX));
	}
	
	Draw_ReDrawCheck(Color2);
	//円周描画
	for(int i = 1 ; i < 180; i++)
	{
		Draw_Line(static_cast<int>(vec[i].x),static_cast<int>(vec[i].y),static_cast<int>(vec[i+1].x),static_cast<int>(vec[i+1].y),Color2,Size,z);
	}	
	Draw_Line(static_cast<int>(vec[179].x),static_cast<int>(vec[179].y),static_cast<int>(vec[1].x),static_cast<int>(vec[1].y),Color2,Size,z);
}
//********************************************************************//
//
//				図形　関連関数　ここまで
//
//********************************************************************//

//********************************************************************//
//
//				システム／デバッグ　関連関数
//
//********************************************************************//
//-------------------------------------------------------------//
//フルスクリーン切り替え
//-------------------------------------------------------------//
//	void OnCreate(){
//		Sys_SetScreenMode();
//		InitDx();
//	}
//-------------------------------------------------------------//
void Sys_SetScreenMode()
{
	if(YESNO("フルスクリーンで実行しますか？") == IDNO){
		WindowMode = TRUE;
	}else{
		WindowMode = FALSE;
		SetWindowLong(hWnd,GWL_STYLE,WS_POPUP | WS_VISIBLE);
	}
}

//--------------------------------------------------------------------------------------------
//エラー文字列表示
//　MBにて直前に発生したエラーを出力
//--------------------------------------------------------------------------------------------
//使用例
//	char Buff[256];
//	GetErrorString(Buff);
//--------------------------------------------------------------------------------------------
void GetErrorString(char* text)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	BToMR("%s\n%s",text,lpMsgBuf);
}

//--------------------------------------------------------------------------------------------
//DirectDraw初期化
//--------------------------------------------------------------------------------------------
void InitDx()
{
	//DX MAIN INIT
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(pD3D == NULL){
		BToMR("D3D Create Error");
		return;
	}

	//WINDOW MODE INIT
	ZeroMemory(&D3DPP,sizeof(D3DPP));
	if(WindowMode == TRUE){
		D3DPP.Windowed			= TRUE;							//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select
		D3DPP.BackBufferFormat	= D3DFMT_UNKNOWN;				//FORMAT AUTO Select
		D3DPP.BackBufferCount	= 1;
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}else{
		D3DPP.BackBufferWidth	= WINW;
		D3DPP.BackBufferHeight	= WINH;
		D3DPP.BackBufferCount	= 1;
		D3DPP.BackBufferFormat	= D3DFMT_X8R8G8B8;				//FORMAT Select
		D3DPP.Windowed			= FALSE;						//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.FullScreen_RefreshRateInHz = 60;
	}
	//Create Device
	HRESULT hr;
	//HAL(pure vp)
	if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
	{
		//HAL(soft vp)
		if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
		{
			//REF
			if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,&D3DPP,&pD3DDevice)))
			{
				BToMR("CreateDevice Error");
				return ;
//				END;
			}
		}
	}
	
	// 両面描画モードの指定
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
	// ノーライティングモード 	
    pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//--------------------------------------
	// テクスチャステージの設定
	//--------------------------------------
	// テクスチャステージの設定 
	pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	// テクスチャフィルターの設定
	pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	D3DXCreateSprite(pD3DDevice,&pSprite);

}

//--------------------------------------------------------------------------------------------
//Dxライブラリの初期化　
//--------------------------------------------------------------------------------------------
//	プログラム起動時に呼び出し
//--------------------------------------------------------------------------------------------
void InitDxLib()
{
	InitDx();
	//ユーザーフォント指定
	Draw_InitFont();
	//システムフォント設定

	for(int i = 0 ; i < IMGTABLEMAX;i++){
		D3DTextureObj[i] = NULL;
	}
}

//-------------------------------------------------------------//
//DirectDraw終了
//-------------------------------------------------------------//
//　プログラム終了時に呼び出すこと
//-------------------------------------------------------------//
void DelDxLib()
{
	for(int i = 0 ; i < IMGTABLEMAX;i++){
		if(D3DTextureObj[i] != NULL)
		{
			RELEASE(D3DTextureObj[i]);
		}
	}

	Draw_DeleteFont();
	RELEASE(pSprite);
	RELEASE(pD3DDevice);
	RELEASE(pD3D);
}


//--------------------------------------------------------------------------------------------
// FPSの表示
//--------------------------------------------------------------------------------------------
//　DD_Clear();
//	DD_ShowFPS();
//	DDDFPS();		でも可
//　DD_Refresh();
//--------------------------------------------------------------------------------------------
void Draw_ShowFPS()
{
	//サンプリングを2秒に広げる
	static LONG FpsTime=(timeGetTime()+1000L);
	static int FpsData = 10;
	static int FpsCnt = 1;
	if (timeGetTime()>=(unsigned)FpsTime){
		FpsData=FpsCnt;
		FpsTime=(timeGetTime()+1000L);
		FpsCnt=1;
	}
	//FPSカウント
	FpsCnt++;
	char Buffer[1024];	
	sprintf_s(Buffer,"fps %3d / FrameTime %0.3f",FpsData,FrameTime);
	Draw_TextXY(0,0,Buffer,ARGB(255,255,200,200),0);
}

//--------------------------------------------------------------------------------------------
//プログラム終了
//--------------------------------------------------------------------------------------------
void Sys_QuitAPI(){
	DShow_AllStop();	//すべてのBGMを終了
	SendMessage(hWnd,WM_CLOSE,0,0);

}

//--------------------------------------------------------------------------------------------
//サーフェス転送
//--------------------------------------------------------------------------------------------
void Sys_Refresh()
{
	if(pSprite != NULL)
		pSprite->End();
	//Draw End
	if(pD3DDevice != NULL)
		pD3DDevice->EndScene();
	//ReDraw
	HRESULT hr;
	if(pD3DDevice != NULL)
		hr = pD3DDevice->Present(NULL,NULL,NULL,NULL);

	if(hr == D3DERR_DEVICELOST){
		//状態チェック
		if(pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			pSprite->OnLostDevice();
			//リセット
			hr = pD3DDevice->Reset(&D3DPP);
			if(hr != D3D_OK){
				return;
			}
			//再取得
			pSprite->OnResetDevice();
		}
	
	}
}
//--------------------------------------------------------------------------------------------
//オフスクリーンの内容を黒で初期化する
//--------------------------------------------------------------------------------------------
void Sys_Clear()
{
	//Scene Clear Black
	if(pD3DDevice != NULL)
		pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,64),1.0f,0);
	//Draw Start
	if(pD3DDevice != NULL)
		pD3DDevice->BeginScene();
	if(pSprite != NULL)
		pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}


//--------------------------------------------------------------------------------------------
//ループ制御
//--------------------------------------------------------------------------------------------
//内部利用
//--------------------------------------------------------------------------------------------
BOOL Sys_LoopControl(int FPS){
	static int _Time = timeGetTime();	//初期時間格納
	static float _FPS;					//FPS計算用バッファ
	if( FPS != 0){
		_FPS = 1000.0f / float(FPS);		//与えられたFPSからFrameTimeを計算	
		if(timeGetTime() - _Time > (unsigned)_FPS){		//FrameTimeが経過するまで待つ
			_Time = timeGetTime();
			return 1;
		}
		return 0;
	}else{
		return 1;
	}
}

//--------------------------------------------------------------------------------------------
//ロードスレッド
//--------------------------------------------------------------------------------------------
void Sys_LoadThread(HWND hWnd)
{
	OnLoading();
	InterlockedExchange((volatile LONG*)&_LoadEnd,1);
//	_LoadEnd = 1;
}

//--------------------------------------------------------------------------------------------
//ロードスレッド開始
//--------------------------------------------------------------------------------------------
void Sys_CreateLoadThread()
{
	HANDLE newThread;
	InterlockedExchange((volatile LONG*)&_LoadEnd,0);
//	_LoadEnd = 0;
	newThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Sys_LoadThread,hWnd,0,NULL);
}

//--------------------------------------------------------------------------------------------
//WINDOW　プロシージャ
//--------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HRESULT res;
	static float tempFrame;
	switch (msg) {
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)		END;
			break;
		case WM_CREATE:
			res = timeBeginPeriod(1);
			break;
		case WM_PAINT:
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
			break;
        case WM_DESTROY:
			timeEndPeriod(1);
			PostQuitMessage(0); 
			break;
		default: return(DefWindowProc(hWnd, msg, wParam, lParam)); 
    }
    return (0L);
}

//--------------------------------------------------------------------------------------------
//WINDOW　設定関数
//--------------------------------------------------------------------------------------------
BOOL DoWindow(HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{

	MSG			lpMsg;
	WNDCLASS	myProg;
	LPCSTR		Name = "Window_Class_Name";
	if (!hPreInst) {                                        
		myProg.style            = CS_HREDRAW | CS_VREDRAW;
   	 	myProg.lpfnWndProc      = WndProc;
   	 	myProg.cbClsExtra       = 0;
   	 	myProg.cbWndExtra       = 0;
    	myProg.hInstance        = hInstance;
		myProg.hIcon            = LoadIcon(hInstance,"MAIN");
    	myProg.hCursor          = LoadCursor(NULL, IDC_ARROW);
    	myProg.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    	myProg.lpszMenuName     = NULL;
    	myProg.lpszClassName    = (Name);
    	if (!RegisterClass(&myProg)) return FALSE;
	}

	hWnd=CreateWindow(
		(Name)	, 
		(USERNAME),
		/*WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU	,*/
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_THICKFRAME | WS_TABSTOP | WS_MINIMIZEBOX | WS_POPUPWINDOW,
		0			,
		0			,

		WINW
		+GetSystemMetrics(SM_CXFRAME)*2			,
		WINH
		+GetSystemMetrics(SM_CYCAPTION)
		+GetSystemMetrics(SM_CYFRAME)*2			,

		NULL					,
		NULL					,
		hInstance				,
		NULL
	);
	//ウィンドウモードの決定
	Sys_SetScreenMode();

	InitDxLib();			//Dx関連初期化
	DSound_Init();			//DSOUND初期化
	DShow_Init();			//DSHOW初期化
	OnCreate();				//初期化関数
	Sys_CreateLoadThread();	//ロード用スレッド起動
	InitPlayerInput();		//ユーザー入力初期化
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	while(true){
		if(PeekMessage(&lpMsg,NULL,0,0,PM_REMOVE)){			//メッセージ処理
			if(lpMsg.message == WM_QUIT)	break;		
			TranslateMessage(&lpMsg);
			DispatchMessage(&lpMsg);
		}else if(Sys_LoopControl(60)){						//ゲームループ
			FrameTime = (timeGetTime()-old_time)/F(1000);	//FrameTime計算
			old_time = timeGetTime();
			if(_LoadEnd){									//メインループ呼び出し
				GameLoop();
				DSound_EndCheck();							//DSOUND終了チェック
				DShow_EndCheck();							//DSHOW終了チェック
			}else{											//ロード画面呼び出し
				DrawLoading();
			}
		}
		Sleep(1);
	}

	/*while(true){
		if(PeekMessage(&lpMsg,NULL,0,0,PM_REMOVE)){
			if(lpMsg.message == WM_QUIT)	break;		
			TranslateMessage(&lpMsg);
			DispatchMessage(&lpMsg);
		}else Sys_MainThread(hwnd);
		Sleep(1);
	}*/
	OnDestroy();			//ゲーム後始末処理
	DSound_Del();			//DSOUND破棄
	DShow_Del();			//DSHOW破棄
	DeletePlayerInput();	//ユーザー入力破棄
	DelDxLib();				//Dx関連破棄
	return(static_cast<int>(lpMsg.wParam));
}

//********************************************************************//
//
//				システム　関連関数 ここまで
//
//********************************************************************//

//********************************************************************//
//
//				デバッグ　関連関数
//
//********************************************************************//
//-------------------------------------------------------------//
//メッセージボックスでのYesNo確認
//-------------------------------------------------------------//
//	使用例
/*
	int res = Dbg_BoxToMessage("フルスクリーンにしますか？");
	if(res == IDYES){
		
	}else{

	}
*/
//-------------------------------------------------------------//
int Dbg_YesNo(LPCSTR str,...)
{
	char buf[256];
//	wsprintf(buf,str,(char*)(&str + 1));
	vsprintf(buf,str,(char*)(&str+1));
	return MessageBox(hWnd,buf,"Message",MB_YESNO);
}

//--------------------------------------------------------------------------------------------
//ファイルOUT関数
//	引数
//		str			フォーマット前文字列
//		...			可変個引数指定
//--------------------------------------------------------------------------------------------
void Dbg_FileOut(char* str,...){

	FILE*	fp;
	char buff[128];
	vsprintf(buff,(char*)str,(char*)(&str+1));
//	strcat(buff,"\n");
	if ((fp=fopen("debug.txt","at"))!=NULL){
		fprintf(fp,"%s",buff);
		fclose(fp);
	}
}
//-------------------------------------------------------------//
//Dbg_FileOutにより作成されるDebug.txtの削除
//-------------------------------------------------------------//
void Dbg_FileDel()
{
	remove("debug.txt");
}

//--------------------------------------------------------------------------------------------
//タイトルへメッセージの表示
//	引数
//		str			フォーマット前文字列
//		...			可変個引数指定
//--------------------------------------------------------------------------------------------
void Dbg_TilteToMessage(char* str,...){

	char buf[256];
	vsprintf(buf,(char*)str,(char*)(&str+1));
	SetWindowText(hWnd,(LPCSTR)buf);
}

//--------------------------------------------------------------------------------------------
//メッセージボックスへの表示
//	引数
//		str			フォーマット前文字列
//		...			可変個引数指定
//--------------------------------------------------------------------------------------------
void Dbg_BoxToMessage(char *str,...){

	char buf[256];
	vsprintf(buf,(char*)str,(char*)(&str+1));
	MessageBox(hWnd,(LPCSTR)buf,"Message",MB_OK);
}

//--------------------------------------------------------------------------------------------
//デバッグ情報表示関数　ウインドウ中の指定位置
//	引数
//		x,y			描画座標
//		str			フォーマット前文字列
//		...			可変個引数指定
//--------------------------------------------------------------------------------------------
//	使用例
//		Dbg_StringOutXY(10,10,"%d回",a); 
//--------------------------------------------------------------------------------------------
void		Dbg_StringOutXY(int x,int y,char* str,...)
{
	char buff[128];
	vsprintf(buff,str,(char*)(&str+1));
	Draw_TextXY(x,y,buff,ARGB(255,255,0,255),0);
}

//--------------------------------------------------------------------------------------------
//	入力関数
//--------------------------------------------------------------------------------------------
#define IDD_DIALOG1                     101
#define ID_DLG_INPUT                    101
#define ID_EDIT                         1001
#define ID_LABEL                        1002

struct DLG_DATA
{
	char title[MAX_PATH];
	char label[MAX_PATH];
	char get[MAX_PATH];
	int size;
};

//--------------------------------------------------------------------------------------------
//ダイアログプロシージャ
//	外部ファイルから呼び出し出来ません
//--------------------------------------------------------------------------------------------
static int CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static DLG_DATA* d_ptr = NULL;

	switch(msg)
	{
		//ダイアログ初期化処理
		case WM_INITDIALOG:
			d_ptr = (DLG_DATA*)lParam;	//呼び出し元からデータへのポインタを受け取る
			if(d_ptr->title[0] != '\0') SetWindowText(hDlg, d_ptr->title);	//タイトル変更
			if(d_ptr->label[0] != '\0') SetWindowText(GetDlgItem(hDlg, ID_LABEL), d_ptr->label);//ラベル変更
			SetFocus(hDlg);
			break;
		//ボタン処理
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				//「ok」ボタンが押された
				case IDOK:
					GetDlgItemText(hDlg, ID_EDIT, d_ptr->get, MAX_PATH);
					EndDialog(hDlg, 1);
					break;
				//「キャンセル」ボタンもしくは「x」ボタンが押された
				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;
				default:
					return 0;	//Windowsに任せるので0を返す
			}
			break;
		//既定の処理：Windowsに任せる
		default:
			return 0;	//Windowsに任せるので0を返す
	}
	return 1;		//自分でメッセージを処理したので1を返す
}

//--------------------------------------------------------------------------------------------
//文字列入力関数
//	引数
//		get			取得した文字の格納先
//		size		getのサイズ
//		title		タイトルの文字列
//		label		エディットボックスに表示する文字列
//--------------------------------------------------------------------------------------------
//	使用例
//		char str[100];
//		Dbg_InputDialog(str, 100, "確認", "名前を入力してください");
//--------------------------------------------------------------------------------------------
DLG_ERROR Dbg_InputDialog(char* get, int size, const char* title, const char* label)
{
	DLG_DATA data;

	if(get == NULL) return DLG_ERROR_INVALIDBUFFER;
	//タイトルコピー
	if(title != NULL)
	{
		memcpy(&data.title, title, MAX_PATH);
	}
	else
	{
		data.title[0] = '\0';
	}
	//ラベルコピー
	if(label != NULL)
	{
		memcpy(&data.label, label, MAX_PATH);
	}
	else
	{
		data.label[0] = '\0';
	}
	//ダイアログ表示
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DLG_INPUT),
					  hWnd, (DLGPROC)DlgProc, (LPARAM)&data) == 0)
	{
		//「キャンセル」ボタンもしくは「x」ボタンが押された
		return DLG_ERROR_CANCEL;
	}
	//「ok」が押された（入力されていなくてもココにくる）
	memcpy(get, data.get, size - 1);
	get[size-1] = '\0';
	return DLG_OK;
}

//********************************************************************//
//
//				デバッグ　関連関数 ここまで
//
//********************************************************************//
//********************************************************************//
//
//				BGM再生　関連関数 
//
//********************************************************************//
//関数一覧
/*
BOOL		DShow_Init()						DShowの初期化
void		DShow_Del();						DShow_の後始末
DSHOWOBJ	DShow_LoadFile(LPCWSTR)				音楽ファイルの読み込み
BOOL		DShow_Play();						再生
void		DShow_RateControl(DSHOWOBJ,float)	再生速度変更
void		DShow_EndCheck()					終了チェック
void		DShow_Stop(DSHOWOBJ)				再生ストップ
void		DShow_AllStop()						全停止
LONGLONG	DShow_GetCurrentPos(int index)		再生位置取得
void		DShow_SetStartPos(int index)		スタート位置に設定
*/
//-------------------------------------------------------
/*
使い方

	グローバルとして宣言
	DSHOWOBJ		test;

	再生処理
	DShow_Play(test);		//通常再生-すべてループ再生

	停止処理
	DShow_Stop(test);		//同じ音声をすべて停止
	DShow_AllStop();		//再生中のすべての音声を停止

	エフェクト
	DShow_RateControl(test,1.0);	//testの再生速度を指定した倍率に変更する。

*/
//--------------------------------------------------------------------------------------------
// DirectShowの初期化
//--------------------------------------------------------------------------------------------
DSHOWOBJ	DShow_LoadFile(char* filename)
{
	wchar_t name[256];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename, -1, name, 256);

	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 0){
			if(FAILED(DShowObj[i].pBuilder->RenderFile(name,NULL))){
				BToM("DShow\nファイル読み込みに失敗しました\nファイル名、コーデックの確認をお願いします。");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaControl,
						(void**)&DShowObj[i].pMediaControl))){
				BToM("DShow\nMediaControlの設定に失敗しました");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaEvent,
						(void**)&DShowObj[i].pMediaEvent))){
					BToM("DShow\nMediaEventの設定に失敗しました");
			}
			if(FAILED(DShowObj[i].pBuilder->QueryInterface(IID_IMediaSeeking,
						(void**)&DShowObj[i].pMediaSeeking))){
					BToM("DShow\nMediaSeekingの設定に失敗しました");
			}
			GUID format = TIME_FORMAT_FRAME;
			DShowObj[i].pMediaSeeking->SetTimeFormat(&format);
			DShowObj[i].use = 1;
			return i;
		}
	}
	BToM("DShow\n読み込み限界数を超えました");
	return -1;
}

//--------------------------------------------------------------------------------------------
// DirectShowの初期化
//--------------------------------------------------------------------------------------------
DSHOWOBJ	DShow_LoadFile(int BgmNo,char* filename)
{
	wchar_t name[256];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename, -1, name, 256);

	if(DShowObj[BgmNo].use == 0){
		if(FAILED(DShowObj[BgmNo].pBuilder->RenderFile(name,NULL))){
			BToM("DShow\nファイル読み込みに失敗しました\nファイル名、コーデックの確認をお願いします。");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaControl,
					(void**)&DShowObj[BgmNo].pMediaControl))){
			BToM("DShow\nMediaControlの設定に失敗しました");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaEvent,
					(void**)&DShowObj[BgmNo].pMediaEvent))){
				BToM("DShow\nMediaEventの設定に失敗しました");
		}
		if(FAILED(DShowObj[BgmNo].pBuilder->QueryInterface(IID_IMediaSeeking,
					(void**)&DShowObj[BgmNo].pMediaSeeking))){
				BToM("DShow\nMediaSeekingの設定に失敗しました");
		}
		GUID format = TIME_FORMAT_FRAME;
		DShowObj[BgmNo].pMediaSeeking->SetTimeFormat(&format);
		DShowObj[BgmNo].use = 1;
		return BgmNo;
	}
	return -1;
}

//--------------------------------------------------------------------------------------------
// DirectShowの後始末
//--------------------------------------------------------------------------------------------
void	DShow_Del()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			DShowObj[i].pMediaControl->Release();
			DShowObj[i].pMediaEvent->Release();
			DShowObj[i].pMediaSeeking->Release();
			DShowObj[i].pBuilder->Release();
		}
	}
	CoUninitialize();
}

//--------------------------------------------------------------------------------------------
// GraphBuilderの生成
//--------------------------------------------------------------------------------------------
BOOL	DShow_Init()
{
	CoInitialize(NULL);

	HRESULT	hr;
	for(int i = 0 ; i < DSHOWMAX ; i++){
		hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
							IID_IGraphBuilder,(void **)&DShowObj[i].pBuilder);
		if(FAILED(hr)){
			BToM("DShow:GraphBuilderオブジェクトの生成に失敗しました%d",i);
			return 0;
		}
		DShowObj[i].use = 0;
	}
	return 1;
}

//--------------------------------------------------------------------------------------------
// ファイルの再生
//--------------------------------------------------------------------------------------------
BOOL	DShow_Play(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		HRESULT	hr = DShowObj[BgmTable[index]].pMediaControl->Run();
		if(FAILED(hr)){
			BToM("DShow:再生に失敗しました1");
			return 0;
		}
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// 再生速度の再生
//--------------------------------------------------------------------------------------------
void DShow_RateControl(DSHOWOBJ index,float rate)
{
	if(rate < 0)	return;
	DShowObj[BgmTable[index]].pMediaSeeking->SetRate(rate);
}

//--------------------------------------------------------------------------------------------
// 停止
//--------------------------------------------------------------------------------------------
void DShow_Stop(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		LONGLONG	start = 0;
		DShowObj[BgmTable[index]].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
				NULL,AM_SEEKING_NoPositioning);
		DShowObj[BgmTable[index]].pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// 一時停止
//--------------------------------------------------------------------------------------------
void DShow_Pause(DSHOWOBJ index)
{
	if(DShowObj[BgmTable[index]].use == 1){
		DShowObj[BgmTable[index]].pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// 全停止
//--------------------------------------------------------------------------------------------
void DShow_AllStop()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			DShowObj[i].pMediaControl->Stop();
		}
	}
}

//--------------------------------------------------------------------------------------------
// 終了確認
//--------------------------------------------------------------------------------------------
void DShow_EndCheck()
{
	for(int i = 0 ; i < DSHOWMAX ; i++){
		if(DShowObj[i].use == 1){
			long code;
			//再生状態取得
			DShowObj[i].pMediaEvent->WaitForCompletion(0,&code);
			//再生終了時はループを行う。
			if(code == EC_COMPLETE){
				LONGLONG	start = 0;
				DShowObj[i].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
					NULL,AM_SEEKING_NoPositioning);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// 再生位置取得
//--------------------------------------------------------------------------------------------
LONGLONG DShow_GetCurrentPos(int index)
{
	LONGLONG	Current;
	DShowObj[BgmTable[index]].pMediaSeeking->GetCurrentPosition(&Current);
	return Current;
}

//--------------------------------------------------------------------------------------------
// 再生位置初期化
//--------------------------------------------------------------------------------------------
void DShow_SetStartPos(int index)
{
	LONGLONG	start = 0;
	DShowObj[BgmTable[index]].pMediaSeeking->SetPositions(&start,AM_SEEKING_AbsolutePositioning,
			NULL,AM_SEEKING_NoPositioning);
}


//********************************************************************//
//
//				BGM再生　関連関数 ここまで
//
//********************************************************************//

//********************************************************************//
//
//				SE再生　関連関数
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
// DirectSoundの初期化
//--------------------------------------------------------------------------------------------
void DSound_Init()
{
	if(DSound_Create()){
		if(!DSound_CreatePrimaryBuffer()){
			BToM("プライマリバッファの生成に失敗しました");
		}
		DSound_CreateSecondaryBuffer();
	}else{
		BToM("DSoundの生成に失敗しました");
	}

}

//--------------------------------------------------------------------------------------------
// DirectSoundの作成
//--------------------------------------------------------------------------------------------
BOOL DSound_Create( void )
{
    HRESULT ret;

    // DirectSound8を作成
    ret = DirectSoundCreate8( NULL,&lpDS,NULL );
    if( FAILED(ret) ) {
        BToM( "サウンドオブジェクト作成失敗\n","" );
        return FALSE;
    }

    // 強調モード
    ret = lpDS->SetCooperativeLevel( hWnd,DSSCL_EXCLUSIVE|DSSCL_PRIORITY );
    if( FAILED(ret) ) {
        BToM( "強調レベル設定失敗\n","" );
        return FALSE;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------
// バッファの作成
//--------------------------------------------------------------------------------------------
void DSound_CreateSecondaryBuffer()
{
	int i ;
	for( i = 0 ; i < DSFILEMAX ; i ++){
		SoundBuffer[i].SoundBuffer = NULL;
		SoundBuffer[i].BufferSize = 0;
	}

}

//--------------------------------------------------------------------------------------------
// サウンド再生
//--------------------------------------------------------------------------------------------
void DSound_Play(DSOUNDOBJ SoundNo)
{
	int i;

	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){		//読み込まれているならば問題ない
		//空いているバッファ検出
		for(i = 0 ; i < DSPLAYMAX ; i ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State == DSNONE){
				//再生開始
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].ID = SoundNo;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State = DSPLAY;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].PlayBuffer->Play(0,0,0);
				//PlayBuffer[i].BufferSize = SoundBuffer[SoundNo].BufferSize;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].Loop = 0;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// サウンドループ再生
//--------------------------------------------------------------------------------------------
void DSound_PlayLoop(DSOUNDOBJ SoundNo)
{
	int i;

	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){		//読み込まれているならば問題ない
		//空いているバッファ検出
		for(i = 0 ; i < DSPLAYMAX ; i ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State == DSNONE){
				//再生開始
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].ID = SoundNo;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].State = DSPLAY;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].PlayBuffer->Play(0,0,0);
				//SoundBuffer[SoundNo].PlayBuffer[i].BufferSize = SoundBuffer[SoundNo].BufferSize;
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[i].Loop = 1;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// サウンド停止
//	同じ効果音をすべて停止する
//--------------------------------------------------------------------------------------------
void DSound_Stop(DSOUNDOBJ SoundNo)
{
	int j;
	if(SoundBuffer[SeTable[SoundNo]].BufferSize != 0){
		for(j = 0 ; j < DSPLAYMAX ; j ++){
			if(SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].State == DSPLAY){
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].PlayBuffer->Stop();
				SoundBuffer[SeTable[SoundNo]].PlayBuffer[j].State = DSNONE;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// すべてのサウンド停止
//--------------------------------------------------------------------------------------------
void DSound_AllStop()
{
	int i,j;
	for(i = 0 ; i < DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){
			for(j = 0 ; j < DSPLAYMAX ; j ++){
				if(SoundBuffer[i].PlayBuffer[j].State == DSPLAY){
					SoundBuffer[i].PlayBuffer[j].PlayBuffer->Stop();
					SoundBuffer[i].PlayBuffer[j].State = DSNONE;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// 終了チェック
//--------------------------------------------------------------------------------------------
void DSound_EndCheck()
{
	int i,j;
	DWORD Status;
	int a = 0;
	for(i = 0 ; i< DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){		//現在読み込み中のバッファ		
			for(j = 0 ; j < DSPLAYMAX ; j++){
				if(SoundBuffer[i].PlayBuffer[j].State == DSPLAY){		//現在再生中の複製バッファ
					//終了と認識する
					SoundBuffer[i].PlayBuffer[j].PlayBuffer->GetStatus(&Status);
					if( (Status & DSBSTATUS_PLAYING) != 1){
						if(SoundBuffer[i].PlayBuffer[j].Loop){
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->SetCurrentPosition(0);
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->Play(0,0,0);
						}else{
							SoundBuffer[i].PlayBuffer[j].State = DSNONE;
							SoundBuffer[i].PlayBuffer[j].BufferSize = 0 ;
							SoundBuffer[i].PlayBuffer[j].PlayBuffer->Stop();
						}
					}else{
					}
					a++;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// プライマリサウンドバッファの作成
//--------------------------------------------------------------------------------------------
BOOL DSound_CreatePrimaryBuffer( void )
{
    HRESULT ret;
    WAVEFORMATEX wf;

    // プライマリサウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
    dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsdesc.dwBufferBytes = 0;
    dsdesc.lpwfxFormat = NULL;
    ret = lpDS->CreateSoundBuffer( &dsdesc,&lpPrimary,NULL );
    if( FAILED(ret) ) {
        BToM( "プライマリサウンドバッファ作成失敗\n","" );
        return FALSE;
    }

    // プライマリバッファのステータスを決定
    wf.cbSize = sizeof( WAVEFORMATEX );
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.nSamplesPerSec = 44100;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    ret = lpPrimary->SetFormat( &wf );
    if( FAILED(ret) ) {
        BToM( "プライマリバッファのステータス失敗\n","" );
        return FALSE;
    }
    return TRUE;
}

//--------------------------------------------------------------------------------------------
//DSoundの後始末
//--------------------------------------------------------------------------------------------
BOOL DSound_Del( void )
{
	int i;
	for( i = 0 ; i < DSFILEMAX ; i ++){
		if(SoundBuffer[i].BufferSize != 0){	
			SoundBuffer[i].SoundBuffer->Release();
		}
	}
    if( lpPrimary ) {
        lpPrimary->Release();
        lpPrimary = NULL;
    }
    if( lpDS ) {
        lpDS->Release();
        lpDS = NULL;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------
// サウンドバッファの作成
//--------------------------------------------------------------------------------------------
int DSound_LoadFile(char* file )
{
    HRESULT ret;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;
	int SoundNo;
	for(SoundNo = 0; SoundNo < DSFILEMAX ; SoundNo ++ ){
		if(SoundBuffer[SoundNo].BufferSize == 0){
			break;
		}
	}
	if(SoundNo == DSFILEMAX)
		BToM("バッファの数が足りません");
	int ID = SoundNo;

    // WAVファイルをロード
    HMMIO hSrc;
    hSrc = mmioOpen( (LPSTR)file,NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
    if( !hSrc ) {
        BToM( "WAVファイルロードエラー\n","" );
        return -1;
    }

    // 'WAVE'チャンクチェック
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
    ret = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
        BToM( "WAVEチャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // 'fmt 'チャンクチェック
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
    ret = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
        BToM( "fmt チャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ヘッダサイズの計算
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // ヘッダメモリ確保
    wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
    if( !wf ) {
        BToM( "メモリ確保エラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVEフォーマットのロード
    ret = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
    if( FAILED(ret) ) {
        BToM( "WAVEフォーマットロードエラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // fmtチャンクに戻る
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // dataチャンクを探す
    while(1) {
        // 検索
        ret = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(ret) ) {
            BToM( "dataチャンクが見つからない\n","" );
            free( wf );
            mmioClose( hSrc,0 );
            return -1;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC("data",0) )
            break;
        // 次のチャンクへ
        ret = mmioAscend( hSrc,&mSrcWaveData,0 );
    }


    // サウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY;
    dsdesc.dwBufferBytes = mSrcWaveData.cksize;
    dsdesc.lpwfxFormat = wf;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer( &dsdesc,&SoundBuffer[ID].SoundBuffer,NULL );
	SoundBuffer[ID].BufferSize = (WORD)dsdesc.dwBufferBytes;
    if( FAILED(ret) ) {
        BToM( "サウンドバッファの作成エラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ロック開始
    LPVOID pMem1,pMem2;
    DWORD dwSize1,dwSize2;
    ret = (SoundBuffer[ID].SoundBuffer)->Lock( 0,mSrcWaveData.cksize,&pMem1,&dwSize1,&pMem2,&dwSize2,0 );
	SoundBuffer[ID].SoundBuffer->GetFrequency(&SoundBuffer[ID].Fre);
    if( FAILED(ret) ) {
        BToM( "ロック失敗\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // データ書き込み
    mmioRead( hSrc,(char*)pMem1,dwSize1 );
    mmioRead( hSrc,(char*)pMem2,dwSize2 );

	// ロック解除
    (SoundBuffer[ID].SoundBuffer)->Unlock( pMem1,dwSize1,pMem2,dwSize2 );
    // ヘッダ用メモリを開放
    free( wf );
    // WAVを閉じる
	mmioClose( hSrc,0 );

	//複製開始
	for(int t = 0 ; t < 10 ; t++){
		if(lpDS->DuplicateSoundBuffer(SoundBuffer[SoundNo].SoundBuffer,
								&(SoundBuffer[SoundNo].PlayBuffer[t].PlayBuffer))
			!= DS_OK){
			BToM( "再生用バッファ作成失敗\n","" );
			break;
		}else{
			SoundBuffer[SoundNo].PlayBuffer[t].ID = SoundNo;
			SoundBuffer[SoundNo].PlayBuffer[t].State = DSNONE;
			SoundBuffer[SoundNo].PlayBuffer[t].BufferSize = SoundBuffer[SoundNo].BufferSize;
			SoundBuffer[SoundNo].PlayBuffer[t].Loop = 0;
		}
	}
    return SoundNo;
}

//--------------------------------------------------------------------------------------------
// サウンドバッファの作成
//--------------------------------------------------------------------------------------------
int DSound_LoadFile(int SoundNo,char* file )
{
    HRESULT ret;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;

	int ID = SoundNo;

    // WAVファイルをロード
    HMMIO hSrc;
    hSrc = mmioOpen( (LPSTR)file,NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
    if( !hSrc ) {
        BToM( "WAVファイルロードエラー\n","" );
        return -1;
    }

    // 'WAVE'チャンクチェック
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
    ret = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
        BToM( "WAVEチャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // 'fmt 'チャンクチェック
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
    ret = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
        BToM( "fmt チャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ヘッダサイズの計算
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // ヘッダメモリ確保
    wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
    if( !wf ) {
        BToM( "メモリ確保エラー\n","" );
        mmioClose( hSrc,0 );
        return -1;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVEフォーマットのロード
    ret = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
    if( FAILED(ret) ) {
        BToM( "WAVEフォーマットロードエラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // fmtチャンクに戻る
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // dataチャンクを探す
    while(1) {
        // 検索
        ret = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(ret) ) {
            BToM( "dataチャンクが見つからない\n","" );
            free( wf );
            mmioClose( hSrc,0 );
            return -1;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC("data",0) )
            break;
        // 次のチャンクへ
        ret = mmioAscend( hSrc,&mSrcWaveData,0 );
    }


    // サウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY;
    dsdesc.dwBufferBytes = mSrcWaveData.cksize;
    dsdesc.lpwfxFormat = wf;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer( &dsdesc,&SoundBuffer[ID].SoundBuffer,NULL );
	SoundBuffer[ID].BufferSize = (WORD)dsdesc.dwBufferBytes;
    if( FAILED(ret) ) {
        BToM( "サウンドバッファの作成エラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // ロック開始
    LPVOID pMem1,pMem2;
    DWORD dwSize1,dwSize2;
    ret = (SoundBuffer[ID].SoundBuffer)->Lock( 0,mSrcWaveData.cksize,&pMem1,&dwSize1,&pMem2,&dwSize2,0 );
	SoundBuffer[ID].SoundBuffer->GetFrequency(&SoundBuffer[ID].Fre);
    if( FAILED(ret) ) {
        BToM( "ロック失敗\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return -1;
    }

    // データ書き込み
    mmioRead( hSrc,(char*)pMem1,dwSize1 );
    mmioRead( hSrc,(char*)pMem2,dwSize2 );

	// ロック解除
    (SoundBuffer[ID].SoundBuffer)->Unlock( pMem1,dwSize1,pMem2,dwSize2 );
    // ヘッダ用メモリを開放
    free( wf );
    // WAVを閉じる
	mmioClose( hSrc,0 );

	//複製開始
	for(int t = 0 ; t < 10 ; t++){
		if(lpDS->DuplicateSoundBuffer(SoundBuffer[SoundNo].SoundBuffer,
								&(SoundBuffer[SoundNo].PlayBuffer[t].PlayBuffer))
			!= DS_OK){
			BToM( "再生用バッファ作成失敗\n","" );
			break;
		}else{
			SoundBuffer[SoundNo].PlayBuffer[t].ID = SoundNo;
			SoundBuffer[SoundNo].PlayBuffer[t].State = DSNONE;
			SoundBuffer[SoundNo].PlayBuffer[t].BufferSize = SoundBuffer[SoundNo].BufferSize;
			SoundBuffer[SoundNo].PlayBuffer[t].Loop = 0;
		}
	}
    return SoundNo;
}


//--------------------------------------------------------------------------------------------
// 周波数変更 再生速度が変わる
//--------------------------------------------------------------------------------------------
void DSound_SetFrequency(DSOUNDOBJ SoundNo,int Fre)
{
	SoundBuffer[SeTable[SoundNo]].SoundBuffer->SetFrequency(SoundBuffer[SoundNo].Fre+Fre);

}

//********************************************************************//
//
//				SE再生　関連関数 ここまで
//
//********************************************************************//


//********************************************************************//
//
//				End Of File
//
//********************************************************************//
