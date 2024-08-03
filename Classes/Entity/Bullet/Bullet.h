#pragma once

#include "cocos2d.h"
#include "Entity/Entity.h"


USING_NS_CC;
class LifeEntity;
class Bullet :public Entity {
public:
	Bullet() { _target = nullptr; _speed = 35; _colisionCallback = nullptr; }
	static Bullet* create(LifeEntity* target, std::string  filename = PATH_IMAGE_BULLET + "enemy_bullet.png");
	bool init(LifeEntity* target, std::string  filename = PATH_IMAGE_BULLET + "enemy_bullet.png");
	virtual	void BeginContact(Entity* entity, b2Contact* contact) {
		contact->SetEnabled(false);
	}
	CC_SYNTHESIZE(LifeEntity*, _target, Target);
	CC_SYNTHESIZE(LifeEntity*, _owner, Owner);

	void update(float dt);
	inline void setColisionCallback(std::function<void()> callback) { _colisionCallback = callback; }
	CC_SYNTHESIZE(float, _speed, Speed);
protected:
	std::function<void()> _colisionCallback;

};