#include "Hero.h"
#include "Weapon/Hoe.h"
#include <iostream>

Hero::Hero(GameMap* pGameMap) : 
	//mArmOffsetX(12), mArmOffsetY(-10)
	mWeaponOffsetX(8), mWeaponOffsetY(-7), mHalfBodyWidth(18)
{
	mGameMap = pGameMap;

	initAnimation();

	this->mBody = CCSprite::createWithSpriteFrameName("stand.png");
	mBody->retain();
	mBody->setAnchorPoint(ccp(0.5f, 0));
	this->mWeapon = new Hoe();
	mWeapon->setAnchorPoint(ccp(0.5f, 0));
	mWeapon->setPosition(ccpAdd(mBody->getPosition(), 
		ccp(mWeaponOffsetX, mWeaponOffsetY)));

	this->mHealthPoint = 10;
	this->mRunSpeed = 1.5f;
	this->mJumpSpeed = 4.0f;
	this->mJumpHeight = 80;

	mState = NONE;
	mDirState = RIGHT;

	mHitBox = createBoundingBox(
		ccp(-18, 0), CCSizeMake(36, 90));

	//this->scheduleUpdate();

	this->idle();

	mIsJumping = false;
	mIsJumpingUp = true;

	init();
}

bool Hero::init()
{
	/*mState = IDLE;
	mDirState = RIGHT;

	mArm->init();

	turnRight();

	mBody->stopAllActions();
	mArm->stopAllActions();
	mBody->runAction(CCRepeatForever::create(mStandAnimate));*/

	return true;
}

Hero::~Hero()
{
	mBody->release();
	delete mWeapon;
	mRunAnimate->release();
	mStandAnimate->release();
}

BoundingBox Hero::createBoundingBox(CCPoint origin, CCSize size)
{
	BoundingBox bBox;
	bBox.original.origin = origin;
	bBox.original.size = size;
	bBox.actual.origin = ccpAdd(getPosition(), 
		ccp(origin.x, origin.y));
	bBox.actual.size = size;

	return bBox;
}

void Hero::transformBoxes()
{
	if (mBody->isFlipX()) {
		mHitBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(-mHitBox.original.origin.x - mHitBox.original.size.width, 
				mHitBox.original.origin.y));
	} else {
		mHitBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(mHitBox.original.origin.x, mHitBox.original.origin.y));
	}
	mHitBox.actual.size = CCSizeMake(
		mHitBox.original.size.width, 
		mHitBox.original.size.height);
}

void Hero::setPosition(const CCPoint &pos)
{
	mBody->setPosition(pos);

	if (mDirState == RIGHT) {
		mWeapon->setPositionX(pos.x + mWeaponOffsetX);
	} else if (mDirState == LEFT) {
		mWeapon->setPositionX(pos.x - mWeaponOffsetX);
	}
	mWeapon->setPositionY(pos.y);

	this->transformBoxes();
}

CCPoint Hero::getPosition()
{
	return mBody->getPosition();
}

float Hero::getPositionX()
{
	return mBody->getPositionX();
}

float Hero::getPositionY()
{
	return mBody->getPositionY();
}

void Hero::attachToLayer(CCLayer* layer, int z_index)
{
	//layer->addChild(this);
	layer->addChild(mBody, z_index);
	layer->addChild(mWeapon, z_index+1);
}

void Hero::die()
{
	//if (mState != DIE) {
		std::cout << "Hero die..." << std::endl;
	//}
}

void Hero::idle()
{
	if (mState != IDLE) {
		mBody->stopAllActions();
		mBody->runAction(CCRepeatForever::create(mStandAnimate));
		mState = IDLE;
	}
}

void Hero::run(DirectState dir)
{
	if (mState != RUN) {
		mBody->stopAllActions();
		mBody->runAction(CCRepeatForever::create(mRunAnimate));
		mState = RUN;
	}

	if (dir == LEFT) {
		if (mDirState == RIGHT) {
			turnLeft();
		}
		if (getPositionX() > (mGameMap->getWalkableLeft() + mHalfBodyWidth)) {
			this->setPosition(ccpAdd(getPosition(), 
				ccp(-mRunSpeed, 0)));
		}
	} else {
		if (mDirState == LEFT) {
			turnRight();
		}
		if (getPositionX() < (mGameMap->getWalkableRight() - mHalfBodyWidth)) {
			this->setPosition(ccpAdd(getPosition(), 
				ccp(mRunSpeed, 0)));
		}
	}
}

Bullet* Hero::attack()
{
	return mWeapon->attack(mDirState);
}

