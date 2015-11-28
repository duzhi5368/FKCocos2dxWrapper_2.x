//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_Base_UtilsI.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	工具类
//  测试结果:		
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
#define CREATE_TEST_BASE_UTILS_I_BUTTON(_TEST_FUNC_NAME_, _LABEL_)\
	FKCW_UIWidget_Button* button##_TEST_FUNC_NAME_  =  FKCW_UIWidget_Button::createWith9Sprite(CCSize(210, 30),\
	s_szSprite9Btn1Path, s_szSprite9Btn2Path );\
	button##_TEST_FUNC_NAME_->initText(FKCW_Base_StringConvert::a2u(#_LABEL_).c_str(), "", 14);\
	button##_TEST_FUNC_NAME_->setPosition(CCPoint(210/2, 15+2));\
	button##_TEST_FUNC_NAME_->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_UtilsILayer::onButton##_TEST_FUNC_NAME_##Click));\
	FKCW_UIWidget_Layout* pLayout##_TEST_FUNC_NAME_ = FKCW_UIWidget_Layout::create();\
	pLayout##_TEST_FUNC_NAME_->setContentSize(CCSize(210, 32));\
	m_pButtonListView->insertNodeAtLast(pLayout##_TEST_FUNC_NAME_);\
	pLayout##_TEST_FUNC_NAME_->addChild(button##_TEST_FUNC_NAME_);\
	m_pButtonListView->reloadData()
//-------------------------------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#endif
//-------------------------------------------------------------------------
class CTest_Base_UtilsILayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create( );
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);
		
		m_pListView = FKCW_UIWidget_ListView::create(CCSize(640, 500));
		m_pListView->setPosition(SCREEN_CENTER_POS + CCPoint(-120, 0));
		m_pListView->setBackgroundColor(cc4BLACK);
		m_pListView->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(m_pListView);

		m_pButtonListView = FKCW_UIWidget_ListView::create(CCSize(210, 500));
		m_pButtonListView->setPosition(SCREEN_CENTER_POS + CCPoint(330, 0));
		m_pButtonListView->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(m_pButtonListView);

		CREATE_TEST_BASE_UTILS_I_BUTTON(ConsoleLog, 在控制台输出普通语句);
		CREATE_TEST_BASE_UTILS_I_BUTTON(ConsoleLogColor, 在控制台输出带颜色的语句);
		CREATE_TEST_BASE_UTILS_I_BUTTON(FKFileLog, 在后台记录Log语句);
		CREATE_TEST_BASE_UTILS_I_BUTTON(GetUTCDataString, 显示当前时间);
		CREATE_TEST_BASE_UTILS_I_BUTTON(GetCurrentTimeMillis, 从1970-1-1到现在的秒数);
		CREATE_TEST_BASE_UTILS_I_BUTTON(ScreenShot, 截屏并保持文件);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Domain2IP, DNS解析);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Random01, 取0-1之间的随机浮点数);
		CREATE_TEST_BASE_UTILS_I_BUTTON(RandomInRange, 随机一定范围内的浮点数);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Pow2, 求平方值);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Lerp, 线性内插);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Ceil, 浮点数一定精度的无条件进位);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Floor, 对浮点数进行一定精度的舍弃);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Round, 浮点数的一定精度的四舍五入);
		CREATE_TEST_BASE_UTILS_I_BUTTON(BinarySearch, 对int数组进行二分查找);
		CREATE_TEST_BASE_UTILS_I_BUTTON(GetPathExtension, 获取文件后缀名);
		CREATE_TEST_BASE_UTILS_I_BUTTON(DeletePathExtension, 删除文件后缀名);
		CREATE_TEST_BASE_UTILS_I_BUTTON(AppendLastPathOfComponment, 为路径添加尾片段);
		CREATE_TEST_BASE_UTILS_I_BUTTON(DeleteLastPathOfComponment, 删除路径字符串最后的片段);
		CREATE_TEST_BASE_UTILS_I_BUTTON(LastPathOfComponent, 获取一个路径字符串最后的片段);
		CREATE_TEST_BASE_UTILS_I_BUTTON(LastDotIndex, 获取字符串中最后的'.'位置);
		CREATE_TEST_BASE_UTILS_I_BUTTON(LastSlashIndex, 获取字符串中最后的'\\'位置);
		CREATE_TEST_BASE_UTILS_I_BUTTON(RemoveChar, 从一个字符串中移除一个字符);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Replace, 替换字符串中的字符并返回);
		CREATE_TEST_BASE_UTILS_I_BUTTON(ReplaceChar, 替换字符串中的所有字符为);
		CREATE_TEST_BASE_UTILS_I_BUTTON(IsEndWith, 判断字符串尾部是否匹配);
		CREATE_TEST_BASE_UTILS_I_BUTTON(IsStartWith, 判断字符串头部是否匹配);
		CREATE_TEST_BASE_UTILS_I_BUTTON(ToLowerCase, 转换字符串为小写);
		CREATE_TEST_BASE_UTILS_I_BUTTON(Trim, 去空格头尾);
		CREATE_TEST_BASE_UTILS_I_BUTTON(GetNumDigits, 获取一个数字的位数);
		CREATE_TEST_BASE_UTILS_I_BUTTON(CalculateIncircleR2, 获取三角形内切圆半径);

		m_pButtonSlider = FKCW_UIWidget_Slider::create();
		m_pButtonSlider->setSliderImage(s_szSliderPath);
		m_pButtonSlider->setProgressImage(s_szProgressVPath);
		m_pButtonSlider->setBackgroundImage(s_szProgressVBGPath);
		m_pButtonSlider->setPosition(SCREEN_CENTER_POS+CCPoint(450, 0));
		m_pButtonSlider->setMaxValue( (int)(m_pButtonListView->getContainer()->getContentSize().height - m_pButtonListView->getContentSize().height) );
		m_pButtonSlider->setMinValue(0);
		m_pButtonSlider->setValue(0);
		m_pButtonSlider->setDirection(eProgressBarDirectionTopToBottom);
		m_pButtonSlider->setOnValueChangedListener(this, fkcw_uiwidget_valuechanged_selector(CTest_Base_UtilsILayer::onButtonValueChanged));
		m_pWindow->addChild(m_pButtonSlider);

		return true;
	}
