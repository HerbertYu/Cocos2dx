#include "GameWorldScene.h"
#include "AppDelegate.h"
#include <vector>
#include "ResourceMgr.h"
#include <random>
using namespace cocos2d;

#define IS_VALID_X(x)	((x) >= 0 && (x) < MAP_HEIGHT_CELL)
#define IS_VALID_Y(y)	((y) >= 0 && (y) < MAP_WIDTH_CELL)

//////////////////////////////////////////////////////////////////////////
void CMapLayer::Init(CGameWorldScene* pParent)
{
	m_pParentScene = pParent;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchEnded = CC_CALLBACK_2(CMapLayer::onTouchEnded, this);
	listener->onTouchBegan = CC_CALLBACK_2(CMapLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CMapLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (touch == nullptr)
	{
		return;
	}

	CCPoint stCurPos = touch->getLocation();
	int y = (stCurPos.x - MAP_LAYER_POS_X) / MAP_CELL_SIZE;
	int x = (MAP_LAYER_POS_Y - stCurPos.y) / MAP_CELL_SIZE;
	if (IS_VALID_X(x) && IS_VALID_Y(y))
	{
		m_pParentScene->OnSelectCell(x, y);
	}
}

//////////////////////////////////////////////////////////////////////////

CGameWorldScene::CGameWorldScene() : CGameSceneBase(enGameScene_World),
	m_pCurScore(nullptr),
	m_pMaxScore(nullptr),
	m_pMoney(nullptr),
	m_pUserData(nullptr),
	m_pMapLayer(nullptr)
{
}

CGameWorldScene::~CGameWorldScene()
{
}

bool CGameWorldScene::Init()
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
		// 填充背景，采用一块小图在整个屏幕上重复填充
		CCSprite* pSprite = CResourceMgr::getSingleton().NewSprite("ui_world", "bkimg");
		pSprite->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
		addChild(pSprite, 0);

		InitData();

		//////////////////////////////////////////////////////////////////////////
		// 数据区
		// 当前积分
		char szBuff[256] = {0};
		sprintf(szBuff, "%d", m_nCurScore);
		m_pCurScore = CCLabelBMFont::create(szBuff, "futura-48.fnt");
		m_pCurScore->setPosition(ccp(80, stWndSize.height-50));
		addChild(m_pCurScore, 1);
		// 目标积分
		sprintf(szBuff, "%d", m_nMaxScore);
		m_pMaxScore = CCLabelBMFont::create(szBuff, "futura-48.fnt");
		m_pMaxScore->setPosition(ccp(130, stWndSize.height-120));
		addChild(m_pMaxScore, 1);
		// 当前金币
		m_pUserData = AppDelegate::getSingleton().GetUserData();
		sprintf(szBuff, "%d", m_pUserData->nMoney);
		m_pMoney = CCLabelBMFont::create(szBuff, "futura-48.fnt");
		m_pMoney->setPosition(ccp(130, stWndSize.height-190));
		addChild(m_pMoney, 1);

		// 当前物品
		m_pCurItem = CCSprite::create();
		m_pCurItem->setPosition(ccp(21+25, stWndSize.height-233-25));
		CResourceMgr::getSingleton().InitSprite(m_pCurItem, "ui_item", g_arItemData[m_enCurItemID].szImage);
		addChild(m_pCurItem, 1);

		//////////////////////////////////////////////////////////////////////////
		// 地图区
		m_pMapLayer = new CMapLayer();
		m_pMapLayer->Init(this);
		float nX = MAP_LAYER_POS_X + MAP_CELL_SIZE/2, nY = MAP_LAYER_POS_Y - MAP_CELL_SIZE/2;
		for (int x = 0; x < MAP_HEIGHT_CELL; x++)
		{
			for (int y = 0; y < MAP_WIDTH_CELL; y++)
			{
				m_vecCells[x][y].pSpriteItem = Sprite::create();
				m_vecCells[x][y].pSpriteFix[0] = Sprite::create();
				m_vecCells[x][y].pSpriteFix[1] = Sprite::create();
				m_vecCells[x][y].pSpriteFix[2] = Sprite::create();
				m_vecCells[x][y].pSpriteFix[3] = Sprite::create();
				m_vecCells[x][y].pSpriteFloor = Sprite::create();
				m_vecCells[x][y].enItemID = enItemID_Null;
				m_vecCells[x][y].pSpriteItem->setPosition(ccp(nX, nY));
				m_vecCells[x][y].pSpriteFix[0]->setPosition(ccp(nX, nY));
				m_vecCells[x][y].pSpriteFix[1]->setPosition(ccp(nX, nY));
				m_vecCells[x][y].pSpriteFix[2]->setPosition(ccp(nX, nY));
				m_vecCells[x][y].pSpriteFix[3]->setPosition(ccp(nX, nY));
				m_vecCells[x][y].pSpriteFloor->setPosition(ccp(nX, nY));
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteItem, 3);
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteFix[0], 2);
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteFix[1], 2);
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteFix[2], 2);
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteFix[3], 2);
				m_pMapLayer->addChild(m_vecCells[x][y].pSpriteFloor, 1);
				nX += MAP_CELL_SIZE;
			}
			nY -= MAP_CELL_SIZE;
			nX = MAP_LAYER_POS_X + MAP_CELL_SIZE/2;
		}

        // 
		UpdateMapLayer();
		
		addChild(m_pMapLayer, 1);
		bRet = true;
    } while (0);
    return bRet;
}

