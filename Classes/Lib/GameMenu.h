#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GameMenu : public ListView {
public:

	enum class  Type {
		DEFAULT,
		CLICK_OUTSIDE_CLOSE
	};

	GameMenu();
	bool init();
	static GameMenu* create(Size size, Type type = Type::DEFAULT, Direction direction = ScrollView::Direction::HORIZONTAL);
	bool init(Size size, Type type = Type::DEFAULT, Direction direction = ScrollView::Direction::HORIZONTAL);

	void open();
	void close();

	void centerItems();
	bool onTouchBegan(Touch* touch, Event* event);

	CC_SYNTHESIZE_READONLY(Type, _type, Type);
	CREATE_FUNC(GameMenu);
};