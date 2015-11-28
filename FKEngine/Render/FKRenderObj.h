//*************************************************************************
//	创建日期:	2015-1-17
//	文件名称:	FKRenderObj.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKRenderObjModel.h"
//-------------------------------------------------------------------------
class FKRenderObj : public IRenderObj, public CCNode
{
public:
	FKRenderObj( RENDER_OBJ_ID id );
	~FKRenderObj();
public:
	// 更新函数
	virtual void			update( float delta );
	// 创建函数
	virtual bool			Create( const char* szFKOFileName );
	// 释放函数
	virtual void			onExit();
public:
	// 获取ID
	virtual RENDER_OBJ_ID	GetID();
	// 设置用户数据
	// 参数：pData 自定义用户数据
	virtual void			SetUserData( void* pData );
	// 获取用户数据
	virtual void*			GetUserData();
	// 获取游戏对象贴图文件名
	virtual const std::string&	GetFileName();
	// 获取游戏对象特征串名(全局唯一字符串)
	virtual const std::string&	GetDescName();
	// 清除全部挂接对象，仅保留空对象
	virtual void			Clear();
	// 完全清理
	virtual void			Release();

	// 绑定到场景上
	// 参数：pParentNode 父节点
	// 参数：nZOrder 深度Z值
	// 参数：nTag 标签
	virtual void			Attach( CCNode* pParentNode, int nZOrder, int nTag = 0 );
	// 从场景中移除
	// 参数：bIsRemove 移除后是否进行删除
	virtual void			Detach( bool bIsRemove = true );
	// 设置位置
	// 参数：tagPoint 位置
	virtual void			SetPos( const CCPoint& tagPoint );
	// 获取位置
	// 参数：[out]tagPoint 位置
	virtual void			GetPos( CCPoint& tagPoint );
	// 设置偏移量
	virtual void			SetOffset( CCPoint& tagPoint );
	// 获取偏移量			 
	virtual CCPoint			GetOffset();
	// 设置颜色
	// 参数：tagColor 颜色值
	virtual void			SetColor( const ccColor3B& tagColor );
	// 设置透明度
	// 参数：byAlpha 透明度[0,255]
	virtual void			SetOpacity( unsigned char byAlpha );
	// 设置缩放
	virtual void			SetScale( CCPoint& tagPoint );
	// 是否可见
	// 参数：bIsVisible 是否可见
	virtual void			IsVisible( bool bIsVisible );
	// 是否允许Update
	virtual void			SetEnableUpdate( bool bIsEnable );

	// 碰撞检测
	// 参数：pt 检测点
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool			HitTest( const CCPoint& pt );
	// 碰撞检测
	// 参数：rt 检测矩形
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool			HitTest( const CCRect& rt );
	// 设置是否可以被选中
	// 参数：bIsEnable 是否开启被选中检测
	virtual void			SetEnableSelect( bool bIsEnable );

	// 挂接对象
	// 参数：szLinkObjName 链接对象名
	// 参数：szLocator 挂节点名字
	// 参数：nDepth 深度
	// 参数：color 颜色修正
	// 返回值：挂接对象的ID
	virtual RENDER_OBJ_ID	AddLinkObj( const char* szLinkObjName, const char* szLocator,
		int nDepth = 0, ccColor4B color = ccc4(255, 255, 255, 255) );
	// 挂接对象
	// 参数：pRenderObj 链接对象
	// 参数：szLocator 挂节点名字
	// 参数：nDepth 深度
	// 参数：color 颜色修正
	// 返回值：挂接对象的ID
	virtual RENDER_OBJ_ID	AddLinkObj( IRenderObj* pRenderObj, const char* szLocator,
		int nDepth = 0, ccColor4B color = ccc4(255, 255, 255, 255) );
	// 删除一个挂接对象
	// 参数：unLinkObjID 挂接对象的ID
	virtual bool			RemoveLinkObj( RENDER_OBJ_ID unLinkObjID );
	// 删除一个挂接对象
	// 参数：unLinkObjID 挂接对象的ID
	// 参数：bIsRemove 是否删除本挂节对象
	virtual IRenderObj*		RemoveLinkObj( RENDER_OBJ_ID unLinkObjID, bool bIsRemove );
	// 删除全部挂接对象
	virtual void			RemoveAllLinkObj();
	// 获取一个挂节对象
	// 参数：RENDER_OBJ_ID 挂节对象ID
	virtual IRenderObj*		GetLinkObjByID( RENDER_OBJ_ID unID );
	// 更新一个部件
	// 参数：szPartObjName 部件对象名
	// 参数：szPartPlaceName 部件位置名
	// 参数：nDepth 深度
	virtual void			ChangePart( const char* szPartObjName, const char* szPartPlaceName, int nDepth = 0 );
	// 更新一个特效
	// 参数：strEffectName 特效名
	// 参数：strLocatorName 绑定点名
	// 参数：strEffectFileName 特效文件名
	// 参数：tagOffset 位置偏移量
	// 参数：color 颜色值
	// 参数：tagScale 缩放比率
	// 参数：nDepth 深度
	virtual void			ChangeEffect( const string& strEffectName, const string& strLocatorName, 
		const string& strEffectFileName, CCPoint& tagOffset, ccColor4B& color, CCPoint& tagScale, int nDepth = 0);