bool CGameWorldScene::UnInit()
{
    for (int x = 0; x < MAP_HEIGHT_CELL; x++)
    {
        for (int y = 0; y < MAP_WIDTH_CELL; y++)
        {
            
        }
    }
	return true;
}

bool CGameWorldScene::OnEnter()
{
    setVisible(true);
	m_pMapLayer->setTouchEnabled(true);
    return true;
}

bool CGameWorldScene::OnLeave()
{
    setVisible(false);
    return true;
}

void CGameWorldScene::InitData()
{
	m_nCurScore = 0;
	m_nMaxScore = 1000;
    
    m_enCurItemID = RandItem();
}

void CGameWorldScene::UpdateCurScore()
{
	if (m_pCurScore == nullptr)
	{
		return;
	}
	char szBuff[32] = {0};
	sprintf(szBuff, "%d", m_nCurScore);
//	m_pCurScore->setCString(szBuff);
	m_pCurScore->setString(szBuff);

	if (m_nCurScore >= m_nMaxScore)
	{
		m_nMaxScore *= 2;
		if (m_pMaxScore != nullptr)
		{
			sprintf(szBuff, "%d", m_nMaxScore);
			m_pMaxScore->setString(szBuff);
		}
	}
}

void CGameWorldScene::UpdateMoney()
{
	if (m_pMoney == nullptr)
	{
		return;
	}
	
	char szBuff[32] = {0};
	sprintf(szBuff, "%d", m_pUserData->nMoney);
	m_pMoney->setString(szBuff);
}

void CGameWorldScene::UpdateSpriteWithItem(int x, int y)
{
	// 地表
	CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFloor, "ui_world", "floor");
	m_vecCells[x][y].pSpriteFloor->setRotation(0);
	m_vecCells[x][y].pSpriteFloor->setVisible(true);

	// 地表修正
	m_vecCells[x][y].pSpriteFix[0]->setVisible(false);
	m_vecCells[x][y].pSpriteFix[1]->setVisible(false);
	m_vecCells[x][y].pSpriteFix[2]->setVisible(false);
	m_vecCells[x][y].pSpriteFix[3]->setVisible(false);

	// 道具
	CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteItem, 
		"ui_item", g_arItemData[m_vecCells[x][y].enItemID].szImage);
	m_vecCells[x][y].pSpriteItem->setRotation(0);
	m_vecCells[x][y].pSpriteItem->setVisible(true);
}

void CGameWorldScene::UpdateSpriteNoItem(int x, int y)
{
	int nDegree = 0;

	// 地表
	CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFloor, "ui_world", GetEmptyCellImge(x, y, nDegree));
	m_vecCells[x][y].pSpriteFloor->setRotation(nDegree);
	m_vecCells[x][y].pSpriteFloor->setVisible(true);

	// 地表修正
	// 修正左上角
	if (x >= 1 && y >= 1 &&
		m_vecCells[x-1][y].enItemID == enItemID_Null &&
		m_vecCells[x][y-1].enItemID == enItemID_Null &&
		m_vecCells[x-1][y-1].enItemID != enItemID_Null)
	{
		CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFix[0], "ui_world", "floor_fix");
		m_vecCells[x][y].pSpriteFix[0]->setRotation(0);
		m_vecCells[x][y].pSpriteFix[0]->setVisible(true);
	}
	else
	{
		m_vecCells[x][y].pSpriteFix[0]->setVisible(false);
	}
	// 修正右上角
	if (x >= 1 && y < MAP_WIDTH_CELL-1 &&
		m_vecCells[x-1][y].enItemID == enItemID_Null &&
		m_vecCells[x][y+1].enItemID == enItemID_Null &&
		m_vecCells[x-1][y+1].enItemID != enItemID_Null)
	{
		CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFix[1], "ui_world", "floor_fix");
		m_vecCells[x][y].pSpriteFix[1]->setRotation(90);
		m_vecCells[x][y].pSpriteFix[1]->setVisible(true);
	}
	else
	{
		m_vecCells[x][y].pSpriteFix[1]->setVisible(false);
	}
	// 修正右下角
	if (x < MAP_HEIGHT_CELL-1 && y < MAP_WIDTH_CELL-1 &&
		m_vecCells[x+1][y].enItemID == enItemID_Null &&
		m_vecCells[x][y+1].enItemID == enItemID_Null &&
		m_vecCells[x+1][y+1].enItemID != enItemID_Null)
	{
		CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFix[2], "ui_world", "floor_fix");
		m_vecCells[x][y].pSpriteFix[2]->setRotation(180);
		m_vecCells[x][y].pSpriteFix[2]->setVisible(true);
	}
	else
	{
		m_vecCells[x][y].pSpriteFix[2]->setVisible(false);
	}
	// 修正左下角
	if (x < MAP_HEIGHT_CELL-1 && y >= 1 &&
		m_vecCells[x+1][y].enItemID == enItemID_Null &&
		m_vecCells[x][y-1].enItemID == enItemID_Null &&
		m_vecCells[x+1][y-1].enItemID != enItemID_Null)
	{
		CResourceMgr::getSingleton().InitSprite(m_vecCells[x][y].pSpriteFix[3], "ui_world", "floor_fix");
		m_vecCells[x][y].pSpriteFix[3]->setRotation(270);
		m_vecCells[x][y].pSpriteFix[3]->setVisible(true);
	}
	else
	{
		m_vecCells[x][y].pSpriteFix[3]->setVisible(false);
	}

	// 道具
	m_vecCells[x][y].pSpriteItem->setVisible(false);
}

