#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "HudLayer.h"
#include "GameLayer.h"

class SceneGame : public CCScene
{
public :
	SceneGame();
	bool init();
	~SceneGame();

private :
	HudLayer* mHudLayer;
	GameLayer* mGameLayer;

	void broadcastInput(float dt);
};

#endif