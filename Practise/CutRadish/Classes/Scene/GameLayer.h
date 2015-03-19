#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include "cocos2d.h"
using namespace cocos2d;
using std::list;

#include "GameMap.h"
#include "Actor/Hero.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon/Bullet.h"
#include "Common.h"

class GameLayer : public CCLayer
{
public :
	GameLayer();
	bool init();
	~GameLayer();

	void setInDirLRState(InDirState pLeftRightState);	//Left or Right
	void setInDirUDState(InDirState pUpDownState);	//Up or Down
	void setInBtnState(InBtnState pBtnState);

private :
	void gameLogic(float dt);
	void inputUpdate();
	void spawnUpdate(float dt);
	void bulletUpdate(float dt);
	void enemyUpdate(float dt);
	void viewpointUpdate();

	GameMap* mGameMap;
	//CCSpriteBatchNode* mActors;
	Hero* mHero;
	InDirState mInDirLRState;
	InDirState mInDirUDState;
	InBtnState mInBtnState;
	list<Enemy*> mEnemies;
	list<Bullet*> mBullets;

	void removeSmoke(CCNode* node);
};

#endif