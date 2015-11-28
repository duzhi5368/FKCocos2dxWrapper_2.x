//-------------------------------------------------------------------------
#include "../Include/FKCW_Story_ClickLayer.h"
#include "../Include/FKCW_Story_Player.h"
//-------------------------------------------------------------------------
FKCW_Story_ClickLayer::FKCW_Story_ClickLayer() :
	m_player(NULL) {

}
//-------------------------------------------------------------------------
FKCW_Story_ClickLayer::~FKCW_Story_ClickLayer() {

}
//-------------------------------------------------------------------------
FKCW_Story_ClickLayer* FKCW_Story_ClickLayer::create(FKCW_Story_Player* player) {
	FKCW_Story_ClickLayer* l = new FKCW_Story_ClickLayer();
	if(l->initWithPlayer(player)) {
		return (FKCW_Story_ClickLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_Story_ClickLayer::initWithPlayer(FKCW_Story_Player* player) {
	if(!CCLayer::init())
		return false;

	m_player = player;

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setKeypadEnabled(true);

	return true;
}
//-------------------------------------------------------------------------
bool FKCW_Story_ClickLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Story_ClickLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
}
//-------------------------------------------------------------------------
void FKCW_Story_ClickLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	m_player->onClickDone();
}
//-------------------------------------------------------------------------
void FKCW_Story_ClickLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
	m_player->onClickDone();
}
//-------------------------------------------------------------------------
void FKCW_Story_ClickLayer::keyBackClicked() {
	m_player->onClickDone();
}
//-------------------------------------------------------------------------