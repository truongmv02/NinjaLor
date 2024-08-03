#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Utility.h"
#include "Card.h"

USING_NS_CC;

class CardMenu : public Card
{
public:
	CardMenu() { _closeCallback = nullptr; }
	static CardMenu* create(float width = 600.0f, float height = 700.0f, std::string title = "MENU");
	bool init(float width = 600.0f, float height = 700.0f, std::string title = "MENU");
	inline void setCloseCallback(std::function<void()> callback) { _closeCallback = callback; }
	Button* createButton(std::string title, Widget::ccWidgetTouchCallback callback = nullptr);
	CC_SYNTHESIZE(ScrollView*, _container, Container);
protected:
	std::function<void()> _closeCallback;
};


