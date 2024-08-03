#pragma once
#include "cocos2d.h"
USING_NS_CC;


class NPC;
class Monster;

class GameMap : public TMXTiledMap
{
public:
	static GameMap* create(std::string name, int startDoor = 0);
	bool init(std::string name, int startDoor = 0);
	void initData();
	void initMonsters();
	void initNPC();
	void clearMap();
	Size getSize();
	CC_SYNTHESIZE(int, _startDoor, _startDoor);
	CC_SYNTHESIZE(int, _id, Id);
	CC_SYNTHESIZE(Vec2, _positionStart, PositionStart);
};

