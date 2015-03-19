#include "ResourceMgr.h"
#define CC_CONTINUE_IF(cond)	if(cond) continue

#pragma comment(lib, "libxml2")
CResourceMgr::CResourceMgr()
{

}

CResourceMgr::~CResourceMgr()
{

}

bool CResourceMgr::Init()
{
	m_mapImageSet.clear();

	bool bResult = false;
	xmlDocPtr pXmlDoc = nullptr;
	do
	{
// 		FileUtils::getInstance()->addSearchPath("Data");
		std::string path = FileUtils::getInstance()->fullPathForFilename("resource.xml");
		pXmlDoc = xmlReadFile(path.c_str(), "utf-8", XML_PARSE_RECOVER);
		xmlNodePtr pRootNode = xmlDocGetRootElement(pXmlDoc);
		CC_BREAK_IF(pRootNode == nullptr);
		CC_BREAK_IF(xmlStrcmp(pRootNode->name, BAD_CAST "Resource") != 0);

		for (xmlNodePtr pCurNode = pRootNode->children; pCurNode != nullptr; pCurNode = pCurNode->next)
		{
			CC_CONTINUE_IF (pCurNode->type != xmlElementType::XML_ELEMENT_NODE ||
				pCurNode->properties == nullptr ||
				xmlStrcmp(pCurNode->name, BAD_CAST "Image") != 0);
			CC_CONTINUE_IF (pCurNode->properties->children == nullptr ||
				pCurNode->properties->children->content == nullptr ||
				xmlStrcmp(pCurNode->properties->name, BAD_CAST "name") != 0);
			CC_CONTINUE_IF (!ReadImageXml((const char*)(pCurNode->properties->children->content)));
		}

		bResult = true;
	} while(0);
	if (pXmlDoc != nullptr)
	{
		xmlFreeDoc(pXmlDoc);
	}
	return bResult;
}

bool CResourceMgr::UnInit()
{
	for (auto it = m_mapImageSet.begin(); it != m_mapImageSet.end(); it++)
	{
		if ((*it).second.pTexture != nullptr)
		{
			(*it).second.pTexture->release();
		}
	}
	m_mapImageSet.clear();
	return true;
}

