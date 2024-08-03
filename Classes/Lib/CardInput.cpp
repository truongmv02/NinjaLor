#include "CardInput.h"
#include "Lib/Utility.h"

CardInput* CardInput::create(std::string title, float width, float height) {
	auto cardInput = new (std::nothrow) CardInput();
	if (cardInput && cardInput->init(title, width, height)) {
		cardInput->autorelease();
		return cardInput;
	}
	CC_SAFE_DELETE(cardInput);
	return nullptr;
}

bool CardInput::init(std::string title, float width, float height) {
	_anchorPoint = Vec2::ANCHOR_MIDDLE;
	_card = Card::create(width, height, title);
	_card->setPosition(VISIBLE_SIZE / 2);
	this->addChild(_card);

	initTextField();

	_closeBtn = Utility::createButton("Close");
	_closeBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_closeBtn->setPosition(Vec2(10, 10));
	this->addChild(_closeBtn);
	_closeBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			if (_closeCallback) {
				_closeCallback();
			}
			_textFieldBg->release();
			_textFieldBgFocus->release();
			this->removeFromParent();
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

	_confirmBtn = Utility::createButton("A");
	_confirmBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_confirmBtn->setPosition(Vec2(VISIBLE_SIZE_WIDTH - 10, 10));
	this->addChild(_confirmBtn);

	auto listenerTouch = EventListenerTouchOneByOne::create();
	listenerTouch->setSwallowTouches(true);
	listenerTouch->onTouchBegan = [&](Touch* touch, Event* envent) {
		_textField->setFocused(false);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, this);
	this->scheduleUpdate();
	return true;
}

void CardInput::update(float dt)
{
	if (_textField->isFocused() && _card->getChildByName("BgNormal")) {
		_textFieldBg->removeFromParentAndCleanup(false);
		_card->addChild(_textFieldBgFocus, 0);
	}
	if (!_textField->isFocused() && _card->getChildByName("BgFocus")) {
		_textFieldBgFocus->removeFromParentAndCleanup(false);
		_card->addChild(_textFieldBg, 0);
	}
	if (_updateCallback) {
		_updateCallback();
	}

}

void CardInput::initButtonConfirm(std::string name, Widget::ccWidgetTouchCallback callback)
{
	((Label*)_confirmBtn->getChildByName("Text"))->setString(name);
	_confirmBtn->addTouchEventListener(callback);
}

void CardInput::setCardPosition(Vec2 position)
{
	_card->setPosition(position);
}

void CardInput::initTextField() {
	_textFieldWidth = _card->getContentSize().width * 0.86;

	_textFieldBg = createTextFieldBackground(PATH_IMAGE_UI + "input");
	_textFieldBg->setName("BgNormal");
	_textFieldBgFocus = createTextFieldBackground(PATH_IMAGE_UI + "input_focus");
	_textFieldBgFocus->setName("BgFocus");
	_textFieldBgFocus->retain();
	_textFieldBg->retain();

	_card->addChild(_textFieldBg);
	_textField = TextField::create("", DEFAULT_FONT, 48);
	_textField->setPosition(Vec2(_card->getContentSize().width * 0.1, _card->getContentSize().height * 0.24));
	_textField->setCursorEnabled(true);
	_textField->setMaxLengthEnabled(true);
	_textField->setMaxLength(30);
	_textField->setTouchAreaEnabled(true);
	_textField->setTouchSize(Size(_textFieldWidth * 0.9, _textFieldBg->getContentSize().height));
	_textField->setContentSize(_textField->getTouchSize());
	_textField->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_textField->setColor(Color3B::BLACK);
	_card->addChild(_textField, 10);
	_textField->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			_textField->setFocused(true);
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
}

Sprite* CardInput::createTextFieldBackground(std::string path)
{
	auto input1 = Sprite::create(path + "1.png");
	auto input2 = Sprite::create(path + "2.png");
	auto input3 = Sprite::create(path + "3.png");
	input2->setName("input2");
	input3->setName("input3");
	//input1->retain();
	//input2->retain();
	//input3->retain();

	input2->setScaleX((_textFieldWidth - input1->getContentSize().width * 2) / input2->getContentSize().width);

	input2->setPosition(input1->getContentSize().width, 0);

	input1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	input2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	input3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto pos = Vec2(_card->getContentSize().width * 0.07, _card->getContentSize().height * 0.24);
	input1->setPosition(pos);
	input2->setPositionX(input1->getContentSize().width);
	input3->setPositionX(input2->getContentSize().width);

	input1->addChild(input2);
	input2->addChild(input3);
	return input1;
}

//void CardInput::initOptions(ccMenuCallback callBack) {
//	auto option = Utility::createMenuItemSmall("");
//	option->setName("Option");
//	option->setCallback(callBack);
//	auto itemClose = Utility::createMenuItemSmall("Close");
//	itemClose->setName("ItemClose");
//	itemClose->setCallback([&](Ref* sender) {
//		this->removeFromParent();
//		});
//
//	_options = Menu::create(option, itemClose, nullptr);
//	auto padding = _card->getContentSize().width - option->getContentSize().width * 2;
//	auto size = Utility::alignItemsHorizontallyWithPadding(_options->getChildren(), padding);
//	_options->setContentSize(size);
//	_options->setIgnoreAnchorPointForPosition(false);
//	_options->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//	_options->setPosition(_card->getContentSize().width / 2, -VISIBLE_SIZE_HEIGHT * 0.04);
//	_card->addChild(_options);
//}
//
//void CardInput::setOption(std::string optionName, ccMenuCallback callBack) {
//	Utility::setMenu(_options, "Option", optionName, callBack);
//}
//
//void CardInput::setItemCloseCallBack(ccMenuCallback callBack) {
//	auto item = (MenuItem*)_options->getChildByName("ItemClose");
//	item->setCallback(callBack);
//}