#include "LoadingScene.h"

#include "Definitions/Definitions.h"
#include "Lib/Utility.h"
#include"GameScene.h"

LoadingScene* LoadingScene::create(std::string mapName, int nextDoor)
{
	auto scene = new (std::nothrow) LoadingScene();

	if (scene && scene->init(mapName, nextDoor)) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

bool LoadingScene::init(std::string mapName, int nextDoor)
{
	if (!Scene::init()) return false;

	return true;
}
