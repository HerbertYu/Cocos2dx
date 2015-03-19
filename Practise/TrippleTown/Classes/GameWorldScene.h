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

	// ˢ��û����Ʒ�ĸ���
	void UpdateSpriteNoItem(int x, int y);
	// ˢ������Ʒ�ĸ���
	void UpdateSpriteWithItem(int x, int y);
	// ��ȡĳ��û����Ʒ�ĸ���Ҫ��ʾ��ͼƬ
	char* GetEmptyCellImge(IN int x, IN int y, OUT int& nDegree);

	void OnSelectCell(int x, int y);
	bool CanSelectCell(int x, int y);
	
	// �����ԣ�x��y��Ϊ������Χ��enItemID��ͬ����Ʒ����
	// nIgnoreType��0��ʾ�������ԣ�1��ʾ������2�����ϣ�3�����ң�4������
	void CalcSameItemNum(int x, int y, EnItemID enItemID, int nIgnoreType, std::vector<std::pair<int,int>>& vec);

	// �ϳ�
	void FixItem(int x, int y);
    // ���һ����Ʒ
    EnItemID RandItem();

protected:
	int m_nCurScore;	// ��ҵ�ǰ�÷�
	int m_nMaxScore;	// ���Ŀ��÷�
	STUserData* m_pUserData;
	EnItemID m_enCurItemID;

	CCLabelBMFont* m_pCurScore;
	CCLabelBMFont* m_pMaxScore;
	CCLabelBMFont* m_pMoney;
	CCSprite* m_pCurItem;
	CMapLayer* m_pMapLayer;

	struct STMapCellData 
	{
		EnItemID	enItemID;		// ��Ʒ���

		
		CCSprite*	pSpriteItem;	// ����ͼƬ
		CCSprite*	pSpriteFix[4];	// �ر�����
		CCSprite*	pSpriteFloor;	// �ر�
	};
	STMapCellData m_vecCells[MAP_HEIGHT_CELL][MAP_WIDTH_CELL];
};