char* CGameWorldScene::GetEmptyCellImge(IN int x, IN int y, OUT int& nDegree)
{
	nDegree = 0;
	
	int arEmptyFlag[4] = {0}; // ← ↑ → ↓，1表示有Item

	if (x < 1)
	{
		arEmptyFlag[1] = 0;
		arEmptyFlag[3] = (m_vecCells[x+1][y].enItemID > enItemID_Null ? 0 : 1);
	}
	else if (x + 1 >= MAP_HEIGHT_CELL)
	{
		arEmptyFlag[1] = (m_vecCells[x-1][y].enItemID > enItemID_Null ? 0 : 1);
		arEmptyFlag[3] = 0;
	}
	else
	{
		arEmptyFlag[1] = (m_vecCells[x-1][y].enItemID > enItemID_Null ? 0 : 1);
		arEmptyFlag[3] = (m_vecCells[x+1][y].enItemID > enItemID_Null ? 0 : 1);
	}
	
	if (y < 1)
	{
		arEmptyFlag[0] = 0;
		arEmptyFlag[2] = (m_vecCells[x][y+1].enItemID > enItemID_Null ? 0 : 1);
	}
	else if (y + 1 >= MAP_WIDTH_CELL)
	{
		arEmptyFlag[0] = (m_vecCells[x][y-1].enItemID > enItemID_Null ? 0 : 1);
		arEmptyFlag[2] = 0;
	}
	else
	{
		arEmptyFlag[0] = (m_vecCells[x][y-1].enItemID > enItemID_Null ? 0 : 1);
		arEmptyFlag[2] = (m_vecCells[x][y+1].enItemID > enItemID_Null ? 0 : 1);
	}
	
	int nSum = arEmptyFlag[0] + arEmptyFlag[1] + arEmptyFlag[2] + arEmptyFlag[3];
	switch (nSum)
	{
	case 0: 
		return "empty_0";
	case 1: 
		if (arEmptyFlag[0] == 1)
		{
			nDegree = 0;
		}
		else if (arEmptyFlag[1] == 1)
		{
			nDegree = 90;
		}
		else if (arEmptyFlag[2] == 1)
		{
			nDegree = 180;
		}
		else
		{
			nDegree = 270;
		}
		return "empty_1";
	case 2: 
		if (arEmptyFlag[0] && arEmptyFlag[0] == arEmptyFlag[1])
		{
			nDegree = 0;
			return "empty_20";
		}
		else if (arEmptyFlag[1] && arEmptyFlag[1] == arEmptyFlag[2])
		{
			nDegree = 90;
			return "empty_20";
		}
		else if (arEmptyFlag[2] && arEmptyFlag[2] == arEmptyFlag[3])
		{
			nDegree = 180;
			return "empty_20";
		}
		else if (arEmptyFlag[3] && arEmptyFlag[3] == arEmptyFlag[0])
		{
			nDegree = 270;
			return "empty_20";
		}
		
		if (arEmptyFlag[0] && arEmptyFlag[0] == arEmptyFlag[2])
		{
			nDegree = 0;
			return "empty_21";
		}
		else if (arEmptyFlag[1] && arEmptyFlag[1] == arEmptyFlag[3])
		{
			nDegree = 90;
			return "empty_21";
		}
	case 3: 
		if (!arEmptyFlag[0])
		{
			nDegree = 0;
		}
		else if (!arEmptyFlag[1])
		{
			nDegree = 90;
		}
		else if (!arEmptyFlag[2])
		{
			nDegree = 180;
		}
		else if (!arEmptyFlag[3])
		{
			nDegree = 270;
		}
		return "empty_3";
	case 4: 
		return "empty_4";
	default:
		return nullptr;
	}
}

