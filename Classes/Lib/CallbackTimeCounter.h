#pragma once
#include "cocos2d.h"
#include "Lib/Utility.h"
#include "Definitions/Definitions.h"
USING_NS_CC;
class LifeEntity;
class CallbackTimeCounter : public Node
{
public:
	enum Type {
		DEFAULT,
		CONDITION
	};
	bool init() { return true; }
	bool init(float time, std::function<void()> callBack);
	bool initWithCondition(std::function<void()> callBack, std::function<bool()> condition);
	void update(float dt);
	CREATE_FUNC(CallbackTimeCounter);
private:
	Type _type;
	float _time;
	float _elapsedTime;
	ScrollView* _container;
	std::function<void()> _callBack;
	std::function<bool()> _condition;
};

