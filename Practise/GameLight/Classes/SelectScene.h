// 关卡选择层
// 一般类似小鳄鱼洗澡那种，分多个关卡，采用网格的方式排列
// 关卡层，需要记录玩家在每个关卡的数据：上一次得分，星星数量，最高得分等

#pragma once
#include "StdAfx.h"
#include "GameSceneBase.h"
#include <cocos-ext.h>

// 关卡选择层
class CSelectScene : public CGameSceneBase, 
    public extension::TableViewDataSource, 
    public extension::TableViewDelegate
{
public:
    CSelectScene();
    virtual ~CSelectScene();

    virtual bool Init();
    virtual bool UnInit();
    virtual void OnEnter();
    virtual void OnLeave();
    void ShowLayer(char nMapID);

    virtual void scrollViewDidScroll(extension::ScrollView* view) {}
	virtual void scrollViewDidZoom(extension::ScrollView* view) {}
    virtual void tableCellTouched(extension::TableView* table, extension::TableViewCell* cell);
    virtual CCSize cellSizeForTable(extension::TableView *table);
	virtual extension::TableViewCell* tableCellAtIndex(extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(extension::TableView *table);

    CREATE_FUNC(CSelectScene);

private:
    char m_nCurMapID;
	Array* m_pTableCells;
    extension::TableView* m_pTableView;
};
