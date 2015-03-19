//////////////////////////////////////////////////////////////////////////
// 资源管理
#pragma once
#include <map>
#include <singleton.h>
#include <cocos2d.h>
#include <SimpleAudioEngine.h>
using namespace cocos2d;
using namespace CocosDenshion;

// xml解析相关
#include <libxml/parser.h>

class CResourceMgr : public TSingleton<CResourceMgr>
{
public:
	CResourceMgr();
	~CResourceMgr();

	bool Init();
	bool UnInit();

    CCSprite* GetSprite(const char* pszName);
    CCSpriteFrame* GetSpriteFrame(const char* pszName);
    CCAnimation* GetAnimation(const char* pszName);
};