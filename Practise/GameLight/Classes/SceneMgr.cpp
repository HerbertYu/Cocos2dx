#include "SceneMgr.h"
#include "LoginScene.h"
#include "SelectScene.h"
#include "WorldScene.h"

CSceneMgr::CSceneMgr() : m_enCurScene(enGameScene_Invalid)
{
    memset(m_pGameScene, 0, sizeof(m_pGameScene));
}

CSceneMgr::~CSceneMgr()
{
}

bool CSceneMgr::Init()
{
    bool bRet = false;
    do 
    {
        // µÇÂ½²ã
        CGameSceneBase* pScene = new CLoginScene();
        pScene->Init();
        m_pGameScene[pScene->GetSceneType()] = pScene;

        // ¹Ø¿¨Ñ¡Ôñ²ã
        pScene = new CSelectScene();
        pScene->Init();
        m_pGameScene[pScene->GetSceneType()] = pScene;

        // ÓÎÏ·ÊÀ½ç²ã
        pScene = new CWorldScene();
        pScene->Init();
        m_pGameScene[pScene->GetSceneType()] = pScene;

        m_enCurScene = enGameScene_Invalid;
        bRet = true;
    } while (false);
    return bRet;
}

bool CSceneMgr::UnInit()
{
    for (int i = 0; i < enGameScene_Count; i++)
    {
        if (m_pGameScene[i] != nullptr)
        {
            m_pGameScene[i]->UnInit();
            CC_SAFE_DELETE(m_pGameScene[i]);
        }
    }
    return true;
}

EnGameSceneType CSceneMgr::GetCurrentSceneType()
{
    return m_enCurScene;
}

CGameSceneBase* CSceneMgr::GetCurrentScene()
{
    if (m_enCurScene > enGameScene_Invalid && m_enCurScene < enGameScene_Count)
    {
        return m_pGameScene[m_enCurScene];
    }
    else
    {
        return nullptr;
    }
}

void CSceneMgr::GotoScene(EnGameSceneType enType)
{
    if (enType == m_enCurScene || enType <= enGameScene_Invalid || enType >= enGameScene_Count)
    {
        return;
    }

    bool bReplace = false;
    CGameSceneBase* pCurScene = GetCurrentScene();
    if (pCurScene != nullptr)
    {
        pCurScene->OnLeave();
        bReplace = true;
    }

    m_enCurScene = enType;
    pCurScene = GetCurrentScene();
    if (pCurScene != nullptr)
    {
        pCurScene->OnEnter();
    }

    CCScene* pScene = CCScene::create();
    pScene->addChild(pCurScene);
    if (bReplace)
    {
        Director::getInstance()->replaceScene(CCTransitionCrossFade::create(1.0f, pScene));
    } 
    else
    {
        Director::getInstance()->runWithScene(pScene);
    }
}

void CSceneMgr::GotoFirstScene()
{
    GotoScene(enGameScene_Select);
}