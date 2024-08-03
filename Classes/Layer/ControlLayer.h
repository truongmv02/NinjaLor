#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Definitions/Definitions.h"

USING_NS_CC;
using namespace ui;

class ControlLayer : public Layer
{
public:
	bool init();
	void update(float dt);
	void initLeftControl();
	void initRightControl();
	void initSkillButton();
	CC_SYNTHESIZE(Button*, _moveLeftBtn, MoveLeftButton);
	CC_SYNTHESIZE(Button*, _moveRightBtn, MoveRightButton);
	CC_SYNTHESIZE(Button*, _jumpBtn, JumpButton);

	CC_SYNTHESIZE(Button*, _attackBtn, AttackButton);
	CC_SYNTHESIZE(Button*, _switchBtn, SwitchButton);
	CC_SYNTHESIZE(Button*, _menuBtn, MenuButton);
	CC_SYNTHESIZE(Button*, _hpBtn, HpButton);
	CC_SYNTHESIZE(Button*, _mpBtn, MpButton);
	CC_SYNTHESIZE(Button*, _openMenuBtn, OpenMenuButton);
	CREATE_FUNC(ControlLayer);
private:
	void attackButtonCallBack(Ref* sender, Widget::TouchEventType type);
	void jumpBtnCallBack(Ref* sender, Widget::TouchEventType type);
	void usePotionCallback(Ref* sender, Widget::TouchEventType type);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	Button* _skillButtons[5] = { nullptr };
	Button* _selectecdSkill;
};
