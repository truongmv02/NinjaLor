#pragma once

#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Box2D/Box2D.h"

USING_NS_CC;
class PhysicsNode :public Node {
public:
	PhysicsNode();
	bool init() override;

	void initBoxBody(float width, float height, Vec2 offset = Vec2::ZERO, float x = 0, float y = 0);
	void initPolygonBody(b2Vec2* points, int count, Vec2 offset = Vec2::ZERO, float x = 0, float y = 0);
	void initCircleBody(float radius, Vec2 offset = Vec2::ZERO, float x = 0, float y = 0);

	void setPosition(float x, float y) override;
	void setBodyPosition(float x, float y);

	void setPosition(Vec2 position);
	void setBodyPosition(Vec2 position);

	void setBodyType(uint16 type);
	void setBodyMask(uint16 mask);

	uint16 getBodyType();
	uint16 getBodyMask();
	void removeBody();
	void autoRemoveBody();
	void remove();

	CC_SYNTHESIZE(b2Body*, _body, Body);
	CC_SYNTHESIZE(b2Fixture*, _fixture, Fixture);
	CC_SYNTHESIZE(Vec2, _bodyOffset, BodyOffset);
	CC_SYNTHESIZE(Size, _bodySize, BodySize);
	CREATE_FUNC(PhysicsNode);
};