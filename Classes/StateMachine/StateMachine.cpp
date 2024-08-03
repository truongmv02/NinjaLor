#include "StateMachine.h"

void StateMachine::pushState(LifeEntityState* state) {
	_states.push_back(state);
	enter();
}

void StateMachine::changeState(LifeEntityState* state) {
	if (!_states.empty()) {
		if (_states.back()->getStateID() == state->getStateID()) {
			delete state;
			return;
		}
		popState();
	}

	_states.push_back(state);
	enter();
}

void StateMachine::popState() {
	if (_states.empty()) {
		return;
	}
	delete _states.back();
	_states.pop_back();
	if (!_states.empty())
		_states.back()->enter();

}

LifeEntityState* StateMachine::handleInput(InputHandler::Input input) {
	if (!_states.empty())
		return _states.back()->handleInput(input);
	return nullptr;
}

void StateMachine::update(float dt) {
	if (!_states.empty())
		_states.back()->update(dt);
}

void StateMachine::enter() {
	if (!_states.empty()) {
		_states.back()->enter();
	}
}

LifeEntityState* StateMachine::getCurrentState() {
	if (_states.empty()) {
		return nullptr;
	}
	return _states.back();
}