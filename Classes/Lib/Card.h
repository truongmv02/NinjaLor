#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Utility.h"
#include "ui/CocosGUI.h"

using namespace ui;

USING_NS_CC;

class Card : public Sprite
{
public:
	Card() { _title = nullptr, _bgMain = nullptr; }
	static Card* create(float width, float height, std::string title = "");
	bool init(float width, float height, std::string title = "");
	CC_SYNTHESIZE(Label*, _title, Title);
protected:
	void initBorder();
	Sprite* _bgMain;
};


