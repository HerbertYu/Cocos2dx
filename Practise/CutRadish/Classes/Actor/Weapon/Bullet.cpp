#include "Bullet.h"

Bullet::Bullet(bool isMultiAttack, float lifeTime, 
			   float speed, int attackPoint, 
			   const CCRect& collisionRect, DirectState dir)
{
	mIsMultiAttack = isMultiAttack;
	mLifeTime = lifeTime;
	mSpeed = speed;
	mAttackPoint = attackPoint;
	mDirection = dir;
	mCollisionRect = CCRect(collisionRect);
}

void Bullet::update(float dt)
{
	if (mDirection == LEFT) {
		mCollisionRect.origin.x -= mSpeed;
	} else {
		mCollisionRect.origin.x += mSpeed;
	}
	if (mLifeTime >= 0) {
		mLifeTime -= dt;
	}
}

bool Bullet::isMultiAttack()
{
	return mIsMultiAttack;
}

bool Bullet::isDie()
{
	return (mLifeTime <= 0);
}

void Bullet::setDie()
{
	mLifeTime = -1;
}

int Bullet::getAP()
{
	return mAttackPoint;
}