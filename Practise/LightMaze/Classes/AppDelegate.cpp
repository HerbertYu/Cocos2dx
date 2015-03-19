#include "AppDelegate.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "MapConfigMgr.h"

CAppDelegate::CAppDelegate()
{
}

CAppDelegate::~CAppDelegate()
{
    CSceneMgr::getSingleton().UnInit();
	CMapConfigMgr::getSingleton().UnInit();
    CResourceMgr::getSingleton().UnInit();
    SimpleAudioEngine::end();
}

bool CAppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    Director *pDirector = Director::getInstance();
	auto glView = pDirector->getOpenGLView();
	if (!glView){
		glView = GLViewImpl::create("MyGame");
		glView->setFrameSize(320, 480);
		pDirector->setOpenGLView(glView);
	}

    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
#ifdef _DEBUG
    pDirector->setDisplayStats(true);
#else
    pDirector->setDisplayStats(false);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // ³¡¾°³õÊ¼»¯
    CResourceMgr::getSingleton().Init();
	CMapConfigMgr::getSingleton().Init();
    CSceneMgr::getSingleton().Init();

    CSceneMgr::getSingleton().GotoFirstScene();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void CAppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void CAppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
