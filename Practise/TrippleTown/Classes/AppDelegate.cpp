#include "Define.h"
#include "AppDelegate.h"

#include "ResourceMgr.h"
#include "LoginScene.h"
#include "RankScene.h"
#include "SettingScene.h"
#include "GameWorldScene.h"

AppDelegate::AppDelegate() : m_pCurScene(nullptr)
{
	memset(m_vecScenes, 0, sizeof(m_vecScenes));
}

AppDelegate::~AppDelegate()
{
	for (int i = 0; i < enGameScene_Count; i++)
	{
		if (m_vecScenes[i] != nullptr)
		{
			m_vecScenes[i]->UnInit();
			delete m_vecScenes[i];
			m_vecScenes[i] = nullptr;
		}
	}
    m_pCurScene = nullptr;
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
	auto glView = pDirector->getOpenGLView();
	if (!glView){
		glView = GLViewImpl::create("TrippleTown");
// 		glView->setFrameSize(320, 480);
		pDirector->setOpenGLView(glView);
	}
    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	FileUtils::getInstance()->addSearchPath("Data");

	// 加载资源
	CResourceMgr::getSingleton().Init();

	m_stUserData.nMoney = 1000;

    // 登陆层
	CLoginScene* pLoginScene = new CLoginScene();
	pLoginScene->Init();
	m_vecScenes[enGameScene_Login] = pLoginScene;
    
    // 游戏世界层
    CGameWorldScene* pWorldScene = new CGameWorldScene();
    pWorldScene->Init();
    m_vecScenes[enGameScene_World] = pWorldScene;

    // 排行榜层
    CRankScene* pRankScene = new CRankScene();
    pRankScene->Init();
    m_vecScenes[enGameScene_Rank] = pRankScene;

    // 设置层
    CSettingScene* pSettingScene = new CSettingScene();
    pSettingScene->Init();
    m_vecScenes[enGameScene_Setting] = pSettingScene;

    GotoScene(enGameScene_Login);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

CGameSceneBase* AppDelegate::GetCurrentScene()
{
	return m_pCurScene;
}

bool AppDelegate::GotoScene(EnGameSceneType enType)
{
    if (enType >= enGameScene_Count || m_vecScenes[enType] == nullptr)
    {
        return false;
    }
    if (m_pCurScene != nullptr)
    {
        m_pCurScene->OnLeave();
    }
    m_pCurScene = m_vecScenes[enType];
    m_pCurScene->OnEnter();
    CCDirector::sharedDirector()->runWithScene(m_pCurScene);
    return true;
}