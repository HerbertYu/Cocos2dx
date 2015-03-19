#include "LevelSelectLayer.h"
#include "MapConfigMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "WorldScene.h"

#define CELL_WIDTH      60      // 单元格宽度
#define CELL_HEIGHT     60      // 单元格高度
#define CELL_OFFSET     8       // 单元格间距
#define OFFSET_LEFT     30      // 左边距
#define OFFSET_RIGHT    30      // 右边距
#define OFFSET_TOP      80      // 上边距
#define OFFSET_BOTTOM   30      // 下边距

CLevelSelectCell::CLevelSelectCell() : m_nMapID(0)
{
}

CLevelSelectCell::~CLevelSelectCell()
{
}
    
bool CLevelSelectCell::Init(CSelectScene* pScene, char nMapID)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(pScene == nullptr);
        m_pSelectScene = pScene;
        STMapConfig* pMapConfig = CMapConfigMgr::getSingleton().GetMapConfig(nMapID);
        CC_BREAK_IF(pMapConfig == nullptr);
        m_nMapID = nMapID;

        CCSize stWndSize = Director::getInstance()->getWinSize();

        // 名称
        CCLabelTTF* pMapName = CCLabelTTF::create(pMapConfig->szName, "Verdana", 20.0);
        CC_BREAK_IF(pMapName == nullptr);
        pMapName->setPosition(ccp(stWndSize.width/2, stWndSize.height-22));
        addChild(pMapName, 2);

        // 遮罩
        CCSprite* pBackImg = CResourceMgr::getSingleton().GetSprite("foreground");
		CC_BREAK_IF(pBackImg == nullptr);
		CCRect rc = pBackImg->getTextureRect();
		pBackImg->setScaleX((stWndSize.width-OFFSET_LEFT-OFFSET_RIGHT)/rc.size.width);
		pBackImg->setScaleY((stWndSize.height-OFFSET_TOP-OFFSET_BOTTOM)/rc.size.height);
		pBackImg->setPosition(ccp(stWndSize.width/2, stWndSize.height/2));
		addChild(pBackImg, 1);

        char chRow = (stWndSize.width - OFFSET_LEFT - OFFSET_RIGHT) / CELL_WIDTH;
        float fX = OFFSET_LEFT + CELL_WIDTH/2;
        float fY = stWndSize.height - OFFSET_TOP - CELL_HEIGHT/2;
        char szText[8];
		CCMenu* pShowMenu = CCMenu::create();
		pShowMenu->setPosition(ccp(0, 0));
		addChild(pShowMenu, 2);
        for (char i = 0; i < pMapConfig->chLevel; i++)
        {
			MenuItemImage* pCell = MenuItemImage::create("", "",
                CC_CALLBACK_1(CLevelSelectCell::OnLevelSelect, this));
            pCell->setNormalSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("block-crystal-shadow"));
            pCell->setDisabledSpriteFrame(CResourceMgr::getSingleton().GetSpriteFrame("button-stop-disabled"));
            pCell->setPosition(ccp(fX, fY));
            pCell->setTag(i+1);
			pCell->setScale(0.8f);
			pShowMenu->addChild(pCell);

			sprintf(szText, "%d", i+1);
            CCLabelTTF* pText = CCLabelTTF::create(szText, "Verdana", 20.0);
            pText->setPosition(ccp(fX, fY));
            addChild(pText, 3);

            if ((i+1) % chRow == 0)
            {
                fX = OFFSET_LEFT + CELL_WIDTH/2;
                fY -= CELL_HEIGHT + CELL_OFFSET;
            } 
            else
            {
                fX += CELL_WIDTH + CELL_OFFSET;
            }
        }

        bRet = true;
    } while(0);
    return bRet;
}

void CLevelSelectCell::OnLevelSelect(CCObject* pSender)
{
    CCMenuItemImage* pCell = (CCMenuItemImage*)pSender;
    int nLevel = pCell->getTag();
    CSceneMgr::getSingleton().GotoScene(enGameScene_World);
    CWorldScene* pWorld = (CWorldScene*)(CSceneMgr::getSingleton().GetCurrentScene());
    pWorld->LoadMap(m_nMapID, nLevel);
}