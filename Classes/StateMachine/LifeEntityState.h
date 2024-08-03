#pragma once

#include "cocos2d.h"
#include "Lib/InputHandler.h"


USING_NS_CC;
class LifeEntity;
class LifeEntityState {
public:
	LifeEntityState(LifeEntity* owner);
	virtual LifeEntityState* handleInput(InputHandler::Input input) { return nullptr; }
	virtual void update(float dt) {};
	virtual void enter() {}
	virtual int getStateID() { return _stateId; }
protected:
	int _stateId;
	LifeEntity* _owner;
};
