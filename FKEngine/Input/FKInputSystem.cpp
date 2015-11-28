//-------------------------------------------------------------------------
#include "FKInputSystemMacros.h"
#include "FKInputSystem.h"
//-------------------------------------------------------------------------
IInputSystem* gs_pInputSystem;
FKInputSystem gs_FKInputSystem;
//-------------------------------------------------------------------------
// 获取输入管理器
IInputSystem* GetInputSystem()
{
	return gs_pInputSystem;
}
//-------------------------------------------------------------------------
FKInputSystem::FKInputSystem()
	: m_pMsgCallback( NULL )
{
	gs_pInputSystem = this;
}
//-------------------------------------------------------------------------
FKInputSystem::~FKInputSystem()
{
	gs_pInputSystem = NULL;
}
//-------------------------------------------------------------------------
// 初始化工作
bool FKInputSystem::Init()
{
	// 添加键盘反馈
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook( FKInputSystem::WinKeyboardHook ); 
#endif
	
	// UI根节点
	if( GetUISystem() == NULL )
	{
		FKLOG("请保证UI系统先于Input之前创建");
	}
	else
	{
		if( GetUISystem()->GetRoot() == NULL )
		{
			FKLOG("UI系统根窗口未创建之前，复杂手势识别无法支持");
		}
		else
		{
			/*
			//长按
			FKCW_GR_LongPressGestureRecognizer* grLongPress = FKCW_GR_LongPressGestureRecognizer::create();
			grLongPress->SetTarget( this, callfuncO_selector(FKInputSystem::onLongPress));
			GetUISystem()->GetRoot()->addChild(grLongPress);

			//滑动操作,慢速拖移
			FKCW_GR_PanGestureRecognizer* grPan = FKCW_GR_PanGestureRecognizer::create();
			grPan->SetTarget(this, callfuncO_selector(FKInputSystem::onPan));
			GetUISystem()->GetRoot()->addChild(grPan);

			//捏合
			FKCW_GR_PinchGestureRecognizer* grPinch = FKCW_GR_PinchGestureRecognizer::create();
			grPinch->SetTarget(this, callfuncO_selector(FKInputSystem::onPinch));
			GetUISystem()->GetRoot()->addChild(grPinch);

			//快速滑动，和pan不同是速度
			FKCW_GR_SwipeGestureRecognizer* grSwipe = FKCW_GR_SwipeGestureRecognizer::create();
			grSwipe->SetTarget(this, callfuncO_selector(FKInputSystem::onSwipe));
			GetUISystem()->GetRoot()->addChild(grSwipe);

			//点击
			FKCW_GR_TapGestureRecognizer* grTap = FKCW_GR_TapGestureRecognizer::create();
			grTap->SetTarget(this, callfuncO_selector(FKInputSystem::onTap));
			GetUISystem()->GetRoot()->addChild(grTap);
			*/
		}
	}
	return true;
}
//-------------------------------------------------------------------------
// 销毁释放
void FKInputSystem::Release()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
		return;
	pDirector->getTouchDispatcher()->removeDelegate( (CCTargetedTouchDelegate*)this );
	pDirector->getKeypadDispatcher()->removeDelegate( (CCKeypadDelegate*)this );
}
//-------------------------------------------------------------------------
// 设置消息回调处理函数
void FKInputSystem::SetInputMsgCallback( IInputMsgHandler* pInputMsgCallback )
{
	m_pMsgCallback = pInputMsgCallback;
}
//-------------------------------------------------------------------------
// 设置是否启动输入处理
void FKInputSystem::SetInputEnabled( bool bEnable )
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
		return;

	if( bEnable )
	{
		pDirector->getTouchDispatcher()->addTargetedDelegate((CCTargetedTouchDelegate*)this, 0, true);
		pDirector->getKeypadDispatcher()->addDelegate( (CCKeypadDelegate*)this );
	}
	else
	{
		pDirector->getTouchDispatcher()->removeDelegate((CCTargetedTouchDelegate*)this);
		pDirector->getKeypadDispatcher()->removeDelegate( (CCKeypadDelegate*)this );
	}
}
//-------------------------------------------------------------------------
// 获取消息回调处理函数
IInputMsgHandler* FKInputSystem::GetInputMsgCallback() const
{
	return m_pMsgCallback;
}
//-------------------------------------------------------------------------
// 触摸开始回调
bool FKInputSystem::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if( m_pMsgCallback == NULL || touch == NULL )
		return false;

	// 触摸消息不需要额外数据
	m_pMsgCallback->OnInputMessage( eIMT_TouchBegin, touch->getID(),
		touch->getLocation().x, touch->getLocation().y, NULL );

	return false;
}
//-------------------------------------------------------------------------
// 触摸移动回调
void FKInputSystem::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if( m_pMsgCallback == NULL || touch == NULL )
		return;

	// 触摸消息不需要额外数据
	m_pMsgCallback->OnInputMessage( eIMT_TouchMove, touch->getID(),
		touch->getLocation().x, touch->getLocation().y, NULL );
}
//-------------------------------------------------------------------------
// 触摸结束回调
void FKInputSystem::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if( m_pMsgCallback == NULL || touch == NULL )
		return;

	// 触摸消息不需要额外数据
	m_pMsgCallback->OnInputMessage( eIMT_TouchEnd, touch->getID(),
		touch->getLocation().x, touch->getLocation().y, NULL );
}
//-------------------------------------------------------------------------
// Android和iOS上的后退键按下回调
void FKInputSystem::keyBackClicked()
{
	if( m_pMsgCallback == NULL )
		return;

	m_pMsgCallback->OnInputMessage( eIMT_BackKey, 0, 0.0f, 0.0f, NULL );
}
//-------------------------------------------------------------------------
// Android和iOS上的Menu键按下回调
void FKInputSystem::keyMenuClicked()
{
	if( m_pMsgCallback == NULL )
		return;

	m_pMsgCallback->OnInputMessage( eIMT_MenuKey, 0, 0.0f, 0.0f, NULL );
}
//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// WINDOWS 消息
	void FKInputSystem::WinKeyboardHook( UINT message,WPARAM wParam, LPARAM lParam )
	{
		if( gs_pInputSystem->GetInputMsgCallback() == NULL )
			return;
		switch ( message )
		{
		case WM_KEYDOWN:
			{
				gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_AnyKeyDown, 0, 0.0f, 0.0f, NULL );
			}
			break;
		case WM_KEYUP:
			{
				switch ( wParam )
				{
				case VK_F1:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF1, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F2:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF2, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F3:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF3, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F4:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF4, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F5:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF5, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F6:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF6, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F7:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF7, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_F8:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyF8, 0, 0.0f, 0.0f, NULL );
					break;
				case 'W':
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyW, 0, 0.0f, 0.0f, NULL );
					break;
				case 'S':
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyS, 0, 0.0f, 0.0f, NULL );
					break;
				case 'A':
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyA, 0, 0.0f, 0.0f, NULL );
					break;
				case 'D':
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyD, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_UP:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyUp, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_DOWN:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyDown, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_LEFT:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyLeft, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_RIGHT:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeyRight, 0, 0.0f, 0.0f, NULL );
					break;
				case VK_SPACE:
					gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_KeySpace, 0, 0.0f, 0.0f, NULL );
					break;
				default:
					break;
				}

				gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_AnyKeyUp, 0, 0.0f, 0.0f, NULL );
			}
			break;
		case WM_MOUSEWHEEL:
			{
				SMouseWheelParam tagParam;
				tagParam.m_sDelta = LOWORD(wParam);
				if( tagParam.m_sDelta >= 0 )
				{
					tagParam.m_bIsFront = true;
				}
				else
				{
					tagParam.m_bIsFront = false;
				}
				gs_pInputSystem->GetInputMsgCallback()->OnInputMessage( eIMT_MouseWheel, 0, 
					LOWORD(lParam), HIWORD(lParam), &tagParam );
			}
			break;
		default:
			break;
		}
	}
