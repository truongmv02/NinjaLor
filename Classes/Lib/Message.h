#pragma once
#include "cocos2d.h"
#include "Lib/Utility.h"
USING_NS_CC;

class Message : public LayerColor
{
public:
	static Message* create(std::string message, Color3B color = Color3B::WHITE);
	bool init(std::string message, Color3B color = Color3B::WHITE);
	void run();
	void update(float dt);
	CC_SYNTHESIZE(std::string, _message, Message);
private:
	ScrollView* _container;
	Label* _label;
};

