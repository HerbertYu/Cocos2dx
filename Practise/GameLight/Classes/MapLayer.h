#pragma once
#include "stdafx.h"
#include "Defines.h"
#include "ResourceMgr.h"

class CMapLayer : public cocos2d::Layer
{
public:
    // ��ͼ�ĵ�Ԫ��
    struct STCellData 
    {
        bool            bShow;
        char            enBlockType;
        char            enBlockState;
        CCSprite*       pSpriteBlock;
        CCSprite*       pSpriteLine;
        CCSprite*       pSpriteShadow;
		CCPoint			ptCenter;

        char            chLine[4];   // �����Ͻǿ�ʼ��˳ʱ�룬�ֱ��ʾ�Ƿ�����

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
    void ChangeLinePos(STLinePos& stPos); // �ı��ı�ʾ��ʽ
    char* GetCellLineImge(STCellData& stCell);
    bool IsTargetSelected(const STLinePos& stPos);   // �ж�Ŀ����Ƿ�����

    void BlockMirror(STLinePos& stPos); // ���䴦��

    void UpdateLine();
    bool LineTo(STLinePos& stSrc); // ����false��ʾ���߽���

	CC_SYNTHESIZE_READONLY(char, m_chCurTarget, CurTargetNum);
	CC_SYNTHESIZE_READONLY(char, m_chMaxTarget, MaxTargetNum);

private:
	STMapConfig* m_pCurMapConfig;
    STLevelConfig* m_pCurLevelConfig;

    CCPoint m_ptSlots;      // ���Ͻ�����
    CCSize  m_sizeSlots;
	CCSprite* m_pSelect;
    STCellData* m_pCellData;
    STCellData* m_pLastSel;
    CCSprite** m_ppTarget;
	CCSprite** m_ppSource;
};