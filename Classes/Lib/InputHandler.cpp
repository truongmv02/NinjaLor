#include "InputHandler.h"
InputHandler* InputHandler::_instance = nullptr;
InputHandler::InputHandler() {

}

InputHandler* InputHandler::getInstance() {
	if (_instance == NULL) {
		_instance = new InputHandler();
	}
	return _instance;
}

bool InputHandler::isKeyPress(Input input) {
	return _inputs[input];
}
void InputHandler::onKeyPress(Input input) {
	_inputs[input] = true;
}
void InputHandler::onKeyReleased(Input input) {
	_inputs[input] = false;
}
void InputHandler::reset() {
	for (int i = 0; i < COUNT; i++)
	{
		_inputs[i] = false;
	}
}