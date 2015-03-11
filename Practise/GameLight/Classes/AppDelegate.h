#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__
#pragma once
#include "StdAfx.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  CAppDelegate : private cocos2d::CCApplication
{
public:
    CAppDelegate();
    virtual ~CAppDelegate();

    /**
    @brief    Implement Director and CCScene init code here.
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
    inline STUserGlobalData& GetUserGlobalData() { return m_stUserGlobalData; }


private:
    STUserGlobalData    m_stUserGlobalData;
};

#endif  // __APP_DELEGATE_H__

