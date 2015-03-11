#pragma once
#include "StdAfx.h"
#include "GameSceneBase.h"

class CLoginScene : public CGameSceneBase
{
public:
    CLoginScene();
    virtual ~CLoginScene();

    virtual bool Init();
    virtual bool UnInit();

    CREATE_FUNC(CLoginScene);
};
