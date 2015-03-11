// ≥°æ∞π‹¿Ì∆˜
#pragma once

#include "GameSceneBase.h"
#include "Defines.h"
#include "singleton.h"

class CSceneMgr : public TSingleton<CSceneMgr>
{
public:
    CSceneMgr();
    ~CSceneMgr();

    bool Init();
    bool UnInit();

    EnGameSceneType GetCurrentSceneType();
    CGameSceneBase* GetCurrentScene();
    void GotoFirstScene();
    void GotoScene(EnGameSceneType enType);

private:
    CCScene* m_pScene;
    CGameSceneBase* m_pGameScene[enGameScene_Count];
    EnGameSceneType m_enCurScene;
};