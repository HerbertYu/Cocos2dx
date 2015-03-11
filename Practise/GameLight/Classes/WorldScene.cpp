#include "WorldScene.h"
#include "AppDelegate.h"
#include "ResourceMgr.h"
#include "MapConfigMgr.h"
#include "SceneMgr.h"
using namespace cocos2d;

#define ZORDER_WORLD_BK     (-2)
#define ZORDER_WORLD_CTRL	(-1)
#define ZORDER_WORLD_MAP    (0)

CWorldScene::CWorldScene() : CGameSceneBase(enGameScene_World),
	m_pMapLayer(nullptr),
	m_pMapName(nullptr),
	m_pNextMap(nullptr),
	m_pPreMap(nullptr),
	m_pShowHint(nullptr)
{

}

CWorldScene::~CWorldScene()
{

}

bool CWorldScene::Init()
{
    bool bRet = false;
    do 
    {
		CCSize stWndSize = Director::getInstance()->getWinSize();

		// 背景图
        CCSprite* pSprite = CResourceMgr::getSingleton().GetSprite("background");
        CC_BREAK_IF(!pSprite);
		pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
        CCRect rc = pSprite->getTextureRect();
        pSprite->setScaleX(stWndSize.width / rc.size.width);
        pSprite->setScaleY(stWndSize.height / rc.size.height);
        addChild(pSprite, ZORDER_WORLD_BK);
        // 拼接背景的顶部
        pSprite = CCSprite::create("header.png");
		CC_BREAK_IF(!pSprite);
        CCRect rcHeader = pSprite->getTextureRect();
        pSprite->setScaleX(stWndSize.width / rcHeader.size.width);
        pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height - rcHeader.size.height/2));
        addChild(pSprite, ZORDER_WORLD_BK);
        // 拼接背景的底部
        pSprite = CCSprite::create("footer.png");
		CC_BREAK_IF(!pSprite);
        CCRect rcFooter = pSprite->getTextureRect();
        pSprite->setScaleX(stWndSize.width * 2 / rcFooter.size.width);
        pSprite->setPosition(ccp(stWndSize.width/2, rcFooter.size.height/2));
        addChild(pSprite, ZORDER_WORLD_BK);

		//////////////////////////////////////////////////////////////////////////
		CCMenu* pShowMenu = CCMenu::create();
		CC_BREAK_IF(!pShowMenu);
		pShowMenu->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
		addChild(pShowMenu, ZORDER_WORLD_MAP);

		// 返回选择
		CCMenuItemImage* pReturnBack = CCMenuItemImage::create("", "", this, menu_selector(CWorldScene::OnReturnBack));
		CC_BREAK_IF(!pReturnBack);
		pReturnBack->setNormalSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-back-normal"));
		pReturnBack->setSelectedSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-back-selected"));
		pReturnBack->setPosition(ccp(-stWndSize.width/2+80/2, stWndSize.height/2-40/2));
		pReturnBack->setEnabled(true);
		pShowMenu->addChild(pReturnBack);

		CCLabelTTF* pText = CCLabelTTF::create("Back", "Verdana", 18.0);
		CC_BREAK_IF(!pText);
		pText->setPosition(ccp(42, stWndSize.height-40/2));
		addChild(pText, 2);

		// 地图名称
		m_pMapName = CCLabelTTF::create("MapName", "Verdana", 20.0);
		CC_BREAK_IF(!m_pMapName);
		m_pMapName->setPosition(ccp(stWndSize.width/2, stWndSize.height-rcHeader.size.height/2));
		addChild(m_pMapName, ZORDER_WORLD_MAP);

		// 下一关
		m_pNextMap = CCMenuItemImage::create("", "", "", this, menu_selector(CWorldScene::OnNextMap));
		CC_BREAK_IF(!m_pNextMap);
		m_pNextMap->setNormalSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-right-normal"));
		m_pNextMap->setSelectedSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-right-selected"));
		m_pNextMap->setDisabledSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-right-disabled"));
		m_pNextMap->setPosition(ccp(stWndSize.width/2-40/2, stWndSize.height/2-42/2));
		m_pNextMap->setEnabled(false);
		pShowMenu->addChild(m_pNextMap);

		// 上一关
		m_pPreMap = CCMenuItemImage::create("", "", "", this, menu_selector(CWorldScene::OnPreMap));
		CC_BREAK_IF(!m_pPreMap);
		m_pPreMap->setNormalSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-left-normal"));
		m_pPreMap->setSelectedSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-left-selected"));
		m_pPreMap->setDisabledSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-left-disabled"));
		m_pPreMap->setPosition(ccp(stWndSize.width/2-40-40/2, stWndSize.height/2-42/2));
		m_pPreMap->setEnabled(false);
        pShowMenu->addChild(m_pPreMap);

		// 提示
		m_pShowHint = CCMenuItemImage::create("", "", "", this, menu_selector(CWorldScene::OnShowHint));
		CC_BREAK_IF(!m_pShowHint);
		m_pShowHint->setNormalSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-hint-normal"));
		m_pShowHint->setSelectedSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-hint-selected"));
		m_pShowHint->setDisabledSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-hint-disabled"));
		m_pShowHint->setPosition(ccp(0, rcFooter.size.height/2 - stWndSize.height/2));
        m_pShowHint->setEnabled(false);
		pShowMenu->addChild(m_pShowHint);

		pText = CCLabelTTF::create("reset", "Verdana", 22.0);
		CC_BREAK_IF(!pText);
		pText->setPosition(ccp(stWndSize.width/2, rcFooter.size.height/2));
		addChild(pText, 2);

		//////////////////////////////////////////////////////////////////////////
        // 初始化地图
        m_pMapLayer = CMapLayer::create();
		CC_BREAK_IF(!m_pMapLayer);
        rc.setRect(0, stWndSize.height-rcHeader.size.height, stWndSize.width,
            stWndSize.height - rcHeader.size.height - rcFooter.size.height);
        m_pMapLayer->setPosition(ccp(rc.origin.x + rc.size.width/2, rc.origin.y - rc.size.height/2));
        m_pMapLayer->setContentSize(rc.size);
		addChild(m_pMapLayer, ZORDER_WORLD_MAP);

		bRet = true;
    } while (0);
    return bRet;
}

