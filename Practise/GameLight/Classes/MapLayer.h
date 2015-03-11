#pragma once
#include "stdafx.h"
#include "Defines.h"
#include "ResourceMgr.h"

class CMapLayer : public cocos2d::Layer
{
public:
    // 地图的单元格
    struct STCellData 
    {
        bool            bShow;
        char            enBlockType;
        char            enBlockState;
        CCSprite*       pSpriteBlock;
        CCSprite*       pSpriteLine;
        CCSprite*       pSpriteShadow;
		CCPoint			ptCenter;

        char            chLine[4];   // 从左上角开始，顺时针，分别表示是否有线

        STCellData()
        {
            memset(this, 0, sizeof(*this));
        }

        void Swap(STCellData& other)
        {
            CC_SWAP(enBlockType, other.enBlockType, char);
            CC_SWAP(enBlockState, other.enBlockState, char);
            CC_SWAP(pSpriteBlock, other.pSpriteBlock, CCSprite*);
            CC_SWAP(pSpriteShadow, other.pSpriteShadow, CCSprite*);
        }
    };

    CMapLayer();
    virtual ~CMapLayer();

    CREATE_FUNC(CMapLayer);

    bool LoadMap(char nMapID, char nLevelID);
    void GetCurMapID(char& nMapID, char& nLevelID);

	virtual void onTouchEnded(Touch *touch, Event *unused_event);
protected:
	void ClearData();
    STCellData* GetCell(const CCPoint& ptCur);
    STCellData* GetCellByLinePos(const STLinePos& stPos);
    STCellData* GetNextCell(const STLinePos& stPos);
    CCPoint GetLinePos(const STLinePos& stPos);
    void ChangeLinePos(STLinePos& stPos); // 改变点的表示方式
    char* GetCellLineImge(STCellData& stCell);
    bool IsTargetSelected(const STLinePos& stPos);   // 判断目标点是否命中

    void BlockMirror(STLinePos& stPos); // 反射处理

    void UpdateLine();
    bool LineTo(STLinePos& stSrc); // 返回false表示划线结束

	CC_SYNTHESIZE_READONLY(char, m_chCurTarget, CurTargetNum);
	CC_SYNTHESIZE_READONLY(char, m_chMaxTarget, MaxTargetNum);

private:
	STMapConfig* m_pCurMapConfig;
    STLevelConfig* m_pCurLevelConfig;

    CCPoint m_ptSlots;      // 左上角坐标
    CCSize  m_sizeSlots;
	CCSprite* m_pSelect;
    STCellData* m_pCellData;
    STCellData* m_pLastSel;
    CCSprite** m_ppTarget;
	CCSprite** m_ppSource;
};