void CGameWorldScene::OnSelectCell(int x, int y)
{
	if (!CanSelectCell(x, y))
	{
		return;
	}
	switch (m_enCurItemID)
	{
	case enItemID_Crystal:
		m_vecCells[x][y].enItemID = enItemID_Stone;
		break;
	default:
		m_vecCells[x][y].enItemID = m_enCurItemID;
		break;
	}
	
	// 合成处理
	FixItem(x, y);
	
	// 刷新界面
	UpdateMapLayer();

	m_enCurItemID = RandItem();
	CResourceMgr::getSingleton().InitSprite(m_pCurItem, "ui_item", g_arItemData[m_enCurItemID].szImage);
}

void CGameWorldScene::UpdateMapLayer()
{
	for (int x = 0; x < MAP_HEIGHT_CELL; x++)
	{
		for (int y = 0; y < MAP_WIDTH_CELL; y++)
		{
			if (m_vecCells[x][y].enItemID > enItemID_Null)
			{
				UpdateSpriteWithItem(x, y);
			}
			else
			{
				UpdateSpriteNoItem(x, y);
			}
		}
	}
}

bool CGameWorldScene::CanSelectCell(int x, int y)
{
	return (m_vecCells[x][y].enItemID == enItemID_Null);
}

// 计算以（x，y）为中心周围和enItemID相同的物品数量
void CGameWorldScene::CalcSameItemNum(int x, int y, EnItemID enItemID, int nIgnoreType, std::vector<std::pair<int,int>>& vec)
{
	if (m_vecCells[x][y].enItemID != enItemID)
	{
		return;
	}
	
	// 如果此点已经在列表中了，就说明已经遍历过了，不需要再遍历了，避免出现死循环
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it).first == x && (*it).second == y)
		{
			return;
		}
	}
	

	vec.push_back(std::make_pair(x, y));
	
	// 左
	if (nIgnoreType != 1 && IS_VALID_X(x) && IS_VALID_Y(y-1) && m_vecCells[x][y-1].enItemID == enItemID)
	{
		CalcSameItemNum(x, y-1, enItemID, 3, vec);
	}

	// 上
	if (nIgnoreType != 2 && IS_VALID_X(x-1) && IS_VALID_Y(y) && m_vecCells[x-1][y].enItemID == enItemID)
	{
		CalcSameItemNum(x-1, y, enItemID, 4, vec);
	}

	// 右
	if (nIgnoreType != 3 && IS_VALID_X(x) && IS_VALID_Y(y+1) && m_vecCells[x][y+1].enItemID == enItemID)
	{
		CalcSameItemNum(x, y+1, enItemID, 1, vec);
	}

	// 下
	if (nIgnoreType != 4 && IS_VALID_X(x+1) && IS_VALID_Y(y) && m_vecCells[x+1][y].enItemID == enItemID)
	{
		CalcSameItemNum(x+1, y, enItemID, 2, vec);
	}
}

void CGameWorldScene::FixItem(int x, int y)
{
	std::vector<std::pair<int,int>> vecPos;
	CalcSameItemNum(x, y, m_vecCells[x][y].enItemID, 0, vecPos);
	int nSameCount = int(vecPos.size());
	if (nSameCount > 1)
	{
		bool bNeedFix = false;
		for (int i = 0; i < sizeof(g_arFixData)/sizeof(g_arFixData[0]); i++)
		{
			if (g_arFixData[i].enSrcItemID == m_vecCells[x][y].enItemID && nSameCount >= g_arFixData[i].nSrcCount)
			{
				bNeedFix = true;
				for (auto it = vecPos.begin(); it != vecPos.end(); it++)
				{
					m_vecCells[(*it).first][(*it).second].enItemID = enItemID_Null;
				}
				m_vecCells[x][y].enItemID = g_arFixData[i].enDstItemID;
                m_nCurScore += g_arItemData[g_arFixData[i].enDstItemID].nFixAward;
                UpdateCurScore();
				break;
			}
		}

		// 再次合成
		if (bNeedFix)
		{
			FixItem(x, y);
		}
		
	}
}

EnItemID CGameWorldScene::RandItem()
{
    int nRand = rand() % 100;
    for (int i = 0; i < sizeof(g_arItemData)/sizeof(g_arItemData[0]); i++)
    {
        if (g_arItemData[i].nRand <= 0)
        {
            continue;
        }
        
        nRand -= g_arItemData[i].nRand;
        if (nRand <= 0)
        {
            return g_arItemData[i].nItemID;
        }
    }
    
    return enItemID_Grass;
}