#include "SceneGame.h"

const int GAME_LAYER_Z_INDEX = 10;
const int HUD_LAYER_Z_INDEX = 20;

SceneGame::SceneGame()
{
	mHudLayer = new HudLayer();
	mHudLayer->setPosition(ccp(0, 0));
	this->addChild(mHudLayer, HUD_LAYER_Z_INDEX);

	mGameLayer = new GameLayer();
	mGameLayer->setPosition(ccp(0, 0));
	this->addChild(mGameLayer, GAME_LAYER_Z_INDEX);

	this->schedule(schedule_selector(SceneGame::broadcastInput));

	init();
}

bool SceneGame::init()
{
	//CCScene::init();

	return true;
}

SceneGame::~SceneGame()
{
	delete mHudLayer;
	delete mGameLayer;
}

void SceneGame::broadcastInput(float dt)
{
	mGameLayer->setInDirLRState(mHudLayer->getInDirLRState());
	mGameLayer->setInDirUDState(mHudLayer->getInDirUDState());
	mGameLayer->setInBtnState(mHudLayer->getInBtnState());
}