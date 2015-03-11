#include "MapConfigMgr.h"

CMapConfigMgr::CMapConfigMgr() : m_nMapCount(0), m_pMapConfig(nullptr)
{

}

CMapConfigMgr::~CMapConfigMgr()
{
	UnInit();
}

bool CMapConfigMgr::LoadMapConfig(char nMapID, const char* pszFile)
{
    bool bRet = false;
    do 
    {
        // 加载数据，为了方便编辑，采用文本文件模式，以后有空可以写关卡编辑器
        FILE* f = fopen(pszFile, "r");
        CC_BREAK_IF(f == nullptr);
        char szBuff[256] = {0};

        // 第一行表示地图名称
        STMapConfig* pMapConfig = GetMapConfig(nMapID);
        STLevelConfig* pLevelConfig = nullptr;

        // 地图名称
        fgets(pMapConfig->szName, MAP_NAME_MAX_LEN, f);
        for (int n = 0; n < MAP_NAME_MAX_LEN; n++)
        {
            if (pMapConfig->szName[n] == ' ' || pMapConfig->szName[n] == '#')
            {
                pMapConfig->szName[n] = '\0';
                break;
            }
        }

        // 关卡数量
        fgets(szBuff, 256, f);
        pMapConfig->chLevel = atoi(szBuff);
        if (pMapConfig->chLevel > 0)
        {
            pMapConfig->pLevelConfig = new STLevelConfig[pMapConfig->chLevel];
            for (char i = 0; i < pMapConfig->chLevel; i++)
            {
                pLevelConfig = pMapConfig->pLevelConfig + i;
                pLevelConfig->chLevel = i + 1;

                // 跳行
                fgets(szBuff, 256, f);

                // 行数
                fgets(szBuff, 256, f);
                pLevelConfig->chCol = atoi(szBuff);
                // 列数
                fgets(szBuff, 256, f);
                pLevelConfig->chRow = atoi(szBuff);
                pLevelConfig->pMapCell = new STMapCell[pLevelConfig->chCol * pLevelConfig->chRow];
                // 原点数
                fgets(szBuff, 256, f);
                pLevelConfig->chSourceNum = atoi(szBuff);
                pLevelConfig->pSourcePos = new STLinePos[pLevelConfig->chSourceNum];
                // 目标数
                fgets(szBuff, 256, f);
                pLevelConfig->chTargetNum = atoi(szBuff);
                pLevelConfig->pTargetPos = new STLinePos[pLevelConfig->chTargetNum];
                // 开始遍历单元格数据
                STMapCell* pMapCell = pLevelConfig->pMapCell;
                for (char xx = 0; xx < pLevelConfig->chCol; xx++)
                {
                    // 每个单元格占3个字符
                    fgets(szBuff, 256, f);
                    char chIdx = 0;
                    for (char yy = 0; yy < pLevelConfig->chRow; yy++)
                    {
                        // 格式"xxx"
                        pMapCell->bShow = (szBuff[chIdx++] == '1');
                        pMapCell->enBlockType = (szBuff[chIdx++] - '0');
                        pMapCell->enBlockState = (szBuff[chIdx++] - '0');
                        pMapCell++;
                        chIdx++; // 跳过空格
                    }
                }
                // 开始遍历原点数据
                for (char xx = 0; xx < pLevelConfig->chSourceNum; xx++)
                {
                    // 格式“x x x x”
                    fgets(szBuff, 256, f);
                    pLevelConfig->pSourcePos[xx].chCol = (szBuff[0] - '0');
                    pLevelConfig->pSourcePos[xx].chRow = (szBuff[2] - '0');
                    pLevelConfig->pSourcePos[xx].enPosDir = (szBuff[4] - '0');
                    pLevelConfig->pSourcePos[xx].enLineDir = (szBuff[6] - '0');
                }
                // 开始遍历目标点数据
                for (char xx = 0; xx < pLevelConfig->chTargetNum; xx++)
                {
                    // 格式“x x x x”
                    fgets(szBuff, 256, f);
                    pLevelConfig->pTargetPos[xx].chCol = (szBuff[0] - '0');
                    pLevelConfig->pTargetPos[xx].chRow = (szBuff[2] - '0');
                    pLevelConfig->pTargetPos[xx].enPosDir = (szBuff[4] - '0');
                    pLevelConfig->pTargetPos[xx].enLineDir = (szBuff[6] - '0');
                }
            }
        }

        fclose(f);
        bRet = true;
    } while (0);
    return bRet;
}

bool CMapConfigMgr::Init()
{
    bool bRet = false;
    do 
    {
        m_nMapCount = 3;
        m_pMapConfig = new STMapConfig[m_nMapCount];

        char szFileName[256] = {0};
        for (char i = 1; i <= m_nMapCount; i++)
        {
            sprintf(szFileName, "Data/map%02d.txt", i);
            m_pMapConfig[i-1].chMapID = i;
            bRet = LoadMapConfig(i, szFileName);
            if (!bRet)
            {
                return bRet;
            }
        }
        bRet = true;
    } while (0);
    return bRet;
}

bool CMapConfigMgr::UnInit()
{
	CC_SAFE_DELETE_ARRAY(m_pMapConfig);
    m_nMapCount = 0;
	return true;
}

STMapConfig* CMapConfigMgr::GetMapConfig(char nMapID)
{
	if (nMapID <= 0 || nMapID > m_nMapCount)
	{
		return nullptr;
	}

	return &(m_pMapConfig[nMapID-1]);
}

char CMapConfigMgr::GetNextMapID(char nMapID)
{
    if (nMapID < 1 || nMapID >= m_nMapCount)
    {
        return 0;
    }
    return nMapID+1;
}