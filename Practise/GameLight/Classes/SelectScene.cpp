#include "SelectScene.h"
#include "AppDelegate.h"
#include "ResourceMgr.h"
#include "MapConfigMgr.h"
#include "LevelSelectLayer.h"
using namespace cocos2d;
#include <cocos-ext.h>

CSelectScene::CSelectScene() : CGameSceneBase(enGameScene_Select), m_pTableView(nullptr)
{
}

CSelectScene::~CSelectScene()
{
}

bool CSelectScene::Init()
{
    bool bRet = false;
    do 
    {
        CCSize stWndSize = Director::getInstance()->getWinSize();
        
		// ±³¾°Í¼
		CCSprite* pSprite = CResourceMgr::getSingleton().GetSprite("background");
        CC_BREAK_IF(!pSprite);
        CCRect rc = pSprite->getTextureRect();
        pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
        pSprite->setScaleX(stWndSize.width / rc.size.width);
        pSprite->setScaleY(stWndSize.height / rc.size.height);
        addChild(pSprite, -2);
		// Æ´½Ó±³¾°µÄ¶¥²¿
        pSprite = CCSprite::create("header.png");
        CCRect rcHeader = pSprite->getTextureRect();
        pSprite->setScaleX(stWndSize.width / rcHeader.size.width);
        pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height - rcHeader.size.height/2));
        addChild(pSprite, -1);
		// Æ´½Ó±³¾°µÄµ×²¿
		pSprite = CCSprite::create("footer.png");
		CCRect rcFooter = pSprite->getTextureRect();
		pSprite->setScaleX(stWndSize.width * 2 / rcFooter.size.width);
		pSprite->setPosition(ccp(stWndSize.width/2, rcFooter.size.height/2));
		addChild(pSprite, -1);
		m_pTableCells = Array::create();
		m_pTableCells->retain();
		for (char i = 0; i < CMapConfigMgr::getSingleton().GetMapCount(); i++)
		{
			CLevelSelectCell* pCell = new CLevelSelectCell();
			pCell->autorelease();
			pCell->Init(this, i+1);
			pCell->setPosition(ccp(0, 0));
			m_pTableCells->addObject(pCell);
		}

        m_pTableView = extension::TableView::create(this, stWndSize);
        m_pTableView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		m_pTableView->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
        //m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(0, 0));
        m_pTableView->setDelegate(this);
        addChild(m_pTableView, 2);

        m_nCurMapID = 1;
		bRet = true;
    } while (0);
    return bRet;
}

bool CSelectScene::UnInit()
{
	m_pTableCells->release();
	return true;
}

void CSelectScene::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    CCLOG("tableCellTouched(%d)", cell->getIdx());
}

extension::TableViewCell* CSelectScene::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
	/*extension::CCTableViewCell* pCell = table->dequeueCell();
	if (pCell == nullptr)
	{
		pCell = new extension::CCTableViewCell();
		pCell->autorelease();
		STMapConfig* pMapConfig = CMapConfigMgr::getSingleton().GetMapConfig(idx+1);
		CCLabelTTF* pMapName = CCLabelTTF::create(pMapConfig->szName, "Verdana", 20.0);
		CCSize stWndSize = Director::getInstance()->getWinSize();
		pMapName->setPosition(ccp(stWndSize.width/2, stWndSize.height-20));
		pMapName->setTag(1);
		pCell->addChild(pMapName, 2);
	} 
	else
	{
		CCLabelTTF* pMapName = (CCLabelTTF*)(pCell->getChildByTag(1));
		pMapName->setString(CMapConfigMgr::getSingleton().GetMapConfig(idx+1)->szName);
	}
	return pCell;*/
    return (extension::TableViewCell*)(m_pTableCells->getObjectAtIndex(idx));
}

cocos2d::CCSize CSelectScene::cellSizeForTable(cocos2d::extension::TableView *table)
{
    return Director::getInstance()->getWinSize();
}

ssize_t CSelectScene::numberOfCellsInTableView(extension::TableView *table)
{
    return CMapConfigMgr::getSingleton().GetMapCount();
}

void CSelectScene::ShowLayer(char nMapID)
{
    /*if (nMapID < 1 || nMapID > CMapConfigMgr::getSingleton().GetMapCount())
    {
        return;
    }

    if (m_nCurMapID >= 1 && m_nCurMapID <= CMapConfigMgr::getSingleton().GetMapCount())
    {
        CLevelSelectLayer* pLayer = (CLevelSelectLayer*)m_arLevelLayers.objectAtIndex(m_nCurMapID-1);
        pLayer->setTouchEnabled(false);
    }
    bool bMoveRight = (nMapID >= m_nCurMapID);

    CLevelSelectLayer* pLayer = (CLevelSelectLayer*)m_arLevelLayers.objectAtIndex(nMapID-1);
    pLayer->setTouchEnabled(true);
    m_nCurMapID = nMapID;

    

    CCActionInterval* pActionMove = CCMoveBy::create(1, 
        ccp((bMoveRight ? 1 : -1) * pLayer->getContentSize().width, 0));
    runAction(pActionMove);*/
}

void CSelectScene::OnEnter()
{
    //m_pTableView->reloadData();
    //ShowLayer(m_nCurMapID);
}

void CSelectScene::OnLeave()
{
    
}