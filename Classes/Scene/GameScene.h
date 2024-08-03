#pragma once

#include "cocos2d.h"
#include "Definitions/Definitions.h"
//#include "Lib/DebugDraw.h"

USING_NS_CC;

class GameScene :public Scene
{
public:
	GameScene();
	static GameScene* create(std::string mapName = "Uchiha Clan", int startDoor = 1, bool resetPos = true);
	bool init(std::string mapName = "Uchiha Clan", int startDoor = 2, bool resetPos = true);
	void update(float dt);
	float getCameraPositionX();
	float getCameraPositionY();
	//void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	CC_SYNTHESIZE(std::string, _mapName, MapName);
	CC_SYNTHESIZE(int, _startDoor, StartDoor);
	CC_SYNTHESIZE(bool, _changeMap, ChangeMap);
protected:
};
