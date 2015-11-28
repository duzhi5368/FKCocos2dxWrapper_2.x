//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Test_Shader_Demo1.h
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
class CTest_Shader_Demo1Layer : public CCLayer
{
public:
	virtual bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );

			m_fFlashTime = 0.0f;

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
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderFlash ));

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("闪光效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 2 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBlur ));
			FKCW_Shader_ShaderManager::SetBlur( p->getContentSize(), CCSize(2.0f, 2.0f) );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("自定义模糊").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 3 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLighting ));

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("高光效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 4 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLaser ));

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("镭射色效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 5 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderMatrix ));
			FKCW_Shader_ShaderManager::SetGray();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("灰度图").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 6 )
		{
			//p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBloom ));
			//FKCW_Shader_ShaderManager::SetBloom();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("Bloom效果[bug]").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 7 )
		{
			//p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderCelShading ));
			//FKCW_Shader_ShaderManager::SetCelShading( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("卡通渲染[bug]").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 8 )
		{
			//p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderEdgeDetection ));
			//FKCW_Shader_ShaderManager::SetEdgeDetectionSize( p->getContentSize() );

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("边缘检测[bug]").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 9 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderShine ));

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("光线效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 10 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderOldPhoto ));
			FKCW_Shader_ShaderManager::SetOldPhoto();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("老照片").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 11 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderNegative ));
			FKCW_Shader_ShaderManager::SetNegative();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("反色效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 12 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBrightness ));
			FKCW_Shader_ShaderManager::SetBrightness();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("高光效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 13 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderContrast ));
			FKCW_Shader_ShaderManager::SetContrast();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("对比效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 14 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSaturation ));
			FKCW_Shader_ShaderManager::SetSaturation();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("饱和效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 15 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBlackWhite ));
			FKCW_Shader_ShaderManager::SetBlackWhite();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("黑白化").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 16 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderSimpleEdgeDetection ));
			FKCW_Shader_ShaderManager::SetSimpleEdgeDetection();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("简易边缘").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}
		else if( nType == 17 )
		{
			p->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderEmboss ));
			FKCW_Shader_ShaderManager::SetEmboss();

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("浮雕效果").c_str(),"Arial", 26);
			pLabel->setPosition( ccp(75, -15));
			p->addChild( pLabel );
		}

		addChild( p );
	}
	void update( float p_fDetla )
	{
		m_fFlashTime += p_fDetla;
		if( m_fFlashTime >= 1.0f )
		{
			m_fFlashTime -= 1.0f;
		}
		float t;
		if(m_fFlashTime > 0.5f) 
		{
			t = 1 - (m_fFlashTime - 0.5f) / 0.5f;
		} 
		else 
		{
			t = m_fFlashTime / 0.5f;
		}
		static ccColor4B c1 = {0,   0, 255, 255};
		static ccColor4B c2 = {0, 255,   0, 255};
		static ccColor4B c3 = {255, 0,   0, 255};
		FKCW_Shader_ShaderManager::SetFlash( 1.0f, 1.0f, 1.0f, t );
		FKCW_Shader_ShaderManager::SetLighting( ccc4(50, 50, 50, 50), 
			ccc3(255 * m_fFlashTime, 255 * m_fFlashTime, 255 * m_fFlashTime) );
		FKCW_Shader_ShaderManager::SetShine( 150.0f,
			ccp(0, 150.0f),
			ccp(150.0f, 0),
			c1,
			c2,
			c3,
			vertex3(0.2f, 0.5f, 0.3f),
			t);
	}
public:
	CREATE_FUNC( CTest_Shader_Demo1Layer );
private:
	float m_fFlashTime;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Shader, Demo1, Shader效果测试1号)
//-------------------------------------------------------------------------