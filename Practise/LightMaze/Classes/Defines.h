#pragma once

//////////////////////////////////////////////////////////////////////////
// ��ҵ�ȫ������
struct STUserGlobalData 
{
    int nScore;
};


//////////////////////////////////////////////////////////////////////////
// �������
enum EnGameSceneType
{
    enGameScene_Invalid = -1,   // ��Ч

    enGameScene_Login = 0,	    // ��½��
    enGameScene_Select,		    // �ؿ�ѡ���
    enGameScene_World,          // ��Ϸ�����

    enGameScene_Count,
};

//////////////////////////////////////////////////////////////////////////
// ���߶���
#define BLOCK_SLOT_NAME     "block-drop"
#define BLOCK_SLOT_WIDTH    58
#define BLOCK_SLOT_HEIGHT   58
#define BLOCK_SLOT_OFFSET   6

// �������Ͷ���
enum EnBlockType
{
    enBlockType_Null = 0,
    enBlockType_Mirror = 1, // ������
    enBlockType_Glass,      // �����䣬�ɷ���
    enBlockType_Stop,       // Stop
    
};

struct STBlockType 
{
    EnBlockType enType;     // Block��ID
    char    chName[32];     // ����
};

static STBlockType g_stBlockType[] = 
{
    {enBlockType_Null,      "null"},
    {enBlockType_Mirror,    "mirror"},
    {enBlockType_Glass,     "glass"},
    {enBlockType_Stop,      "stop"},
};

// ����״̬����
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

// ����
enum EnPosDir
{
    enPosDir_Left = 0,
    enPosDir_Top,
    enPosDir_Right,
    enPosDir_Bottom
};

// ���߳���
enum EnLineDir
{
    enLineDir_LeftTop,  // �����ϳ���
    enLineDir_RightTop,
    enLineDir_RightBottom,
    enLineDir_LeftBottom,
};

// ��ͼ�ĵ�Ԫ��
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

// �ؿ�����
struct STLevelConfig 
{
	char	chLevel;

    char    chCol;      // ����
    char    chRow;      // ����
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

// ��ͼ����
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