#pragma once
#include <cocos2d.h>
#include <SimpleAudioEngine.h>
#include "singleton.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define PI (3.1415926f)

#define WND_SIZE_WIDTH		800
#define WND_SIZE_HEIGHT		480
#define FONT_DEFAULT_NAME	"微软雅黑"
#define FONT_DEFAULT_SIZE	32

enum EnGameSceneType
{
	enGameScene_Login,		// 登陆层
	enGameScene_World,		// 世界层
    enGameScene_Rank,       // 排行榜
    enGameScene_Setting,    // 设置

	enGameScene_Count,
};

// 人物数据
struct STUserData
{
	int nMoney;	// 当前金币
};


//////////////////////////////////////////////////////////////////////////
// 游戏阶段数值
struct STStageData 
{
	int nTargetScore;	// 目标积分
};

#define MAP_CELL_SIZE		50	// 图片大小
#define MAP_LAYER_POS_X		215
#define MAP_LAYER_POS_Y		460

#define MAP_WIDTH_CELL		11
#define MAP_HEIGHT_CELL		9

// 地面物品类型
enum EnItemID
{
	enItemID_Null				= 0, // 空地

	enItemID_Grass,         // 小草
	enItemID_Bush,          // 草丛
	enItemID_Tress,         // 树
	enItemID_Huts,          // 小屋
	enItemID_House,         // 大厦
	enItemID_Cottage,       // 别墅
	enItemID_City,          // 城堡
	enItemID_Tower,         // 塔
	enItemID_Palace,        // 宫殿

	enItemID_Bear,          // 小熊
    enItemID_GraveStone,    // 墓碑
	enItemID_Church,        // 教堂
	enItemID_Cathedral,     // 大教堂

	enItemID_Crystal,		// 水晶
	enItemID_Stone,         // 石头
	enItemID_BigStone,      // 大石头
		  
	enItemID_SilverBox,     // 银宝箱
	enItemID_GoldBox,       // 金宝箱

    enItemID_Robot,         // 机器人
};

struct STItemData 
{
	EnItemID	nItemID;	    // 物品ID
	char*		szImage;	    // 物品图片
    int         nRand;          // 随机出来的概率
    int         nCancelComsume; // 铲除物品需要扣除的费用
    int         nFixAward;      // 合成奖励
};

// 物品的基本数据，以物品ID为下标
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

// 物品合成表
struct STFixData 
{
	EnItemID	enSrcItemID;	// 源物品ID
	int			nSrcCount;		// 源物品个数
	EnItemID	enDstItemID;	// 目标物品ID
};
static STFixData g_arFixData[] = 
{
	{enItemID_Grass,	    3,	enItemID_Bush},			// 3小草 -> 1草丛
	{enItemID_Bush,		    3,	enItemID_Tress},		// 3草丛 -> 1大树
	{enItemID_Tress,	    3,	enItemID_Huts},			// 3大树 -> 1小屋
	{enItemID_Huts,		    3,	enItemID_House},		// 3小屋 -> 1大厦
	{enItemID_House,	    3,	enItemID_Cottage},		// 3大厦 -> 1别墅
	{enItemID_Cottage,	    3,	enItemID_City},			// 3别墅 -> 1城堡
	{enItemID_City,		    3,	enItemID_Tower},		// 3城堡 -> 1塔
	{enItemID_Tower,	    3,	enItemID_Palace},		// 3塔 -> 1宫殿

	{enItemID_GraveStone,	3,	enItemID_Church},		// 3熊 -> 1教堂
	{enItemID_Church,	    3,	enItemID_Cathedral},	// 3教堂 -> 1大教堂
	{enItemID_Stone,	    3,	enItemID_BigStone},		// 3石头 -> 1大石头
	{enItemID_BigStone,	    3,	enItemID_SilverBox},	// 3大石头 -> 1银宝箱
	{enItemID_SilverBox,    3,	enItemID_GoldBox},		// 3银宝箱 -> 1金宝箱
};