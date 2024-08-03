#include "GameScene.h"
#include "Definitions/Definitions.h"
#include "Map/Wall.h";
#include "Map/GameMap.h"
#include "Layer/ControlLayer.h"
#include "Lib/Pointer.h"
#include "Database/DataManager.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"

GameScene::GameScene()
{
	_mapName = "Konoha";
	_startDoor = 0;
	_changeMap = false;
}

GameScene* GameScene::create(std::string mapName, int startDoor, bool resetPos) {
	GameScene* scene = new (std::nothrow) GameScene();
	if (scene && scene->init(mapName, startDoor, resetPos)) {

		scene->autorelease();
		auto camPos = Vec2(scene->getCameraPositionX(), scene->getCameraPositionY());
		scene->getDefaultCamera()->setPosition(camPos);
		GameManager::getInstance()->getHUDLayer()->setPosition(camPos);
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return scene;
}


bool GameScene::init(std::string mapName, int startDoor, bool resetPos) {
	if (!Scene::init()) { return false; }
	_startDoor = startDoor;
	_mapName = mapName;
	auto gameLayer = Layer::create();
	this->addChild(gameLayer);
	GameManager::getInstance()->setGameLayer(gameLayer);

	auto character = GameManager::getInstance()->getCharacter();
	if (character == nullptr) {
		character = CharacterManager::create()->getCharacter();
		GameManager::getInstance()->setCharacter(character);
	}
	auto hudLayer = GameManager::getInstance()->getHUDLayer();

	if (!hudLayer->getControlLayer()) {
		auto controlLayer = ControlLayer::create();
		hudLayer->addChild(controlLayer, 1);
		hudLayer->setControlLayer(controlLayer);
	}

	this->addChild(hudLayer, 100);
	auto map = GameMap::create(_mapName, _startDoor);
	auto cm = CharacterManager::create()->updateMap(map->getId());

	auto p = GameManager::getInstance()->getPointer();
	map->addChild(p, 6);
	if (resetPos || character->getPosition().equals(Vec2(-1, -1))) {
		character->setPosition(map->getPositionStart());
	}

	GameManager::getInstance()->setMap(map);
	hudLayer->getControlLayer()->resume();

	gameLayer->addChild(map, -1);
	map->addChild(character, 10);
	this->scheduleUpdate();
	return true;
}


void GameScene::update(float dt) {
	if (_changeMap) {
		this->unscheduleUpdate();

		GameManager::getInstance()->clear();
		InputHandler::getInstance()->reset();
		auto hudLayer = GameManager::getInstance()->getHUDLayer();
		hudLayer->getControlLayer()->removeFromParent();
		hudLayer->setControlLayer(nullptr);
		auto cha = GameManager::getInstance()->getCharacter();
		cha->removeFromParentAndCleanup(false);
		cha->getBody()->SetLinearVelocity(b2Vec2(0, 0));

		hudLayer->removeFromParentAndCleanup(false);
		auto scene = GameScene::create(_mapName, _startDoor);
		Director::getInstance()->replaceScene(scene);
		return;
	}
	GameManager::getInstance()->step(dt);
	GameManager::getInstance()->updateMap(dt);
}



float GameScene::getCameraPositionX()
{
	auto cha = GameManager::getInstance()->getCharacter();
	auto charPos = cha->getPosition();
	auto mapSize = GameManager::getInstance()->getMap()->getSize();
	auto camPos = _defaultCamera->getPosition();

	if (charPos.x <= VISIBLE_SIZE_WIDTH / 2)
		return VISIBLE_SIZE_WIDTH / 2;

	if (charPos.x >= mapSize.width - VISIBLE_SIZE_WIDTH / 2)
		return mapSize.width - VISIBLE_SIZE_WIDTH / 2;
	return charPos.x;
}

float GameScene::getCameraPositionY()
{
	auto cha = GameManager::getInstance()->getCharacter();
	auto charPos = cha->getPosition();
	auto mapSize = GameManager::getInstance()->getMap()->getSize();
	auto camPos = _defaultCamera->getPosition();

	if (charPos.y <= VISIBLE_SIZE_HEIGHT * (0.6))
		return VISIBLE_SIZE_HEIGHT * (0.6);

	if (charPos.y >= mapSize.height - VISIBLE_SIZE_HEIGHT * 0.6) {
		return mapSize.height - VISIBLE_SIZE_HEIGHT * 0.6;
	}

	return charPos.y + VISIBLE_SIZE_HEIGHT * 0.1;
}


//void GameScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t parentFlags)
//{
//	Node::draw(renderer, transform, parentFlags);
//
//	auto director = Director::getInstance();
//	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
//
//	GameManager::getInstance()->_debugDrawInst->GetDrawNode()->clear();
//	GameManager::getInstance()->getWorld()->DrawDebugData();
//	GameManager::getInstance()->_debugDrawInst->GetDrawNode()->draw(renderer, transform, parentFlags);
//	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//}
