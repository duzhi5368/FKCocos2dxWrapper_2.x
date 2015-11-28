//*************************************************************************
//	创建日期:	2015-1-6
//	文件名称:	BaseScene.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseScene.h"
#include "Resource.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
#define LAYER_TAG		10000
#define SPIRTE_TAG		10001
//-------------------------------------------------------------------------
static std::string Tchar2String(wchar_t* src)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
	char* chRtn =new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, src, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	return str;
}
//-------------------------------------------------------------------------
CCScene* BaseLayer::scene()
{
    CCScene *scene = CCScene::create();
    BaseLayer *layer = BaseLayer::create();
	layer->setTag( LAYER_TAG );
    scene->addChild(layer);
    return scene;
}
//-------------------------------------------------------------------------
BaseLayer::BaseLayer()
{

}
//-------------------------------------------------------------------------
bool BaseLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// 添加键盘反馈
	CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook( BaseLayer::KeyboardHook ); 
	// add your code here

	// 创建底色
	m_pBackgroundLayer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	addChild( m_pBackgroundLayer );

	// 创建DB精灵
	m_pDBSprite = new CCocos2dxDBSprite();
	if( m_pDBSprite == NULL )
		return false;
	m_pDBSprite->setTag( SPIRTE_TAG );
	m_pDBSprite->setPosition( SCREEN_CENTER_POS + ccp(0, -200) );
	this->addChild( m_pDBSprite );

	// 添加帧回调事件
	CTestAnimCallbackHandler* pTestEvent = new CTestAnimCallbackHandler;
	if( m_pDBSprite->getAnimation() )
		m_pDBSprite->getAnimation()->SetAniCallback( pTestEvent,NULL );

	// 创建UI组
	if( !CreateUI() )
	{
		return false;
	}

	// 显示精灵骨骼数
	UpdateBoneNum();

	// 显示精灵动画名
	UpdateFileName();

	// 显示精灵动作列表
	UpdateActionList();
    
    return true;
}
//-------------------------------------------------------------------------
// WINDOWS 消息
void BaseLayer::KeyboardHook( UINT message,WPARAM wParam, LPARAM lParam )
{
	CCLayer* pLayer = (CCLayer*)( CCDirector::sharedDirector()->getRunningScene()->getChildByTag( LAYER_TAG ) );
	if( pLayer == NULL )
		return;
	CCocos2dxDBSprite* pSprite = (CCocos2dxDBSprite*)pLayer->getChildByTag( SPIRTE_TAG );
	if( pSprite == NULL )
		return;
	switch (message)  
	{  
	//case WM_MOUSEWHEEL:
	//	{
	//		if( (INT)wParam > 0 )
	//		{
	//			pSprite->setScale( pSprite->getScale() * 1.25f );
	//		}
	//		else
	//		{
	//			pSprite->setScale( pSprite->getScale() * 0.8f );
	//		}
	//	}
	//	break;
	case WM_KEYDOWN:  
		switch ( wParam )
		{
		case 'W':
			pSprite->setPosition( ccp(pSprite->getPositionX(), pSprite->getPositionY() + 20) );
			break;
		case 'S':
			pSprite->setPosition( ccp(pSprite->getPositionX(), pSprite->getPositionY() - 20) );
			break;
		case 'A':
			pSprite->setPosition( ccp(pSprite->getPositionX() - 30, pSprite->getPositionY() ) );
			break;
		case 'D':
			pSprite->setPosition( ccp(pSprite->getPositionX() + 30, pSprite->getPositionY()) );
			break;
		case VK_UP:
			pSprite->setScale( pSprite->getScale() * 1.25f );
			break;
		case VK_DOWN:
			pSprite->setScale( pSprite->getScale() * 0.8f );
			break;
		default:
			break;
		}
		return;  
	case WM_KEYUP:  
		switch (wParam)
		{
		case VK_SPACE:
			if( pSprite->getAnimation() )
			{
				if( pSprite->getAnimation()->IsPause() )
				{
					std::list< string > listAnimationName;
					pSprite->getAnimation()->GetAnimationList( listAnimationName );
					if( !listAnimationName.empty() )
						pSprite->getAnimation()->Play( listAnimationName.begin()->c_str() );
				}
				else
				{
					pSprite->getAnimation()->Pause();
				}
			}
			break;
		default:
			break;
		}
		return;
	default:
		return;
	}
}
//-------------------------------------------------------------------------
bool BaseLayer::CreateUI()
{
	CCLabelTTF* pControlInfo1 = CCLabelTTF::create( A2U("WSAD控制位置,Space暂停动作").c_str(), "Arial", 20 );
	pControlInfo1->setAnchorPoint(ccp(0.0f, 0.5f));
	pControlInfo1->setColor( ccRED );
	pControlInfo1->setPosition( ccp( 20, SCERRN_HEIGHT - 40 ) );
	this->addChild( pControlInfo1 );
	CCLabelTTF* pControlInfo2 = CCLabelTTF::create( A2U("向上箭头放大,向下箭头缩小").c_str(), "Arial", 20 );
	pControlInfo2->setAnchorPoint(ccp(0.0f, 0.5f));
	pControlInfo2->setColor( ccRED );
	pControlInfo2->setPosition( ccp( 20, SCERRN_HEIGHT - 65 ) );
	this->addChild( pControlInfo2 );
	CCLabelTTF* pControlInfo3 = CCLabelTTF::create( A2U("下方动作列表可点击更变动作").c_str(), "Arial", 20 );
	pControlInfo3->setAnchorPoint(ccp(0.0f, 0.5f));
	pControlInfo3->setColor( ccRED );
	pControlInfo3->setPosition( ccp( 20, SCERRN_HEIGHT - 90 ) );
	this->addChild( pControlInfo3 );

	FKCW_UI_MenuItemColor* pBtn = FKCW_UI_MenuItemColor::Create( ccc4( 255, 255, 0, 0 ),
		ccc4( 255, 128, 0, 0 ), ccc4( 255, 255, 0, 0 ), this, 
		menu_selector(BaseLayer::OnChangeBGColor) );
	pBtn->setContentSize( CCSizeMake(50, 50) );
	CCMenu* pMenu = CCMenu::create( pBtn, NULL );
	pMenu->alignItemsVertically();
	pMenu->setPosition( ccp( SCREEN_CENTER_POS_X + 150, SCERRN_HEIGHT - 100 ));
	this->addChild( pMenu );

	m_pBonesArmasNumLabel = CCLabelTTF::create( A2U("骨骼信息").c_str(), "Arial", 26 );
	m_pBonesArmasNumLabel->setPosition( ccp( SCREEN_CENTER_POS_X, 30 ) );
	this->addChild( m_pBonesArmasNumLabel );

	m_pDBFileLabel = CCLabelTTF::create( A2U("暂无DragonBones文件").c_str(), "Arial", 20 );
	m_pDBFileLabel->setAnchorPoint( ccp(0.0f, 0.5f) );
	m_pDBFileLabel->setPosition( ccp( SCREEN_CENTER_POS_X-250, SCERRN_HEIGHT - 40 ) );
	this->addChild( m_pDBFileLabel );

	m_pActionListMenu = CCMenu::create();
	m_pActionListMenu->setPosition( ccp(0,0) );
	this->addChild( m_pActionListMenu );

	CCMenu* pSelFileMenu = CCMenu::create();
	pSelFileMenu->setPosition(CCPointZero);
	this->addChild(pSelFileMenu);

	CCMenuItemImage* pSelFileBtn = CCMenuItemImage::create( s_szButtonNormalPath, s_szButtonPressPath);
	pSelFileBtn->setTarget(this, menu_selector(BaseLayer::OnSelDBFile));
	pSelFileBtn->setPosition( ccp( SCREEN_CENTER_POS_X, SCERRN_HEIGHT - 100 ));
	pSelFileMenu->addChild(pSelFileBtn);

	CCLabelTTF* pBtnLabel = CCLabelTTF::create( A2U("更换加载文件").c_str(), "Arial", 26 );
	pBtnLabel->setPosition( ccp( SCREEN_CENTER_POS_X, SCERRN_HEIGHT - 100 ) );
	pBtnLabel->setColor( ccRED );
	this->addChild( pBtnLabel );

	return true;
}
//-------------------------------------------------------------------------
// 显示精灵骨骼数
void BaseLayer::UpdateBoneNum()
{
	if( m_pDBSprite == NULL )
		return;
	if( m_pDBSprite->GetDBNode() == NULL )
		return;
	if( m_pDBSprite->GetDBNode()->GetArmature() == NULL )
		return;
	// 显示骨骼数
	int nTotalBoneNum = 0;
	int nArmatureNum = 1;
	unsigned int nMainBoneNum = m_pDBSprite->GetDBNode()->GetArmature()->m_vecBoneList.size();	// 主骨架数
	nTotalBoneNum += nMainBoneNum;
	// 遍历子骨架
	for( unsigned int i = 0; i < nMainBoneNum; ++i )
	{
		CBone* pSubBone = m_pDBSprite->GetDBNode()->GetArmature()->m_vecBoneList[i];
		if( pSubBone != NULL )
		{
			list<CSlot*> slotList = pSubBone->GetSlots();
			list<CSlot*>::iterator slotIte = slotList.begin();
			for( ; slotIte != slotList.end(); ++slotIte )
			{
				vector<IObject*> vecObj = (*slotIte)->GetDisplayList();
				for( unsigned int j = 0; j < vecObj.size(); ++j )
				{
					if( dynamic_cast<CArmature*>(vecObj[j]) )
					{
						nTotalBoneNum += static_cast<CArmature*>(vecObj[j])->m_vecBoneList.size();
						nArmatureNum++;
					}
				}
			}
		}
	}

	char szInfo[128];
	memset( szInfo, 0, 128 );
	sprintf( szInfo, "骨骼数量: %d, Armature数量: %d", nTotalBoneNum, nArmatureNum );
	m_pBonesArmasNumLabel->setString( A2U(szInfo).c_str() );
}
//-------------------------------------------------------------------------
// 显示精灵动画名
void BaseLayer::UpdateFileName()
{
	if( m_pDBSprite == NULL )
		return;
	string strFileName = m_pDBSprite->getFileName();
	if( strFileName.empty() )
		return;

	m_pDBFileLabel->setString( A2U(strFileName.c_str()).c_str() );
}
//-------------------------------------------------------------------------
// 显示精灵动作列表
void BaseLayer::UpdateActionList()
{
	static CCPoint s_UpBtnPos = ccp( 150, SCERRN_HEIGHT - 180 );
	// 清理全部动作
	m_pActionListMenu->removeAllChildrenWithCleanup( true );

	CCPoint tagBeginPos = s_UpBtnPos;
	if( m_pDBSprite->getAnimation() == NULL )
	{
		CCMenuItemFont* p = CCMenuItemFont::create( A2U("无动作").c_str(), this, 
			NULL );
		p->setPosition( tagBeginPos );
		m_pActionListMenu->addChild( p );
		return;
	}
	std::list< string > listAnimationName;
	m_pDBSprite->getAnimation()->GetAnimationList( listAnimationName );
	if( listAnimationName.empty() )
	{
		CCMenuItemFont* p = CCMenuItemFont::create( A2U("无动作").c_str(), this, 
			NULL );
		p->setPosition( tagBeginPos );
		m_pActionListMenu->addChild( p );
		return;
	}

	for( std::list<string>::iterator Ite = listAnimationName.begin();
		Ite != listAnimationName.end();
		++Ite )
	{
		CCMenuItemFont* p = CCMenuItemFont::create( Ite->c_str(), this, 
			menu_selector(BaseLayer::OnSelActionBtn) );
		p->setPosition( tagBeginPos );
		tagBeginPos.y -= 34;
		m_pActionListMenu->addChild( p );
	}
}
//-------------------------------------------------------------------------
// 按下播放动作标签
void BaseLayer::OnSelActionBtn(CCObject* pSender )
{
	CCMenuItemFont* p = ( CCMenuItemFont* )pSender;
	string strActionName = dynamic_cast<CCLabelProtocol*>(p->getLabel())->getString();
	if( m_pDBSprite->getAnimation() == NULL )
		return;
	m_pDBSprite->getAnimation()->Play( strActionName );
}
//-------------------------------------------------------------------------
// 选择了DB文件
void BaseLayer::OnSelDBFile(CCObject* sender)
{
	// 清除之前的
	if( m_pDBSprite != NULL )
	{
		removeChild( m_pDBSprite );
		delete m_pDBSprite;
		m_pDBSprite = NULL;
	}

	string strDBFile;
	strDBFile.clear();

	OPENFILENAME ofn;			// 公共对话框结构。     
	wchar_t szFile[MAX_PATH];	// 保存获取文件名称的缓冲区。               
	// 初始化选择文件对话框。     
	ZeroMemory(&ofn, sizeof(OPENFILENAME));    
	ofn.lStructSize = sizeof(OPENFILENAME);    
	ofn.hwndOwner = NULL;    
	ofn.lpstrFile = szFile;    
	ofn.lpstrFile[0] = '\0';   
	ofn.nMaxFile = sizeof(szFile); 
	ofn.lpstrFilter = L"skeleton文件(*.xml)\0*.XML\0\0";
	ofn.nFilterIndex = 1;    
	ofn.lpstrFileTitle = NULL;    
	ofn.nMaxFileTitle = 0;    
	ofn.lpstrInitialDir = NULL;    
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// 显示打开选择文件对话框。     
	if ( GetOpenFileName(&ofn) )    
	{ 
		strDBFile = Tchar2String(szFile);
		// 设置新的DB文件
		m_pDBSprite = new CCocos2dxDBSprite();
		if( m_pDBSprite == NULL )
			return;
		m_pDBSprite->setTag( SPIRTE_TAG );
		if( !m_pDBSprite->create( strDBFile.c_str() ) )
		{
			delete m_pDBSprite;
			m_pDBSprite = NULL;
			return;
		}
		m_pDBSprite->setPosition( SCREEN_CENTER_POS + ccp(0, -200) );
		this->addChild( m_pDBSprite );
	}

	// 显示精灵骨骼数
	UpdateBoneNum();

	// 显示精灵动画名
	UpdateFileName();

	// 显示精灵动作列表
	UpdateActionList();
}
//-----------------------------------------------------------------
// 更换背景颜色
void BaseLayer::OnChangeBGColor(CCObject* sender)
{
	if( m_pBackgroundLayer )
		m_pBackgroundLayer->setColor( ccc3( FKCW_Base_Utils::RandomInRange(5, 255),
		FKCW_Base_Utils::RandomInRange(5, 255),
		FKCW_Base_Utils::RandomInRange(5, 255) ) );
}
//-----------------------------------------------------------------
// CTestAnimCallbackHandler
//-----------------------------------------------------------------
void CTestAnimCallbackHandler::OnAnimaitonCallback( const SAnimationCallbackInfo& p_tagInfo, void* p_pContext )
{
	FKLOG("回调：动作名 = %s 回调名 = %s", p_tagInfo.m_szActName, p_tagInfo.m_szName );
}
//-------------------------------------------------------------------------