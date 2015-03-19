#include "GameRoot.h"

SceneGame* GameRoot::mSceneGame = NULL;

void GameRoot::InitializeResource()
{
	CCSpriteFrameCache* cache = 
			CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("UI.plist", "UI.png");
	cache->addSpriteFramesWithFile("GameBasic.plist", "GameBasic.png");
	cache->addSpriteFramesWithFile("character_res.plist", "character_res.png");
	cache->addSpriteFramesWithFile("smoke.plist", "smoke.png");

	char str[64] = {0};
	for (int i = 1; i <= 24; i++) {
		sprintf(str, "carrot_die (%d).png", i);
		CCSpriteFrame* frame = 
			cache->spriteFrameByName(str);
		frame->setOffsetInPixels(ccp(
			frame->getOffsetInPixels().x - 40.0f, 
			frame->getOffsetInPixels().y - 40.0f));
	}
	for (int i = 1; i <= 23; i++) {
		sprintf(str, "carrot_attack (%d).png", i);
		CCSpriteFrame* frame = 
			cache->spriteFrameByName(str);
		frame->setOffsetInPixels(ccp(
			frame->getOffsetInPixels().x, 
			frame->getOffsetInPixels().y - 4.0f));
	}
	for (int i = 1; i <= 18; i++) {
		sprintf(str, "carrot_walk (%d).png", i);
		CCSpriteFrame* frame = 
			cache->spriteFrameByName(str);
		frame->setOffsetInPixels(ccp(
			frame->getOffsetInPixels().x, 
			frame->getOffsetInPixels().y - 4.0f));
	}
}

SceneGame* GameRoot::getSceneGame()
{
	if (mSceneGame == NULL)
		mSceneGame = new SceneGame();
	return mSceneGame;
}