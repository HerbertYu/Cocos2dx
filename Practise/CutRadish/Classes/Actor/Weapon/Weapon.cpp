#include "Weapon.h"

Weapon::Weapon(bool isMultiAttack, float lifeTime, 
			   float speed, CCRect collisionRect, 
			   float attackInterval, int attackPoint) :
	mIsBulletMultiAttack(isMultiAttack), mBulletLifeTime(lifeTime), 
	mBulletSpeed(speed), mBulletCollisionRect(collisionRect), 
	mAttackInterval(attackInterval), mAttackPoint(attackPoint)
{
	CCSprite::initWithSpriteFrameName("hoe.png");

	mAttackAnimate = NULL;
	initAnimation();

	init();

	//this->scheduleUpdate();
}

bool Weapon::init()
{
	mAttackTimeLeft = -1.0f;
	return true;
}

void Weapon::update(float dt)
{
	if (mAttackTimeLeft >= 0) {
		mAttackTimeLeft -= dt;
	}
	CCSprite::update(dt);
}

bool Weapon::initAnimation()
{
	mAttackAnimate = NULL;
	return true;
}

Bullet* Weapon::attack(DirectState dir)
{
	this->stopAllActions();
	this->runAction(mAttackAnimate);
	mAttackTimeLeft = mAttackInterval;

	CCRect rect = Rect::ZERO;
	if (dir == RIGHT) {
		rect = CCRect(
			this->getPositionX() + mBulletCollisionRect.origin.x, 
			this->getPositionY() + mBulletCollisionRect.origin.y, 
			mBulletCollisionRect.size.width, 
			mBulletCollisionRect.size.height);
	} else {
		rect = CCRect(
			this->getPositionX() - mBulletCollisionRect.origin.x - mBulletCollisionRect.size.width, 
			this->getPositionY() + mBulletCollisionRect.origin.y, 
			mBulletCollisionRect.size.width, 
			mBulletCollisionRect.size.height);
	}
	/*CCRect rect = CCRect(this->getPositionX() + 15, 
		this->getPositionY() + 20, 
		mBulletCollisionRect.size.width, 
		mBulletCollisionRect.size.height);*/
	Bullet* bptr = new Bullet(mIsBulletMultiAttack, mBulletLifeTime, 
		mBulletSpeed, mAttackPoint, rect, dir);
	return bptr;
}

WeaponState Weapon::getState()
{
	WeaponState retState = WEAPON_READY;
	//TODO : bullet count~~
	if (mAttackTimeLeft <= 0) {
		retState = WEAPON_READY;
	} else {
		retState = NO_COOL_DOWN;
	}
	return retState;
}

int Weapon::getAP()
{
	return mAttackPoint;
}