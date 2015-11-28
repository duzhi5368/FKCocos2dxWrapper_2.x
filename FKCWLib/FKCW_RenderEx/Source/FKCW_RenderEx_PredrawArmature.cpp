//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_PredrawArmature.h"
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawArmature::FKCW_RenderEx_PredrawArmature()
	: m_pPreDrawFunction( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawArmature::~FKCW_RenderEx_PredrawArmature()
{
	CC_SAFE_RELEASE( m_pPreDrawFunction );
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawArmature* FKCW_RenderEx_PredrawArmature::Create()
{
	FKCW_RenderEx_PredrawArmature * pArmature = new FKCW_RenderEx_PredrawArmature();
	if(pArmature && pArmature->init())
	{
		pArmature->autorelease();
		return pArmature;
	}
	CC_SAFE_DELETE(pArmature);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawArmature* FKCW_RenderEx_PredrawArmature::Create(const char *name)
{
	FKCW_RenderEx_PredrawArmature * pArmature = new FKCW_RenderEx_PredrawArmature();
	if (pArmature && pArmature->init(name))
	{
		pArmature->autorelease();
		return pArmature;
	}
	CC_SAFE_DELETE(pArmature);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawArmature* FKCW_RenderEx_PredrawArmature::Create(const char *name, CCBone *parentBone)
{
	FKCW_RenderEx_PredrawArmature * pArmature = new FKCW_RenderEx_PredrawArmature();
	if (pArmature && pArmature->init(name, parentBone))
	{
		pArmature->autorelease();
		return pArmature;
	}
	CC_SAFE_DELETE(pArmature);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_PredrawArmature::draw()
{
	if(m_pPreDrawFunction)
		m_pPreDrawFunction->execute();

	if (m_pParentBone == NULL && m_pBatchNode == NULL)
	{
		CC_NODE_DRAW_SETUP();
		ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
	}

	CCObject *object = NULL;
	CCARRAY_FOREACH(m_pChildren, object)
	{
		if (CCBone *bone = dynamic_cast<CCBone *>(object))
		{
			CCNode *node = bone->getDisplayRenderNode();

			if (NULL == node)
				continue;

			switch (bone->getDisplayRenderNodeType())
			{
			case CS_DISPLAY_SPRITE:
				{
					CCSkin *skin = (CCSkin *)node;

					CCTextureAtlas *textureAtlas = skin->getTextureAtlas();
					bool blendDirty = bone->isBlendDirty();
					if(m_pAtlas != textureAtlas || blendDirty)
					{
						if (m_pAtlas)
						{
							m_pAtlas->drawQuads();
							m_pAtlas->removeAllQuads();
						}
					}

					m_pAtlas = textureAtlas;
					if (m_pAtlas->getCapacity() == m_pAtlas->getTotalQuads() && !m_pAtlas->resizeCapacity(m_pAtlas->getCapacity() * 2))
						return;

					skin->updateTransform();

					if (blendDirty)
					{
						ccBlendFunc func = bone->getBlendFunc();
						ccGLBlendFunc(func.src, func.dst);

						m_pAtlas->drawQuads();
						m_pAtlas->removeAllQuads();

						ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
						bone->setBlendDirty(false);
					}
				}
				break;
			case CS_DISPLAY_ARMATURE:
				{
					CCArmature *armature = (CCArmature *)(node);

					CCTextureAtlas *textureAtlas = armature->getTextureAtlas();
					if(m_pAtlas != textureAtlas)
					{
						if (m_pAtlas)
						{
							m_pAtlas->drawQuads();
							m_pAtlas->removeAllQuads();
						}
					}
					armature->draw();

					m_pAtlas = armature->getTextureAtlas();
				}
				break;
			default:
				{
					if (m_pAtlas)
					{
						m_pAtlas->drawQuads();
						m_pAtlas->removeAllQuads();
					}
					node->visit();

					CC_NODE_DRAW_SETUP();
					ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
				}
				break;
			}
		}
		else if(CCNode *node = dynamic_cast<CCNode *>(object))
		{
			if (m_pAtlas)
			{
				m_pAtlas->drawQuads();
				m_pAtlas->removeAllQuads();
			}
			node->visit();

			CC_NODE_DRAW_SETUP();
			ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
		}
	}

	if(m_pAtlas && !m_pBatchNode && m_pParentBone == NULL)
	{
		m_pAtlas->drawQuads();
		m_pAtlas->removeAllQuads();
	}
}
//-------------------------------------------------------------------------
CCTextureAtlas* FKCW_RenderEx_PredrawArmature::getTexureAtlasWithTexture( CCTexture2D* texture )
{
	int key = texture->getName();

	if (m_pParentBone && m_pParentBone->getArmature())
	{
		return m_pParentBone->getArmature()->getTexureAtlasWithTexture(texture);
	}
	else if (m_pBatchNode)
	{
		m_pBatchNode->getTexureAtlasWithTexture(texture);
	}

	CCTextureAtlas *atlas = (CCTextureAtlas *)m_pTextureAtlasDic->objectForKey(key);
	if (atlas == NULL)
	{
		atlas = CCTextureAtlas::createWithTexture(texture, 4);
		m_pTextureAtlasDic->setObject(atlas, key);
	}
	return atlas;
}
//-------------------------------------------------------------------------