public:
	void onButtonValueChanged(CCObject* pSender, int nValue)
	{
		m_pButtonListView->setContentOffset(CCPoint( 0, (float)(-nValue) ));
	}
	void onInfoValueChanged(CCObject* pSender, int nValue)
	{
		m_pListView->setContentOffset(CCPoint( 0, (float)(-nValue) ));
	}
public:
	void onButtonConsoleLogClick(CCObject* pSender)
	{
		FKCW_Base_Utils::FKConsoleLog("这是一条测试语句");
		addStringInListView(m_pListView, ">>FKCW_Base_Utils::FKConsoleLog(\"这是一条测试语句\");  已经执行，请注意控制台日志。");
	}
	void onButtonConsoleLogColorClick(CCObject* pSender)
	{
		int randA = (int)FKCW_Base_Utils::RandomInRange(0, 3);
		int randB = (int)FKCW_Base_Utils::RandomInRange(0, 3);
		int randC = (int)FKCW_Base_Utils::RandomInRange(0, 3);

		short color = 0;

		switch (randA)
		{
		case 0:
			color = color + FOREGROUND_BLUE;
			break;
		case 1:
			color = color + FOREGROUND_GREEN;
			break;
		case 2:
			color = color + FOREGROUND_RED;
			break;
		default:
			color = color + FOREGROUND_BLUE;
			break;
		}

		switch (randB)
		{
		case 0:
			color = color + BACKGROUND_BLUE;
			break;
		case 1:
			color = color + BACKGROUND_GREEN;
			break;
		case 2:
			color = color + BACKGROUND_RED;
			break;
		default:
			color = color + BACKGROUND_BLUE;
			break;
		}

		switch (randC)
		{
		case 0:
			color = color + FOREGROUND_INTENSITY;
			break;
		case 1:
			color = color + BACKGROUND_INTENSITY;
			break;
		}

		FKCW_Base_Utils::FKConsoleLogColor(color, "在控制台输出带颜色的语句");
		addStringInListView(m_pListView, ">>FKCW_Base_Utils::FKConsoleLogColor(color, \"在控制台输出带颜色的语句\");  已经执行，请注意控制台日志。");
	}
	void onButtonFKFileLogClick(CCObject* pSender)
	{
		FKCW_Base_Utils::FKFileLog("这是一条后台测试Log");
		addStringInListView(m_pListView, ">>FKCW_Base_Utils::FKFileLog(\"这是一条测试语句Log\");  请到本exe所在文件夹查找当日log文件确认测试成功。");
	}
	void onButtonGetUTCDataStringClick(CCObject* pSender)
	{
		std::string str = ">>FKCW_Base_Utils::GetUTCDataString();  reutrn > ";
		str = str + FKCW_Base_Utils::GetUTCDataString();
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonGetCurrentTimeMillisClick(CCObject* pSender)
	{
		int64_t t = FKCW_Base_Utils::GetStaticCurrentTimeSeconds();
		char buff[1024];
		sprintf(buff, ">>GetCurrentTimeMillis(); 已执行。 return > %I64d", t);
		addStringInListView(m_pListView, buff);
	}
	void onButtonScreenShotClick(CCObject* pSender)
	{
		std::string fileName = FKCW_Base_Utils::ScreenShot(NULL, "ScreenShot.png");
#if _DEBUG
		fileName = ">>FKCW_Base_Utils::ScreenShot(NULL, \"ScreenShot.png\"); 截图已经保存到:./Debug.win32/ScreenShot.png";
#else
		fileName = ">>FKCW_Base_Utils::ScreenShot(NULL, \"ScreenShot.png\"); 截图已经保存到:./Release.win32/ScreenShot.png";
#endif
		addStringInListView(m_pListView, fileName.c_str());
	}
	void onButtonDomain2IPClick(CCObject* pSender)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
		WSADATA wsa; 
		WORD wVersionRequested; 
		wVersionRequested = MAKEWORD( 2, 0 );  
		if (WSAStartup(wVersionRequested , &wsa)!=0) 
		{
			FKLOG("初始化WSA失败,解析域名必然失败...");
		}; 
		std::string ip = ">>FKCW_Base_Utils::Domain2IP(\"www.google.com\"); 已执行。 return > " + FKCW_Base_Utils::Domain2IP("www.google.com");
		addStringInListView(m_pListView, ip.c_str());
#endif
		
	}
	void onButtonCalculateIncircleR2Click(CCObject* pSender)
	{
		CCPoint points[3];
		points[0].x = 200; points[0].y = 200;
		points[1].x = 500; points[1].y = 200;
		points[2].x = FKCW_Base_Utils::RandomInRange(0, 1000); points[2].y = FKCW_Base_Utils::RandomInRange(0, 1000);

		char str[512];
		memset( str, 0, 512 );
		sprintf( str, ">>FKCW_Base_Utils::CalculateIncircleR2( x1 = %.2f, y1 = %.2f, x2 = %.2f, y2 = %.2f, \n x3 = %.2f, y3 = %.2f, ); 已执行。return > %.5f",
			points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, sqrtf(FKCW_Base_Utils::CalculateIncircleR2(points[0], points[1], points[2])));
		addStringInListView(m_pListView, str);
	}
	void onButtonRandom01Click(CCObject* pSender)
	{
		char buff[20];
		sprintf(buff, "%f", FKCW_Base_Utils::Random01());
		std::string str = ">>FKCW_Base_Utils::Random01(); 已执行。 return > ";
		str = str + buff;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonRandomInRangeClick(CCObject* pSender)
	{
		char buff[20];
		sprintf(buff, "%f", FKCW_Base_Utils::RandomInRange(10, 20));
		std::string str = ">>FKCW_Base_Utils::RandomInRange(10, 20); 已执行。 return > ";
		str = str + buff;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonPow2Click(CCObject* pSender)
	{
		float y = FKCW_Base_Utils::RandomInRange(5, 20);

		char buff[20];
		sprintf(buff, "%f", y);
		std::string str = ">>FKCW_Base_Utils::Pow2(";
		str = str + buff + "); 已执行。 return > ";

		char math[20];
		sprintf(math, "%f", FKCW_Base_Utils::Pow2(y));
		str = str + math;

		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonLerpClick(CCObject* pSender)
	{
  		float a = FKCW_Base_Utils::RandomInRange(0, 20);
		float b = FKCW_Base_Utils::RandomInRange(0, 20);
		float c = FKCW_Base_Utils::RandomInRange(0, 20);

		float d = FKCW_Base_Utils::Lerp(a, b, c);

		char cA[20];
		char cB[20];
		char cC[20];
		char cD[20];

		sprintf(cA, "%.2f", a);
		sprintf(cB, "%.2f", b);
		sprintf(cC, "%.2f", c);
		sprintf(cD, "%f", d);

		std::string str = ">>FKCW_Base_Utils::Lerp(";
		str = str + cA + ", " + cB + ", " + cC + "); 已执行。  return > " + cD;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonCeilClick(CCObject* pSender)
	{
		double a	= FKCW_Base_Utils::RandomInRange(0, 100);
		int b		= static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 5));
		double c	= FKCW_Base_Utils::Ceil(a, b);

		char cA[20];
		char cB[20];
		char cC[20];

		sprintf(cA, "%lf", a);
		sprintf(cB, "%d", b);
		sprintf(cC, "%lf", c);

		std::string str = ">>FKCW_Base_Utils::Ceil(";
		str = str + cA + ", " + cB + "); 已执行。 return > " + cC;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonFloorClick(CCObject* pSender)
	{
		double a = FKCW_Base_Utils::RandomInRange(0, 100);
		int b	= static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 5));
		double c = FKCW_Base_Utils::Floor(a, b);

		char cA[20];
		char cB[20];
		char cC[20];

		sprintf(cA, "%lf", a);
		sprintf(cB, "%d", b);
		sprintf(cC, "%lf", c);

		std::string str = ">>FKCW_Base_Utils::Floor(";
		str = str + cA + ", " + cB + "); 已执行。 return > " + cC;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonRoundClick(CCObject* pSender)
	{
		double a = FKCW_Base_Utils::RandomInRange(0, 100);
		int b = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 5));
		double c = FKCW_Base_Utils::Round(a, b);

		char cA[20];
		char cB[20];
		char cC[20];

		sprintf(cA, "%lf", a);
		sprintf(cB, "%d", b);
		sprintf(cC, "%lf", c);

		std::string str = ">>FKCW_Base_Utils::Round(";
		str = str + cA + ", " + cB + "); 已执行。 return > " + cC;
		addStringInListView(m_pListView, str.c_str());
	}
	void onButtonBinarySearchClick(CCObject* pSender)
	{
		int map[10];
		std::string sMap;
		for(int i=0; i<10; i++)
		{
			map[i] = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 10000));
			char buff[8];
			sprintf(buff, "%d, ", map[i]);
			sMap = sMap + buff;
		}
		sMap = ">>现有int数组，元素10个："+sMap;
		addStringInListView(m_pListView, sMap.c_str());

		int index = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 9));
		index = map[index];

		// 排序
		sMap.clear();
		FKCW_Base_Utils::QuickSort(map, 10);
		for(int i = 0; i < 10; ++i)
		{
			char buff[8];
			sprintf(buff, "%d, ", map[i]);
			sMap = sMap + buff;
		}
		sMap = "    >排序后："+sMap;
		addStringInListView(m_pListView, sMap.c_str());

		int out = FKCW_Base_Utils::BinarySearch(map, 10, index);
		char str[512]; 
		memset( str, 0, 512 );
		sprintf(str, "    >FKCW_Base_Utils::BinarySearch(int数组, 10, %d); 现已执行。 return > %d", index, out);
		addStringInListView(m_pListView, str);
	}
	void onButtonGetPathExtensionClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			std::string str = ">>FKCW_Base_Utils::GetPathExtension(\"Qwe.png\"); 已执行 return > " + FKCW_Base_Utils::GetPathExtension("Qwe.png");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			 std::string str = ">>FKCW_Base_Utils::GetPathExtension(\"./321.exe\"); 已执行 return > " + FKCW_Base_Utils::GetPathExtension("./321.exe");
			 addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			std::string str = ">>FKCW_Base_Utils::GetPathExtension(\"..\\Qwe.text\"); 已执行 return > " + FKCW_Base_Utils::GetPathExtension("..\\Qwe.text");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			std::string str = ">>FKCW_Base_Utils::GetPathExtension(\"E:/asd/_temp.data\"); 已执行 return > " + FKCW_Base_Utils::GetPathExtension("E:/asd/_temp.data");
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonDeletePathExtensionClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			std::string str = ">>FKCW_Base_Utils::DeletePathExtension(\"Qwe.png\"); 已执行 return > " + FKCW_Base_Utils::DeletePathExtension("Qwe.png");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			std::string str = ">>FKCW_Base_Utils::DeletePathExtension(\"./321.exe\"); 已执行 return > " + FKCW_Base_Utils::DeletePathExtension("./321.exe");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			std::string str = ">>FKCW_Base_Utils::DeletePathExtension(\"..\\Qwe.text\"); 已执行 return > " + FKCW_Base_Utils::DeletePathExtension("..\\Qwe.text");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			std::string str = ">>FKCW_Base_Utils::DeletePathExtension(\"E:/asd/_temp.data\"); 已执行 return > " + FKCW_Base_Utils::DeletePathExtension("E:/asd/_temp.data");
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonAppendLastPathOfComponmentClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			std::string str = ">>FKCW_Base_Utils::AppendLastPathOfComponment(\"/tmp\", \"/scratch.tiff\"); \n    >已执行 return > " + FKCW_Base_Utils::AppendLastPathOfComponment("/tmp", "/scratch.tiff");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			std::string str = ">>FKCW_Base_Utils::AppendLastPathOfComponment(\"/tmp//\", \"/scratch\"); \n    已执行 return > " + FKCW_Base_Utils::AppendLastPathOfComponment("/tmp//", "/scratch");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			std::string str = ">>FKCW_Base_Utils::AppendLastPathOfComponment(\"/tmp\", \"/\"); 已执行 return > " + FKCW_Base_Utils::AppendLastPathOfComponment("/tmp", "/");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			std::string str = ">>FKCW_Base_Utils::AppendLastPathOfComponment(\"/\", \"tmp/\"); 已执行 return > " + FKCW_Base_Utils::AppendLastPathOfComponment("/", "tmp/");
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonDeleteLastPathOfComponmentClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 5));
		if(i == 0)
		{
			std::string str = ">>FKCW_Base_Utils::DeleteLastPathOfComponment(\"/tmp/scratch.tiff\"); \n    >已执行 return > " + FKCW_Base_Utils::DeleteLastPathOfComponment("/tmp/scratch.tiff");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			std::string str = ">>FKCW_Base_Utils::DeleteLastPathOfComponment(\"/tmp/scratch\"); \n    已执行 return > " + FKCW_Base_Utils::DeleteLastPathOfComponment("/tmp/scratch");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			std::string str = ">>FKCW_Base_Utils::DeleteLastPathOfComponment(\"/tmp/\"); \n    已执行 return > " + FKCW_Base_Utils::DeleteLastPathOfComponment("/tmp/");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			std::string str = ">>FKCW_Base_Utils::DeleteLastPathOfComponment(\"scratch\"); \n    已执行 return > " + FKCW_Base_Utils::DeleteLastPathOfComponment("scratch");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 4)
		{
			std::string str = ">>FKCW_Base_Utils::DeleteLastPathOfComponment(\"/\"); \n    已执行 return > " + FKCW_Base_Utils::DeleteLastPathOfComponment("/");
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonLastPathOfComponentClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			std::string str = ">>FKCW_Base_Utils::LastPathOfComponent(\"/tmp\\\\scratch.tiff\"); \n    >已执行 return > " + FKCW_Base_Utils::LastPathOfComponent("/tmp\\\\scratch.tiff");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			std::string str = ">>FKCW_Base_Utils::LastPathOfComponent(\"\\\\tmp\\\\scratch\"); \n    已执行 return > " + FKCW_Base_Utils::LastPathOfComponent("\\\\tmp\\\\scratch");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			std::string str = ">>FKCW_Base_Utils::LastPathOfComponent(\"E:\\\\a/b.png\"); \n    已执行 return > " + FKCW_Base_Utils::LastPathOfComponent("E:\\\\a/b.png");
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			std::string str = ">>FKCW_Base_Utils::LastPathOfComponent(\"\\\\scratch\"); \n    已执行 return > " + FKCW_Base_Utils::LastPathOfComponent("\\\\scratch");
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonLastDotIndexClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			int index = FKCW_Base_Utils::LastDotIndex("/tmp/scratch.tiff");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastDotIndex(\"/tmp/scratch.tiff\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			int index = FKCW_Base_Utils::LastDotIndex("qwe.aswd.qwe.qqqqq.q");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastDotIndex(\"qwe.aswd.qwe.qqqqq.q\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			int index = FKCW_Base_Utils::LastDotIndex("....");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastDotIndex(\"....\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			int index = FKCW_Base_Utils::LastDotIndex(".aaaa.");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastDotIndex(\".aaaa.\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonLastSlashIndexClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 4));
		if(i == 0)
		{
			int index = FKCW_Base_Utils::LastSlashIndex("\\\\");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastSlashIndex(\"\\\\\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 1)
		{
			int index = FKCW_Base_Utils::LastSlashIndex("12345\\");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastSlashIndex(\"12345\\\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 2)
		{
			int index = FKCW_Base_Utils::LastSlashIndex("\\123\\\\aaa");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastSlashIndex(\"\\123\\\\aaa\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
		else if(i == 3)
		{
			int index = FKCW_Base_Utils::LastSlashIndex("aaa\\aaa.png");
			char buff[10];
			sprintf(buff, "%d", index);

			std::string str = ">>FKCW_Base_Utils::LastSlashIndex(\"aaa\\\\aaa.png\"); \n    >已执行 return > ";
			str = str + buff;
			addStringInListView(m_pListView, str.c_str());
		}
	}
	void onButtonRemoveCharClick(CCObject* pSender)
	{
		std::string str = "this is a food~";
		FKCW_Base_Utils::RemoveChar( str, 'i');

		std::string out = ">>string str = this is a food\n    >FKCW_Base_Utils::RemoveChar(str, \"'i'\"); \n    >已执行 return > str结果为" + str;
		addStringInListView(m_pListView, out.c_str());
	}
	void onButtonReplaceClick(CCObject* pSender)
	{
		std::string str = "this is a food!";
		str = FKCW_Base_Utils::Replace( str, "food", "pig");

		std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::Replace(str, \"food\", \"pig\"); \n    >已执行 return > \"" + str + "\"";
		addStringInListView(m_pListView, out.c_str());
	}
	void onButtonReplaceCharClick(CCObject* pSender)
	{
		std::string str = "this is a food!";
		FKCW_Base_Utils::ReplaceChar( str, 's', 'z');

		std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::ReplaceChar(str, 's', 'z'); \n    >已执行 return > str结果为:\"" + str + "\"";
		addStringInListView(m_pListView, out.c_str());
	}
	void onButtonIsEndWithClick(CCObject* pSender)
	{
		std::string str = "this is a food!";
		bool tag = FKCW_Base_Utils::IsEndWith(str, "ood");
		if (tag)
		{
			std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::IsEndWith(str, \"ood\"); \n    >已执行 return > true";
			addStringInListView(m_pListView, out.c_str());
		}
		else
		{
			std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::IsEndWith(str, \"ood\"); \n    >已执行 return > false";
			addStringInListView(m_pListView, out.c_str());
		}
	}
	void onButtonIsStartWithClick(CCObject* pSender)
	{
		std::string str = "this is a food!";
		bool tag = FKCW_Base_Utils::IsStartWith(str, "tha");
		if (tag)
		{
			std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::IsStartWith(str, \"th\"); \n    >已执行 return > true";
			addStringInListView(m_pListView, out.c_str());
		}
		else
		{
			std::string out = ">>string str = this is a food!\n    >FKCW_Base_Utils::IsStartWith(str, \"th\"); \n    >已执行 return > false";
			addStringInListView(m_pListView, out.c_str());
		}
	}
	void onButtonToLowerCaseClick(CCObject* pSender)
	{
		std::string str = "This Is A Food!";
		FKCW_Base_Utils::ToLowerCase(str);
		std::string out = ">>string str = This Is A Food!\n    >FKCW_Base_Utils::ToLowerCase(str); \n    >已执行 return > str结果为" + str;
		addStringInListView(m_pListView, out.c_str());
	}
	void onButtonTrimClick(CCObject* pSender)
	{
		std::string str = "    this is a Food!    ";
		str = FKCW_Base_Utils::Trim(str);
		std::string out = ">>string str = \"    this is a Food!    \"\n    >FKCW_Base_Utils::Trim(str); \n    >已执行 return > str结果\"" + str + "\"";
		addStringInListView(m_pListView, out.c_str());
	}
	void onButtonGetNumDigitsClick(CCObject* pSender)
	{
		int i = static_cast<int>(FKCW_Base_Utils::RandomInRange(-1000, 10000));
		int len = FKCW_Base_Utils::GetNumDigits(i);

		char math[20];
		sprintf(math, "%d", i);
		char lenC[8];
		sprintf(lenC, "%d", len);

		std::string str;
		str = str + ">>FKCW_Base_Utils::GetNumDigits(" + math + "); 已执行。 return > " + lenC;
		addStringInListView(m_pListView, str.c_str());
	}


	void exitProgress()
	{
		FKCW_UI_ProgressIndicator::Hide();
	}
public:
	CREATE_FUNC( CTest_Base_UtilsILayer );
protected:
	void addStringInListView(FKCW_UIWidget_ListView* listView, const char* contextString)
	{
		FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();

		CCLabelTTF* label = CCLabelTTF::create(FKCW_Base_StringConvert::a2u(contextString).c_str(), "", 12);
		label->setPositionX(label->getContentSize().width/2.0f + 10);
		label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
		pLayout->setContentSize(label->getContentSize() + CCSize(0, 6));
		pLayout->addChild(label);

		listView->insertNodeAtLast(pLayout);

		if(listView->getNodeCount() > 100)
			listView->removeFrontNode();

		listView->reloadData();
		listView->setContentOffsetToBottom();
	}
protected:
	FKCW_UIWidget_WidgetWindow*		m_pWindow;
	FKCW_UIWidget_ListView*			m_pListView;
	FKCW_UIWidget_Slider*			m_pButtonSlider;
	FKCW_UIWidget_ListView*			m_pButtonListView;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, UtilsI, 工具类)
//-------------------------------------------------------------------------