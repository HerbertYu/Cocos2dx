#include "GameLayer.h"
#include "Actor/Carrot.h"

const float MAP_POS_X = WIN_WIDTH / 2;
const float MAP_POS_Y = WIN_HEIGHT / 2;
const int MAP_Z_INDEX_GAME_LAYER = 0;

const float HERO_POS_X = WIN_WIDTH / 2;
const int HERO_Z_INDEX_GAME_LAYER = 10;

const float ENEMY_SPAWN_INTERVAL = 3.0f;
const int ENEMY_Z_INDEX_GAME_LAYER = 20;

GameLayer::GameLayer()
{
	mGameMap = NULL;
	mGameMap = new GameMap();
	mGameMap->setPosition(ccp(MAP_POS_X, MAP_POS_Y));
	this->addChild(mGameMap, MAP_Z_INDEX_GAME_LAYER);
	/*CCSprite* bg = CCSprite::createWithSpriteFrameName("background.png");
	bg->setPosition(ccp(MAP_POS_X, MAP_POS_Y));
	this->addChild(bg, MAP_Z_INDEX_GAME_LAYER);*/

	mHero = NULL;
	mHero = new Hero(mGameMap);
	mHero->setPosition(ccp(HERO_POS_X, mGameMap->getWalkableBottom()));
	mHero->attachToLayer(this, HERO_Z_INDEX_GAME_LAYER);

	this->schedule(schedule_selector(GameLayer::gameLogic));

	mInDirLRState = IN_DIR_NO_PRESSED;
	mInDirUDState = IN_DIR_NO_PRESSED;
	mInBtnState = IN_BTN_NO_PRESSED;

	init();
}

bool GameLayer::init()
{
	//CCLayer::init();

	/*mGameMap->setPosition(ccp(MAP_POS_X, MAP_POS_Y));
	mHero->setPosition(ccp(HERO_POS_X, mGameMap->getWalkableBottom()));
	mHero->init();

	mInDirLRState = IN_DIR_NO_PRESSED;
	mInDirUDState = IN_DIR_NO_PRESSED;
	mInBtnState = IN_BTN_NO_PRESSED;

	list<Enemy*>::iterator enemyItr;
	for (enemyItr = mEnemies.begin(); enemyItr != mEnemies.end();) {
		delete *enemyItr;
		enemyItr = mEnemies.erase(enemyItr);
	}
	list<Bullet*>::iterator bulletItr;
	for (bulletItr = mBullets.begin(); bulletItr != mBullets.end();) {
		delete *bulletItr;
		bulletItr = mBullets.erase(bulletItr);
	}*/

	return true;
}

GameLayer::~GameLayer()
{
	delete mGameMap;
	delete mHero;

	list<Enemy*>::iterator enemyItr;
	for (enemyItr = mEnemies.begin(); enemyItr != mEnemies.end();) {
		delete *enemyItr;
		enemyItr = mEnemies.erase(enemyItr);
	}
	list<Bullet*>::iterator bulletItr;
	for (bulletItr = mBullets.begin(); bulletItr != mBullets.end();) {
		delete *bulletItr;
		bulletItr = mBullets.erase(bulletItr);
	}
}

void GameLayer::setInDirLRState(InDirState pLeftRightState)
{
	mInDirLRState = pLeftRightState;
}

void GameLayer::setInDirUDState(InDirState pUpDownState)
{
	mInDirUDState = pUpDownState;
}

void GameLayer::setInBtnState(InBtnState pBtnState)
{
	mInBtnState = pBtnState;
}

void GameLayer::gameLogic(float dt)
{
	mHero->update(dt);
	inputUpdate();
	spawnUpdate(dt);
	bulletUpdate(dt);
	enemyUpdate(dt);
	viewpointUpdate();
}

void GameLayer::inputUpdate()
{
	HeroBodyState newMS = mHero->mState;
	DirectState newDS = mHero->mDirState;

	if (mInDirLRState == IN_DIR_LEFT) {
		newMS = RUN;
		newDS = LEFT;
	} else if (mInDirLRState == IN_DIR_RIGHT) {
		newMS = RUN;
		newDS = RIGHT;
	} else {
		newMS = IDLE;
	}

	if (newMS == RUN) {
		mHero->run(newDS);
	} else if (newMS == IDLE) {
		if (mHero->mState != IDLE) {
			mHero->idle();
		}
	}

	if (mHero->mDirState != newDS) {
		if (newDS == LEFT) {
			mHero->turnLeft();
		} else {
			mHero->turnRight();
		}
	}

	if (mInDirUDState == IN_DIR_UP) {
		mHero->jump();
	}

	if (mInBtnState == IN_BTN_PRESSED && 
		mHero->getWeaponState() == WEAPON_READY) {
		Bullet* bullet = mHero->attack();
		mBullets.push_back(bullet);
	}
}

