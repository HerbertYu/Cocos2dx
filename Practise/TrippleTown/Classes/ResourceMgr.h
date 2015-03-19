//////////////////////////////////////////////////////////////////////////
// 资源管理
// 主要管理图片资源，类似的功能在cocos中有没有，我没具体考察
// 每个应用程序，配置一个resource.xml，然后管理器将根据xml文件进行加载资源
#pragma once
#include <map>
#include "Define.h"

// xml解析相关
#include <libxml/parser.h>
class CResourceMgr : public TSingleton<CResourceMgr>
{
public:
	CResourceMgr();
	~CResourceMgr();

	bool Init();
	bool UnInit();

	bool InitSprite(CCSprite* pSprite, const char* pszImageSetName, const char* pszImageName);
	CCSprite* NewSprite(const char* pszImageSetName, const char* pszImageName);

private:
	bool ReadImageXml(const char* pszImageXml);


private:
	// 图片子项
	struct STImageItem 
	{
		char	szName[32];		// 图片名字
		int		nPosX;			// 左上角X坐标
		int		nPosY;			// 左上角Y坐标，采用屏幕坐标系
		int		nWidth;			// 宽
		int		nHeight;		// 高

		STImageItem()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	// 图片集合
	struct STImageSet 
	{
		char	szName[32];		// 名称
		char	szFile[32];		// 文件
		CCTexture2D* pTexture;	// 纹理指针

		std::map<std::string, STImageItem> mapItems;

		STImageSet()
		{
			memset(szName, 0, sizeof(szName));
			memset(szFile, 0, sizeof(szFile));
			pTexture = nullptr;
			mapItems.clear();
		}
	};

	std::map<std::string, STImageSet> m_mapImageSet;
};