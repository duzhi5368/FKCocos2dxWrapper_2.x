#ifndef _LOG_VIEW_H_
#define _LOG_VIEW_H_

#include "FKCWLib/FKCocos2dxWrapper.h"

struct SafeModeStackItem
{
	string log;
	ccColor3B fontColor;
	ccColor4B bkColor;
};

/**
 * LogView
 * 日志
 */
class LogView : public FKCW_UIWidget_Layout
{
public:
	static LogView* create(CCSize size);
public:
	//设置日志标题
	void setTitle(const char * title);
	//打开线程安全模式
	void openSafeMode();
	//输出一行记录
	void log(const char * clog);
	void log(string clog);
	void log(ccColor3B fontColor, const char * clog);
	void log(ccColor3B fontColor, string clog);
	void log(ccColor3B fontColor, ccColor4B bkColor, const char * clog);
	void log(ccColor3B fontColor, ccColor4B bkColor, string clog);
	//安全模式下输出一行记录
	//必须先打开线程安全模式，否则不会显示
	void safeModeLog(const char * clog);
	void safeModeLog(string clog);
	void safeModeLog(ccColor3B fontColor, const char * clog);
	void safeModeLog(ccColor3B fontColor, string clog);
	void safeModeLog(ccColor3B fontColor, ccColor4B bkColor, const char * clog);
	void safeModeLog(ccColor3B fontColor, ccColor4B bkColor, string clog);
	//清空日志
	void clearLog();
protected:
	virtual bool init(CCSize size);
	virtual void update(float delta);
	LogView();
	virtual ~LogView();
private:
	CCLabelTTF* m_pLabel;
	FKCW_UIWidget_ListView* m_pListView;
	bool m_isSafeMode;
	vector<SafeModeStackItem> safeLogStringStack;
};

#endif