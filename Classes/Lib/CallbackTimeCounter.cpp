#include "CallbackTimeCounter.h"
#include "Entity/Entities.h"
bool CallbackTimeCounter::init(float time, std::function<void()> callBack)
{
	_time = time;
	_elapsedTime = 0.0f;
	_callBack = callBack;
	_type = DEFAULT;
	this->scheduleUpdate();
	return true;
}

bool CallbackTimeCounter::initWithCondition(std::function<void()> callBack, std::function<bool()> condition)
{
	_callBack = callBack;
	_type = CONDITION;
	_condition = condition;
	this->scheduleUpdate();
	return  true;
}

void CallbackTimeCounter::update(float dt)
{
	if (_type == DEFAULT) {
		_elapsedTime += dt;
		if (_elapsedTime >= _time) {
			_callBack();
		}
	}
	else {
		if (_condition && _condition()) {
			_callBack();
		}
	}
}
