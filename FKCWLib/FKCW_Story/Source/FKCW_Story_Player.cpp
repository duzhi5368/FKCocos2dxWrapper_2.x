//-------------------------------------------------------------------------
#include "../Include/FKCW_Story_Player.h"
#include "../Include/FKCW_Story_Layer.h"
#include "../Include/FKCW_Story_CommandSet.h"
#include "../Include/FKCW_Story_MessageLayer.h"
#include "../Include/FKCW_Story_ClickLayer.h"
#include "SimpleAudioEngine.h"
#include "../../FKCW_Action/FKCW_Action.h"
#include <fstream>
//-------------------------------------------------------------------------
using namespace CocosDenshion;
using namespace cocos2d::extension;
//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import <AppKit/AppKit.h>
#endif
//-------------------------------------------------------------------------
FKCW_Story_Player::FKCW_Story_Player() :
	m_owner(NULL),
	m_curCmd(NULL),
	m_done(false),
	m_needRestoreDesignSize(false),
	m_requiredPolicy(kResolutionShowAll),
	m_curCmdIndex(-1),
	m_msgSize(20),
	m_nameSize(20),
	m_labelSize(20),
	m_msgFont("Helvetica"),
	m_nameFont("Helvetica"),
	m_labelFont("Helvetica"),
	m_msgColor(0xffffffff),
	m_nameColor(0xffffffff),
	m_labelColor(0xffffffff),
	m_msgAlignment(kCCTextAlignmentCenter),
	m_msgPos(CCPointZero),
	m_namePos(CCPointZero),
	m_msgConstraints(CCSizeZero),
	m_labelConstraints(CCSizeZero),
	m_labelAlignment(kCCTextAlignmentCenter),
	m_labelAnchor(ccp(0.5f, 0.5f)),
	m_msgAnchor(ccp(0.5f, 0.5f)),
	m_nameAnchor(ccp(0.5f, 0.5f)) {
}
//-------------------------------------------------------------------------
FKCW_Story_Player::~FKCW_Story_Player()
{
	if(m_needRestoreDesignSize) {
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_oldDesignSize.x, m_oldDesignSize.y, m_requiredPolicy);

#ifdef FKCW_STORY_DESIGNER_TOOL
		CCNotificationCenter::sharedNotificationCenter()->postNotification("kCCNotificationFrameSizeChanged");
#endif
	}
}
//-------------------------------------------------------------------------
FKCW_Story_Player* FKCW_Story_Player::create(FKCW_Story_Layer* owner) 
{
	FKCW_Story_Player* p = new FKCW_Story_Player();
	if(p->initWithOwner(owner)) {
		p->autorelease();
		return p;
	}
	CC_SAFE_RELEASE(p);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_Story_Player::initWithOwner(FKCW_Story_Layer* owner) 
{
	m_owner = owner;
	m_msgPos	= FKCW_Base_Utils::GetLocalCenter(m_owner);
	m_namePos	= FKCW_Base_Utils::GetLocalCenter(m_owner);
	m_bgPos		= FKCW_Base_Utils::GetLocalCenter(m_owner);

	return true;
}
//-------------------------------------------------------------------------
int FKCW_Story_Player::ParserStoryScriptString( const char* szStr )
{
	std::string line;
	std::stringstream stream, output;

	stream << szStr;

	// 循环检查body
	while (!stream.eof())
	{
		size_t found_pound_sign = std::string::npos;
		// 处理下一行
		std::getline(stream, line);
		// 找到注释行
		found_pound_sign = line.find_first_of("#");
		// 检查是否有注释
		if (found_pound_sign != std::string::npos)
		{
			// 从行中移除注释
			line.erase(found_pound_sign, line.size() - found_pound_sign);
		}
		// 检查本行是否为空
		if (!line.empty())
		{
			// 当前文本是已经去除注释的文本
			// 现在开始删除头尾的空格
			line = FKCW_Base_Utils::Trim( line );
			if( !line.empty() )
			{
				// 查找关键字， 先进行拆解
				string head, in, tail;
				int nIndex = -1;
				FKCW_Base_Utils::TrimWithBrackets( line, head, in, tail );
				for( unsigned int i = 0; i < FKCW_Story_Command::s_nKeyLen-1; ++i )
				{
					if( head == s_CommandKey[i] )
					{
						nIndex = i;
						break;
					}
				}
				// 前后括号拆解
				if( nIndex == -1 )
				{
					break;
				}
				else
				{
					CCArray pParam;
					pParam.addObjectsFromArray( &FKCW_Base_Utils::ComponentOfString(in, ',') );
					switch( nIndex+1 )
					{
					case FKCW_Story_Command::WIN_SIZE:
						winsize( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::POLICY:
						policy( (ResolutionPolicy)(((CCString*)pParam.objectAtIndex(1))->intValue()));
						break;
					case FKCW_Story_Command::MSG:
						msg( ((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString,
							((CCString*)pParam.objectAtIndex(2))->floatValue() );
						break;
					case FKCW_Story_Command::MSG_SIZE:
						msgsize( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::MSG_COLOR:
						msgcolor( FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(0))->m_sString.c_str()) );
						break;
					case FKCW_Story_Command::MSG_POS:
						msgpos( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::MSG_ANCHOR:
						msganchor( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::MSG_WIDTH:
						msgwidth( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::MSG_ALIGN:
						msgalign( (cocos2d::CCTextAlignment)((CCString*)pParam.objectAtIndex(0))->intValue() );
						break;
					case FKCW_Story_Command::MSG_FONT:
						msgfont( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::NAME_POS:
						namepos( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::NAME_COLOR:
						namecolor( FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(0))->m_sString.c_str()) );
						break;
					case FKCW_Story_Command::NAME_SIZE:
						namesize( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::NAME_ANCHOR:
						nameanchor( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::NAME_FONT:
						namefont( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::LABEL_COLOR:
						labelcolor( FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(0))->m_sString.c_str()) );
						break;
					case FKCW_Story_Command::LABEL_SIZE:
						labelsize( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::LABEL_FONT:
						labelfont( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::LABEL_ANCHOR:
						labelanchor( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::LABEL_WIDTH:
						labelwidth( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::LABEL_ALIGN:
						labelalign( (cocos2d::CCTextAlignment)((CCString*)pParam.objectAtIndex(0))->intValue() );
						break;
					case FKCW_Story_Command::MOVE:
						move( ((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::WAIT:
						wait( ((CCString*)pParam.objectAtIndex(0))->floatValue() );
						break;
					case FKCW_Story_Command::WAIT_ARM:
						waitarm();
						break;
					case FKCW_Story_Command::WAIT_CLICK:
						waitclick();
						break;
					case FKCW_Story_Command::FADE_IN:
						fadein(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::FADE_OUT:
						fadeout(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::FLASH:
						flash(((CCString*)pParam.objectAtIndex(0))->m_sString,
							FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(1))->m_sString.c_str()),
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::BLINK:
						blink(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::SHAKE:
						shake(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::SCALE_TO:
						scaleto(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::IMG:
						img(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString,
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::LABEL:
						label(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString,
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::ARM:
						arm(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString,
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::ARM_PLAY:
						armplay(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString );
						break;
					case FKCW_Story_Command::ANGLE:
						angle(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::ROTATE_BY:
						rotateby(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::BG_POS:
						bgpos( ((CCString*)pParam.objectAtIndex(0))->floatValue(),
							((CCString*)pParam.objectAtIndex(1))->floatValue() );
						break;
					case FKCW_Story_Command::BG:
						bg( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::BG_COLOR:
						bgcolor( FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(0))->m_sString.c_str()) );
						break;
					case FKCW_Story_Command::Z:
						z( ((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->intValue() );
						break;
					case FKCW_Story_Command::BGM:
						bgm( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::SOUND:
						sound( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::REMOVE:
						remove( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::SHOW:
						show( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::HIDE:
						hide( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::FLIP_X:
						flipx( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::FLIP_Y:
						flipy( ((CCString*)pParam.objectAtIndex(0))->m_sString );
						break;
					case FKCW_Story_Command::POS:
						pos(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::NODE_POS:
						npos(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->m_sString,
							((CCString*)pParam.objectAtIndex(2))->floatValue(),
							((CCString*)pParam.objectAtIndex(3))->floatValue());
						break;
					case FKCW_Story_Command::SCREEN_POS:
						spos(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::ANCHOR:
						anchor(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					case FKCW_Story_Command::COLOR:
						color(((CCString*)pParam.objectAtIndex(0))->m_sString,
							FKCW_Base_Utils::Hex2I(((CCString*)pParam.objectAtIndex(1))->m_sString.c_str()) );
						break;
					case FKCW_Story_Command::SCALE:
						scale(((CCString*)pParam.objectAtIndex(0))->m_sString,
							((CCString*)pParam.objectAtIndex(1))->floatValue(),
							((CCString*)pParam.objectAtIndex(2))->floatValue());
						break;
					default:
						FKLOG("unknown story script command.");
						break;
					}
				}
			}
		}
	}

	// 更新脚本Body
	return 0;
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::start() 
{
	if(hasError() || m_done)
		return;

	// 查找第一个命令
	fetchNextCommand();

	// 找到了则执行
	if(m_curCmd) {
		executeCurrentCommand();
	} else 
	{
		m_done = true;
		m_owner->retain();
		m_owner->onStoryDone();
		m_owner->autorelease();
	}
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::onMessageDone() 
{
	// 移除消息层
	m_owner->removeChildByTag(TAG_MESSAGE_LAYER);

	// 处理下一个
	start();
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::onClickDone() 
{
	// 移除点击层
	m_owner->removeChildByTag(TAG_CLICK_LAYER);

	// 下一层
	start();
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::executeCurrentCommand() 
{
	switch (m_curCmd->getType()) 
	{
	case FKCW_Story_Command::WIN_SIZE:
		{
			// 比较显示大小
			m_oldDesignSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
			m_requiredDesignSize = ccp(m_curCmd->m_param.winsize.w, m_curCmd->m_param.winsize.h);
			m_needRestoreDesignSize = true;
			CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_requiredDesignSize.x, m_requiredDesignSize.y, kResolutionShowAll);

#ifdef FKCW_STORY_DESIGNER_TOOL
			CCNotificationCenter::sharedNotificationCenter()->postNotification("kCCNotificationFrameSizeChanged");
#endif
			start();
			break;
		}
	case FKCW_Story_Command::POLICY:
		{
			m_oldDesignSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
			m_needRestoreDesignSize = true;
			m_requiredPolicy = m_curCmd->m_param.policy.p;
			CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_requiredDesignSize.x, m_requiredDesignSize.y, m_requiredPolicy);

#ifdef FKCW_STORY_DESIGNER_TOOL
			CCNotificationCenter::sharedNotificationCenter()->postNotification("kCCNotificationFrameSizeChanged");
#endif
			start();
			break;
		}
	case FKCW_Story_Command::MSG:
		{
			FKCW_Story_MessageLayer* dl = FKCW_Story_MessageLayer::create(this);
			m_owner->addChild(dl, Z_MESSAGE_LAYER, TAG_MESSAGE_LAYER);

			// 显示信息
			dl->showMessage(m_curCmd);
			break;
		}
	case FKCW_Story_Command::MSG_ANCHOR:
		{
			m_msgAnchor.x = m_curCmd->m_param.msganchor.x;
			m_msgAnchor.y = m_curCmd->m_param.msganchor.y;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_SIZE:
		{
			m_msgSize = m_curCmd->m_param.msgsize.size;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_COLOR:
		{
			m_msgColor = m_curCmd->m_param.msgcolor.c;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_POS:
		{
			m_msgPos.x = m_curCmd->m_param.msgpos.x;
			m_msgPos.y = m_curCmd->m_param.msgpos.y;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_WIDTH:
		{
			m_msgConstraints.width = m_curCmd->m_param.msgwidth.w;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_ALIGN:
		{
			m_msgAlignment = m_curCmd->m_param.msgalign.a;

			start();
			break;
		}
	case FKCW_Story_Command::MSG_FONT:
		{
			m_msgFont = m_curCmd->m_param.msgfont.fontName;

			start();
			break;
		}
	case FKCW_Story_Command::NAME_SIZE:
		{
			m_nameSize = m_curCmd->m_param.namesize.size;

			start();
			break;
		}
	case FKCW_Story_Command::NAME_COLOR:
		{
			m_nameColor = m_curCmd->m_param.namecolor.c;

			start();
			break;
		}
	case FKCW_Story_Command::NAME_FONT:
		{
			m_nameFont = m_curCmd->m_param.namefont.fontName;

			start();
			break;
		}
	case FKCW_Story_Command::NAME_POS:
		{
			m_namePos.x = m_curCmd->m_param.namepos.x;
			m_namePos.y = m_curCmd->m_param.namepos.y;

			start();
			break;
		}
	case FKCW_Story_Command::NAME_ANCHOR:
		{
			m_nameAnchor.x = m_curCmd->m_param.nameanchor.x;
			m_nameAnchor.y = m_curCmd->m_param.nameanchor.y;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_SIZE:
		{
			m_labelSize = m_curCmd->m_param.labelsize.size;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_FONT:
		{
			m_labelFont = m_curCmd->m_param.labelfont.fontName;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_COLOR:
		{
			m_labelColor = m_curCmd->m_param.labelcolor.c;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_ANCHOR:
		{
			m_labelAnchor.x = m_curCmd->m_param.labelanchor.x;
			m_labelAnchor.y = m_curCmd->m_param.labelanchor.y;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_WIDTH:
		{
			m_labelConstraints.width = m_curCmd->m_param.labelwidth.w;

			start();
			break;
		}
	case FKCW_Story_Command::LABEL_ALIGN:
		{
			m_labelAlignment = m_curCmd->m_param.labelalign.a;

			start();
			break;
		}
	case FKCW_Story_Command::WAIT:
		{
			m_owner->runAction(CCSequence::create(CCDelayTime::create(m_curCmd->m_param.wait.time),
				CCCallFunc::create(this, callfunc_selector(FKCW_Story_Player::start)),
				NULL));
			break;
		}
	case FKCW_Story_Command::WAIT_ARM:
		{
			if(m_curCmdIndex > 0) {
				FKCW_Story_Command* prevCmd = (FKCW_Story_Command*)gStoryCommandSet.objectAtIndex(m_curCmdIndex - 1);
				if(prevCmd->getType() == FKCW_Story_Command::ARM_PLAY) {
					cocos2d::extension::CCArmature* arm = getArmatureRole(prevCmd->m_param.armplay.name);
					arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FKCW_Story_Player::onArmatureAnimationDone));
				} else {
					setError("waitarm must follow an armplay command");
				}
			} else {
				setError("waitarm must follow an armplay command");
			}

			break;
		}
	case FKCW_Story_Command::WAIT_CLICK:
		{
			FKCW_Story_ClickLayer* cl = FKCW_Story_ClickLayer::create(this);
			m_owner->addChild(cl, Z_CLICK_LAYER, TAG_CLICK_LAYER);
			break;
		}
	case FKCW_Story_Command::IMG:
		{
			CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_curCmd->m_param.img.frameName);
			if(pFrame)
			{
				if(!m_roleMap.objectForKey(m_curCmd->m_param.img.name)) 
				{
					// 创建角色
					CCSprite* role = CCSprite::createWithSpriteFrame(pFrame);
					role->setPosition(ccp(m_curCmd->m_param.img.x,
						m_curCmd->m_param.img.y));
					m_owner->addChild(role);

					// 保存
					m_roleMap.setObject(role, m_curCmd->m_param.img.name);
				} 
				else 
				{
					char buf[512];
					sprintf(buf, "image role %s is already existent", m_curCmd->m_param.img.name);
					setError(buf);
				}
			} 
			else 
			{
				string filename(m_curCmd->m_param.img.frameName);
				string ext = FKCW_Base_Utils::GetPathExtension(filename);
				string path;
				if(ext != ".png" && ext != ".jpg" && ext != ".jpeg") 
				{
					path = filename + ".png";
				} 
				else
				{
					path = filename;
				}
				CCSprite* role = CCSprite::create(path.c_str());
				if(role)
				{
					role->setPosition(ccp(m_curCmd->m_param.img.x,
						m_curCmd->m_param.img.y));
					m_owner->addChild(role);

					// 保存
					m_roleMap.setObject(role, m_curCmd->m_param.img.name);
				} 
				else 
				{
					char buf[512];
					sprintf(buf, "image role: %s is not existent", m_curCmd->m_param.img.frameName);
					setError(buf);
				}
			}

			start();
			break;
		}
	case FKCW_Story_Command::LABEL:
		{
			if(!m_roleMap.objectForKey(m_curCmd->m_param.label.name)) 
			{
				// 创建Label
				CCLabelTTF* label = CCLabelTTF::create(m_curCmd->m_param.label.text,
					m_labelFont.c_str(),
					m_labelSize,
					m_labelConstraints,
					m_labelAlignment);
				label->setAnchorPoint(m_labelAnchor);
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
				label->setFontFillColor(ccc3FromInt(m_labelColor));
#endif
				label->setOpacity((m_labelColor >> 24) & 0xff);
				label->setPosition(ccp(m_curCmd->m_param.label.x,
					m_curCmd->m_param.label.y));
				m_owner->addChild(label);

				// 保存
				m_roleMap.setObject(label, m_curCmd->m_param.label.name);
			} else {
				char buf[512];
				sprintf(buf, "label role %s is already existent", m_curCmd->m_param.label.name);
				setError(buf);
			}

			start();
			break;
		}
	case FKCW_Story_Command::ARM:
		{
			cocos2d::extension::CCArmatureDataManager* adm = cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager();
			if(adm->getAnimationData(m_curCmd->m_param.arm.armName)) 
			{
				if(!m_roleMap.objectForKey(m_curCmd->m_param.arm.name)) 
				{
					cocos2d::extension::CCArmature* arm = cocos2d::extension::CCArmature::create(m_curCmd->m_param.arm.armName);
					arm->setPosition(ccp(m_curCmd->m_param.arm.x,
						m_curCmd->m_param.arm.y));
					m_owner->addChild(arm);

					// 保存
					m_roleMap.setObject(arm, m_curCmd->m_param.arm.name);
				} else {
					char buf[512];
					sprintf(buf, "armature role %s is already existent", m_curCmd->m_param.arm.name);
					setError(buf);
				}
			} else {
				char buf[512];
				sprintf(buf, "armature %s is not existent", m_curCmd->m_param.arm.armName);
				setError(buf);
			}

			start();
			break;
		}
	case FKCW_Story_Command::ARM_PLAY:
		{
			cocos2d::extension::CCArmature* arm = getArmatureRole(m_curCmd->m_param.armplay.name);
			if(arm) 
			{
				if(arm->getAnimation()->getAnimationData()->getMovement(m_curCmd->m_param.armplay.animName))
				{
					arm->getAnimation()->play(m_curCmd->m_param.armplay.animName);
					arm->updateOffsetPoint();
				} 
				else 
				{
					char buf[512];
					sprintf(buf, "animation %s is not existent", m_curCmd->m_param.armplay.animName);
					setError(buf);
				}
			}

			start();
			break;
		}
	case FKCW_Story_Command::FLASH:
		{
			CCNode* role = getRole(m_curCmd->m_param.flash.name);
			if(role) 
			{
				unsigned int unTimes = static_cast<unsigned int>(m_curCmd->m_param.flash.times);
				role->runAction(CCRepeat::create(FKCW_Action_Flash::Create(
					m_curCmd->m_param.flash.time / m_curCmd->m_param.flash.times,
					ccc3FromInt(m_curCmd->m_param.flash.c)),
					unTimes));
			}

			start();
			break;
		}
	case FKCW_Story_Command::BLINK:
		{
			CCNode* role = getRole(m_curCmd->m_param.blink.name);
			if(role) 
			{
				role->runAction(FKCW_Action_Flash::Create(m_curCmd->m_param.blink.time, ccWHITE));
			}

			start();
			break;
		}
	case FKCW_Story_Command::FLIP_X:
		{
			CCNode* role = getRole(m_curCmd->m_param.flipx.name);
			if(role)
			{
				CCSprite* sprite = dynamic_cast<CCSprite*>(role);
				cocos2d::extension::CCArmature* arm = dynamic_cast<cocos2d::extension::CCArmature*>(role);
				if(sprite) 
				{
					sprite->setFlipX(!sprite->isFlipX());
				} 
				else if(arm) 
				{
					arm->setScaleX(-arm->getScaleX());
				}
			}

			start();
			break;
		}
	case FKCW_Story_Command::FLIP_Y:
		{
			CCNode* role = getRole(m_curCmd->m_param.flipy.name);
			if(role) {
				CCSprite* sprite = dynamic_cast<CCSprite*>(role);
				cocos2d::extension::CCArmature* arm = dynamic_cast<cocos2d::extension::CCArmature*>(role);
				if(sprite) 
				{
					sprite->setFlipY(!sprite->isFlipY());
				} 
				else if(arm) 
				{
					arm->setScaleY(-arm->getScaleY());
				}
			}

			start();
			break;
		}
	case FKCW_Story_Command::SHAKE:
		{
			CCNode* role = getRole(m_curCmd->m_param.shake.name);
			if(role) {
				role->runAction(FKCW_Action_Shake::Create(m_curCmd->m_param.shake.time,
					m_curCmd->m_param.shake.amplitude));
			}

			start();
			break;
		}
	case FKCW_Story_Command::MOVE:
		{
			CCNode* role = getRole(m_curCmd->m_param.move.name);
			if(role) {
				role->runAction(CCMoveTo::create(m_curCmd->m_param.move.time,
					ccp(m_curCmd->m_param.move.x, m_curCmd->m_param.move.y)));
			}

			start();
			break;
		}
	case FKCW_Story_Command::ROTATE_BY:
		{
			CCNode* role = getRole(m_curCmd->m_param.rotateby.name);
			if(role) {
				role->runAction(CCRotateBy::create(m_curCmd->m_param.rotateby.time,
					m_curCmd->m_param.rotateby.delta));
			}

			start();
			break;
		}
	case FKCW_Story_Command::SCALE_TO:
		{
			CCNode* role = getRole(m_curCmd->m_param.scaleto.name);
			if(role) {
				role->runAction(CCScaleTo::create(m_curCmd->m_param.scaleto.time,
					m_curCmd->m_param.scaleto.to));
			}

			start();
			break;
		}
	case FKCW_Story_Command::FADE_IN:
		{
			CCNode* role = getRole(m_curCmd->m_param.fadein.name);
			if(role) {
				role->runAction(CCFadeIn::create(m_curCmd->m_param.fadein.time));
			}

			start();
			break;
		}
	case FKCW_Story_Command::FADE_OUT:
		{
			CCNode* role = getRole(m_curCmd->m_param.fadeout.name);
			if(role) {
				role->runAction(CCFadeOut::create(m_curCmd->m_param.fadeout.time));
			}

			start();
			break;
		}
	case FKCW_Story_Command::ANGLE:
		{
			CCNode* role = getRole(m_curCmd->m_param.angle.name);
			if(role) {
				role->setRotation(m_curCmd->m_param.angle.degree);
			}

			start();
			break;
		}
	case FKCW_Story_Command::BG_POS:
		{
			m_bgPos.x = m_curCmd->m_param.bgpos.x;
			m_bgPos.y = m_curCmd->m_param.bgpos.y;

			start();
			break;
		}
	case FKCW_Story_Command::BG:
		{
			CCSprite* bg = (CCSprite*)m_owner->getChildByTag(TAG_BG);
			if(!bg) 
			{
				CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_curCmd->m_param.bg.frameName);
				if(!pFrame) 
				{
					char buf[512];
					sprintf(buf, "bg frame: %s is not existent", m_curCmd->m_param.bg.frameName);
					setError(buf);
				} 
				else 
				{
					bg = CCSprite::createWithSpriteFrame(pFrame);
					bg->setPosition(m_bgPos);
					m_owner->addChild(bg, Z_BG, TAG_BG);
				}
			}

			start();
			break;
		}
	case FKCW_Story_Command::BG_COLOR:
		{
			CCLayerColor* bg = (CCLayerColor*)m_owner->getChildByTag(TAG_BG_COLOR_LAYER);
			if(!bg) 
			{
				bg = CCLayerColor::create();
				m_owner->addChild(bg, Z_BG_COLOR_LAYER);
			}
			ccColor4B c = ccc4FromInt(m_curCmd->m_param.bgcolor.c);
			bg->setColor(ccc3FromCCC4(c));
			bg->setOpacity(c.a);

			start();
			break;
		}
	case FKCW_Story_Command::BGM:
		{
			// 停止老音乐
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			// 开始新音乐
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_curCmd->m_param.bgm.musicFile);

			start();
			break;
		}
	case FKCW_Story_Command::SOUND:
		{
			// 使用音效
			SimpleAudioEngine::sharedEngine()->playEffect(m_curCmd->m_param.bgm.musicFile);

			start();
			break;
		}
	case FKCW_Story_Command::Z:
		{
			// 设置Z值
			CCNode* role = getRole(m_curCmd->m_param.z.name);
			if(role) {
				role->setZOrder(m_curCmd->m_param.z.z);
			}

			start();
			break;
		}
	case FKCW_Story_Command::REMOVE:
		{
			// 移除角色
			CCNode* role = getRole(m_curCmd->m_param.remove.name);
			if(role) {
				m_roleMap.removeObjectForKey(m_curCmd->m_param.remove.name);
				role->removeFromParent();
			}

			start();
			break;
		}
	case FKCW_Story_Command::SHOW:
		{
			// 显示角色
			CCNode* role = getRole(m_curCmd->m_param.show.name);
			if(role) {
				role->setVisible(true);
			}

			start();
			break;
		}
	case FKCW_Story_Command::HIDE:
		{
			// 隐藏角色
			CCNode* role = getRole(m_curCmd->m_param.hide.name);
			if(role) {
				role->setVisible(false);
			}

			start();
			break;
		}
	case FKCW_Story_Command::POS:
		{
			// 设置坐标
			CCNode* role = getRole(m_curCmd->m_param.pos.name);
			if(role) {
				role->setPosition(ccp(m_curCmd->m_param.pos.x, m_curCmd->m_param.pos.y));
			}

			start();
			break;
		}
	case FKCW_Story_Command::NODE_POS:
		{
			// 设置坐标
			CCNode* role = getRole(m_curCmd->m_param.npos.name);
			CCNode* refRole = getRole(m_curCmd->m_param.npos.refName);
			if(role && refRole) {
				role->setPosition(FKCW_Base_Utils::GetPoint(refRole, m_curCmd->m_param.npos.x, m_curCmd->m_param.npos.y));
			}

			start();
			break;
		}
	case FKCW_Story_Command::SCREEN_POS:
		{
			CCNode* role = getRole(m_curCmd->m_param.spos.name);
			if(role) {
				CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
				CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
				role->setPosition(ccp(origin.x + visibleSize.width * m_curCmd->m_param.spos.x,
					origin.y + visibleSize.height * m_curCmd->m_param.spos.y));
			}

			start();
			break;
		}
	case FKCW_Story_Command::ANCHOR:
		{
			CCNode* role = getRole(m_curCmd->m_param.anchor.name);
			if(role) {
				role->setAnchorPoint(ccp(m_curCmd->m_param.anchor.x, m_curCmd->m_param.anchor.y));
			}

			start();
			break;
		}
	case FKCW_Story_Command::COLOR:
		{
			CCNode* role = getRole(m_curCmd->m_param.color.name);
			if(role) {
				CCNodeRGBA* n = dynamic_cast<CCNodeRGBA*>(role);
				if(n) {
					n->setColor(ccc3FromInt(m_curCmd->m_param.color.c));
					n->setOpacity((m_curCmd->m_param.color.c >> 24) & 0xff);
				}

				CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(role);
				if(label) 
				{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
					label->setFontFillColor(ccc3FromInt(m_curCmd->m_param.color.c));
#endif
				}
			}
			start();
			break;
		}
	case FKCW_Story_Command::SCALE:
		{
			CCNode* role = getRole(m_curCmd->m_param.scale.name);
			if(role) {
				role->setScaleX(m_curCmd->m_param.scale.x);
				role->setScaleY(m_curCmd->m_param.scale.y);
			}

			start();
			break;
		}
	default:
		break;
	}
}
//-------------------------------------------------------------------------
cocos2d::extension::CCArmature* FKCW_Story_Player::getArmatureRole(const char* name) 
{
	CCObject* obj = m_roleMap.objectForKey(name);
	if(obj) {
		cocos2d::extension::CCArmature* arm = dynamic_cast<cocos2d::extension::CCArmature*>(obj);
		if(arm) {
			return arm;
		} else {
			char buf[512];
			sprintf(buf, "role %s is not an armature!", name);
			setError(buf);
			return NULL;
		}
	} else {
		char buf[512];
		sprintf(buf, "role %s is not found", name);
		setError(buf);
		return NULL;
	}
}
//-------------------------------------------------------------------------
CCNode* FKCW_Story_Player::getRole(const char* name)
{
	CCObject* obj = m_roleMap.objectForKey(name);
	if(obj) {
		CCNode* role = dynamic_cast<CCNode*>(obj);
		if(role) {
			return role;
		} else {
			char buf[512];
			sprintf(buf, "role %s is not valid!", name);
			setError(buf);
			return NULL;
		}
	} else {
		char buf[512];
		sprintf(buf, "role %s is not found", name);
		setError(buf);
		return NULL;
	}
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::setError(const string& e) 
{
	m_error = e;
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::fetchNextCommand()
{
	if(m_curCmdIndex < (int)gStoryCommandSet.count() - 1) 
	{
		m_curCmdIndex++;
		m_curCmd = (FKCW_Story_Command*)gStoryCommandSet.objectAtIndex(m_curCmdIndex);
	}
	else 
	{
		m_curCmd = NULL;
	}
}
//-------------------------------------------------------------------------
void FKCW_Story_Player::onArmatureAnimationDone(cocos2d::extension::CCArmature* armature, cocos2d::extension::MovementEventType e, const char* name) 
{
	if(e >= cocos2d::extension::COMPLETE) 
	{
		// 移除回调
		armature->getAnimation()->setMovementEventCallFunc(NULL, NULL);

		// 执行下一个命令
		start();
	}
}