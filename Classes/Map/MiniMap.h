#pragma once
#include "cocos2d.h"
USING_NS_CC;

class MiniMap : public LayerColor
{
public:
	bool init();
	void initMaps();
	CREATE_FUNC(MiniMap);
private:
	Button* _mapSelected;
	void touchMapCallback(Ref* sender, Widget::TouchEventType type);
	void teleCallback(Ref* sender, Widget::TouchEventType type);
	Label* _mapName;
	Sprite* _tick;
	TMXTiledMap* _map;
};

