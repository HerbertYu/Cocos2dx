#pragma once
#include <cocos2d.h>
#include <SimpleAudioEngine.h>
#include "singleton.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define PI (3.1415926f)

#define WND_SIZE_WIDTH		800
#define WND_SIZE_HEIGHT		480
#define FONT_DEFAULT_NAME	"΢���ź�"
#define FONT_DEFAULT_SIZE	32

enum EnGameSceneType
{
	enGameScene_Login,		// ��½��
	enGameScene_World,		// �����
    enGameScene_Rank,       // ���а�
    enGameScene_Setting,    // ����

	enGameScene_Count,
};

// ��������
struct STUserData
{
	int nMoney;	// ��ǰ���
};


//////////////////////////////////////////////////////////////////////////
// ��Ϸ�׶���ֵ
struct STStageData 
{
	int nTargetScore;	// Ŀ�����
};

#define MAP_CELL_SIZE		50	// ͼƬ��С
#define MAP_LAYER_POS_X		215
#define MAP_LAYER_POS_Y		460

#define MAP_WIDTH_CELL		11
#define MAP_HEIGHT_CELL		9

// ������Ʒ����
enum EnItemID
{
	enItemID_Null				= 0, // �յ�

	enItemID_Grass,         // С��
	enItemID_Bush,          // �ݴ�
	enItemID_Tress,         // ��
	enItemID_Huts,          // С��
	enItemID_House,         // ����
	enItemID_Cottage,       // ����
	enItemID_City,          // �Ǳ�
	enItemID_Tower,         // ��
	enItemID_Palace,        // ����

	enItemID_Bear,          // С��
    enItemID_GraveStone,    // Ĺ��
	enItemID_Church,        // ����
	enItemID_Cathedral,     // �����

	enItemID_Crystal,		// ˮ��
	enItemID_Stone,         // ʯͷ
	enItemID_BigStone,      // ��ʯͷ
		  
	enItemID_SilverBox,     // ������
	enItemID_GoldBox,       // ����

    enItemID_Robot,         // ������
};

struct STItemData 
{
	EnItemID	nItemID;	    // ��ƷID
	char*		szImage;	    // ��ƷͼƬ
    int         nRand;          // ��������ĸ���
    int         nCancelComsume; // ������Ʒ��Ҫ�۳��ķ���
    int         nFixAward;      // �ϳɽ���
};

// ��Ʒ�Ļ������ݣ�����ƷIDΪ�±�
static STItemData g_arItemData[] = 
{
	{enItemID_Null,			"",             0,  0,  0},

	{enItemID_Grass,		"grass",        60, 5,  10},
	{enItemID_Bush,			"bush",         20, 20, 40},
	{enItemID_Tress,		"tress",        5, 70, 140},
	{enItemID_Huts,			"huts",         4,  70, 140},
	{enItemID_House,		"house",        3,  70, 140},
	{enItemID_Cottage,		"cottage",      2,  70, 140},
	{enItemID_City,			"city",         0,  70, 140},
	{enItemID_Tower,		"tower",        0,  70, 140},
	{enItemID_Palace,		"palace",       0,  70, 140},
	{enItemID_Bear,			"bear",         0,  70, 140},
    {enItemID_GraveStone,   "gravestone",   0,  70, 140},
	{enItemID_Church,		"church",       0,  70, 140},
	{enItemID_Cathedral,	"cathedral",    0,  70, 140},
	{enItemID_Crystal,		"crystal",		6,	70,	140},
	{enItemID_Stone,		"stone",        0,  70, 140},
	{enItemID_BigStone,		"bigstone",     0,  70, 140},
	{enItemID_SilverBox,	"silverbox",    0,  70, 140},
	{enItemID_GoldBox,		"goldbox",      0,  70, 140},

};

// ��Ʒ�ϳɱ�
struct STFixData 
{
	EnItemID	enSrcItemID;	// Դ��ƷID
	int			nSrcCount;		// Դ��Ʒ����
	EnItemID	enDstItemID;	// Ŀ����ƷID
};
static STFixData g_arFixData[] = 
{
	{enItemID_Grass,	    3,	enItemID_Bush},			// 3С�� -> 1�ݴ�
	{enItemID_Bush,		    3,	enItemID_Tress},		// 3�ݴ� -> 1����
	{enItemID_Tress,	    3,	enItemID_Huts},			// 3���� -> 1С��
	{enItemID_Huts,		    3,	enItemID_House},		// 3С�� -> 1����
	{enItemID_House,	    3,	enItemID_Cottage},		// 3���� -> 1����
	{enItemID_Cottage,	    3,	enItemID_City},			// 3���� -> 1�Ǳ�
	{enItemID_City,		    3,	enItemID_Tower},		// 3�Ǳ� -> 1��
	{enItemID_Tower,	    3,	enItemID_Palace},		// 3�� -> 1����

	{enItemID_GraveStone,	3,	enItemID_Church},		// 3�� -> 1����
	{enItemID_Church,	    3,	enItemID_Cathedral},	// 3���� -> 1�����
	{enItemID_Stone,	    3,	enItemID_BigStone},		// 3ʯͷ -> 1��ʯͷ
	{enItemID_BigStone,	    3,	enItemID_SilverBox},	// 3��ʯͷ -> 1������
	{enItemID_SilverBox,    3,	enItemID_GoldBox},		// 3������ -> 1����
};