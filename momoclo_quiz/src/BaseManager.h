//*********************************************************************************************************
//	ファイル名	：	BaseManager.h
//	作成者		：	kageyama taiki
//	作成日		：	2011/10/6
//	機能		：	現在のスクリーン中でオブジェクトを全て管理出来る、派生クラスで継承すること
//*********************************************************************************************************
//==============================================================
#ifndef BASE_MANAGER_H_
#define BASE_MANAGER_H_

//==============================================================
#include	"utility/Gp3Lib.h"
#include	<vector>
#include	<list>
#include	"Image.h"

//==============================================================
// クラス宣言
//==============================================================
class C_Base;

class C_BaseManager{
	typedef std::list<C_Base*> listObj;

	size_t mAllObjNo;
	size_t mObjNo;

	void ItDelete(const size_t& t, listObj::iterator& it);
protected:
	static C_Image mImageInfo;
	static std::vector<listObj> mObj;

	void GetDraw();
	void GetControl();
	void GetDebug();
	
	explicit C_BaseManager();
	C_BaseManager(const C_BaseManager&){}
	C_BaseManager operator=(const C_BaseManager&){}
public:
	enum ObjName;
	virtual ~C_BaseManager();

	virtual void Draw() = 0;
	virtual void Control() = 0;

	static void SetObjPush(const ObjName& objName, C_Base* b);
	static IMGOBJ SetLoadImage(const char* fileName);

	//----------------------------------------------------------------
	//　オブジェクト情報の受け取り
	//----------------------------------------------------------------
	//	[例 : 敵の場合]	C_Stage::GetObj<const C_StageEnemy*>(C_Stage::ENEMY);
	//	をすることによりインスタンスの情報を受け取ることが出来る。
	//	テンプレートの指定は場面に応じて指定すること。(constを付けるか付けないか(書き換えるか書き換えないか))
	//----------------------------------------------------------------
	template<typename T>
	static T GetObj(ObjName name){
		return dynamic_cast<T>(mObj[name].front());
	}
	static size_t GetObjSize(ObjName name){
		return (mObj[name].size());
	}
	static void DeleteObj(ObjName name){
		mObj[name].clear();
}
	static bool IsCollision(ObjName n1, ObjName n2);
	static bool IsCollision(float posX,float posY,ObjName n2);
};

#define SET_OBJECT(className,objName) (C_BaseManager::SetObjPush(static_cast<C_BaseManager::ObjName>(objName),className));
#define GET_OBJECT(className,objName) (C_BaseManager::GetObj<className>(static_cast<C_BaseManager::ObjName>(objName)));
#define GET_OBJECTSIZE(className,objName) (C_BaseManager::GetObjSize<className>(static_cast<C_BaseManager::ObjName>(objName)));
#define DELETE_OBJECT(className,objName) (C_BaseManager::DeleteObj/*<className>*/(static_cast<C_BaseManager::ObjName>(objName)));
#define IS_COLLISION(objName1,objName2) (C_BaseManager::IsCollision(static_cast<C_BaseManager::ObjName>(objName1), static_cast<C_BaseManager::ObjName>(objName2)) )
#define IS_POSCOLLISION(posX,posY,objName2) (C_BaseManager::IsCollision(static_cast<float>(posX),static_cast<float>(posY), static_cast<C_BaseManager::ObjName>(objName2)) )

////==============================================================
#endif
//==============================================================