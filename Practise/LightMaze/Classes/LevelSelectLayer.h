#pragma once
#include "stdafx.h"
#include "SelectScene.h"
#include <cocos2d.h>
using namespace cocos2d;

class CLevelSelectCell : public cocos2d::extension::TableViewCell
{
public:
    CLevelSelectCell();
    virtual ~CLevelSelectCell();

    bool Init(CSelectScene* pScene, char nMapID);

    void OnLevelSelect(CCObject* pSender);

private:
    CSelectScene* m_pSelectScene;
    char m_nMapID;
};