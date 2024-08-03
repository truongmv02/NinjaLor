#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Utility.h"
#include "ui/CocosGUI.h"

using namespace ui;

USING_NS_CC;

class Dialog : public Sprite
{
public:
	Dialog() { _closeCallback = nullptr; _title = nullptr; _text = nullptr; }
	static Dialog* create(std::string content, std::string title = "", Vector<Button*> buttons = Vector<Button*>());
	bool init(std::string content, std::string title = "", Vector<Button*> buttons = Vector<Button*>());
	void setCloseCallback(std::function<void()> callback);
	CC_SYNTHESIZE(Button*, _closeBtn, CloseButton);
	CC_SYNTHESIZE(Label*, _title, Title);
	CC_SYNTHESIZE(Label*, _text, Text);
protected:
	std::function<void()> _closeCallback;
	void initBorder();
};