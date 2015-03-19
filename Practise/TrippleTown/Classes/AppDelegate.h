#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__
#pragma once
#include <cocos2d.h>
// #include <apples.h>

#include "Define.h"
#include "GameSceneBase.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : public TSingleton<AppDelegate>, private cocos2d::CCApplication
{
public:
	AppDelegate();
	~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

public:
	CGameSceneBase* GetCurrentScene();
    bool GotoScene(EnGameSceneType enType);

	STUserData* GetUserData(){ return &m_stUserData; }

private:
	CGameSceneBase* m_vecScenes[enGameScene_Count];
	CGameSceneBase* m_pCurScene;

	STUserData m_stUserData;
};

#endif  // __APP_DELEGATE_H__

