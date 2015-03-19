#include "Carrot.h"

Carrot::Carrot()
{
	CCSprite::initWithSpriteFrameName("carrot.png");
	this->setAnchorPoint(ccp(0.5f, 0));

	this->mAttackPoint = 1;
	this->mHealthPoint = 2;

	int randSpeed = rand() % 5;
	this->mRunSpeed = 0.8f + randSpeed/10.0f;

	mState = E_NONE;
	mDirState = RIGHT;

	mHitBox = createBoundingBox(
		ccp(-18, 0), CCSizeMake(36, 65));
	mAttackBox = createBoundingBox(
		ccp(9, 24), CCSizeMake(15, 34));

	initAnimation();
}

Carrot::~Carrot()
{
	mRunAnimate->release();
	mAttackAnimate->release();
	mDieAnimate->release();
}

bool Carrot::initAnimation()
{
	bool bRet = false;
	do
	{
		mRunAnimate = NULL;
		mAttackAnimate = NULL;
		mDieAnimate = NULL;

		CCSpriteFrameCache* cache = 
			CCSpriteFrameCache::sharedSpriteFrameCache();

		//CCArray* animFrames = new CCArray(30);
		char str[64] = {0};
		
		//Run
		CCAnimation* runAnim = CCAnimation::create();
		for (int i = 1; i <= 18; i++) {
			sprintf(str, "carrot_walk (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			runAnim->addSpriteFrame(frame);
		}
		runAnim->setDelayPerUnit(0.04f);
		runAnim->setRestoreOriginalFrame(true);
		mRunAnimate = CCAnimate::create(runAnim);
		CC_BREAK_IF(! mRunAnimate);
		mRunAnimate->retain();
		/*for (int i = 1; i <= 18; i++) {
			sprintf(str, "carrot_walk (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			//frame->setOffsetInPixels(ccp(0, -10));
			animFrames->addObject(frame);
		}
		CCAnimation* runAnim = CCAnimation::create(animFrames, 0.04f);
		mRunAnimate = CCAnimate::create(runAnim);
		CC_BREAK_IF(! mRunAnimate);
		mRunAnimate->retain();*/

		//Attack
		CCAnimation* attackAnim = CCAnimation::create();
		for (int i = 1; i <= 23; i++) {
			sprintf(str, "carrot_attack (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			attackAnim->addSpriteFrame(frame);
		}
		attackAnim->setDelayPerUnit(0.03f);
		attackAnim->setRestoreOriginalFrame(true);
		mAttackAnimate = CCAnimate::create(attackAnim);
		CC_BREAK_IF(! mAttackAnimate);
		mAttackAnimate->retain();
		/*animFrames->removeAllObjects();
		for (int i = 1; i <= 23; i++) {
			sprintf(str, "carrot_attack (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			animFrames->addObject(frame);
		}
		CCAnimation* attackAnim = CCAnimation::create(animFrames, 0.03f);
		mAttackAnimate = CCAnimate::create(attackAnim);
		CC_BREAK_IF(! mAttackAnimate);
		mAttackAnimate->retain();*/

		//Die
		CCAnimation* dieAnim = CCAnimation::create();
		for (int i = 1; i <= 24; i++) {
			sprintf(str, "carrot_die (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			dieAnim->addSpriteFrame(frame);
		}
		dieAnim->setDelayPerUnit(0.032f);
		dieAnim->setRestoreOriginalFrame(true);
		mDieAnimate = CCAnimate::create(dieAnim);
		CC_BREAK_IF(! mDieAnimate);
		mDieAnimate->retain();
		/*animFrames->removeAllObjects();
		for (int i = 1; i <= 24; i++) {
			sprintf(str, "carrot_die (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			animFrames->addObject(frame);
		}
		CCAnimation* dieAnim = CCAnimation::create(animFrames, 0.032f);
		mDieAnimate = CCAnimate::create(dieAnim);
		CC_BREAK_IF(! mDieAnimate);
		mDieAnimate->retain();

		delete animFrames;*/

		bRet = true;
	} while (0);
	return bRet;
}