//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Test_RenderEx_SwayNode.h
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
class CTest_RenderEx_SwayNodeLayer : public CCLayer
{
public:
	bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );

			FKCW_RenderEx_SwayNode* pNode = FKCW_RenderEx_SwayNode::create( s_szGrassPath );
			pNode->setPosition( SCREEN_CENTER_POS );
			addChild( pNode );

			return true;
		} while ( 0 );
		return false;
	}
public:
	CREATE_FUNC( CTest_RenderEx_SwayNodeLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, SwayNode, 可上部摇曳的精灵节点，适合做草花等)
//-------------------------------------------------------------------------