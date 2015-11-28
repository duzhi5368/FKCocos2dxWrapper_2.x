//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionCrossFade.h"
//-------------------------------------------------------------------------
const unsigned int kSceneFade = 0xFADEFADE;
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionCrossFade::FKCW_SceneExMgr_SceneExTransitionCrossFade()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionCrossFade::~FKCW_SceneExMgr_SceneExTransitionCrossFade()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionCrossFade* FKCW_SceneExMgr_SceneExTransitionCrossFade::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionCrossFade* pScene = new FKCW_SceneExMgr_SceneExTransitionCrossFade();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionCrossFade:: draw()
{
	// 渲染已经在onEnter执行的动作中做了
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionCrossFade::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();

	// 创建一个带颜色的Layer，用以添加我们自己的renderTexture
	ccColor4B  color = {0,0,0,0};
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCLayerColor* layer = CCLayerColor::create(color);

	// 为进入场景创建一个渲染纹理
	CCRenderTexture* inTexture = CCRenderTexture::create((int)size.width, (int)size.height);

	if (NULL == inTexture)
	{
		return;
	}

	inTexture->getSprite()->setAnchorPoint( ccp(0.5f,0.5f) );
	inTexture->setPosition( ccp(size.width/2, size.height/2) );
	inTexture->setAnchorPoint( ccp(0.5f,0.5f) );

	// 渲染新进度场景到textureBuffer中
	inTexture->begin();
	m_pInScene->visit();
	inTexture->end();

	// 为即将消失的老场景创建一个渲染纹理
	CCRenderTexture* outTexture = CCRenderTexture::create((int)size.width, (int)size.height);
	outTexture->getSprite()->setAnchorPoint( ccp(0.5f,0.5f) );
	outTexture->setPosition( ccp(size.width/2, size.height/2) );
	outTexture->setAnchorPoint( ccp(0.5f,0.5f) );

	// 渲染老进度场景到textureBuffer中
	outTexture->begin();
	m_pOutScene->visit();
	outTexture->end();

	// 创建混合函数
	ccBlendFunc blend1 = {GL_ONE, GL_ONE};							// 新场景做为背景层，不使用alpha
	ccBlendFunc blend2 = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};	// 老场景做前景alpha

	// 设置混合函数
	inTexture->getSprite()->setBlendFunc(blend1);
	outTexture->getSprite()->setBlendFunc(blend2);    

	// 添加渲染纹理到Layer中
	layer->addChild(inTexture);
	layer->addChild(outTexture);

	// 初始化其Alpha值
	inTexture->getSprite()->setOpacity(255);
	outTexture->getSprite()->setOpacity(255);

	// 创建混合动作
	CCAction* layerAction = CCSequence::create
		(
		CCFadeTo::create(m_fDuration, 0),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::hideOutShowIn)),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		);


	// 执行混合动作
	outTexture->getSprite()->runAction( layerAction );

	// 添加带两个renderTexture的Layer到当前场景中
	addChild(layer, 2, kSceneFade);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionCrossFade::onExit()
{
	// 移除自己的Layer
	this->removeChildByTag(kSceneFade, false);
	FKCW_SceneExMgr_SceneExTransition::onExit();
}
//-------------------------------------------------------------------------