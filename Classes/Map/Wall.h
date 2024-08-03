#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Entity/Entity.h"

USING_NS_CC;

class Wall :public Entity
{
public:
	static Wall* create(float width, float height, int type);
	bool init(float width, float height, int type);
	void PreSolve(Entity* entity, b2Contact* contact, const b2Manifold* manifold);
	CC_SYNTHESIZE(int, _type, Type);
};