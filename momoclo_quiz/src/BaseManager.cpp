//******************************************************************************
//	�t�@�C����	�F	BaseManager.cpp
//	�쐬��		�F	kageyama taiki
//	�쐬��		�F	2011/10/6
//	�@�\		�F	���݂̃X�N���[�����ŃI�u�W�F�N�g��S�ĊǗ��o����A�h���N���X�Ōp�����邱��
//******************************************************************************
#include	<algorithm>
#include	"BaseManager.h"
#include	"Base.h"

C_Image C_BaseManager::mImageInfo;
std::vector<C_BaseManager::listObj> C_BaseManager::mObj;
//==========================================================//
//	�֐���`
//==========================================================//
//------------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------------
C_BaseManager::C_BaseManager()
{
	mAllObjNo = mObjNo = 0;

	OutputDebugString("constructor:C_BaseManager\n");
}

//------------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------------
C_BaseManager::~C_BaseManager()
{
	size_t sizeMax = mObj.size();

	for(size_t t = 0; t < sizeMax; ++t){

		listObj::iterator it = mObj[t].begin();

		while(it != mObj[t].end())
		{
			ItDelete(t,it);
		}
	}

	mImageInfo.Delete();

	OutputDebugString("destructor:C_BaseManager\n");
}

//------------------------------------------------------------
//	�`��̎󂯎��
//------------------------------------------------------------
void C_BaseManager::GetDraw()
{
	size_t sizeMax = mObj.size();

	for(size_t t = 0; t < sizeMax; ++t){
		for_each(mObj[t],FE_Draw);
	}

	#ifndef	NDEBUG	
	GetDebug();
	#endif

}	

//------------------------------------------------------------
//	����̎󂯎��
//------------------------------------------------------------
void C_BaseManager::GetControl()
{
	size_t sizeMax = mObj.size();

	for(size_t t = 0; t < sizeMax; ++t){
		for(listObj::iterator it = mObj[t].begin(); it != mObj[t].end();){
			if((*it)->Control()){
				ItDelete(t,it);
				continue;
			}
			++it;
		}
	}
}

//------------------------------------------------------------
//	�f�o�b�O�󂯎��
//------------------------------------------------------------
void C_BaseManager::GetDebug()
{
	//�I�u�W�F�N�g
	if(CheckPush(KEY_DEBUG1))
	{
		int size = static_cast<int>(mObj.size());
		if(static_cast<int>(mAllObjNo) + 1 < size)
		{
			++mAllObjNo;
			mObjNo = 0;		
		}
		else
		{
			mAllObjNo = 0;
			mObjNo = 0;
		}
	}
	/*else if(CheckPush(KEY_BTN5))
	{
		if(static_cast<int>(mAllObjNo) - 1 >= 0)
		{
			if(!mObj[mAllObjNo - 1].empty())
			{
				--mAllObjNo;
				mObjNo = 0;
			}
			
		}
		else
		{
			size_t t = mObj.size();

			int i = t - 1;

			for(;i >= 0; --i)
			{
				if(!mObj[i].empty())
				{
					mAllObjNo = size_t(i);
					mObjNo = 0;
					break;
				}
			}
		}
	}*/

	//�I�u�W�F�N�g�̔z��
	else if(CheckPush(KEY_DEBUG2))
	{
		size_t t = mObj[mAllObjNo].size();

		if(static_cast<int>(mObjNo) < static_cast<int>(t - 1) ){
			++mObjNo;
		}
		else{
			mObjNo = 0;
		}
	}
	/*else if(CheckPush(KEY_BTN7))
	{
		if(static_cast<int>(mObjNo) > 0 ){
			--mObjNo;
		}
		else{
			size_t t = mObj[mAllObjNo].size();
			mObjNo = t - 1;
		}
	}*/
	
	listObj::iterator it = mObj[mAllObjNo].begin();

	for(int i = 0; i < static_cast<int>(mObjNo); ++i)
	{
		++it;
	}
	if(it == mObj[mAllObjNo].end()){}
	else{FE_Debug((*it));}
}

//------------------------------------------------------------
//	�폜
//------------------------------------------------------------
void C_BaseManager::ItDelete(const size_t& t, listObj::iterator& it)
{
	//new�����炩�Ȃ炸������delete������Aerase��delete�͂��Ă���Ȃ�
	delete *it;
	mObj[t].erase(it++);

	mObjNo = 0;
	if(mObj[mAllObjNo].empty()){mAllObjNo = 0;}
}

//------------------------------------------------------------
//	�I�u�W�F�N�g�̐���
//------------------------------------------------------------
//	objName : ��������I�u�W�F�N�g�ԍ�
//	b		: ��������I�u�W�F�N�g�̃|�C���^
//------------------------------------------------------------
void C_BaseManager::SetObjPush(const ObjName& objName, C_Base* b)
{
	mObj[objName].push_back(b);
}

//------------------------------------------------------------
//	�摜�̃��[�h
//------------------------------------------------------------
//	fileName : �摜�t�@�C����
//------------------------------------------------------------
IMGOBJ C_BaseManager::SetLoadImage(const char* fileName)
{
	return mImageInfo.IsLoadImage(fileName);
}
//------------------------------------------------------------
//	�����蔻��
//	�������Ă�����true
//------------------------------------------------------------
bool C_BaseManager::IsCollision(ObjName n1, ObjName n2)
{
	listObj::iterator it1 = mObj[n1].begin();

	for(listObj::iterator it2 = mObj[n2].begin(); it2 != mObj[n2].end();++it2){
		if(	(*it1)->GetPosX() < (*it2)->GetPosX() + (*it2)->GetImgSize().right  &&
			(*it2)->GetPosX() < (*it1)->GetPosX() + (*it1)->GetImgSize().right  &&
			(*it1)->GetPosY() < (*it2)->GetPosY() + (*it2)->GetImgSize().bottom &&
			(*it2)->GetPosY() < (*it1)->GetPosY() + (*it1)->GetImgSize().bottom)
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------
//	�ЂƂɃI�u�W�F�N�g�ɑ΂��đ��̃I�u�W�F�N�g�Ƒ�������
//------------------------------------------------------------
bool C_BaseManager::IsCollision(float posX,float posY,ObjName n2)
{
	for(listObj::iterator it2 = mObj[n2].begin(); it2 != mObj[n2].end();++it2){
		if(posX == (*it2)->GetPosX() && posY == (*it2)->GetPosY())
		{
			return true;
		}
	}
	return false;
}