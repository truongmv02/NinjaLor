#include "Door.h"
#include "Entity/Character/Character.h"
#include "Scene/GameScene.h"
#include "Manager/GameManager.h"
#include "GameMap.h"

Door* Door::create(float width, float height, int nextDoor, std::string nextMap) {
	Door* door = new (std::nothrow) Door();
	if (door && door->init(width, height, nextDoor, nextMap)) {

		door->autorelease();
		return door;
	}
	CC_SAFE_DELETE(door);
	return nullptr;
}

bool Door::init(float width, float height, int nextDoor, std::string nextMap) {

	_nextMap = nextMap;
	_nextDoor = nextDoor;

	this->setContentSize(Size(width, height));
	initBoxBody(width, height);
	_body->SetType(b2_staticBody);
	return true;
}

void Door::BeginContact(Entity* entity, b2Contact* contact)
{

	if (!dynamic_cast<Character*> (entity)) return;
	contact->SetEnabled(false);
	GameManager::getInstance()->changeMap(_nextMap, _nextDoor);
}
