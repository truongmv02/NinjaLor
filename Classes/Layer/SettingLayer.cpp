#include "SettingLayer.h"
#include "Scene/MainMenuScene.h";
#include "Manager/GameManager.h"

bool SettingLayer::init()
{
	if (!InfoLayer::init("Setting")) return false;
	this->setTag(4);
	setButtonActive();
	_mainContainer->setBounceEnabled(false);
	auto btnExit = Utility::createButton("Exit");
	btnExit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	btnExit->setPosition(Vec2(_mainContainer->getContentSize().width / 2, _mainContainer->getContentSize().height * 0.1));
	btnExit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			Director::getInstance()->end();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
	_mainContainer->addChild(btnExit);
	return true;
}

void SettingLayer::clear()
{

}
