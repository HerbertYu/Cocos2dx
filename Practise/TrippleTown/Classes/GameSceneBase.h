#pragma once
#include <cocos2d.h>
using namespace cocos2d;

#include "Define.h"
class CGameSceneBase : public cocos2d::CCScene
{
public:
	CGameSceneBase(EnGameSceneType enType) : m_enSceneType(enType) {}
	virtual ~CGameSceneBase(){}

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool Init() { return true; }
	virtual bool UnInit() { return true; }

    virtual bool OnEnter() = 0;
    virtual bool OnLeave() = 0;

protected:
    EnGameSceneType m_enSceneType;
};