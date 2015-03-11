// �ؿ�ѡ���
// һ������С����ϴ�����֣��ֶ���ؿ�����������ķ�ʽ����
// �ؿ��㣬��Ҫ��¼�����ÿ���ؿ������ݣ���һ�ε÷֣�������������ߵ÷ֵ�

#pragma once
#include "StdAfx.h"
#include "GameSceneBase.h"
#include <cocos-ext.h>

// �ؿ�ѡ���
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
