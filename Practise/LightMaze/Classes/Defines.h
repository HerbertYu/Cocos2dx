#pragma once

//////////////////////////////////////////////////////////////////////////
// 玩家的全局数据
struct STUserGlobalData 
{
    int nScore;
};


//////////////////////////////////////////////////////////////////////////
// 场景相关
enum EnGameSceneType
{
    enGameScene_Invalid = -1,   // 无效

    enGameScene_Login = 0,	    // 登陆层
    enGameScene_Select,		    // 关卡选择层
    enGameScene_World,          // 游戏世界层

    enGameScene_Count,
};

//////////////////////////////////////////////////////////////////////////
// 道具定义
#define BLOCK_SLOT_NAME     "block-drop"
#define BLOCK_SLOT_WIDTH    58
#define BLOCK_SLOT_HEIGHT   58
#define BLOCK_SLOT_OFFSET   6

// 道具类型定义
enum EnBlockType
{
    enBlockType_Null = 0,
    enBlockType_Mirror = 1, // 仅反射
    enBlockType_Glass,      // 可折射，可反射
    enBlockType_Stop,       // Stop
    
};

struct STBlockType 
{
    EnBlockType enType;     // Block的ID
    char    chName[32];     // 名称
};

static STBlockType g_stBlockType[] = 
{
    {enBlockType_Null,      "null"},
    {enBlockType_Mirror,    "mirror"},
    {enBlockType_Glass,     "glass"},
    {enBlockType_Stop,      "stop"},
};

// 道具状态定义
enum EnBlockState
{
    enBlockState_Normal = 0,
    enBlockState_Select,
    enBlockState_Fixed,
    enBlockState_Hint,
    enBlockState_Place,
};
struct STBlockState 
{
    EnBlockState chState;
    char chName[16];
};
static STBlockState g_stBlockState[] =
{
    {enBlockState_Normal,   "normal"},
    {enBlockState_Select,   "selected"},
    {enBlockState_Fixed,    "fixed"},
    {enBlockState_Hint,     "hint"},
    {enBlockState_Place,    "place"},
};

// 方向
enum EnPosDir
{
    enPosDir_Left = 0,
    enPosDir_Top,
    enPosDir_Right,
    enPosDir_Bottom
};

// 射线朝向
enum EnLineDir
{
    enLineDir_LeftTop,  // 从左上出发
    enLineDir_RightTop,
    enLineDir_RightBottom,
    enLineDir_LeftBottom,
};

// 地图的单元格
struct STMapCell 
{
    bool    bShow;
    char    enBlockType;
    char    enBlockState;

	STMapCell()
	{
		bShow = true;
		enBlockType = enBlockType_Null;
		enBlockState = enBlockState_Normal;
	}
};
struct STLinePos 
{
    char chCol;
    char chRow;
    char enPosDir;
    char enLineDir;

    STLinePos()
    {
        memset(this, 0, sizeof(*this));
    }

    bool BeEqual(const STLinePos& other)
    {
        return chCol == other.chCol && chRow == other.chRow &&
            enPosDir == other.enPosDir && enLineDir == other.enLineDir;
    }
};

// 关卡配置
struct STLevelConfig 
{
	char	chLevel;

    char    chCol;      // 行数
    char    chRow;      // 列数
    char    chSourceNum;
    char    chTargetNum;
    
    STMapCell* pMapCell;
    STLinePos* pSourcePos;
    STLinePos* pTargetPos;

	STLevelConfig()
	{
		memset(this, 0, sizeof(*this));
	}

    ~STLevelConfig()
    {
        CC_SAFE_DELETE_ARRAY(pMapCell);
        CC_SAFE_DELETE_ARRAY(pSourcePos);
        CC_SAFE_DELETE_ARRAY(pTargetPos);
    }
};

// 地图配置
#define MAP_NAME_MAX_LEN    32
struct STMapConfig
{
    char chMapID;
    char szName[MAP_NAME_MAX_LEN];
    
    char chLevel;
    STLevelConfig* pLevelConfig;

    STMapConfig()
    {
        memset(this, 0, sizeof(*this));
    }

    ~STMapConfig()
    {
        CC_SAFE_DELETE_ARRAY(pLevelConfig);
    }

    STLevelConfig* GetLevelConfig(char chLevelID)
    {
        if (chLevelID <= 0 || chLevelID > chLevel)
        {
            return nullptr;
        }
        return &(pLevelConfig[chLevelID-1]);
    }

    STLevelConfig* GetPreLevelConfig(char chLevelID)
    {
        return GetLevelConfig(chLevelID-1);
    }

    STLevelConfig* GetNextLevelConfig(char chLevelID)
    {
        return GetLevelConfig(chLevelID+1);
    }
};