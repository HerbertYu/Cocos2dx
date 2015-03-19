#include <cocos2d.h>
#include "GameSceneBase.h"

class CRankScene : public CGameSceneBase
{
public:
    CRankScene();
    virtual ~CRankScene();

    virtual bool Init();  
	virtual bool UnInit();
    virtual bool OnEnter();
    virtual bool OnLeave();
};
