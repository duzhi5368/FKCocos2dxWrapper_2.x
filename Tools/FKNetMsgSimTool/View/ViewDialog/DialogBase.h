#ifndef _DIALOG_BASE_H_
#define _DIALOG_BASE_H_

#include "FKCWLib/FKCocos2dxWrapper.h"
/**
 * 需要继承实现
 */
class DialogBase : public CCLayer
{
public:
	//设置对话框背景色
	virtual void setDialogColor(ccColor4B color);
	//设置对话框标题栏文字
	virtual void setTitle(const char* title);
	//向对话中添加Node
	virtual void addDialogChild(CCNode* pChild);
	virtual CCSize getContentSize();
protected:
	DialogBase();
	virtual ~DialogBase();
	virtual bool init();

	//强制设置对话框大小
	virtual CCSize GetDialogSize() = 0;
	//关闭按钮事件回调
	void onCloseButtonClick(CCObject* pSender);
	virtual bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent );
private:
	FKCW_UIWidget_Label* m_pLabel;
	FKCW_UIWidget_Layout* m_pLayout;
};

#endif