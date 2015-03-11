#include "LoginScene.h"
#include "AppDelegate.h"
#include "ResourceMgr.h"
using namespace cocos2d;

CLoginScene::CLoginScene() : CGameSceneBase(enGameScene_Login)
{

}

CLoginScene::~CLoginScene()
{

}

bool CLoginScene::Init()
{
    bool bRet = false;
    do 
    {
        CCSize stWndSize = Director::getInstance()->getWinSize();

		// 背景图
        //CCSprite* pSprite = CResourceMgr::getSingleton().GetSprite("login_bk.png");
        //CC_BREAK_IF(!pSprite);
		//pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
        //addChild(pSprite, 0);

        // 初始化各控件

		bRet = true;
    } while (0);
    return bRet;
}

bool CLoginScene::UnInit()
{
	return true;
}