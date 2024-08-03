#include "GameMap.h"
#include "Definitions/Definitions.h"
#include "Wall.h"
#include "Door.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"
#include "Database/EnemyManager.h"
#include "Database/MapManager.h"

GameMap* GameMap::create(std::string name, int startDoor) {
	GameMap* gameMap = new (std::nothrow) GameMap();
	if (gameMap && gameMap->init(name, startDoor)) {

		gameMap->autorelease();
		return gameMap;
	}
	CC_SAFE_DELETE(gameMap);
	return nullptr;
}

bool GameMap::init(std::string name, int startDoor) {
	if (!TMXTiledMap::initWithTMXFile(PATH_MAP + name + ".tmx")) {
		return false;
	}
	_name = name;
	_id = AManager::create()->getMapId(name);
	_startDoor = startDoor;
	initData();
	auto background = Sprite::create(PATH_MAP + "/Background/" + name + ".png");
	if (background) {

		background->setAnchorPoint(Vec2::ZERO);
		this->addChild(background, -1);
	}
	return true;
}


void GameMap::initData() {
	auto objects = getObjectGroup("walls");
	ValueVector walls = objects->getObjects();
	for (int i = 0; i < walls.size(); i++)
	{
		ValueMap value = walls.at(i).asValueMap();

		int type = value.at("type").asInt();

		float w = value.at("width").asFloat();
		float h = value.at("height").asFloat();

		int x = value.at("x").asInt();
		int y = value.at("y").asInt();

		auto wall = Wall::create(w, h, type);
		wall->setPosition(x + w / 2, y);
		this->addChild(wall);
	}
	objects = getObjectGroup("doors");
	ValueVector doors = objects->getObjects();

	for (int i = 0; i < doors.size(); i++)
	{
		ValueMap value = doors.at(i).asValueMap();
		int x = value.at("x").asInt();
		int y = value.at("y").asInt();
		if (value.at("name").asString() == "door") {
			float w = value.at("width").asFloat();
			float h = value.at("height").asFloat();
			int nextDoor = value.at("next_door").asInt();
			std::string nextMap = value.at("next_map").asString();

			auto door = Door::create(w, h, nextDoor, nextMap);
			door->setPosition(x + w / 2, y);
			this->addChild(door);
			auto arow = Sprite::create(PATH_IMAGE_UI + "arow_map.png");
			auto action = Utility::createAnimationWaggle(20);
			arow->runAction(action);
			arow->setPositionY(y + 60);

			auto label = Label::createWithTTF(nextMap, DEFAULT_FONT, 30);
			auto labelWidth = label->getContentSize().width;
			labelWidth = labelWidth < 250 - 40 ? 250 : labelWidth + 40;
			auto nextMapSprite = Sprite::create(PATH_IMAGE_UI + "black.png", Rect(0, 0, labelWidth, 80));
			label->setPosition(nextMapSprite->getContentSize() / 2);
			nextMapSprite->addChild(label);
			nextMapSprite->setPositionY(y + 150);

			if (x > 0) {
				nextMapSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
				nextMapSprite->setPositionX(x - 70);
				arow->setPositionX(x - 120);
			}
			else {
				nextMapSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				nextMapSprite->setPositionX(x + 70);
				arow->setPositionX(x + 120);
				arow->setFlippedX(true);
			}
			this->addChild(nextMapSprite, 3);
			this->addChild(arow, 3);

		}
		else if (value.at("name").asString() == std::to_string(_startDoor)) {
			_positionStart = Vec2(x, y);
		}
	}
	initMonsters();
	initNPC();
}

void GameMap::initMonsters() {
	auto objects = getObjectGroup("monsters");
	if (!objects) return;
	ValueVector monsters = objects->getObjects();
	auto em = EnemyManager::create();
	for (int i = 0; i < monsters.size(); i++)
	{
		ValueMap value = monsters.at(i).asValueMap();

		std::string name = value.at("name").asString();

		float w = value.at("width").asFloat();
		float h = value.at("height").asFloat();

		int x = value.at("x").asInt();
		int y = value.at("y").asInt();
		auto enemy = em->getEnemy(name);
		if (!enemy) continue;
		enemy->setPosition(x, y);
		enemy->setPositionStart(enemy->getPosition());
		GameManager::getInstance()->pushEntity(enemy);
		this->addChild(enemy, 5);
	}
}

void GameMap::initNPC()
{
	auto objects = getObjectGroup("npcs");
	if (!objects) return;
	ValueVector walls = objects->getObjects();
	auto em = EnemyManager::create();
	for (int i = 0; i < walls.size(); i++)
	{
		ValueMap value = walls.at(i).asValueMap();

		std::string name = value.at("name").asString();

		float w = value.at("width").asFloat();
		float h = value.at("height").asFloat();

		int x = value.at("x").asInt();
		int y = value.at("y").asInt();
		NPC* npc;
		if (name == "Neko Sennin") {
			npc = Instructor::create();
		}
		else {
			npc = Seller::create();
		}
		npc->setName(name);
		this->addChild(npc, 5);
		npc->setPosition(x, y);
		GameManager::getInstance()->pushEntity(npc);
	}
}

void GameMap::clearMap()
{
	for (auto child : _children) {
		if (dynamic_cast<Door*>(child) || dynamic_cast<Wall*>(child)) {
			dynamic_cast<PhysicsNode*> (child)->autoRemoveBody();
		}
	}
}

Size GameMap::getSize()
{
	return Size(_mapSize.width * _tileSize.width, _mapSize.height * _tileSize.height);
}
