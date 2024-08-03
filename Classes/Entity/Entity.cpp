#include "Entity.h"
#include "Manager/GameManager.h"

Entity::Entity() {
	_dir = Vec2(1, 0);
	_sprite = nullptr;
}

bool Entity::init() {

	return true;
}

void Entity::setFlippedX(bool flip)
{
	if (_sprite) {
		_sprite->setFlippedX(flip);
	}
}

void Entity::setSprite(Sprite* sprite)
{
	_sprite = sprite;
	this->setContentSize(sprite->getContentSize());
	_sprite->setPosition(_contentSize / 2);
	this->addChild(_sprite);
}