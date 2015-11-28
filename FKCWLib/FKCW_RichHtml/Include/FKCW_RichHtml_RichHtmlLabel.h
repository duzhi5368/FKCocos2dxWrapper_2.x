//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlLabel.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RichHtml_RichHtmlNode.h"
//-------------------------------------------------------------------------
#if FKCW_RICHHTML_USING_LUA
#include "CCLuaEngine.h"
#include "FKCW_RichHtml_RichHtmlElement.h"
#endif
//-------------------------------------------------------------------------
#if FKCW_RICHHTML_USING_LUA
class REvLuaHandler : public FKCW_RichHtml_IRichEventHandler
{
public:
	REvLuaHandler(int hclick, int hmoved) 
		: m_clickhandle(hclick), m_movedhandle(hmoved)
	{
	}

	virtual void onClick(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id)
	{
		if ( m_clickhandle )
		{
			FKCW_RichHtml_EleHTMLTouchable* touchable = dynamic_cast<FKCW_RichHtml_EleHTMLTouchable*>(ele);
			if ( CCLuaEngine::defaultEngine() && touchable )
			{
				CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
				stack->pushInt(_id);
				stack->pushString(touchable->getName().c_str(), touchable->getName().size());
				stack->pushString(touchable->getValue().c_str(), touchable->getValue().size());
				stack->executeFunctionByHandler(m_clickhandle, 3);
			}
		}
	}

	virtual void onMoved(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id, const CCPoint& location, const CCPoint& delta)
	{
		if ( m_movedhandle )
		{
			FKCW_RichHtml_EleHTMLTouchable* touchable = dynamic_cast<FKCW_RichHtml_EleHTMLTouchable*>(ele);
			if ( CCLuaEngine::defaultEngine() && touchable )
			{
				CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
				stack->pushInt(_id);
				stack->pushString(touchable->getName().c_str(), touchable->getName().size());
				stack->pushString(touchable->getValue().c_str(), touchable->getValue().size());
				stack->pushFloat(location.x);
				stack->pushFloat(location.y);
				stack->pushFloat(delta.x);
				stack->pushFloat(delta.y);
				stack->executeFunctionByHandler(m_movedhandle, 7);
			}
		}
	}

protected:
	int m_clickhandle;
	int m_movedhandle;
};
#endif	// FKCW_RICHHTML_USING_LUA
//-------------------------------------------------------------------------
#define CCNODE_UTILITY_SETTER(func_name, type_name) \
	virtual void func_name(type_name v) \
{ m_rRichNode->func_name(v); }

#define CCNODE_UTILITY_GETTER(func_name, type_name) \
	virtual type_name func_name() \
{ return m_rRichNode->func_name(); }

//-------------------------------------------------------------------------
// 自动解析HTML格式并渲染的HTML Label
// 标准HTML标签支持
//	- <p>		: id; style="white-space:nowrap; color; font; text-align:left|center|right; margin; padding; line-height(no use)"
//	- <br>		: id; 
//	- <hr>		: id; width; size; style="color"
//	- <font>	: id; face; color
//	- <u>		: id; 
//	- <table>	: id; width; align; cellspadding; cellsspacing; border; bgcolor; bordercolor; frame; rules
//	- <tr>		: id; align; valign
//	- <td>		: id; width; height; align; valign; padding; spacing; nowrap; bgcolor; bg-image; bg-rect
//	- <img>		: id; src; alt(no use); texture-rect="<TOP>px <LEFT>px <BOTTOM>px <LEFT>px"
//	- <a>		: id; name; href; bgcolor
//	- <button>	: id; name; value; bgcolor
//
//	额外支持ccbi格式标签
//	- <ccb>		: id; src; play="auto"; anim;
//-------------------------------------------------------------------------
class FKCW_RichHtml_HTMLLabel : public CCNode, public CCLabelProtocol
{
public:
	static FKCW_RichHtml_HTMLLabel* create();
	static FKCW_RichHtml_HTMLLabel* createWithString(const char* utf8_str, const CCSize& preferred_size, const char* font_alias = FKCW_Font_Default_FontAlias);
	bool initWithString(const char* utf8_str, const CCSize& preferred_size, const char* font_alias = FKCW_Font_Default_FontAlias);

	virtual void setString(const char *utf8_str);
	virtual const char* getString(void);

	// 添加字符串便捷函数
	// 如果你仅仅想在尾部添加部分HTML格式字符串，使用此函数
	virtual void appendString(const char *utf8_str);

	virtual void draw();

	// 事件处理器
	template<typename T>
	void registerClickListener(T* _target, typename FKCW_RichHtml_EvHandler<T>::mfunc_click_t _f)
	{
		registerListener(_target, new FKCW_RichHtml_EvHandler<T>(_target, _f));
	}
	template<typename T>
	void registerMovedListener(T* _target, typename FKCW_RichHtml_EvHandler<T>::mfunc_moved_t _f)
	{
		registerListener(_target, new FKCW_RichHtml_EvHandler<T>(_target, _f));
	}
	template<typename T>
	void registerListener(T* _target, typename FKCW_RichHtml_EvHandler<T>::mfunc_click_t _cf, typename FKCW_RichHtml_EvHandler<T>::mfunc_moved_t _mf)
	{
		registerListener(_target, new FKCW_RichHtml_EvHandler<T>(_target, _cf, _mf));
	}

	void registerListener(void* target, FKCW_RichHtml_IRichEventHandler* listener);
	void removeListener(void* target);

#if FKCW_RICHHTML_USING_LUA
	void registerLuaClickListener(int click_handle)
	{
		if (click_handle)
			registerListener((void*)click_handle, new REvLuaHandler(click_handle, 0));
	}
	void registerLuaMovedListener(int moved_handle)
	{
		if (moved_handle)
			registerListener((void*)moved_handle, new REvLuaHandler(0, moved_handle));
	}
	void removeLuaListener(int handle)
	{
		removeListener((void*)handle);
	}
#endif//FKCW_RICHHTML_USING_LUA


	// 常量
	CCNODE_UTILITY_SETTER(setPreferredSize,			FKCW_RichHtml_Size);
	CCNODE_UTILITY_SETTER(setPlainMode,				bool);
	CCNODE_UTILITY_SETTER(setDefaultFontAlias,		const char*);
	CCNODE_UTILITY_SETTER(setDefaultColor,			unsigned int);
	CCNODE_UTILITY_SETTER(setDefaultAlignment,		ENUM_Alignment);
	CCNODE_UTILITY_SETTER(setDefaultWrapline,		bool);
	CCNODE_UTILITY_SETTER(setDefaultSpacing,		short);
	CCNODE_UTILITY_SETTER(setDefaultPadding,		short);

	CCNODE_UTILITY_GETTER(getPreferredSize,			FKCW_RichHtml_Size);
	CCNODE_UTILITY_GETTER(isPlainMode,				bool);
	CCNODE_UTILITY_GETTER(getDefaultFontAlias,		const char*);
	CCNODE_UTILITY_GETTER(getDefaultColor,			unsigned int);
	CCNODE_UTILITY_GETTER(getDefaultAlignment,		ENUM_Alignment);
	CCNODE_UTILITY_GETTER(isDefaultWrapline,		bool);
	CCNODE_UTILITY_GETTER(getDefaultSpacing,		short);
	CCNODE_UTILITY_GETTER(getDefaultPadding,		short);


	FKCW_RichHtml_HTMLLabel();
	virtual ~FKCW_RichHtml_HTMLLabel();

private:
	class FKCW_RichHtml_RichNode* m_rRichNode;
};