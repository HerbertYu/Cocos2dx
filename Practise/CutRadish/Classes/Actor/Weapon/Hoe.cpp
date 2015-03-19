#include "Hoe.h"

Hoe::Hoe() : 
	Weapon(false, 0.2f, 0.01f, CCRect(15, 15, 60, 40), 0.6f, 1)
{
	CCSprite::initWithSpriteFrameName("hoe.png");

	mAttackAnimate = NULL;
	initAnimation();

	Weapon::init();
}

Hoe::~Hoe()
{
	mAttackAnimate->release();
}

bool Hoe::initAnimation()
{
	bool bRet = false;

	mAttackAnimate = NULL;

	do 
    {
		CCSpriteFrameCache* cache = 
			CCSpriteFrameCache::sharedSpriteFrameCache();
		//CCArray* animFrames = new CCArray(30);
		char str[64] = {0};

		CCAnimation* attackAnim = CCAnimation::create();
		for (int i = 1; i <= 15; i++) {
			sprintf(str, "hoe (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			attackAnim->addSpriteFrame(frame);
		}
		attackAnim->setDelayPerUnit(0.02f);
		attackAnim->setRestoreOriginalFrame(true);
		mAttackAnimate = CCAnimate::create(attackAnim);
		CC_BREAK_IF(! mAttackAnimate);
		mAttackAnimate->retain();
		/*animFrames->removeAllObjects();
		for (int i = 1; i <= 15; i++) {
			sprintf(str, "hoe (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			animFrames->addObject(frame);
		}
		CCAnimation* attackAnim = CCAnimation::create(animFrames, 0.02f);
		mAttackAnimate = CCAnimate::create(attackAnim);
		CC_BREAK_IF(! mAttackAnimate);
		mAttackAnimate->retain();

		delete animFrames;*/

		bRet = true;
	} while (0);
	return bRet;
}