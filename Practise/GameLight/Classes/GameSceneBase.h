// 场景显示层基类
#pragma once
#include "StdAfx.h"

#include "Defines.h"
class CGameSceneBase : public cocos2d::CCLayer
{
public:
	CGameSceneBase(EnGameSceneType enType) : m_enSceneType(enType) {}
	virtual ~CGameSceneBase() {}

    virtual bool Init() { return true; }
    virtual bool UnInit() { return true; }

    virtual void OnEnter() {}
    virtual void OnLeave() {}

    inline EnGameSceneType GetSceneType() const { return m_enSceneType; }

protected:
    EnGameSceneType m_enSceneType;
};