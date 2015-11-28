//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Test_Shader_Demo2.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Shader_Demo2Layer : public CCLayer
{
public:
	virtual bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );

			m_fPassedTime = 0;

			// 这里做部分初始化工作
			for( unsigned int i = 0; i < 6; ++i )
			{
				for( unsigned int j = 0; j < 3; ++j )
				{
					CreateSprite( i,j );
				}
			}

			return true;
		} while ( 0 );
		return false;
	}

	virtual void onEnter()
	{
		CCLayer::onEnter();
		this->scheduleUpdate();
	}
	virtual void onExit()
	{
		this->unscheduleUpdate();
		CCLayer::onExit();
	}
public:
	void CreateSprite( int i, int j )
	{
		CCSprite* p = CCSprite::create( s_szMidCocosLogoPicPath );
		p->setPosition( ccp(i * 180 + 28 + 75, 180 * j + 75 + 40) );

		int nType = i + j * 6;
		if( nType == 0 )
		{
			// 原文件。donothing
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("原文件").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 1 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSmallBlur ));
			FKCW_Shader_ShaderManager::SetSmallBlur( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("轻度模糊").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 2 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBigBlur ));
			FKCW_Shader_ShaderManager::SetBigBlur( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("重度模糊").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 3 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSmallSharpen ));
			FKCW_Shader_ShaderManager::SetSmallSharpen(  p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("轻度磨砂").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 4 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBigSharpen ));
			FKCW_Shader_ShaderManager::SetBigSharpen( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("重度磨砂[bug]").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 5 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSimpleSharpen ));
			FKCW_Shader_ShaderManager::SetSimpleSharpen( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("简易磨砂").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 6 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderDilate ));
			FKCW_Shader_ShaderManager::SetDilate( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("膨胀效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 7 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderErode ));
			FKCW_Shader_ShaderManager::SetErode( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("侵蚀效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 8 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLaplacian ));
			FKCW_Shader_ShaderManager::SetLaplacian( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Laplacian描边").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 9 )
		{
			//此shader在安卓下会报错.错误已记录
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID

			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSobel ));
			FKCW_Shader_ShaderManager::SetSobel( p->getContentSize() );
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Sobel描边").c_str(),"Arial", 26);
#else
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Sobel-BUG").c_str(),"Arial", 26);
#endif
			
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );

			
		}
		else if( nType == 10 )
		{
			//此shader在安卓下会报错.错误已记录
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID

			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderPrewitt ));
			FKCW_Shader_ShaderManager::SetPrewitt( p->getContentSize() );
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Prewitt描边").c_str(),"Arial", 26);
#else
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Prewitt-BUG").c_str(),"Arial", 26);
#endif
			
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );

			
		}
		else if( nType == 11 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderMotionBlur ));
			FKCW_Shader_ShaderManager::SetMotionBlur( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("运动模糊").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 12 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBigEmboss ));

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("动态浮雕").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 13 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderColorRamp ));
			FKCW_Shader_ShaderManager::SetColorRamp();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("底片效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 14 )
		{
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Todo").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 15 )
		{
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Todo").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 16 )
		{
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Todo").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 17 )
		{
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Todo").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}

		addChild( p );
	}

	void update( float p_fDetla )
	{
		m_fPassedTime += p_fDetla;

		FKCW_Shader_ShaderManager::SetBigEmboss( m_fPassedTime );
	}

public:
	CREATE_FUNC( CTest_Shader_Demo2Layer );
private:
	float m_fPassedTime;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Shader, Demo2, Shader效果测试2号)
	//-------------------------------------------------------------------------