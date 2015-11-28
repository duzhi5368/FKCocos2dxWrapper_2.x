//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_Interface.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RichHtml_Macros.h"
#include "FKCW_RichHtml_BaseStruct.h"
#include "FKCW_RichHtml_HelpFunc.h"
//-------------------------------------------------------------------------
// RichHtml元素列表
typedef std::vector<class FKCW_RichHtml_IRichElement*> element_list_t;
//-------------------------------------------------------------------------
// RichHtml元素
class FKCW_RichHtml_IRichElement
{
public:
	virtual ~FKCW_RichHtml_IRichElement() {}

	//-----------------------
	// 对外接口
	// Parser解析器接口
	virtual bool parse(class FKCW_RichHtml_IRichParser* parser, const char** attr = NULL) = 0;
	// 混合器接口
	virtual bool composit(class FKCW_RichHtml_IRichCompositor* compositor) = 0;
	// 渲染器接口
	virtual void render(FKCW_RichHtml_Canvas canvas) = 0;
	//-----------------------
	// 堆栈管理接口
	virtual bool pushMetricsState() = 0;
	virtual bool pushRenderState() = 0;
	//-----------------------
	// 位置矩阵接口
	virtual FKCW_RichHtml_Pos getLocalPosition() const = 0;			// 本地坐标
	virtual void setLocalPosition(FKCW_RichHtml_Pos pos) = 0;
	virtual void setLocalPositionX(short x) = 0;
	virtual void setLocalPositionY(short y) = 0;
	virtual FKCW_RichHtml_Pos getGlobalPosition() = 0;				// 全局坐标
	virtual FKCW_RichHtml_Metrics* getMetrics() = 0;					// 元素矩阵
	virtual bool scaleToElementSize() = 0;				// 进行纹理缩放，将其缩放为元素大小
	//-----------------------
	// 渲染属性接口
	virtual FKCW_RichHtml_Texture* getTexture() = 0;
	virtual bool isBatchedDrawable() = 0;
	virtual unsigned int getColor() = 0;
	virtual const char* getFontAlias() = 0;
	//-----------------------
	// 混合器属性接口
	virtual bool isCachedComposit() = 0;				// 添加到缓冲中，等待Flush
	virtual bool canLinewrap() = 0;
	virtual bool isNewlineBefore() = 0;
	virtual bool isNewlineFollow() = 0;
	virtual short getBaseline() = 0;					// 底边线Y值坐标
	virtual bool needBaselineCorrect() = 0;
	virtual void onCachedCompositBegin(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen) = 0;
	virtual void onCachedCompositEnd(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen) = 0;
	//-----------------------
	// 父子关系接口
	virtual void addChildren(FKCW_RichHtml_IRichElement* child) = 0;
	virtual element_list_t* getChildren() = 0;
	virtual void removeAllChildren() = 0;
	virtual void setParent(FKCW_RichHtml_IRichElement* parent) = 0;
	virtual FKCW_RichHtml_IRichElement* getParent() = 0;
	virtual int	getID() = 0;
	virtual FKCW_RichHtml_IRichElement* findChildByID(int _id) = 0;
};
//-------------------------------------------------------------------------
// RichHtml 解析器
// 解析富文本，注意：文本或者文件必须是UFT8格式
class FKCW_RichHtml_IRichParser
{
public:
	virtual ~FKCW_RichHtml_IRichParser() {}

	// 解析UTF8格式字符串
	virtual element_list_t* parseString(const char* utf8_str) = 0;
	// 解析一个UTF8文件
	virtual element_list_t* parseFile(const char* filename) = 0;
	// 获取RichNode容器
	virtual class FKCW_RichHtml_IRichNode* getContainer() = 0;

	// 判断是否是解释备注，若是，则无需解析
	virtual bool isPlainMode() = 0;
	virtual void setPlainMode(bool on) = 0;
};
//-------------------------------------------------------------------------
// 组合器
// 将RichHtml元素进行组合
class FKCW_RichHtml_IRichCompositor
{
public:
	virtual ~FKCW_RichHtml_IRichCompositor() {}

	virtual bool composit(FKCW_RichHtml_IRichElement* root) = 0;

	// 获取包围矩形
	virtual const FKCW_RichHtml_Rect& getRect() const = 0;
	// 重置全部状态和被缓冲的数据
	virtual void reset() = 0;

	// 获取当前混合器状态
	virtual FKCW_RichHtml_MetricsState* getMetricsState() = 0;
	virtual FKCW_RichHtml_MetricsState* pushMetricsState() = 0;
	virtual void popMetricsState() = 0;
	// 重置初始状态
	virtual FKCW_RichHtml_MetricsState* initMetricsState(const FKCW_RichHtml_MetricsState* new_init_state = NULL) = 0;

