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
        // �������ݣ�Ϊ�˷���༭�������ı��ļ�ģʽ���Ժ��пտ���д�ؿ��༭��
        FILE* f = fopen(pszFile, "r");
        CC_BREAK_IF(f == nullptr);
        char szBuff[256] = {0};

        // ��һ�б�ʾ��ͼ����
        STMapConfig* pMapConfig = GetMapConfig(nMapID);
        STLevelConfig* pLevelConfig = nullptr;

        // ��ͼ����
        fgets(pMapConfig->szName, MAP_NAME_MAX_LEN, f);
        for (int n = 0; n < MAP_NAME_MAX_LEN; n++)
        {
            if (pMapConfig->szName[n] == ' ' || pMapConfig->szName[n] == '#')
            {
                pMapConfig->szName[n] = '\0';
                break;
            }
        }

        // �ؿ�����
        fgets(szBuff, 256, f);
        pMapConfig->chLevel = atoi(szBuff);
        if (pMapConfig->chLevel > 0)
        {
            pMapConfig->pLevelConfig = new STLevelConfig[pMapConfig->chLevel];
            for (char i = 0; i < pMapConfig->chLevel; i++)
            {
                pLevelConfig = pMapConfig->pLevelConfig + i;
                pLevelConfig->chLevel = i + 1;

                // ����
                fgets(szBuff, 256, f);

                // ����
                fgets(szBuff, 256, f);
                pLevelConfig->chCol = atoi(szBuff);
                // ����
                fgets(szBuff, 256, f);
                pLevelConfig->chRow = atoi(szBuff);
                pLevelConfig->pMapCell = new STMapCell[pLevelConfig->chCol * pLevelConfig->chRow];
                // ԭ����
                fgets(szBuff, 256, f);
                pLevelConfig->chSourceNum = atoi(szBuff);
                pLevelConfig->pSourcePos = new STLinePos[pLevelConfig->chSourceNum];
                // Ŀ����
                fgets(szBuff, 256, f);
                pLevelConfig->chTargetNum = atoi(szBuff);
                pLevelConfig->pTargetPos = new STLinePos[pLevelConfig->chTargetNum];
                // ��ʼ������Ԫ������
                STMapCell* pMapCell = pLevelConfig->pMapCell;
                for (char xx = 0; xx < pLevelConfig->chCol; xx++)
                {
                    // ÿ����Ԫ��ռ3���ַ�
                    fgets(szBuff, 256, f);
                    char chIdx = 0;
                    for (char yy = 0; yy < pLevelConfig->chRow; yy++)
                    {
                        // ��ʽ"xxx"
                        pMapCell->bShow = (szBuff[chIdx++] == '1');
                        pMapCell->enBlockType = (szBuff[chIdx++] - '0');
                        pMapCell->enBlockState = (szBuff[chIdx++] - '0');
                        pMapCell++;
                        chIdx++; // �����ո�
                    }
                }
                // ��ʼ����ԭ������
                for (char xx = 0; xx < pLevelConfig->chSourceNum; xx++)
                {
                    // ��ʽ��x x x x��
                    fgets(szBuff, 256, f);
                    pLevelConfig->pSourcePos[xx].chCol = (szBuff[0] - '0');
                    pLevelConfig->pSourcePos[xx].chRow = (szBuff[2] - '0');
                    pLevelConfig->pSourcePos[xx].enPosDir = (szBuff[4] - '0');
                    pLevelConfig->pSourcePos[xx].enLineDir = (szBuff[6] - '0');
                }
                // ��ʼ����Ŀ�������
                for (char xx = 0; xx < pLevelConfig->chTargetNum; xx++)
                {
                    // ��ʽ��x x x x��
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