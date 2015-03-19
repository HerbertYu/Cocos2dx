#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Common.h"
#include "Bullet.h"

class Weapon : public CCSprite
{
public :
	Weapon(bool isMultiAttack, float lifeTime, 
			   float speed, CCRect collisionRect, 
			   float attackInterval, int attackPoint);
	bool init();
	void update(float dt);
	Bullet* attack(DirectState dir);
	bool initAnimation();

	WeaponState getState();
	int getAP();

protected :
	const bool mIsBulletMultiAttack;
	const float mBulletLifeTime;
	const float mBulletSpeed;
	const CCRect mBulletCollisionRect;
	const int mAttackPoint;

	const float mAttackInterval;
	float mAttackTimeLeft;

	CCAnimate* mAttackAnimate;
};

#endif