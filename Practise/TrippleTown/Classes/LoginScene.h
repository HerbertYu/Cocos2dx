#pragma once
#include <cocos2d.h>
#include "GameSceneBase.h"

class CLoginScene : public CGameSceneBase
{
public:
    CLoginScene();
    virtual ~CLoginScene();

    virtual bool Init();  
	virtual bool UnInit();
    virtual bool OnEnter();
    virtual bool OnLeave();

protected:
	void OnMenuNewGame(CCObject* pSender);
	void OnMenuSetting(CCObject* pSender);
	void OnMenuRank(CCObject* pSender);
	void OnMenuQuit(CCObject* pSender);
};
