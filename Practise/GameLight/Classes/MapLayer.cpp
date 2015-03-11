#include "MapLayer.h"
#include "MapConfigMgr.h"

#define ZORDER_MAP_SLOT     (1)     // 插槽
#define ZORDER_MAP_SHADOW   (2)     // 阴影
#define ZORDER_MAP_LINE     (3)     // 光线
#define ZORDER_MAP_POINT    (4)     // 点
#define ZORDER_MAP_BLOCK    (5)     // 方块
#define ZORDER_MAP_SELECT   (6)     // 选择遮罩

CMapLayer::CMapLayer()
{
    m_pCellData = nullptr;
	m_pLastSel = nullptr;
	m_pSelect = nullptr;
    m_ppTarget = nullptr;
	m_ppSource = nullptr;
	m_pCurMapConfig = nullptr;
    m_pCurLevelConfig = nullptr;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchEnded = CC_CALLBACK_2(CMapLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

CMapLayer::~CMapLayer()
{
}

void CMapLayer::GetCurMapID(char& nMapID, char& nLevel)
{
    nMapID = -1;
    nLevel = -1;
	if (m_pCurMapConfig == nullptr)
	{
		return;
	}
	nMapID = m_pCurMapConfig->chMapID;

    if (m_pCurLevelConfig == nullptr)
    {
        return;
    }
    nLevel = m_pCurLevelConfig->chLevel;
}

bool CMapLayer::LoadMap(char nMapID, char nLevelID)
{
    bool bRet = false;
    do
    {
        ClearData();

        CCSize ptSize = getContentSize();
        CCPoint ptWnd(-ptSize.width/2, ptSize.height/2);
        m_pCurMapConfig = CMapConfigMgr::getSingleton().GetMapConfig(nMapID);
        m_pCurLevelConfig = m_pCurMapConfig->GetLevelConfig(nLevelID);

        m_chCurTarget = 0;
        m_chMaxTarget = m_pCurLevelConfig->chTargetNum;

        m_pCellData = new STCellData[m_pCurLevelConfig->chCol * m_pCurLevelConfig->chRow];
        CC_BREAK_IF(m_pCellData == nullptr);

        // 插槽区域大小
        m_sizeSlots.width = m_pCurLevelConfig->chRow * (BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET) - BLOCK_SLOT_OFFSET;
        m_sizeSlots.height = m_pCurLevelConfig->chCol * (BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET) - BLOCK_SLOT_OFFSET;
        // 插槽左上角坐标
        m_ptSlots.x = ptWnd.x + (ptSize.width - m_sizeSlots.width)/2;
        m_ptSlots.y = ptWnd.y - (ptSize.height - m_sizeSlots.height)/2;

        // 第一个插槽的中心点
        CCPoint ptSlot(m_ptSlots.x + BLOCK_SLOT_WIDTH/2, m_ptSlots.y - BLOCK_SLOT_HEIGHT/2);

        // 加载插槽和方块
        char nIndex = 0;
        for (char nCol = 0; nCol < m_pCurLevelConfig->chCol; nCol++)
        {
            for (char nRow = 0; nRow < m_pCurLevelConfig->chRow; nRow++)
            {
                nIndex = nCol * m_pCurLevelConfig->chRow + nRow;
                const STMapCell& cell = m_pCurLevelConfig->pMapCell[nIndex];
                m_pCellData[nIndex].bShow = cell.bShow;
                m_pCellData[nIndex].enBlockType = cell.enBlockType;
                m_pCellData[nIndex].enBlockState = cell.enBlockState;
                m_pCellData[nIndex].pSpriteBlock = NULL;
                m_pCellData[nIndex].pSpriteLine = NULL;
                m_pCellData[nIndex].pSpriteShadow = NULL;
                m_pCellData[nIndex].ptCenter = ptSlot;

                if (cell.bShow)
                {
                    char szName[128];
                    if (cell.enBlockType > enBlockType_Null)
                    {
                        sprintf_s(szName, "block-%s-shadow", g_stBlockType[cell.enBlockType].chName);
                        m_pCellData[nIndex].pSpriteShadow = CResourceMgr::getSingleton().GetSprite(szName);
                        m_pCellData[nIndex].pSpriteShadow->setPosition(ptSlot);
                        addChild(m_pCellData[nIndex].pSpriteShadow, ZORDER_MAP_SHADOW);

                        sprintf_s(szName, "block-%s-%s", g_stBlockType[cell.enBlockType].chName,
                            g_stBlockState[cell.enBlockState].chName);
                        m_pCellData[nIndex].pSpriteBlock = CResourceMgr::getSingleton().GetSprite(szName);
                        m_pCellData[nIndex].pSpriteBlock->setPosition(ptSlot);
                        addChild(m_pCellData[nIndex].pSpriteBlock, ZORDER_MAP_BLOCK);
                    }
                    else
                    {
                        strcpy_s(szName, BLOCK_SLOT_NAME);
                        m_pCellData[nIndex].pSpriteBlock = CResourceMgr::getSingleton().GetSprite(szName);
                        m_pCellData[nIndex].pSpriteBlock->setPosition(ptSlot);
                        addChild(m_pCellData[nIndex].pSpriteBlock, ZORDER_MAP_SLOT);
                    }

                }
                ptSlot.x += BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET;
            }
            ptSlot.x = ptWnd.x + (ptSize.width - m_sizeSlots.width + BLOCK_SLOT_WIDTH)/2;
            ptSlot.y -= BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET;
        }

        // 加载光线起点
        m_ppSource = new CCSprite*[m_pCurLevelConfig->chSourceNum];
        for (char i = 0; i < m_pCurLevelConfig->chSourceNum; i++)
        {
            m_ppSource[i] = CResourceMgr::getSingleton().GetSprite("source-selected");
            m_ppSource[i]->setPosition(GetLinePos(m_pCurLevelConfig->pSourcePos[i]));
            addChild(m_ppSource[i], ZORDER_MAP_POINT);
        }

        // 加载目标点
        m_ppTarget = new CCSprite*[m_pCurLevelConfig->chTargetNum];
        for (char i = 0; i < m_pCurLevelConfig->chTargetNum; i++)
        {
            m_ppTarget[i] = CResourceMgr::getSingleton().GetSprite("target-normal");
            m_ppTarget[i]->setPosition(GetLinePos(m_pCurLevelConfig->pTargetPos[i]));
            addChild(m_ppTarget[i], ZORDER_MAP_POINT);
        }

        // 启动射线
        UpdateLine();

        bRet = true;
    } while(0);
    return bRet;
}

void CMapLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (touch == nullptr)
	{
		return;
	}
	CCPoint ptCur = touch->getLocation();
	CCSize ss = getContentSize();
	ptCur.x -= ss.width/2;
	ptCur.y -= ss.height/2 + 45;

    STCellData* pCurCell = GetCell(ptCur);

	if (m_pLastSel == nullptr || m_pLastSel->enBlockType == enBlockType_Null || m_pLastSel->enBlockState == enBlockState_Fixed)
	{
		// 如果当前选择的是空的或者没有Block的
        if (pCurCell != nullptr && pCurCell->enBlockType > enBlockType_Null && pCurCell->enBlockState != enBlockState_Fixed)
		{
            m_pLastSel = pCurCell;
            char szName[128];
            sprintf_s(szName, "block-%s-%s", g_stBlockType[m_pLastSel->enBlockType].chName,
                g_stBlockState[enBlockState_Select].chName);
            if (m_pSelect == nullptr)
            {
                m_pSelect = CResourceMgr::getSingleton().GetSprite(szName);
                addChild(m_pSelect, ZORDER_MAP_SELECT);
            }
            else
            {
                m_pSelect->setSpriteFrame(szName);
            }

            m_pSelect->setPosition(m_pLastSel->ptCenter);
            m_pSelect->setVisible(true);
		}
        else
        {
            m_pLastSel = nullptr;
        }
        return;
	}
	
    if (pCurCell != m_pLastSel && 
        pCurCell != nullptr && pCurCell->bShow && pCurCell->enBlockType == enBlockType_Null)
    {
        m_pLastSel->pSpriteShadow->setPosition(pCurCell->ptCenter);
        m_pLastSel->pSpriteBlock->setPosition(pCurCell->ptCenter);
        pCurCell->pSpriteBlock->setPosition(m_pLastSel->ptCenter);
        m_pLastSel->Swap(*pCurCell);
        UpdateLine();
    }
    m_pSelect->setVisible(false);
    m_pLastSel = nullptr;
}

CMapLayer::STCellData* CMapLayer::GetCell(const CCPoint& ptCur)
{
    if (ptCur.x <= m_ptSlots.x || ptCur.x >= m_ptSlots.x + m_sizeSlots.width ||
        ptCur.y >= m_ptSlots.y || ptCur.y <= m_ptSlots.y - m_sizeSlots.height)
    {
        return nullptr;
    }

    int nOffset = (int(ptCur.x - m_ptSlots.x)) % (BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET);
    int nRow = (ptCur.x - m_ptSlots.x) / (BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET);
    if (nOffset > BLOCK_SLOT_WIDTH)
    {
        nRow++;
    }
        
    nOffset = (int(m_ptSlots.y - ptCur.y)) % (BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET);
    int nCol = (m_ptSlots.y - ptCur.y) / (BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET);
    if (nOffset > BLOCK_SLOT_HEIGHT)
    {
        nCol++;
    }

    return &(m_pCellData[nCol * m_pCurLevelConfig->chRow + nRow]);
}

CCPoint CMapLayer::GetLinePos(const STLinePos& stPos)
{
    CCPoint pt(0, 0);

    if (stPos.chCol < 0 || stPos.chCol >= m_pCurLevelConfig->chCol ||
        stPos.chRow < 0 || stPos.chRow >= m_pCurLevelConfig->chRow)
    {
        return pt;
    }
    
    pt = m_pCellData[stPos.chCol * m_pCurLevelConfig->chRow + stPos.chRow].ptCenter;
    switch (stPos.enPosDir)
    {
    case enPosDir_Left:
        pt.x -= (BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET)/2;
        break;
    case enPosDir_Right:
        pt.x += (BLOCK_SLOT_WIDTH + BLOCK_SLOT_OFFSET)/2;
        break;
    case enPosDir_Top:
        pt.y += (BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET)/2;
        break;
    case enPosDir_Bottom:
        pt.y -= (BLOCK_SLOT_HEIGHT + BLOCK_SLOT_OFFSET)/2;
        break;
    default:
        break;
    }
    return pt;
}

CMapLayer::STCellData* CMapLayer::GetCellByLinePos(const STLinePos& stPos)
{
    if (stPos.chCol < 0 || stPos.chCol >= m_pCurLevelConfig->chCol ||
        stPos.chRow < 0 || stPos.chRow >= m_pCurLevelConfig->chRow)
    {
        return nullptr;
    }

    return &(m_pCellData[stPos.chCol * m_pCurLevelConfig->chRow + stPos.chRow]);
}

void CMapLayer::UpdateLine()
{
    // 清空光线
    for (char nCol = 0; nCol < m_pCurLevelConfig->chCol; nCol++)
    {
        for (char nRow = 0; nRow < m_pCurLevelConfig->chRow; nRow++)
        {
            STCellData& stCell = m_pCellData[nCol * m_pCurLevelConfig->chRow + nRow];
            if (stCell.pSpriteLine != nullptr)
            {
                stCell.pSpriteLine->setVisible(false);
            }
            memset(stCell.chLine, 0, sizeof(stCell.chLine));
        }
    }

    // 计算每个Cell中的线条数
    STCellData* pSrcCell = nullptr;
    STCellData* pDstCell = nullptr;
    STLinePos stSrc;
    for (char i = 0; i < m_pCurLevelConfig->chSourceNum; i++)
    {
        stSrc = m_pCurLevelConfig->pSourcePos[i];
        LineTo(stSrc);
    }

    // 刷新线条图片
    char* pszLineName = nullptr;
    for (char nCol = 0; nCol < m_pCurLevelConfig->chCol; nCol++)
    {
        for (char nRow = 0; nRow < m_pCurLevelConfig->chRow; nRow++)
        {
            STCellData& stCell = m_pCellData[nCol * m_pCurLevelConfig->chRow + nRow];
            pszLineName = GetCellLineImge(stCell);
            if (pszLineName != nullptr)
            {
                if (stCell.pSpriteLine == nullptr)
                {
                    stCell.pSpriteLine = CResourceMgr::getSingleton().GetSprite(pszLineName);
                    stCell.pSpriteLine->setPosition(stCell.ptCenter);
                    addChild(stCell.pSpriteLine, ZORDER_MAP_LINE);
                } 
                else
                {
					stCell.pSpriteLine->setSpriteFrame(pszLineName);
                    stCell.pSpriteLine->setVisible(true);
                }
            }
            else if (stCell.pSpriteLine != nullptr)
            {
                stCell.pSpriteLine->setVisible(false);
            }
        }
    }

    // 刷新目标图片
    char nCount = 0;
    for (char i = 0; i < m_pCurLevelConfig->chTargetNum; i++)
    {
        bool bSelected = false;
        
        bSelected = IsTargetSelected(m_pCurLevelConfig->pTargetPos[i]);
        if (!bSelected)
        {
            STLinePos stPos = m_pCurLevelConfig->pTargetPos[i];
            ChangeLinePos(stPos);
            bSelected = IsTargetSelected(stPos);
        }

        if (bSelected)
        {
			m_ppTarget[i]->setSpriteFrame("target-selected");
            nCount++;
        }
        else
        {
			m_ppTarget[i]->setSpriteFrame("target-normal");
        }
    }
    if (nCount == m_pCurLevelConfig->chTargetNum)
    {
        // 吼吼，赢了
        
    }
}

bool CMapLayer::LineTo(STLinePos& stSrc)
{
    static STLinePos s_pos1;
    static STLinePos s_pos2;
    if (stSrc.BeEqual(s_pos1))
    {
        return false;
    }
    s_pos1 = s_pos2;
    s_pos2 = stSrc;

    STCellData* pCell = GetCellByLinePos(stSrc);
    if (pCell == nullptr)
    {
        return false;
    }

    // 如果是反射，就先执行反射
    if (pCell->enBlockType == enBlockType_Mirror)
    {
        BlockMirror(stSrc);
        ChangeLinePos(stSrc);
        return LineTo(stSrc);
    }
    else if (pCell->enBlockType == enBlockType_Glass)
    {
        STLinePos pos(stSrc);
        BlockMirror(pos);
        ChangeLinePos(pos);
        LineTo(pos);
    }
    else if (pCell->enBlockType == enBlockType_Stop)
    {
        return false;
    }
    
    // 如果既反射又折射，就先执行折射
    // 计算下一个点，并画1/4线
    char chIndex = -1;
    if (stSrc.enPosDir == enPosDir_Top && stSrc.enLineDir == enLineDir_RightTop)
    {
        stSrc.enPosDir = enPosDir_Left;
        chIndex = 0;
    }
    else if (stSrc.enPosDir == enPosDir_Bottom && stSrc.enLineDir == enLineDir_RightBottom)
    {
        stSrc.enPosDir = enPosDir_Left;
        chIndex = 3;
    }
    else if (stSrc.enPosDir == enPosDir_Top && stSrc.enLineDir == enLineDir_LeftTop)
    {
        stSrc.enPosDir = enPosDir_Right;
        chIndex = 1;
    }
    else if (stSrc.enPosDir == enPosDir_Bottom && stSrc.enLineDir == enLineDir_LeftBottom)
    {
        stSrc.enPosDir = enPosDir_Right;
        chIndex = 2;
    } 
    else if (stSrc.enPosDir == enPosDir_Left && stSrc.enLineDir == enLineDir_LeftBottom)
    {
        stSrc.enPosDir = enPosDir_Top;
        chIndex = 0;
    }
    else if (stSrc.enPosDir == enPosDir_Right && stSrc.enLineDir == enLineDir_RightBottom)
    {
        stSrc.enPosDir = enPosDir_Top;
        chIndex = 1;
    } 
    else if (stSrc.enPosDir == enPosDir_Left && stSrc.enLineDir == enLineDir_LeftTop)
    {
        stSrc.enPosDir = enPosDir_Bottom;
        chIndex = 3;
    }
    else if (stSrc.enPosDir == enPosDir_Right && stSrc.enLineDir == enLineDir_RightTop)
    {
        stSrc.enPosDir = enPosDir_Bottom;
        chIndex = 2;
    }

    if (chIndex >= 0 && chIndex < 4)
    {
        if (pCell->chLine[chIndex] == 1)
        {
            // 出现环了
            return false;
        }
        pCell->chLine[chIndex] = 1;
    }

    ChangeLinePos(stSrc);

    if (stSrc.chCol < 0 || stSrc.chCol >= m_pCurLevelConfig->chCol ||
        stSrc.chRow < 0 || stSrc.chRow >= m_pCurLevelConfig->chRow)
    {
        return false;
    }

    return LineTo(stSrc);
}

void CMapLayer::ChangeLinePos(STLinePos& stPos)
{
    switch (stPos.enPosDir)
    {
    case enPosDir_Left:
        stPos.chRow--;
        stPos.enPosDir = enPosDir_Right;
        break;
    case enPosDir_Right:
        stPos.chRow++;
        stPos.enPosDir = enPosDir_Left;
        break;
    case enPosDir_Top:
        stPos.chCol--;
        stPos.enPosDir = enPosDir_Bottom;
        break;
    case enPosDir_Bottom:
        stPos.chCol++;
        stPos.enPosDir = enPosDir_Top;
        break;
    default:
        break;
    }
}

char* CMapLayer::GetCellLineImge(STCellData& stCell)
{
    int nSum = stCell.chLine[0] + stCell.chLine[1] + stCell.chLine[2] + stCell.chLine[3];
    switch (nSum)
    {
    case 0: 
        return nullptr;
    case 1: 
        if (stCell.chLine[0] == 1)
        {
            return "laser-1";
        }
        else if (stCell.chLine[1] == 1)
        {
            return "laser-2";
        }
        else if (stCell.chLine[2] == 1)
        {
            return "laser-4";
        }
        else
        {
            return "laser-8";
        }
    case 2: 
        if (stCell.chLine[0] && stCell.chLine[0] == stCell.chLine[1])
        {
            return "laser-3";
        }
        else if (stCell.chLine[1] && stCell.chLine[1] == stCell.chLine[2])
        {
            return "laser-6";
        }
        else if (stCell.chLine[2] && stCell.chLine[2] == stCell.chLine[3])
        {
            return "laser-12";
        }
        else if (stCell.chLine[3] && stCell.chLine[3] == stCell.chLine[0])
        {
            return "laser-9";
        }

        if (stCell.chLine[0] && stCell.chLine[0] == stCell.chLine[2])
        {
            return "laser-5";
        }
        else if (stCell.chLine[1] && stCell.chLine[1] == stCell.chLine[3])
        {
            return "laser-10";
        }
        return nullptr;
    case 3: 
        if (!stCell.chLine[0])
        {
            return "laser-14";
        }
        else if (!stCell.chLine[1])
        {
            return "laser-13";
        }
        else if (!stCell.chLine[2])
        {
            return "laser-11";
        }
        else if (!stCell.chLine[3])
        {
            return "laser-7";
        }
        return nullptr;
    case 4: 
        return "laser-15";
    default:
        return nullptr;
    }
}

void CMapLayer::BlockMirror(STLinePos& stPos)
{
    switch (stPos.enPosDir)
    {
    case enPosDir_Left:
        if (stPos.enLineDir == enLineDir_LeftBottom)
        {
            stPos.enLineDir = enLineDir_RightBottom;
        }
        else if (stPos.enLineDir == enLineDir_LeftTop)
        {
            stPos.enLineDir = enLineDir_RightTop;
        }
        break;
    case enPosDir_Top:
        if (stPos.enLineDir == enLineDir_RightTop)
        {
            stPos.enLineDir = enLineDir_RightBottom;
        }
        else if (stPos.enLineDir == enLineDir_LeftTop)
        {
            stPos.enLineDir = enLineDir_LeftBottom;
        }
        break;
    case enPosDir_Right:
        if (stPos.enLineDir == enLineDir_RightBottom)
        {
            stPos.enLineDir = enLineDir_LeftBottom;
        }
        else if (stPos.enLineDir == enLineDir_RightTop)
        {
            stPos.enLineDir = enLineDir_LeftTop;
        }
        break;
    case enPosDir_Bottom:
        if (stPos.enLineDir == enLineDir_RightBottom)
        {
            stPos.enLineDir = enLineDir_RightTop;
        }
        else if (stPos.enLineDir == enLineDir_LeftBottom)
        {
            stPos.enLineDir = enLineDir_LeftTop;
        }
        break;
    default:
        break;
    }
}

bool CMapLayer::IsTargetSelected(const STLinePos& stPos)
{
    STCellData* pCell = GetCellByLinePos(stPos);
    if (pCell == nullptr)
    {
        return false;
    }

    switch (stPos.enPosDir)
    {
    case enPosDir_Left:
        if (pCell->chLine[3] || pCell->chLine[0])
        {
            return true;
        }
        break;
    case enPosDir_Top:
        if (pCell->chLine[0] || pCell->chLine[1])
        {
            return true;
        }
        break;
    case enPosDir_Right:
        if (pCell->chLine[1] || pCell->chLine[2])
        {
            return true;
        }
        break;
    case enPosDir_Bottom:
        if (pCell->chLine[2] || pCell->chLine[3])
        {
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

void CMapLayer::ClearData()
{
	if (m_pCellData != nullptr && m_pCurLevelConfig->chCol > 0 && m_pCurLevelConfig->chRow > 0)
	{
		for (int nCol = 0; nCol < m_pCurLevelConfig->chCol; nCol++)
		{
			for (int nRow = 0; nRow < m_pCurLevelConfig->chRow; nRow++)
			{
				STCellData& stCell = m_pCellData[nCol * m_pCurLevelConfig->chRow + nRow];
				if (stCell.pSpriteShadow != nullptr)
				{
					removeChild(stCell.pSpriteShadow, true);
				}
				if (stCell.pSpriteLine != nullptr)
				{
					removeChild(stCell.pSpriteLine, true);
				}
				if (stCell.pSpriteBlock != nullptr)
				{
					removeChild(stCell.pSpriteBlock, true);
				}
			}
		}
		CC_SAFE_DELETE_ARRAY(m_pCellData);
	}

	if (m_ppTarget != nullptr && m_pCurLevelConfig->chTargetNum > 0)
	{
		for (int i = 0; i < m_pCurLevelConfig->chTargetNum; i++)
		{
			if (m_ppTarget[i] != nullptr)
			{
				removeChild(m_ppTarget[i]);
			}
		}
		CC_SAFE_DELETE_ARRAY(m_ppTarget);
	}

	if (m_ppSource != nullptr && m_pCurLevelConfig->chSourceNum > 0)
	{
		for (int i = 0; i < m_pCurLevelConfig->chSourceNum; i++)
		{
			if (m_ppSource[i] != nullptr)
			{
				removeChild(m_ppSource[i]);
			}
		}
		CC_SAFE_DELETE_ARRAY(m_ppSource);
	}

	m_pLastSel = nullptr;
	m_pCurLevelConfig = nullptr;
}