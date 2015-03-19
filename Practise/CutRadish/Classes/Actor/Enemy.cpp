#include "Enemy.h"

Enemy::Enemy()
{
	mAttackPoint = 0;
	mHealthPoint = 0;
	mRunSpeed = 0;

	mState = E_NONE;
	mDirState = RIGHT;

	mHitBox.actual = Rect::ZERO;
	mHitBox.original = Rect::ZERO;
	mAttackBox.actual = Rect::ZERO;
	mAttackBox.original = Rect::ZERO;

	init();

	initAnimation();
}

bool Enemy::init()
{
	//CCSprite::init();
	return true;
}

Enemy::~Enemy()
{
}

BoundingBox Enemy::createBoundingBox(CCPoint origin, CCSize size)
{
	BoundingBox bBox;
	bBox.original.origin = origin;
	bBox.original.size = size;
	bBox.actual.origin = ccpAdd(getPosition(), 
		ccp(origin.x, origin.y));
	bBox.actual.size = size;

	return bBox;
}

void Enemy::transformBoxes()
{
	if (isFlipX()) {
		mHitBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(-mHitBox.original.origin.x - mHitBox.original.size.width, 
				mHitBox.original.origin.y));
		mAttackBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(-mAttackBox.original.origin.x - mAttackBox.original.size.width, 
				mAttackBox.original.origin.y));
	} else {
		mHitBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(mHitBox.original.origin.x, mHitBox.original.origin.y));
		mAttackBox.actual.origin = ccpAdd(this->getPosition(), 
			ccp(mAttackBox.original.origin.x, mAttackBox.original.origin.y));
	}
	mHitBox.actual.size = CCSizeMake(
		mHitBox.original.size.width, 
		mHitBox.original.size.height);
	mAttackBox.actual.size = CCSizeMake(
		mAttackBox.original.size.width, 
		mAttackBox.original.size.height);
}

void Enemy::setPosition(const CCPoint& pos)
{
	CCSprite::setPosition(pos);

	this->transformBoxes();
}

void Enemy::die()
{
	if (mState != E_DIE) {
		this->stopAllActions();
		this->runAction(CCSequence::create(
			mDieAnimate, CCHide::create(), NULL));
		mState = E_DIE;
	}
}

void Enemy::run(DirectState dir)
{
	if (mState != E_RUN) {
		this->stopAllActions();
		this->runAction(CCRepeatForever::create(mRunAnimate));
		mState = E_RUN;
	}

	if (dir == LEFT) {
		if (mDirState == RIGHT) {
			turnLeft();
		}
		this->setPosition(ccpAdd(getPosition(), 
			ccp(-mRunSpeed, 0)));
	} else {
		if (mDirState == LEFT) {
			turnRight();
		}
		this->setPosition(ccpAdd(getPosition(), 
			ccp(mRunSpeed, 0)));
	}
}

void Enemy::attack()
{
	if (mState != E_ATTACK) {
		this->stopAllActions();
		this->runAction(CCRepeatForever::create(mAttackAnimate));
		mState = E_ATTACK;
	}
}

void Enemy::hurt(int damage, float repelDist)
{
	mHealthPoint -= damage;

	if (mDirState == LEFT) {
		setPosition(ccpAdd(getPosition(), 
			ccp(repelDist, 0)));
	} else {
		setPosition(ccpAdd(getPosition(), 
			ccp(-repelDist, 0)));
	}
	//run action? -- collision rect??

	if (getHP() <= 0) {
		this->die();
	}
}

void Enemy::turnLeft()
{
	this->setFlipX(true);

	mDirState = LEFT;
}

void Enemy::turnRight()
{
	this->setFlipX(false);

	mDirState = RIGHT;
}

bool Enemy::initAnimation()
{
	mRunAnimate = NULL;
	mAttackAnimate = NULL;
	mDieAnimate = NULL;

	return true;
}

int Enemy::getHP()
{
	return mHealthPoint;
}

int Enemy::getAP()
{
	return mAttackPoint;
}