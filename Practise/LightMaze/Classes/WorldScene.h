#pragma once
#include "StdAfx.h"
#include "GameSceneBase.h"
#include "MapLayer.h"

class CWorldScene : public CGameSceneBase
{
public:
    CWorldScene();
    virtual ~CWorldScene();

    virtual bool Init();
    virtual bool UnInit();

    bool LoadMap(char nMapID, char nLevelID);

    CREATE_FUNC(CWorldScene);

protected:
	void OnMapName(CCObject* pSender);
	void OnNextMap(CCObject* pSender);
	void OnPreMap(CCObject* pSender);
	void OnShowHint(CCObject* pSender);
	void OnReturnBack(CCObject* pSender);

private:
    CMapLayer* m_pMapLayer;

	CCLabelTTF* m_pMapName;
	CCMenuItemImage* m_pPreMap;
	CCMenuItemImage* m_pNextMap;
	CCMenuItemImage* m_pShowHint;
};
