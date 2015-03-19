#include <cocos2d.h>
#include "GameSceneBase.h"

class CSettingScene : public CGameSceneBase
{
public:
    CSettingScene();
    virtual ~CSettingScene();

    virtual bool Init();  
	virtual bool UnInit();
    virtual bool OnEnter();
    virtual bool OnLeave();
};
