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
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCScene::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////
        CCSize stWndSize = CCDirector::sharedDirector()->getWinSize();

		// 背景图
        CCSprite* pSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "bkimg");
        CC_BREAK_IF(!pSprite);
		pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
        addChild(pSprite, 0);
        
        Vector<MenuItem*> arMenuItem;

        // 设置按钮
		CCMenuItemImage* pSetting = CCMenuItemImage::create("", "", this, menu_selector(CLoginScene::OnMenuSetting));
        CC_BREAK_IF(!pSetting);
		CCSprite* pNormalSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "setting_normal");
		CCSprite* pSelectSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "setting_select");
		CC_BREAK_IF(pNormalSprite == nullptr || pSelectSprite == nullptr);
		pSetting->setNormalImage(pNormalSprite);
		pSetting->setSelectedImage(pSelectSprite);
        arMenuItem.pushBack(pSetting);

        // 新游戏按钮
		CCMenuItemImage* pNewGame = CCMenuItemImage::create("", "", this, menu_selector(CLoginScene::OnMenuNewGame));
        CC_BREAK_IF(!pNewGame);
		pNormalSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "startgame_normal");
		pSelectSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "startgame_select");
		CC_BREAK_IF(pNormalSprite == nullptr || pSelectSprite == nullptr);
		pNewGame->setNormalImage(pNormalSprite);
		pNewGame->setSelectedImage(pSelectSprite);
		arMenuItem.pushBack(pNewGame);

		// 排行榜按钮
		CCMenuItemImage* pRank = CCMenuItemImage::create("", "", this, menu_selector(CLoginScene::OnMenuRank));
		CC_BREAK_IF(!pRank);
		pNormalSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "rank_normal");
		pSelectSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "rank_select");
		CC_BREAK_IF(pNormalSprite == nullptr || pSelectSprite == nullptr);
		pRank->setNormalImage(pNormalSprite);
		pRank->setSelectedImage(pSelectSprite);
		arMenuItem.pushBack(pRank);

		CCMenu* pButtonMenu = CCMenu::createWithArray(arMenuItem);
		CC_BREAK_IF(!pButtonMenu);
//		pButtonMenu->initWithArray(arMenuItem);
        pButtonMenu->setPosition(ccp(stWndSize.width/2, 50));
        pButtonMenu->alignItemsHorizontallyWithPadding(20.0f);
        addChild(pButtonMenu, 1);

		// 金币图片
		CC_BREAK_IF(!pButtonMenu);
		arMenuItem.clear();
		CCMenuItemImage* pCoins = CCMenuItemImage::create("", "", this, nullptr);
		pNormalSprite = CResourceMgr::getSingleton().NewSprite("ui_login", "coin");
		CC_BREAK_IF(pNormalSprite == nullptr);
		pCoins->setNormalImage(pNormalSprite);
		arMenuItem.pushBack(pCoins);

		// 玩家数据显示：
		char szBuff[256] = {0};
		STUserData* pUserData = AppDelegate::getSingleton().GetUserData();
		sprintf(szBuff, "%d", pUserData->nMoney);
		CCLabelBMFont* pMoney = CCLabelBMFont::create(szBuff, "futura-48.fnt");
		CCMenuItemLabel* pMoneyItem = CCMenuItemLabel::create(pMoney, this, nullptr);
		arMenuItem.pushBack(pMoneyItem);
        
		CCMenu* pShowMenu = CCMenu::createWithArray(arMenuItem);
//		pShowMenu->initWithArray(arMenuItem);
		pShowMenu->setPosition(ccp(100, stWndSize.height-60));
		pShowMenu->alignItemsHorizontally();
		addChild(pShowMenu, 1);

		bRet = true;
    } while (0);
    return bRet;
}

bool CLoginScene::UnInit()
{
	return true;
}

bool CLoginScene::OnEnter()
{
    setVisible(true);
    return true;
}

bool CLoginScene::OnLeave()
{
    setVisible(false);
    return true;
}

void CLoginScene::OnMenuNewGame(CCObject* pSender)
{
	AppDelegate::getSingleton().GotoScene(enGameScene_World);
}

void CLoginScene::OnMenuSetting(CCObject* pSender)
{
	AppDelegate::getSingleton().GotoScene(enGameScene_Setting);
}

void CLoginScene::OnMenuRank(CCObject* pSender)
{
	AppDelegate::getSingleton().GotoScene(enGameScene_Rank);
}

void CLoginScene::OnMenuQuit(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}