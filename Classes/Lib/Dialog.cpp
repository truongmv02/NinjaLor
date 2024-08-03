#include "Dialog.h"
#include "Lib/Utility.h"


Dialog* Dialog::create(std::string content, std::string title, Vector<Button*>buttons) {
	Dialog* dialog = new (std::nothrow) Dialog();
	if (dialog && dialog->init(content, title, buttons)) {

		dialog->autorelease();
		return dialog;
	}

	CC_SAFE_DELETE(dialog);
	return nullptr;
}

bool Dialog::init(std::string content, std::string title, Vector<Button*>buttons) {

	for (auto btn : buttons) {
		this->addChild(btn);
	}
	_closeBtn = Utility::createButton("Close");
	_closeBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_closeBtn->setScale(0.65);
	if (!title.empty()) {
		_title = Label::createWithTTF(title, DEFAULT_FONT, 48);
		_title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		this->addChild(_title);
	}
	_text = Label::createWithTTF(content, DEFAULT_FONT, 30);
	if (_text->getContentSize().width > 1300 * 0.9) {
		_text->setDimensions(1300 * 0.9, _text->getContentSize().height);
	}
	_text->setHorizontalAlignment(TextHAlignment::CENTER);
	_text->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	_text->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	auto textSize = _text->getContentSize();
	float width = textSize.width < 500 ? 600 : textSize.width + 100;
	_text->setDimensions(width - 100, _text->getContentSize().height);
	textSize = _text->getContentSize();
	float height = 40;
	if (_title) {
		height += _title->getContentSize().height + 20;
	}
	height += textSize.height + 30;
	auto btnHeight = _closeBtn->getContentSize().height * _closeBtn->getScaleY();
	if (buttons.size() >= 2) {
		height += btnHeight + 20;
	}
	else {
		height += btnHeight;
	}

	this->initWithFile(PATH_IMAGE_UI + "bg_black.png", Rect(0, 0, width, height));

	if (buttons.size() == 0)
		_closeBtn->setPosition(Vec2(width / 2, 20));
	else if (buttons.size() == 1) {
		_closeBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		_closeBtn->setPosition(Vec2(width - 40, 20));
		auto button = buttons.at(0);
		button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		button->setPosition(Vec2(40, 30));
	}
	if (_title) {
		_title->setPosition(40, _contentSize.height - 30);
	}
	Vec2 pos = _title ? Vec2(40, _title->getPosition().y - _title->getContentSize().height - 20) :
		Vec2(40, _contentSize.height - 20);
	_text->setPosition(pos);
	this->addChild(_text);


	_closeCallback = nullptr;
	_closeBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			if (_closeCallback != nullptr) {
				_closeCallback();
			}
			this->removeFromParent();
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
	this->addChild(_closeBtn);

	initBorder();
	this->setPosition(Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.1 + _contentSize.height / 2));

	auto touchEvent = EventListenerTouchOneByOne::create();
	touchEvent->setSwallowTouches(true);
	touchEvent->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

	return true;
}

void Dialog::setCloseCallback(std::function<void()> callback)
{
	_closeCallback = callback;
}

void Dialog::initBorder()
{
	auto borderTop = Utility::createBorderHorizontal(_contentSize.width);
	borderTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	borderTop->setPosition(Vec2(0, _contentSize.height));
	this->addChild(borderTop);

	auto borderBot = Utility::createBorderHorizontal(_contentSize.width);
	borderBot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	borderBot->setPosition(Vec2(0, 0));
	this->addChild(borderBot);

	auto borderLeft = Utility::createBorderVertical(_contentSize.height);
	borderLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	borderLeft->setPosition(Vec2(0, 0));
	this->addChild(borderLeft);

	auto borderRight = Utility::createBorderVertical(_contentSize.height);
	borderRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	borderRight->setPosition(Vec2(_contentSize.width, 0));
	this->addChild(borderRight);

	auto cornerTopLeft = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerTopLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cornerTopLeft->setPosition(Vec2(0, _contentSize.height));
	this->addChild(cornerTopLeft);

	auto cornerTopRight = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerTopRight->setFlippedX(true);
	cornerTopRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	cornerTopRight->setPosition(Vec2(_contentSize.width, _contentSize.height));
	this->addChild(cornerTopRight);

	auto cornerBotLeft = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerBotLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	cornerBotLeft->setFlippedY(true);
	this->addChild(cornerBotLeft);

	auto cornerBotRight = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerBotRight->setFlippedX(true);
	cornerBotRight->setFlippedY(true);
	cornerBotRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	cornerBotRight->setPosition(Vec2(_contentSize.width, 0));
	this->addChild(cornerBotRight);
}
