#include "RankScene.h"
#include "AppDelegate.h"
using namespace cocos2d;

CRankScene::CRankScene() : CGameSceneBase(enGameScene_Rank)
{

}

CRankScene::~CRankScene()
{

}

bool CRankScene::Init()
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

bool CRankScene::UnInit()
{
	return true;
}

bool CRankScene::OnEnter()
{
    setVisible(true);
    return true;
}

bool CRankScene::OnLeave()
{
    setVisible(false);
    return true;
}