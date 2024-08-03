#pragma once

#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Box2D/Box2D.h"
#include "Lib/Lib.h"

USING_NS_CC;
class Entity :public PhysicsNode {
public:
	Entity();
	bool init();
	virtual	void BeginContact(Entity* entity, b2Contact* contact) { }
	virtual	void EndContact(Entity* entity, b2Contact* contact) {  }
	virtual	void PreSolve(Entity* entity, b2Contact* contact, const b2Manifold* manifold) {  }
	virtual	void PostSolve(Entity* entity, b2Contact* contact, const b2ContactImpulse* impulse) {  }

	virtual void setFlippedX(bool flip);

	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(Vec2, _dir, Dir);
	void setSprite(Sprite* sprite);
	CC_SYNTHESIZE_READONLY(Sprite*, _sprite, Sprite);

	CREATE_FUNC(Entity);
};