#include "Message.h"
#include "CallbackTimeCounter.h"

Message* Message::create(std::string message, Color3B color)
{
	auto mess = new (std::nothrow) Message();
	if (mess && mess->init(message, color)) {
		mess->retain();
		return mess;
	}
	CC_SAFE_DELETE(mess);
	return nullptr;
}

void Message::run() {
	auto timeCounter = CallbackTimeCounter::create();
	timeCounter->init(3, [&]() {
		this->removeFromParent();
		this->release(); });
	this->addChild(timeCounter);
}

bool Message::init(std::string message, Color3B color)
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180))) return false;

	_message = message;

	this->setIgnoreAnchorPointForPosition(false);
	this->setContentSize(Size(800, 100));

	_position = Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.8);

	_container = Utility::createScrollView(_contentSize);
	_container->setBounceEnabled(false);
	_container->setEnabled(false);
	_container->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(_container);

	_label = Label::createWithTTF(message, DEFAULT_FONT, 48);
	_label->setTextColor(Utility::color3BToColor4B(color));
	_container->addChild(_label);

	if (_label->getContentSize().width <= _contentSize.width * 0.9) {
		_label->setPosition(_contentSize / 2);

	}
	return true;
}

void Message::update(float dt)
{
}

