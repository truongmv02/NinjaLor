#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class LoadingScene : public Scene
{
public:
	static LoadingScene* create(std::string mapName, int nextDoor);
	bool init(std::string mapName, int nextDoor);

private:
};