void Hero::hurt(int damage, float repelDist)
{
	mHealthPoint -= damage;

	if (mDirState == LEFT) {
		setPosition(ccpAdd(getPosition(), 
			ccp(repelDist, 0)));
	} else {
		setPosition(ccpAdd(getPosition(), 
			ccp(-repelDist, 0)));
	}
}

void Hero::jump()
{
	if (mIsJumping == false)
		mJumpOriginY = getPositionY();
	mIsJumping = true;
}

void Hero::turnLeft()
{
	mBody->setFlipX(true);
	mWeapon->setFlipX(true);
	mWeapon->setPosition(ccpAdd(mBody->getPosition(), 
		ccp(-mWeaponOffsetX, mWeaponOffsetY)));

	mDirState = LEFT;
}

void Hero::turnRight()
{
	mBody->setFlipX(false);
	mWeapon->setFlipX(false);
	mWeapon->setPosition(ccpAdd(mBody->getPosition(), 
		ccp(mWeaponOffsetX, mWeaponOffsetY)));

	mDirState = RIGHT;
}

int Hero::getHP()
{
	return mHealthPoint;
}

int Hero::getAP()
{
	return mWeapon->getAP();
}

WeaponState Hero::getWeaponState()
{
	/*WeaponState ws = mWeapon->getState();
	std::cout << ws << std::endl;
	return ws;*/
	return mWeapon->getState();
}

bool Hero::initAnimation()
{
	bool bRet = false;

	mRunAnimate = NULL;
	mStandAnimate = NULL;
    do 
    {
		CCSpriteFrameCache* cache = 
			CCSpriteFrameCache::sharedSpriteFrameCache();

		//CCArray* animFrames = new CCArray(30);
		char str[64] = {0};
		
		//Run
		CCAnimation* runAnim = CCAnimation::create();
		for (int i = 1; i <= 19; i++) {
			sprintf(str, "walk (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			runAnim->addSpriteFrame(frame);
		}
		runAnim->setDelayPerUnit(0.02f);
		runAnim->setRestoreOriginalFrame(true);
		mRunAnimate = CCAnimate::create(runAnim);
		CC_BREAK_IF(! mRunAnimate);
		mRunAnimate->retain();
		/*for (int i = 1; i <= 19; i++) {
			sprintf(str, "walk (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			animFrames->addObject(frame);
		}
		CCAnimation* runAnim = CCAnimation::create(animFrames, 0.02f);
		mRunAnimate = CCAnimate::create(runAnim);
		CC_BREAK_IF(! mRunAnimate);
		mRunAnimate->retain();*/

		//Stand
		CCAnimation* standAnim = CCAnimation::create();
		for (int i = 1; i <= 15; i++) {
			sprintf(str, "stand (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			standAnim->addSpriteFrame(frame);
		}
		standAnim->setDelayPerUnit(0.04f);
		standAnim->setRestoreOriginalFrame(true);
		mStandAnimate = CCAnimate::create(standAnim);
		CC_BREAK_IF(! mStandAnimate);
		mStandAnimate->retain();
		/*animFrames->removeAllObjects();
		for (int i = 1; i <= 15; i++) {
			sprintf(str, "stand (%d).png", i);
			CCSpriteFrame* frame = 
				cache->spriteFrameByName(str);
			animFrames->addObject(frame);
		}
		CCAnimation* standAnim = CCAnimation::create(animFrames, 0.04f);
		mStandAnimate = CCAnimate::create(standAnim);
		CC_BREAK_IF(! mStandAnimate);
		mStandAnimate->retain();

		delete animFrames;*/

		bRet = true;
	} while (0);
	return bRet;
}

inline void Hero::handleJump(float dt)
{
	if (mIsJumping) {
		if (mIsJumpingUp) {
			this->setPosition(ccpAdd(getPosition(), 
				ccp(0, mJumpSpeed)));
			if (getPositionY() >= mGameMap->getWalkableTop() || 
				getPositionY() >= (mJumpOriginY + mJumpHeight)) {
				mIsJumpingUp = false;
			}
		} else {
			this->setPosition(ccpAdd(getPosition(), 
				ccp(0, -mJumpSpeed)));
			if (getPositionY() <= mGameMap->getWalkableBottom()/* || 
				getPositionY() >= mJumpOrigin*/) {
				mIsJumping = false;
				mIsJumpingUp = true;
			}
		}
	}
}

void Hero::update(float dt)
{
	//CCNode::update(dt);
	mWeapon->update(dt);
	handleJump(dt);
}