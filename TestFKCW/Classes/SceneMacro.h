//*************************************************************************
//	创建日期:	2014-11-26
//	文件名称:	SceneMacro.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 测试选择面板字体间隔
const static unsigned int TEST_MENU_LINE_SPACE	= 40;
//-------------------------------------------------------------------------
// 测试场景类型
enum ENUM_TEST_SCENE_TYPE
{
	eTestSceneType_Default	= 0,

	eTestSceneType_GR_Examples,

	eTestSceneType_Font_Test,

	eTestSceneType_Database_Database,

	eTestSceneType_SimpleNet_NetObj,

	eTestSceneType_Json_Json,

	eTestSceneType_SceneEx_UISceneEx,

	eTestSceneType_3D_FKSimpleModel,

	eTestSceneType_RichHtml_HtmlLabel,

	eTestSceneType_Story_StoryLayer,

	eTestSceneType_Pixel_Demo,

	eTestSceneType_Shader_Demo1,
	eTestSceneType_Shader_Demo2,

	eTestSceneType_Network_FileDownLoader,
	eTestSceneType_Network_HttpGet,
	eTestSceneType_Network_TCP,
	eTestSceneType_Network_UDP,

	eTestSceneType_UI_AutoRenderMenuItemSprite,
	eTestSceneType_UI_MenuItemColor,
	eTestSceneType_UI_ToastLayer,
	eTestSceneType_UI_SimpleRookieGuider,
	eTestSceneType_UI_DynamicNumLabel,
	eTestSceneType_UI_MenuItemProgressTimer,
	eTestSceneType_UI_ProgressIndicator,
	eTestSceneType_UI_ScrollableLayer,
	eTestSceneType_UI_Table,
	
	eTestSceneType_Action_Blur,
	eTestSceneType_Action_ClipIn,
	eTestSceneType_Action_Count,
	eTestSceneType_Action_Flash,
	eTestSceneType_Action_Missile,
	eTestSceneType_Action_Shake,
	eTestSceneType_Action_Shine,
	eTestSceneType_Action_TreeFadeIn,
	eTestSceneType_Action_TreeFadeOut,
	eTestSceneType_Action_TrailMoveTo,
	eTestSceneType_Action_CallFuncT,
	eTestSceneType_Action_Curl,
	eTestSceneType_Action_Suck,

	eTestSceneType_UIWidget_DrawZOrder,
	eTestSceneType_UIWidget_Button,
	eTestSceneType_UIWidget_CheckBox,
	eTestSceneType_UIWidget_ColorView,
	eTestSceneType_UIWidget_ControlView,
	eTestSceneType_UIWidget_ExpandableListView,
	eTestSceneType_UIWidget_GradientView,
	eTestSceneType_UIWidget_ImageView,
	eTestSceneType_UIWidget_GridPageView,
	eTestSceneType_UIWidget_GridView,
	eTestSceneType_UIWidget_Label,
	eTestSceneType_UIWidget_Layout,
	eTestSceneType_UIWidget_ProgressBar,
	eTestSceneType_UIWidget_ScrollView,
	eTestSceneType_UIWidget_ToggleView,
	eTestSceneType_UIWidget_ListView,
	eTestSceneType_UIWidget_Slider,
	eTestSceneType_UIWidget_TextRich,
	eTestSceneType_UIWidget_LabelAtlas,
	eTestSceneType_UIWidget_PageView,
	eTestSceneType_UIWidget_TableView,

	eTestSceneType_Base_AStar,
	eTestSceneType_Base_UtilsI,
	eTestSceneType_Base_Encode,
	eTestSceneType_Base_DrawingPrimitives,
	eTestSceneType_Base_CVSLoader,
	eTestSceneType_Base_DecodeHtml,
	eTestSceneType_Base_Localization,
	eTestSceneType_Base_Memory,
	eTestSceneType_Base_PinYin,
	eTestSceneType_Base_ResourceLoader,
	eTestSceneType_Base_SecureUserDefault,
	eTestSceneType_Base_FileZip,

