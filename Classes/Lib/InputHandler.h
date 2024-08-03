#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"

USING_NS_CC;

class InputHandler
{
public:
	enum  Input {
		MOVE_LEFT,
		MOVE_RIGHT,
		JUMP,
		COUNT
	};
	static InputHandler* getInstance();
	bool isKeyPress(Input input);
	void onKeyPress(Input input);
	void onKeyReleased(Input input);
	void reset();
private:
	static	InputHandler* _instance;
	InputHandler();
	bool _inputs[COUNT] = { false };
};


