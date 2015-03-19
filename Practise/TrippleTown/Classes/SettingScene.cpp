#include "SettingScene.h"
#include "AppDelegate.h"
using namespace cocos2d;

CSettingScene::CSettingScene() : CGameSceneBase(enGameScene_Setting)
{

}

CSettingScene::~CSettingScene()
{

}

bool CSettingScene::Init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCScene::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////
        CCSize size = CCDirector::sharedDirector()->getWinSize();

		
		bRet = true;
    } while (0);
    return bRet;
}

bool CSettingScene::UnInit()
{
	return true;
}

bool CSettingScene::OnEnter()
{
    setVisible(true);
    return true;
}

bool CSettingScene::OnLeave()
{
    setVisible(false);
    return true;
}