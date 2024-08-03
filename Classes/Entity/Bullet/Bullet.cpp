#include "Bullet.h"
#include "Entity/Entities.h"
#include "Manager/GameManager.h"
Bullet* Bullet::create(LifeEntity* target, std::string filename)
{
	auto b = new (std::nothrow) Bullet();
	if (b && b->init(target, filename)) {
		b->autorelease();
		return b;
	}

	CC_SAFE_DELETE(b);
	return nullptr;
}

bool Bullet::init(LifeEntity* target, std::string filename)
{
	if (!Entity::init()) return false;
	_target = target;
	auto sprite = Sprite::create(filename);
	setSprite(sprite);
	initCircleBody(_contentSize.width / 2, Vec2(_contentSize.width / 2, 0));
	setBodyType(0);
	setBodyMask(0);
	auto action = RotateBy::create(0.03, _speed);
	_sprite->runAction(RepeatForever::create(action));
	this->scheduleUpdate();
	GameManager::getInstance()->pushBullet(this);
	return true;
}

void Bullet::update(float dt)
{
	if (_target == nullptr) return;
	auto  position = Vec2(_position.x, _position.y + _contentSize.height / 2);
	auto  targetPosition = Vec2(_target->getPositionX(), _target->getPositionY() + _target->getContentSize().height / 2);
	auto dir = (targetPosition - position).getNormalized() * _speed;
	_body->SetLinearVelocity(b2Vec2(dir.x, dir.y));
	if (Vec2(targetPosition - position).getLength() < 35) {
		if (_colisionCallback) {
			_colisionCallback();
		}
		GameManager::getInstance()->removeBullet(this);
		this->unscheduleUpdate();
		this->autoRemoveBody();
		this->removeFromParent();
	}
}