	eTestSceneType_RenderEx_GradientSprite,
	eTestSceneType_RenderEx_TiledSprite,
	eTestSceneType_RenderEx_LaserSprite,
	eTestSceneType_RenderEx_TurnCardSprite,
	eTestSceneType_RenderEx_EraserSprite,
	eTestSceneType_RenderEx_ScrollingBackgroundNode,
	eTestSceneType_RenderEx_JoystickNode,
	eTestSceneType_RenderEx_NumberCountNode,
	eTestSceneType_RenderEx_SimpleShaderColorPainter,
	eTestSceneType_RenderEx_CatmullRomSprite,
	eTestSceneType_RenderEx_SwayNode,
	eTestSceneType_RenderEx_BlurTextureFilter,
	eTestSceneType_RenderEx_SpotLightSprite,

	eTestSceneType_EffectNode_2DSoftShadow,
	eTestSceneType_EffectNode_BreakSprite,
	eTestSceneType_EffectNode_ColorAdjustSprite,
	eTestSceneType_EffectNode_GhostLikeSprite,
	eTestSceneType_EffectNode_Laser,
	eTestSceneType_EffectNode_LightingBolt,
	eTestSceneType_EffectNode_NormalMapped,
	eTestSceneType_EffectNode_RippleSprite,
	eTestSceneType_EffectNode_Ripple2DSprite,
	eTestSceneType_EffectNode_ShatterSprite,
	eTestSceneType_EffectNode_TailSprite,
	eTestSceneType_EffectNode_TinyWindTerrain,