bool CWorldScene::UnInit()
{
	return true;
}

bool CWorldScene::LoadMap(char nMapID, char nLevelID)
{
    STMapConfig* pCurMap = CMapConfigMgr::getSingleton().GetMapConfig(nMapID);
    if (pCurMap == nullptr || pCurMap->GetLevelConfig(nLevelID) == nullptr)
    {
        return false;
    }

    if (!m_pMapLayer->LoadMap(nMapID, nLevelID))
    {
        return false;
    }

	char szBuff[32];
	sprintf(szBuff, "%s : %d", pCurMap->szName, nLevelID);
    m_pMapName->setString(szBuff);
	m_pPreMap->setEnabled(pCurMap->GetPreLevelConfig(nLevelID) != nullptr);
	m_pNextMap->setEnabled(pCurMap->GetNextLevelConfig(nLevelID) != nullptr);
    m_pShowHint->setEnabled(true);
//    m_pMapLayer->setTouchEnabled(true);
    return true;
}

void CWorldScene::OnMapName(CCObject* pSender)
{
	
}

void CWorldScene::OnPreMap(CCObject* pSender)
{
    char nMapID = 0, nLevelID = 0;
    m_pMapLayer->GetCurMapID(nMapID, nLevelID);
    STMapConfig* pCurMap = CMapConfigMgr::getSingleton().GetMapConfig(nMapID);
    if (pCurMap == nullptr)
    {
        m_pPreMap->setEnabled(false);
        return;
    }
    STLevelConfig* pPreLevel = pCurMap->GetPreLevelConfig(nLevelID);
    if (pPreLevel == nullptr)
    {
        m_pPreMap->setEnabled(false);
        return;
    }

    LoadMap(nMapID, pPreLevel->chLevel);
}

void CWorldScene::OnNextMap(CCObject* pSender)
{
    char nMapID = 0, nLevelID = 0;
    m_pMapLayer->GetCurMapID(nMapID, nLevelID);
    STMapConfig* pCurMap = CMapConfigMgr::getSingleton().GetMapConfig(nMapID);
    if (pCurMap == nullptr)
    {
        m_pNextMap->setEnabled(false);
        return;
    }
    STLevelConfig* pNextLevel = pCurMap->GetNextLevelConfig(nLevelID);
    if (pNextLevel == nullptr)
    {
        m_pNextMap->setEnabled(false);
        return;
    }

    LoadMap(nMapID, pNextLevel->chLevel);
}

void CWorldScene::OnShowHint(CCObject* pSender)
{
	char nMapID = 0;
    char nLevel = 1;
    m_pMapLayer->GetCurMapID(nMapID, nLevel);
	if (nMapID >= 0 && nLevel >= 1)
	{
		LoadMap(nMapID, nLevel);
	}
}

void CWorldScene::OnReturnBack(CCObject* pSender)
{
	CSceneMgr::getSingleton().GotoScene(enGameScene_Select);
}