#pragma once

#include "cocos2d.h"
#include "Lib/InputHandler.h"
#include "LifeEntityState.h"
USING_NS_CC;
class Character;

class StateMachine
{
public:

	void pushState(LifeEntityState* state);
	void changeState(LifeEntityState* state);
	void popState();

	LifeEntityState* handleInput(InputHandler::Input input);
	void enter();
	void update(float dt);
	LifeEntityState* getCurrentState();
private:
	std::vector<LifeEntityState*> _states;
};