	virtual FKCW_RichHtml_RenderState* getRenderState() = 0;
	virtual FKCW_RichHtml_RenderState* pushRenderState() = 0;
	virtual void popRenderState() = 0;
	virtual FKCW_RichHtml_RenderState* initRenderState(const FKCW_RichHtml_RenderState* new_init_state = NULL) = 0;

	// 获取当前字体
	virtual class FKCW_Font_Catalog* getFont() = 0;
	// 获取RichNode容器
	virtual class FKCW_RichHtml_IRichNode* getContainer() = 0;
	// 获取根缓冲
	virtual class FKCW_RichHtml_ICompositCache* getRootCache() = 0;
};
//-------------------------------------------------------------------------
// 缓冲元素
// 组合器缓冲管理的元素
class FKCW_RichHtml_ICompositCache
{
public:
	virtual ~FKCW_RichHtml_ICompositCache() {}

	virtual void appendElement(FKCW_RichHtml_IRichElement* ele) = 0;
	virtual FKCW_RichHtml_Rect flush(class FKCW_RichHtml_IRichCompositor* compositor) = 0;
	virtual void clear() = 0;

	virtual ENUM_Alignment getHAlign() = 0;
	virtual ENUM_Alignment getVAlign() = 0;
	virtual void setHAlign(ENUM_Alignment align) = 0;
	virtual void setVAlign(ENUM_Alignment align) = 0;
	virtual void setWrapline(bool wrap) = 0;
	virtual bool isWrapline() = 0;
	virtual short getSpacing() = 0;
	virtual short getPadding() = 0;
	virtual void setSpacing(short v) = 0;
	virtual void setPadding(short v) = 0;
};
//-------------------------------------------------------------------------
// Batch元素集渲染
class FKCW_RichHtml_IRichAtlas
{
public:
	virtual ~FKCW_RichHtml_IRichAtlas() {}
	virtual void appendRichElement(FKCW_RichHtml_IRichElement* element) = 0;
	virtual void resetRichElements() = 0;
	virtual void updateRichRenderData() = 0;
};
//-------------------------------------------------------------------------
// Rich Node容器
class FKCW_RichHtml_IRichNode
{
public:
	virtual ~FKCW_RichHtml_IRichNode() {}
	virtual FKCW_RichHtml_IRichParser* getParser() = 0;
	virtual FKCW_RichHtml_IRichCompositor* getCompositor() = 0;
	virtual FKCW_RichHtml_Size getActualSize() = 0;

	virtual FKCW_RichHtml_Size getPreferredSize() = 0;
	virtual void setPreferredSize(FKCW_RichHtml_Size size) = 0;

	virtual void setStringUTF8(const char* utf8_str) = 0;
	virtual void appendStringUTF8(const char* utf8_str) = 0;
	virtual const char* getStringUTF8() = 0;

	virtual void addOverlay(FKCW_RichHtml_IRichElement* overlay) = 0;
	virtual void addCCNode(class CCNode* node) = 0;
	virtual void removeCCNode(class CCNode* node) = 0;

	virtual FKCW_RichHtml_IRichAtlas* findAtlas(class CCTexture2D* texture, unsigned int color_rgba, int zorder = ZORDER_CONTEXT) = 0;
};
//-------------------------------------------------------------------------
// 触碰消息回调
class FKCW_RichHtml_IRichEventHandler
{
public:
	virtual void onClick(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id) = 0;
	virtual void onMoved(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id, const CCPoint& location, const CCPoint& delta) = 0;
};
//-------------------------------------------------------------------------
template<typename T>
class FKCW_RichHtml_EvHandler : public FKCW_RichHtml_IRichEventHandler
{
public:
	typedef void (T::*mfunc_click_t)(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id);
	typedef void (T::*mfunc_moved_t)(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id, const CCPoint& location, const CCPoint& delta);

	FKCW_RichHtml_EvHandler(T* _t, mfunc_click_t _cf, mfunc_moved_t _mf) 
		: m_target(_t), m_clickfunc(_cf), m_movedfunc(_mf)
	{
	}
	FKCW_RichHtml_EvHandler(T* _t, mfunc_click_t _cf) 
		: m_target(_t), m_clickfunc(_cf), m_movedfunc(NULL)
	{
	}
	FKCW_RichHtml_EvHandler(T* _t, mfunc_moved_t _mf) 
		: m_target(_t), m_clickfunc(NULL), m_movedfunc(_mf)
	{
	}

	virtual void onClick(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id)
	{
		if ( m_target && m_clickfunc )
		{
			(m_target->*m_clickfunc)(root, ele, _id);
		}
	}

	virtual void onMoved(FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id, const CCPoint& location, const CCPoint& delta)
	{
		if ( m_target && m_movedfunc )
		{
			(m_target->*m_movedfunc)(root, ele, _id, location, delta);
		}
	}

protected:
	T* m_target;
	mfunc_click_t m_clickfunc;
	mfunc_moved_t m_movedfunc;
};
//-------------------------------------------------------------------------