void GameLayer::spawnUpdate(float dt)
{
	//TODO : ~~
	static float timepast = 0.0f;

	timepast += dt;
	if (timepast > ENEMY_SPAWN_INTERVAL) {
		timepast = 0.0f;

		Enemy* carrot = new Carrot();
		int spawnX = 0;
		if ((rand()%2) == 0) {
			spawnX = mGameMap->getWalkableLeft();
		} else {
			spawnX = mGameMap->getWalkableRight();
		}
		carrot->setPosition(ccp(spawnX, mGameMap->getWalkableBottom()));
		this->addChild(carrot, ENEMY_Z_INDEX_GAME_LAYER);
		mEnemies.push_back(carrot);
	}
}

void GameLayer::bulletUpdate(float dt)
{
	list<Bullet*>::iterator bulletItr;
	for (bulletItr = mBullets.begin(); bulletItr != mBullets.end(); ) {
		(*bulletItr)->update(dt);

		list<Enemy*>::iterator enemyItr;
		for (enemyItr = mEnemies.begin(); enemyItr != mEnemies.end(); ++enemyItr) {
			if ((*enemyItr)->mState == E_DIE) {
				continue;
			} else if ((*bulletItr)->mCollisionRect.intersectsRect((*enemyItr)->mHitBox.actual)) {
				(*enemyItr)->hurt((*bulletItr)->getAP(), 10);

				///////////
				char str[64] = {0};
				//CCArray* animFrames = new CCArray(30);
				CCAnimation* smokeAnim = CCAnimation::create();
				CCSpriteFrameCache* cache = 
					CCSpriteFrameCache::sharedSpriteFrameCache();
				for (int i = 1; i <= 11; i++) {
					sprintf(str, "smoke (%d).png", i);
					CCSpriteFrame* frame = 
						cache->spriteFrameByName(str);
					smokeAnim->addSpriteFrame(frame);
					//animFrames->addObject(frame);
				}
				smokeAnim->setDelayPerUnit(0.02f);
				smokeAnim->setRestoreOriginalFrame(true);
				//CCAnimation* smokeAnim = CCAnimation::create(animFrames, 0.02f);
				CCAnimate* smokeAnimate = CCAnimate::create(smokeAnim);
				CCSprite* smoke = CCSprite::createWithSpriteFrameName("smoke (1).png");
				CCCallFuncN* removeAction = 
					CCCallFuncN::create(this, callfuncN_selector(GameLayer::removeSmoke)) ;
				smoke->runAction(CCSequence::create(smokeAnimate, CCHide::create(), removeAction, NULL));
				smoke->setPositionX((*enemyItr)->getPositionX());
				smoke->setPositionY((*enemyItr)->getPositionY() + 30);
				this->addChild(smoke, 20);
				///////////

				if ((*bulletItr)->isMultiAttack() == false) {
					(*bulletItr)->setDie();
					break;
				}
			}
			if ((*bulletItr)->isMultiAttack()) {
				(*bulletItr)->setDie();
			}
		}

		if ((*bulletItr)->isDie() == true) {
			delete *bulletItr;
			bulletItr = mBullets.erase(bulletItr);
		} else {
			++bulletItr;
		}
	}
}

void GameLayer::enemyUpdate(float dt)
{
	list<Enemy*>::iterator enemyItr;
	for (enemyItr = mEnemies.begin(); enemyItr != mEnemies.end(); ) {
		if ((*enemyItr)->mState == E_DIE) {
			if ((*enemyItr)->isVisible() == false) {
				(*enemyItr)->stopAllActions();
				(*enemyItr)->removeFromParentAndCleanup(true);
				delete *enemyItr;
				enemyItr = mEnemies.erase(enemyItr);
			} else {
				++enemyItr;
			}
			continue;
		} else if (mHero->mHitBox.actual.intersectsRect((*enemyItr)->mAttackBox.actual)) {
			(*enemyItr)->attack();
			//TODO : Do harm to the hero~~~
			//(should have a time value)
		} else {
			if (mHero->getPositionX() < (*enemyItr)->getPositionX()) {
				(*enemyItr)->run(LEFT);
			} else {
				(*enemyItr)->run(RIGHT);
			}
		}

		++enemyItr;
	}
}

void GameLayer::viewpointUpdate()
{
	if (mHero->getPositionX() < (mGameMap->getWalkableRight() - WIN_WIDTH/2)
		&& mHero->getPositionX() > (mGameMap->getWalkableLeft() + WIN_WIDTH/2)) {
		float offsetx = mHero->getPositionX() - HERO_POS_X;
		this->setPositionX(-offsetx);
	}
}

void GameLayer::removeSmoke(CCNode* node)
{
	node->stopAllActions();
	node->removeFromParentAndCleanup(true);
}