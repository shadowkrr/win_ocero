//******************************************************************************
//	ファイル名	：	StageMap.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/21
//	機能		：	マップ
//------------------------------------------------------------------------------
//	更新履歴	：	
//
//******************************************************************************
//==============================================================
#ifndef STAGEMAP_H_
#define STAGEMAP_H_
//==============================================================
//==============================================================
#include	"../Base.h"
#include	<vector>
//==============================================================
//==========================================================//
//	クラス定義
//==========================================================//

namespace stage
{
	class C_Human;

	class C_Map : public C_Base
	{
		public:
			enum CHIPSIZE	{CHIP_SIZE = 32,CHIP_SCREENX = 21,CHIP_SCREENY = 19,CHIP_MAX};
			enum ITEM_KINDS	{ITEM_HP = 107,ITEM_BOMB,ITEM_MOVE,ITEM_BLAST,ITEM_MAX};
			enum MAP_CHIP	{MAP_SPACE	= -1,MAP_NONE,MAP_KABE = 1,MAP_HAKA = 105,MAP_YAKINOHARA = 120,MAP_MAX};
			enum MAXSIZE	{MSIZE_XLEFT = 0,MSIZE_XRIGHT = 34,MSIZE_YTOP = 0,MSIZE_YBOTTOM = 40, MSIZE_MAX};

		private:
			static const int DISP_X		=	33;	//画面内のX方向のチップ描画数
			static const int DISP_Y		=	41;	//画面内のY方向のチップ描画数
			static const int CHIP		=	15; //アップイメージの最大マス
			static const int SCREENX	=	21;	//画面上のチップ数
			static const int SCREENY	=	19;	//画面上のチップ数

			static const int PLAYER_DRAWX	=(21/2);	//画面上のチップ数
			static const int PLAYER_DRAWY	=(19/2);	//画面上のチップ数

			//アイテムでもこれを使用してるので変更するときチェック
			static const int BLOCKX	=	34;				//マップのサイズ
			static const int BLOCKY	=	41;				//マップのサイズ


			//ブロックサイズ
			static const int blocksize = 60;

			int IsHitmap[BLOCKY][BLOCKX];

			//アイテム関係
			float ItemScaleX,ItemScaleY;
			int Item[BLOCKY][BLOCKX];
			void SetRandomItem();
			bool LoadItem(const char* filename);
			void ItemStep();
			//-------------------

			void InitMap();
			bool BingoCheckX(const int x, const int y, int &cnt, int ARGB);
			bool BingoCheckY(const int x, const int y, int &cnt, int ARGB);
			bool BingoCheckRY(const int x, const int y, int &cnt, int ARGB);
			bool BingoCheckLY(const int x, const int y, int &cnt, int ARGB);
			int	 bingocnt;

			std::vector<int> mMap;
			std::vector<int> mIsHitMap;
			std::vector<int> mARGBMap;
			int mWidthSize;
			int mHeightSize;
			int mStageCnt;


			int BlockNum;
			int WhiteNum;
			bool DownCheck(int x, int y, int ocerono,int cnt);
			bool UpCheck(int x, int y, int ocerono,int cnt);
			bool RightCheck(int x, int y, int ocerono,int cnt);
			bool LeftCheck(int x, int y, int ocerono,int cnt);
			bool RightDownCheck(int x, int y, int ocerono,int cnt);
			bool RightUpCheck(int x, int y, int ocerono,int cnt);
			bool LeftDownCheck(int x, int y, int ocerono,int cnt);
			bool LeftUpCheck(int x, int y, int ocerono,int cnt);

			explicit C_Map(){}
			C_Map(const C_Map&){}
			C_Map operator=(const C_Map&){}

			bool Load(const char* fileName, std::vector<int>& map);
			bool IsSpace(const int& x,const int& y, const C_Human* human)const;
			bool LoadIsHitMap(const char* fileName);
	public:
		C_Map(const char* fileName/*, const int stageCnt*/);
		virtual ~C_Map();

		void Draw();
		void Create(){}
		bool Control();

		int GetIsHitMap(int y,int x);
		int SetIsHitMap(int y,int x,int n);
		RECT GetmImg()const{return mImgSize;}
		int GetmImgSizeX()const{return mImgSize.right;}
		int GetmImgSizeY()const{return mImgSize.bottom;}
		int GetMap(int y,int x);
		int SetMap(int y,int x,int n);
		int GetWidthSize()const{return mWidthSize;}
		int GetHeightSize ()const{return mHeightSize;}
		int GetMapArray(const int x, const int y);
		void SetMapArray(const int x, const int y, const int date);
		int GetARGBArray(const int x, const int y);
		void SetARGBArray(const int x, const int y, const int date);

		int GetBlockSize()const{return blocksize;}
		int  GetItem(int y,int x);
		int  SetItem(int y,int x,int n);
		bool ReverceCheck(int x, int y, int ocerono);

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
