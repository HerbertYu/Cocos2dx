#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Common.h"

class Enemy : public CCSprite
{
public :
	Enemy();
	bool init();
	~Enemy();

	void setPosition(const CCPoint& pos);
	void die();
	void run(DirectState dir);
	void attack();
	void hurt(int damage, float repelDist);
	void turnLeft();
	void turnRight();
	bool initAnimation();

	int getHP();
	int getAP();

	EnemyState mState;
	DirectState mDirState;
	BoundingBox mAttackBox;
	BoundingBox mHitBox;

protected :
	CCAnimate* mRunAnimate;
	CCAnimate* mAttackAnimate;
	CCAnimate* mDieAnimate;

	int mAttackPoint;
	int mHealthPoint;
	float mRunSpeed;

	BoundingBox createBoundingBox(CCPoint origin, CCSize size);

private :
	void transformBoxes();
};

#endif