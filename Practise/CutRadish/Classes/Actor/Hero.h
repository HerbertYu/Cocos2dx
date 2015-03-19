#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Weapon/Weapon.h"
#include "Common.h"
#include "Scene/GameMap.h"

class Hero// : public CCNode
{
public :
	Hero(GameMap* pGameMap);
	bool init();
	void update(float dt);
	~Hero();

	void setPosition(const CCPoint& pos);
	CCPoint getPosition();
	float getPositionX();
	float getPositionY();
	void attachToLayer(CCLayer* layer, int z_index);

	void die();
	void idle();
	void run(DirectState dir);
	void jump();
	Bullet* attack();
	void hurt(int damage, float repelDist);
	void turnLeft();
	void turnRight();

	bool initAnimation();

	int getHP();
	int getAP();
	WeaponState getWeaponState();

	HeroBodyState mState;
	DirectState mDirState;
	BoundingBox mHitBox;

protected :
	CCAnimate* mRunAnimate;
	CCAnimate* mStandAnimate;

	int mHealthPoint;
	float mRunSpeed;
	float mJumpSpeed;
	float mJumpHeight;

	BoundingBox createBoundingBox(CCPoint origin, CCSize size);

private :
	GameMap* mGameMap;

	CCSprite* mBody;
	Weapon* mWeapon;

	const int mWeaponOffsetX;
	const int mWeaponOffsetY;
	const int mHalfBodyWidth;

	void transformBoxes();

	bool mIsJumping;
	bool mIsJumpingUp;
	float mJumpOriginY;
	inline void handleJump(float dt);
};

#endif