#endif
//-------------------------------------------------------------------------
void FKInputSystem::onLongPress(CCObject* object)
{
	FKCW_GR_LongPress* p = dynamic_cast<FKCW_GR_LongPress*>(object);
	if( p == NULL )
		return;
	if( m_pMsgCallback == NULL )
		return;
	m_pMsgCallback->OnInputMessage( eIMT_LongPress, 0, 
		p->m_tagLocation.x, p->m_tagLocation.y, NULL );
}
//-------------------------------------------------------------------------
void FKInputSystem::onPan(CCObject* object)
{
	FKCW_GR_Pan* p = dynamic_cast<FKCW_GR_Pan*>(object);
	if( p == NULL )
		return;
	if( m_pMsgCallback == NULL )
		return;
	SPanParam tagParam;
	tagParam.m_fDeltaX = p->m_tagDelta.x;
	tagParam.m_fDeltaY = p->m_tagDelta.y;
	m_pMsgCallback->OnInputMessage( eIMT_Pan, 0, 
		p->m_tagLocation.x, p->m_tagLocation.y, &tagParam );
}
//-------------------------------------------------------------------------
void FKInputSystem::onPinch(CCObject* object)
{
	FKCW_GR_Pinch* p = dynamic_cast<FKCW_GR_Pinch*>(object);
	if( p == NULL )
		return;
	if( m_pMsgCallback == NULL )
		return;
	SPinchParam tagParam;
	if( p->m_eType == eGRPinchType_Open )
	{
		tagParam.m_bIsOpen = true;
	}
	else
	{
		tagParam.m_bIsOpen = false;
	}
	m_pMsgCallback->OnInputMessage( eIMT_Pinch, 0, 0.0f, 0.0f, &tagParam );
}
//-------------------------------------------------------------------------
void FKInputSystem::onSwipe(CCObject* object)
{
	FKCW_GR_Swipe* p = dynamic_cast<FKCW_GR_Swipe*>(object);
	if( p == NULL )
		return;
	if( m_pMsgCallback == NULL )
		return;
	SSwipeParam tagParam;
	if( (p->m_eDirection & eSwipeDirection_Right) == eSwipeDirection_Right )
		tagParam.m_bIsRight = true;
	if( (p->m_eDirection & eSwipeDirection_Left) == eSwipeDirection_Left )
		tagParam.m_bIsLeft = true;
	if( (p->m_eDirection & eSwipeDirection_Up) == eSwipeDirection_Up )
		tagParam.m_bIsUp = true;
	if( (p->m_eDirection & eSwipeDirection_Down) == eSwipeDirection_Down )
		tagParam.m_bIsDown = true;
	m_pMsgCallback->OnInputMessage( eIMT_Swipe, 0, 
		p->m_tagLocation.x, p->m_tagLocation.y, &tagParam );
}
//-------------------------------------------------------------------------
void FKInputSystem::onTap(CCObject* object)
{
	FKCW_GR_Tap* p = dynamic_cast<FKCW_GR_Tap*>(object);
	if( p == NULL )
		return;
	if( m_pMsgCallback == NULL )
		return;
	m_pMsgCallback->OnInputMessage( eIMT_Tap, 0, 
		p->m_tagLocation.x, p->m_tagLocation.y, NULL );
}
//-------------------------------------------------------------------------