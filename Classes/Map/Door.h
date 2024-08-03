#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Entity/Entity.h"
USING_NS_CC;

class Door : public Entity
{
public:
	static Door* create(float width, float height, int nextDoor, std::string nextMap);
	bool init(float width, float height, int nextDoor, std::string nextMap);
	void BeginContact(Entity* entity, b2Contact* contact) override;
	CC_SYNTHESIZE(int, _nextDoor, NextDoor);
	CC_SYNTHESIZE(std::string, _nextMap, NextMap);
};

