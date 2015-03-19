#include <cocos2d.h>
#include "GameSceneBase.h"

class CGameWorldScene;
class CMapLayer : public CCLayer
{
public:
	void Init(CGameWorldScene* pParent);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
//	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

private:
	CGameWorldScene* m_pParentScene;
};

class CGameWorldScene : public CGameSceneBase
{
public:
	friend class CMapLayer;

    CGameWorldScene();
    virtual ~CGameWorldScene();

    virtual bool Init();
	virtual bool UnInit();
    virtual bool OnEnter();
    virtual bool OnLeave();

protected:
	void InitData();
	void UpdateCurScore();
	void UpdateMoney();
	void UpdateMapLayer();

	// 刷新没有物品的格子
	void UpdateSpriteNoItem(int x, int y);
	// 刷新有物品的格子
	void UpdateSpriteWithItem(int x, int y);
	// 获取某个没有物品的格子要显示的图片
	char* GetEmptyCellImge(IN int x, IN int y, OUT int& nDegree);

	void OnSelectCell(int x, int y);
	bool CanSelectCell(int x, int y);
	
	// 计算以（x，y）为中心周围和enItemID相同的物品数量
	// nIgnoreType：0表示都不忽略，1表示忽略左，2忽略上，3忽略右，4忽略下
	void CalcSameItemNum(int x, int y, EnItemID enItemID, int nIgnoreType, std::vector<std::pair<int,int>>& vec);

	// 合成
	void FixItem(int x, int y);
    // 随机一个物品
    EnItemID RandItem();

protected:
	int m_nCurScore;	// 玩家当前得分
	int m_nMaxScore;	// 玩家目标得分
	STUserData* m_pUserData;
	EnItemID m_enCurItemID;

	CCLabelBMFont* m_pCurScore;
	CCLabelBMFont* m_pMaxScore;
	CCLabelBMFont* m_pMoney;
	CCSprite* m_pCurItem;
	CMapLayer* m_pMapLayer;

	struct STMapCellData 
	{
		EnItemID	enItemID;		// 物品编号

		
		CCSprite*	pSpriteItem;	// 道具图片
		CCSprite*	pSpriteFix[4];	// 地表修正
		CCSprite*	pSpriteFloor;	// 地表
	};
	STMapCellData m_vecCells[MAP_HEIGHT_CELL][MAP_WIDTH_CELL];
};