	eTestSceneType_TMX_Isometric,
	eTestSceneType_TMX_Hexagonal,
	eTestSceneType_TMX_Orthogonal,
	eTestSceneType_TMX_Flip,
	// to do add...
	eTestSceneType_Max,
};
const std::string g_aTestNames[eTestSceneType_Max] = {
	"样例场景",

	"FKCW_GR_GestureRecognizer - 手势识别",

	"FKCW_Font_Test - 字体库测试",

	"FKCW_Database_Database - 数据库",

	"FKCW_SimpleNet_NetObject - TCP连接封装",

	"FKCW_Json_Parser - Json文件解析",

	"FKCW_SceneEx_UISceneEx - 场景管理对象",

	"FKCW_3D_FKSimpleModel - 3D渲染测试以及FKSimpleModel文件支持测试",

	"FKCW_RichHtml_HtmlLabel - 解析HTML并渲染富文本",

	"FKCW_Story_StoryLayer - AVG故事板脚本测试",

	"FKCW_Pixel_Demo - 像素支持库Demo测试",

	"FKCW_Shader_Demo1 - Shader效果测试1",
	"FKCW_Shader_Demo2 - Shader效果测试2",

	"FKCW_Network_FileDownLoader - 文件下载器",
	"FKCW_Network_HttpGet - Http的Get方法下载",
	"FKCW_Network_TCP - TCP连接",
	"FKCW_Network_UDP - UDP连接",

	"FKCW_UI_AutoRenderMenuItemSprite - 自亮暗菜单选项精灵",
	"FKCW_UI_MenuItemColor - 纯色选项精灵",
	"FKCW_UI_ToastLayer - 弹出Tip框",
	"FKCW_UI_SimpleRookieGuiderLayer - 新手引导层",
	"FKCW_UI_DynamicNumLabel - 动态数字标签",
	"FKCW_UI_MenuItemProgressTimer - 带进度时间的菜单选项",
	"FKCW_UI_ProgressIndicator - 进度提示器",
	"FKCW_UI_ScrollableLayer - 滚动标签层",
	"FKCW_UI_Table - 表单Table控件",
	
	"FKCW_Action_Blur - 模糊动作",
	"FKCW_Action_ClipIn - 渐入渐出动作",
	"FKCW_Action_Count - 数字翻转动作",
	"FKCW_Action_Flash - 闪光动作",
	"FKCW_Action_Missile - 导弹动作",
	"FKCW_Action_Shake - 震动动作",
	"FKCW_Action_Shine - 光线动作",
	"FKCW_Action_TreeFadeIn - 渐显动作",
	"FKCW_Action_TreeFadeOut - 渐隐动作",
	"FKCW_Action_TrailMoveTo - 拖尾移动动作",
	"FKCW_Action_CallFuncT - 模板参数回调动作",
	"FKCW_Action_Curl - 螺旋旋转动作",
	"FKCW_Action_Suck - 黑洞吸入动作",

	"FKCW_UIWidget_DrawZOrder - 调整UIWidget控件绘制顺序",
	"FKCW_UIWidget_Button - 按钮",
	"FKCW_UIWidget_CheckBox - 选择框",
	"FKCW_UIWidget_ColorView - 颜色块View",
	"FKCW_UIWidget_ControlView - 摇杆控件",
	"FKCW_UIWidget_ExpandableListView - 可扩展列表",
	"FKCW_UIWidget_GradientView - 渐变控件",
	"FKCW_UIWidget_ImageView - 图片控件",
	"FKCW_UIWidget_GridPageView - 网格容器控件(横向)",
	"FKCW_UIWidget_GridView - 网格容器控件(纵向)",
	"FKCW_UIWidget_Label - 静态文字标签",
	"FKCW_UIWidget_Layout - 空容器布局",
	"FKCW_UIWidget_ProgressBar - 进度条",
	"FKCW_UIWidget_ScrollView - 滑动容器",
	"FKCW_UIWidget_ToggleView - 开关View",
	"FKCW_UIWidget_ListView - 链表滑动容器",
	"FKCW_UIWidget_Slider - 滑块",
	"FKCW_UIWidget_TextRich - 富文本",
	"FKCW_UIWidget_LabelAtlas - 静态标签集",
	"FKCW_UIWidget_PageView - 页面滑动的容器",
	"FKCW_UIWidget_TableView - 纵向滚动表格",

	"FKCW_Base_AStar - A*寻路演示",
	"FKCW_Base_Utils - 工具类",
	"FKCW_Base_MD5_Base64_SHA1 - 加密解密类",
	"FKCW_Base_DrawingPrimitives - 基本绘制元素",
	"FKCW_Base_CVSLoader - CVS格式加载器",
	"FKCW_Base_DecodeHtml - HTML格式解析处理器",
	"FKCW_Base_Localization - 自适应多国语言表",
	"FKCW_Base_Memroy - 内存泄露检查",
	"FKCW_Base_PinYin - 拼音库",
	"FKCW_Base_ResourceLoader - 独立线程资源加载",
	"FKCW_Base_SecureUserDefault - 加密标准用户配置文件",
	"FKCW_Base_FileZip - 文件压缩解压缩",

	"FKCW_RenderEx_GradientSprite - 会渐变效果的精灵",
	"FKCW_RenderEx_TiledSprite - 填充精灵",
	"FKCW_RenderEx_LaserSprite - 镭射激光精灵",
	"FKCW_RenderEx_TurnCardSprite - 卡牌（翻页）精灵",
	"FKCW_RenderEx_EraserSprite - 橡皮擦擦除精灵",
	"FKCW_RenderEx_ScrollingBackgroundNode - 可无尽滚动的节点",
	"FKCW_RenderEx_JoystickNode - 虚拟摇杆节点",
	"FKCW_RenderEx_NumberCountNode - 可变速翻动的数字精灵",
	"FKCW_RenderEx_SimpleShaderColorPainter - 简单的node颜色修正涂色器",
	"FKCW_RenderEx_CatmullRomSprite - 笛卡尔差值扭曲精灵",
	"FKCW_RenderEx_SwayNode - 可上部摇曳的精灵节点",
	"FKCW_RenderEx_BlurTextureFilter - 纹理模糊处理器",
	"FKCW_RenderEx_SpotLightSprite - 聚光灯效果精灵",

	"FKCW_EffectNode_2DSoftShadow - 2D软阴影节点",
	"FKCW_EffectNode_BreakSprite - 破碎掉落精灵",
	"FKCW_EffectNode_ColorAdjustSprite - hsv色彩调整精灵",
	"FKCW_EffectNode_GhostLikeSprite - 幽灵精灵",
	"FKCW_EffectNode_Laser - 激光射线",
	"FKCW_EffectNode_LightingBolt - 闪电精灵",
	"FKCW_EffectNode_NormalMapped - 法线精灵",
	"FKCW_EffectNode_RippleSprite - 波纹涟漪精灵",
	"FKCW_EffectNode_Ripple2DSprite - 2D波纹涟漪精灵",
	"FKCW_EffectNode_ShatterSprite - 中心破碎精灵",
	"FKCW_EffectNode_TailSprite - 拖尾残影精灵",
	"FKCW_EffectNode_TinyWindTerrain - 游戏TinyWind类似的地形",

	"FKCW_TMX_Map[Isometric] - 斜45度地图测试",
	"FKCW_TMX_Map[Hexagonal] - 六角形地图测试",
	"FKCW_TMX_Map[Orthogonal] - 多层直角地图测试",
	"FKCW_TMX_Map[Flip] - 单元镜像地图测试",
	// to do add...
};
//--------------------------------------------------------