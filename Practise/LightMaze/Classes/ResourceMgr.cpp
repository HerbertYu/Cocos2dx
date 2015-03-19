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
	std::vector<std::string> paths;
	paths.push_back("Data");
	FileUtils::getInstance()->setSearchPaths(paths);

    // ����ͼƬ����*.plist�ļ���ʽ
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCAnimationCache *pAnimCache = CCAnimationCache::sharedAnimationCache();

    bool bResult = false;
    xmlDocPtr pXmlDoc = nullptr;
    do
    {
		auto s = FileUtils::getInstance()->fullPathForFilename("resource.xml");
		const char* pszFullPath = s.c_str();
        pXmlDoc = xmlReadFile(pszFullPath, "utf-8", XML_PARSE_RECOVER);
        xmlNodePtr pRootNode = xmlDocGetRootElement(pXmlDoc);
        CC_BREAK_IF(pRootNode == nullptr);
        CC_BREAK_IF(xmlStrcmp(pRootNode->name, BAD_CAST "Resource") != 0);

        for (xmlNodePtr pCurNode = pRootNode->children; pCurNode != nullptr; pCurNode = pCurNode->next)
        {
            CC_CONTINUE_IF(pCurNode->type != xmlElementType::XML_ELEMENT_NODE || pCurNode->properties == nullptr);
            CC_CONTINUE_IF(pCurNode->properties->children == nullptr || pCurNode->properties->children->content == nullptr);
            if (xmlStrcmp(pCurNode->name, BAD_CAST "Imageset") == 0)
            {
                // ͼƬ��
                CC_CONTINUE_IF(xmlStrcmp(pCurNode->properties->name, BAD_CAST "file") != 0);
                pFrameCache->addSpriteFramesWithFile((const char*)(pCurNode->properties->children->content));
            }
            else if (xmlStrcmp(pCurNode->name, BAD_CAST "Animations") == 0)
            {
                // ������
                CC_CONTINUE_IF(xmlStrcmp(pCurNode->properties->name, BAD_CAST "file") != 0);
                pAnimCache->addAnimationsWithFile((const char*)(pCurNode->properties->children->content));
            }
            else if (xmlStrcmp(pCurNode->name, BAD_CAST "CustomAnim") == 0)
            {
                // �Զ��嶯��
                // ��������
                xmlAttr* pAttr = pCurNode->properties;
                char szName[32] = {0};
                strcpy_s(szName, (const char*)(pAttr->children->content));

                // ��ͼƬ������
                pAttr = pAttr->next;
                CC_CONTINUE_IF(pAttr == nullptr || pAttr->children == nullptr || pAttr->children->content == nullptr);
                int nCol = atoi((const char*)(pAttr->children->content));
                if (nCol < 0)
                {
                    nCol = 1;
                }
                
                // ��ͼƬ������
                pAttr = pAttr->next;
                CC_CONTINUE_IF(pAttr == nullptr || pAttr->children == nullptr || pAttr->children->content == nullptr);
                int nRow = atoi((const char*)(pAttr->children->content));
                if (nRow < 0)
                {
                    nRow = 1;
                }

                // �ӳ�
                pAttr = pAttr->next;
                CC_CONTINUE_IF(pAttr == nullptr || pAttr->children == nullptr || pAttr->children->content == nullptr);
                float fDelay = atof((const char*)(pAttr->children->content));

                // ѭ������
                pAttr = pAttr->next;
                CC_CONTINUE_IF(pAttr == nullptr || pAttr->children == nullptr || pAttr->children->content == nullptr);
                unsigned int nLoop = atoi((const char*)(pAttr->children->content));

                // ͼƬ�ļ�
                pAttr = pAttr->next;
                CC_CONTINUE_IF(pAttr == nullptr || pAttr->children == nullptr || pAttr->children->content == nullptr);
                CCImage* pImage = new CCImage();
                CC_BREAK_IF(!pImage->initWithImageFile((const char*)(pAttr->children->content)));
                CCTexture2D* pTexture = new CCTexture2D();
                CC_BREAK_IF(!pTexture->initWithImage(pImage));
                pImage->release();

                Vector<SpriteFrame*> array;
                int nWidth = pTexture->getPixelsWide() / nCol;
                int nHeight = pTexture->getPixelsHigh() / nRow;
                for (int i = 0; i < nRow; i++)
                {
                    for (int j = 0; j < nCol; j++)
                    {
                        CCSpriteFrame* pFrame = CCSpriteFrame::createWithTexture(pTexture,
                            CCRectMake(j*nWidth, i*nHeight, nWidth, nHeight));
                        array.pushBack(pFrame);
                    }
                }
                CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(array, fDelay);
                pAnimation->setLoops(nLoop);
                pAnimCache->addAnimation(pAnimation, szName);
            }
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
	return true;
}

CCSprite* CResourceMgr::GetSprite(const char* pszName)
{
    if (pszName == nullptr)
    {
        return nullptr;
    }

    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::getInstance();
	return CCSprite::createWithSpriteFrame(pFrameCache->getSpriteFrameByName(pszName));
}

CCSpriteFrame* CResourceMgr::GetSpriteFrame(const char* pszName)
{
    if (pszName == nullptr)
    {
        return nullptr;
    }

    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    return pFrameCache->spriteFrameByName(pszName);
}

CCAnimation* CResourceMgr::GetAnimation(const char* pszName)
{
    if (pszName == nullptr)
    {
        return nullptr;
    }
    CCAnimationCache *pAnimCache = CCAnimationCache::sharedAnimationCache();
    return pAnimCache->animationByName(pszName);
}