bool CResourceMgr::ReadImageXml(const char* pszImageXml)
{
	if (pszImageXml == nullptr)
	{
		return false;
	}

	bool bResult = false;
	xmlDocPtr pXmlDoc = nullptr;
	do
	{
// 		FileUtils::getInstance()->addSearchPath("Data");
		std::string path = FileUtils::getInstance()->fullPathForFilename(pszImageXml);
		pXmlDoc = xmlReadFile(path.c_str(), "utf-8", XML_PARSE_RECOVER);
		xmlNodePtr pRootNode = xmlDocGetRootElement(pXmlDoc);
		CC_BREAK_IF(pRootNode == nullptr);
		CC_BREAK_IF(xmlStrcmp(pRootNode->name, BAD_CAST "Imageset") != 0);
		
		// Name
		xmlAttrPtr pAttr = pRootNode->properties;
		CC_BREAK_IF(pAttr == nullptr);
		CC_BREAK_IF(xmlStrcmp(pAttr->name, BAD_CAST "Name")!= 0);
		CC_BREAK_IF(pAttr->children == nullptr);
		CC_BREAK_IF(pAttr->children->content == nullptr);

		STImageSet imageSet;
		strcpy_s(imageSet.szName, sizeof(imageSet.szName), (const char*)(pAttr->children->content));

		// Imagefile
		pAttr = pAttr->next;
		CC_BREAK_IF(pAttr == nullptr);
		CC_BREAK_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "Imagefile") != 0);
		CC_BREAK_IF(pAttr->children->content == nullptr);
		strcpy_s(imageSet.szFile, sizeof(imageSet.szFile), (const char*)(pAttr->children->content));
		CCImage* pImage = new CCImage();
		CC_BREAK_IF(!pImage->initWithImageFile((const char*)(pAttr->children->content)));
		imageSet.pTexture = new CCTexture2D();
		CC_BREAK_IF(!imageSet.pTexture->initWithImage(pImage));
		pImage->release();

		if (pRootNode->children != nullptr && pRootNode->children->next != nullptr)
		{
			// Image
			STImageItem item;
			for (xmlNodePtr pCurNode = pRootNode->children->next; pCurNode != nullptr; pCurNode = pCurNode->next)
			{
				CC_CONTINUE_IF (pCurNode->type != xmlElementType::XML_ELEMENT_NODE ||
					xmlStrcmp(pCurNode->name, BAD_CAST "Image") != 0);

				// Name
				xmlAttrPtr pAttr = pCurNode->properties;
				CC_CONTINUE_IF(pAttr == nullptr);
				CC_CONTINUE_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "Name") != 0);
				CC_CONTINUE_IF(pAttr->children->content == nullptr);
				strcpy_s(item.szName, sizeof(item.szName), (const char*)(pAttr->children->content));

				// XPos
				pAttr = pAttr->next;
				CC_CONTINUE_IF(pAttr == nullptr);
				CC_CONTINUE_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "XPos") != 0);
				CC_CONTINUE_IF(pAttr->children->content == nullptr);
				item.nPosX = atoi((const char*)(pAttr->children->content));

				// YPos
				pAttr = pAttr->next;
				CC_CONTINUE_IF(pAttr == nullptr);
				CC_CONTINUE_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "YPos") != 0);
				CC_CONTINUE_IF(pAttr->children->content == nullptr);
				item.nPosY = atoi((const char*)(pAttr->children->content));

				// Width
				pAttr = pAttr->next;
				CC_CONTINUE_IF(pAttr == nullptr);
				CC_CONTINUE_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "Width") != 0);
				CC_CONTINUE_IF(pAttr->children->content == nullptr);
				item.nWidth = atoi((const char*)(pAttr->children->content));

				// Height
				pAttr = pAttr->next;
				CC_CONTINUE_IF(pAttr == nullptr);
				CC_CONTINUE_IF(pAttr->children == nullptr || xmlStrcmp(pAttr->name, BAD_CAST "Height") != 0);
				CC_CONTINUE_IF(pAttr->children->content == nullptr);
				item.nHeight = atoi((const char*)(pAttr->children->content));

				imageSet.mapItems.insert(std::make_pair(item.szName, item));
			}
		}

		m_mapImageSet.insert(std::make_pair(imageSet.szName, imageSet));
		bResult = true;
	} while (0);
	return bResult;
}

bool CResourceMgr::InitSprite(CCSprite* pSprite, const char* pszImageSetName, const char* pszImageName)
{
	bool bResult = false;
	do 
	{
		CC_BREAK_IF(pSprite == nullptr || pszImageSetName == nullptr || pszImageName == nullptr);
		auto it = m_mapImageSet.find(pszImageSetName);
		CC_BREAK_IF(it == m_mapImageSet.end());
		CC_BREAK_IF((*it).second.pTexture == nullptr);
		auto iter = (*it).second.mapItems.find(pszImageName);
		CC_BREAK_IF(iter == (*it).second.mapItems.end());
		pSprite->setTexture((*it).second.pTexture);
		pSprite->setTextureRect(CCRectMake((*iter).second.nPosX, (*iter).second.nPosY, (*iter).second.nWidth, (*iter).second.nHeight));
		bResult = true;
	} while (0);
	return bResult;
}

CCSprite* CResourceMgr::NewSprite(const char* pszImageSetName, const char* pszImageName)
{
	CCSprite* pSprite = nullptr;
	do 
	{
		CC_BREAK_IF(pszImageSetName == nullptr || pszImageName == nullptr);
		auto it = m_mapImageSet.find(pszImageSetName);
		CC_BREAK_IF(it == m_mapImageSet.end());
		CC_BREAK_IF((*it).second.pTexture == nullptr);
		auto iter = (*it).second.mapItems.find(pszImageName);
		CC_BREAK_IF(iter == (*it).second.mapItems.end());
// 		pSprite = CCSprite::create();
//		pSprite->autorelease();
		pSprite = Sprite::createWithTexture((*it).second.pTexture, 
			CCRectMake((*iter).second.nPosX, (*iter).second.nPosY, (*iter).second.nWidth, (*iter).second.nHeight));
	} while (0);
	return pSprite;
}