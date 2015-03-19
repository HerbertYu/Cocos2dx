#ifndef _BULLET_H_
#define _BULLET_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Common.h"

class Bullet
{
public :
	Bullet(bool isMultiAttack, float lifeTime, 
		float speed, int attackPoint, 
		const CCRect& collisionRect, DirectState dir);
	void update(float dt);
	bool isMultiAttack();
	bool isDie();
	void setDie();
	int getAP();

	CCRect mCollisionRect;

private :
	bool mIsMultiAttack;
	float mLifeTime;
	float mSpeed;
	int mAttackPoint;
	DirectState mDirection;
};

#endif