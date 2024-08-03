#include "CardMenu.h"
#include "Lib/Utility.h"


CardMenu* CardMenu::create(float width, float height, std::string title) {
	CardMenu* card = new (std::nothrow) CardMenu();
	if (card && card->init(width, height, title)) {

		card->autorelease();
		return card;
	}

	CC_SAFE_DELETE(card);
	return nullptr;
}

bool CardMenu::init(float width, float height, std::string title) {
	if (!Card::init(width, height, title)) return false;
	auto closeBtn = Button::create(PATH_IMAGE_UI + "close_btn.png", PATH_IMAGE_UI + "close_btn_press.png");
	closeBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	closeBtn->setPosition(Vec2(_contentSize.width - 20, _contentSize.height - 54));
	this->addChild(closeBtn);

	closeBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			if (_closeCallback) {
				_closeCallback();
			}
			this->removeFromParent();
			break;
		}
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

	_container = Utility::createScrollView(Size(_bgMain->getContentSize().width, height - 140), ScrollView::Direction::VERTICAL);
	_container->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_container->setPosition(Vec2(0, _bgMain->getContentSize().height - 20));
	_bgMain->addChild(_container, 10);
	auto touchEvent = EventListenerTouchOneByOne::create();
	touchEvent->setSwallowTouches(true);
	touchEvent->onTouchBegan = [&](Touch* touch, Event* event) {
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

	this->setPosition(VISIBLE_SIZE / 2);
	return true;
}

Button* CardMenu::createButton(std::string title, Widget::ccWidgetTouchCallback callback)
{

	auto btn = Button::create();
	btn->ignoreContentAdaptWithSize(false);
	btn->setContentSize(Size(_container->getContentSize().width * 0.9, 100));
	btn->addTouchEventListener(callback);
	Utility::addText(btn, title, DEFAULT_FONT, 48, Color3B::WHITE);

	DrawNode* line = DrawNode::create();
	line->drawLine(Vec2(0, 0), Vec2(btn->getContentSize().width, 0), Utility::color3BToColor4F(BORDER_BROWN));
	line->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	btn->addChild(line);
	_container->addChild(btn);
	btn->getRendererClicked()->initWithFile(PATH_IMAGE_UI + "bg_white.png", Rect(0, 0, btn->getContentSize().width, btn->getContentSize().height));
	return btn;
}