	// 设置动画播放速度
	virtual void			SetAnimationSpeed( float fSpeed );
	// 设置动画播放回调
	virtual void			SetAnimationCallback( IAnimationCallback* pCallback, void* pContext );
	// 播放动画，若动作处于停止状态，则默认继续播放（也可强制从头播放）
	// 参数：szAniName 动作名
	// 参数：fSpeed 播放速度
	// 参数：nLoops 播放重复次数，若为-1表示无限循环
	// 参数：bIsForceUpdate 是否强制从头播放
	virtual void			Play( const char* szAniName, float fSpeed = 1.0f, int nLoops = -1, bool bIsForceUpdate = false );
	// 暂停动画播放
	virtual void			Pause();
	// 获取动画动作列表
	// 参数：[out]AnimationNameList 动画动作列表
	virtual void			GetAnimationNames( list<string>& AnimationNameList );

	// 添加文字渲染对象
	// 参数：pRenderText 链接渲染字体
	// 参数：szLocator 挂节点名字
	// 参数：tagOffset 挂接偏移
	virtual bool			AddLinkRenderText( IRenderText* pRenderText, const char* szLocator,
		const CCPoint& tagOffset );
	// 删除文字渲染对象
	// 参数：pRenderText 需要删除的链接渲染字体
	virtual void			RemveLinkRenderText( IRenderText* pRenderText );

	// 设置模板数据
	virtual void			SetObjModel( IRenderObjModel* pObjModel );
	// 获取模板数据
	virtual IRenderObjModel* GetObjModel();
	// 设置是否在场景
	virtual void			SetInScene( bool bIsInScene );
	// 判断是否在场景中
	virtual bool			IsInScene();
	// 是否被选中
	virtual bool			IsSelected();
public:
	// 获取部件数据
	FKRenderObjModel::ObjPartMap&		GetParts();
	// 获取绑定点数据
	FKRenderObjModel::ObjLocatorMap&	GetLocators();
	// 获取特效数据
	FKRenderObjModel::ObjEffectMap&		GetEffects();
	// 删除部件
	bool RemovePart( const string& strName, bool bRemoveData = true );
	// 删除绑定点
	bool RemoveLocator( const string& strName, bool bRemoveData = true );
	// 删除特效
	bool RemoveEffect( const string& strName, bool bRemoveData = true );
	// 保存到文件中
	bool SaveToFile( const string& strFileName );
private:
	// 创建部件组
	void					__CreateParts();
	// 创建特效组
	void					__CreateEffects();
	// 创建部件
	bool					__CreatePart( FKRenderObjModel::SObjPart& tagPart );
	// 创建特效
	bool					__CreateEffect( FKRenderObjModel::SObjEffect& tagEffect );
private:
	struct SLinkObj
	{
		string			m_strLocator;
		FKRenderObj*	m_pLinkObj;
	};
	typedef map<string, IRenderable*>	ObjPartMap;
	typedef map<string, IRenderable*>	ObjLocatorMap;
	typedef map<string, IRenderable*>	ObjEffectMap;
	typedef map<RENDER_OBJ_ID,SLinkObj> ObjLinkMap;
	typedef set<IRenderText*>			ObjRenderTextSet;

	RENDER_OBJ_ID		m_unID;					// 对象唯一ID
	void*				m_pUserData;			// 用户数据指针
	string				m_strFKOFileName;		// FKO文件名
	IRenderable*		m_pRootRenderable;		// 主渲染对象

	CCPoint				m_tagPos;				// 当前位置
	CCPoint				m_tagOffset;			// 偏移位置，用以设置锚点
	FKRenderObjModel*	m_pModel;				// 当前渲染模板
	string				m_strCurAniName;		// 当前动作名

	bool				m_bIsNeedUpdate;		// 是否开启了帧更新
	bool				m_bIsCanbeSelected;		// 是否可以被选中
	bool				m_bIsInScene;			// 是否在场景中

	ObjPartMap			m_mapObjParts;			// 对象部件
	ObjLocatorMap		m_mapObjLocators;		// 绑定点
	ObjEffectMap		m_mapObjEffects;		// 特效数据
	ObjLinkMap			m_mapObjLinks;			// 连接对象
	ObjRenderTextSet	m_setObjRenderTexts;	// 连接文字渲染对象
};