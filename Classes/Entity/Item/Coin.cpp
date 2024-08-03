#include "Coin.h"
#include "Manager/GameManager.h"

bool Coin::init(int value)
{
	if (!Entity::init()) return false;
	_value = value;
	_sprite = Sprite::create(PATH_IMAGE_ITEM + "coin.png");
	this->setContentSize(_sprite->getContentSize());
	_sprite->setPosition(_contentSize / 2);
	this->addChild(_sprite);
	initBoxBody(_contentSize.width, _contentSize.height);
	setBodyType(TYPE_ITEM);
	setBodyMask(TYPE_WALL);
	auto effect = Sprite::create(PATH_IMAGE_EFFECT + "yellow.png");
	effect->runAction(RepeatForever::create(Blink::create(0.15, 1)));
	this->addChild(effect, -1);
	effect->setPosition(_contentSize / 2);
	autoRemove();
	return true;
}

void Coin::autoRemove()
{
	auto timeCounter = CallbackTimeCounter::create();
	this->addChild(timeCounter);
	timeCounter->init(30, [&, timeCounter]() {
		if (this->getOnGround()) {
			this->autoRemoveBody();
			GameManager::getInstance()->removeItem(this);
			this->removeFromParent();
		}
		});
}
