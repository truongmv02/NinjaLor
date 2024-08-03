#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
USING_NS_CC;
using namespace ui;
class MainMenuScene :public Scene
{
public:
	bool init();
	MenuItemImage* createMenuItem(std::string title);
	void goToGameScene();
	void newGame();
	CREATE_FUNC(MainMenuScene);

};
