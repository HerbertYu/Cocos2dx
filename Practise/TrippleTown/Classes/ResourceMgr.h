//////////////////////////////////////////////////////////////////////////
// ��Դ����
// ��Ҫ����ͼƬ��Դ�����ƵĹ�����cocos����û�У���û���忼��
// ÿ��Ӧ�ó�������һ��resource.xml��Ȼ�������������xml�ļ����м�����Դ
#pragma once
#include <map>
#include "Define.h"

// xml�������
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
	// ͼƬ����
	struct STImageItem 
	{
		char	szName[32];		// ͼƬ����
		int		nPosX;			// ���Ͻ�X����
		int		nPosY;			// ���Ͻ�Y���꣬������Ļ����ϵ
		int		nWidth;			// ��
		int		nHeight;		// ��

		STImageItem()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	// ͼƬ����
	struct STImageSet 
	{
		char	szName[32];		// ����
		char	szFile[32];		// �ļ�
		CCTexture2D* pTexture;	// ����ָ��

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