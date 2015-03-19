// 地图文件解析
#pragma once
#include "stdafx.h"
#include "singleton.h"

class CMapConfigMgr : public TSingleton<CMapConfigMgr>
{
public:
	CMapConfigMgr();
	~CMapConfigMgr();

	bool Init();
	bool UnInit();

	char GetMapCount(){return m_nMapCount;}
	STMapConfig* GetMapConfig(char nMapID);
    char GetNextMapID(char nMapID);

protected:
    bool LoadMapConfig(char nMapID, const char* pszFile);

private:
	char m_nMapCount;
	STMapConfig* m_pMapConfig;
};