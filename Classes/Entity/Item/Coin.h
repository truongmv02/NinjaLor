#pragma once
#include "cocos2d.h"
#include "Entity/Entity.h"

USING_NS_CC;

class Coin : public Entity
{
public:
	Coin() { _onGround = true; }
	bool init(int value);
	void autoRemove();
	CC_SYNTHESIZE(int, _value, Value);
	CC_SYNTHESIZE(bool, _onGround, OnGround);
	CREATE_FUNC1(Coin, int);
private:
};
