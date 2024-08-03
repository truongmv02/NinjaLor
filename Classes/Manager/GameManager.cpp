#include "GameManager.h"
#include "Definitions/Definitions.h"
#include "Lib/Pointer.h"
#include "Map/GameMap.h"
#include "Lib/DebugDraw.h"
#include "Scene/GameScene.h"
#include "Entity/Character/Character.h"
#include "Entity/Enemy/Enemy.h"
#include "Database/DataManager.h"

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager() {
	initPhysicsWorld(120);
	_character = nullptr;
	_HUDLayer = nullptr;
	_pointer = nullptr;
	_map = nullptr;
	_isPause = false;
}

GameManager* GameManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new GameManager();
		_instance->retain();
	}
	return _instance;
}


Pointer* GameManager::getPointer()
{
	if (!_pointer) {
		_pointer = Pointer::create();
	}

	return _pointer;;
}

HUDLayer* GameManager::getHUDLayer()
{
	if (!_HUDLayer) {
		_HUDLayer = HUDLayer::create();
	}
	return _HUDLayer;
}

void GameManager::pause(Node* node)
{
	node->pause();
	for (auto child : node->getChildren()) {
		child->pause();
		pause(child);
	}
}

void GameManager::pause() {
	_isPause = true;
	_HUDLayer->getControlLayer()->setVisible(false);
	pause(_gameLayer);
	Director::getInstance()->getRunningScene()->unscheduleUpdate();
}

void GameManager::resume()
{
	_isPause = false;
	_HUDLayer->getControlLayer()->setVisible(true);
	resume(_gameLayer);
	InputHandler::getInstance()->reset();
	Director::getInstance()->getRunningScene()->scheduleUpdate();
}

void GameManager::resume(Node* node)
{
	node->resume();
	for (auto child : node->getChildren()) {
		child->resume();
		resume(child);
	}
	auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
	auto camera = scene->getDefaultCamera();
	Vec2 newPos = Vec2(scene->getCameraPositionX(), scene->getCameraPositionY());
	camera->setPosition(newPos);
	_HUDLayer->setPosition(newPos - VISIBLE_SIZE / 2);
}


void GameManager::pushDeleteBody(b2Body* body)
{
	_deleteBody.push_back(body);
}

void GameManager::pushEntity(Entity* entity)
{
	_entities.pushBack(entity);
}

void GameManager::removeEntity(Entity* entity)
{
	_pointer->removeEntityFocus(entity);
	_entities.eraseObject(entity);
}

void GameManager::pushBullet(Bullet* bullet)
{
	_bullets.pushBack(bullet);
}

void GameManager::pushItem(Entity* item)
{
	_items.pushBack(item);
}

void GameManager::removeItem(Entity* item)
{
	_pointer->removeEntityFocus(item);
	_items.eraseObject(item);
}

void GameManager::removeBullet(Bullet* bullet)
{
	_bullets.eraseObject(bullet);
}

void GameManager::changeMap(std::string map, int door)
{
	if (_character->knowMap(AManager::create()->getMapId(map)) || true) {
		auto scene = dynamic_cast<GameScene*> (Director::getInstance()->getRunningScene());
		scene->setChangeMap(true);
		scene->setMapName(map);
		scene->setStartDoor(door);
	}
	else {
		if (!_HUDLayer->getChildByName("MessageMap")) {
			auto dialog = Dialog::create("You can't go to this area yet!");
			dialog->setName("MessageMap");
			_HUDLayer->addChild(dialog, 1000);
		}
	}
}

Enemy* GameManager::getEnemyFocus()
{
	auto enemy = _pointer->getEntityFocus();
	return dynamic_cast<Enemy*>(enemy);
}

void GameManager::reset()
{
	if (_character) {
		_character->clear();
		CC_SAFE_DELETE(_character);
	}
	if (_HUDLayer) {
		_HUDLayer->release();
	}
	if (_pointer) {
		_pointer->release();
	}
	clear();
}

void GameManager::initPhysicsWorld(float gravity) {
	_world = new b2World(b2Vec2(0, -gravity));
	_world->SetContactListener(this);

	//_debugDrawInst = std::make_unique<CcxBox2dDebugDraw>(32);

	//uint32 flags = 0;
	//flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;

	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;

	//_debugDrawInst->SetFlags(flags);
	//_world->SetDebugDraw(_debugDrawInst.get());
}

void GameManager::step(float dt) {
	_world->Step(dt, 8, 3);
	for (auto body : _deleteBody)
	{
		_world->DestroyBody(body);
		body = nullptr;
	}
	_deleteBody.clear();
	for (b2Body* body = _world->GetBodyList(); body; body = body->GetNext())
	{
		auto node = (Node*)body->GetUserData();
		Entity* entity = dynamic_cast<Entity*>(node);
		if (entity == NULL)
			continue;
		entity->setPosition(
			body->GetPosition().x * PTM_RATIO,
			body->GetPosition().y * PTM_RATIO - entity->getBodyOffset().y - entity->getBodySize().height / 2);
		entity->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
}

void GameManager::updateMap(float dt) {
	auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
	auto camera = scene->getDefaultCamera();
	Vec2 newPos = Vec2(scene->getCameraPositionX(), scene->getCameraPositionY());
	auto pos = camera->getPosition().lerp(newPos, 0.1);
	camera->setPosition(pos);
	_HUDLayer->setPosition(pos - VISIBLE_SIZE / 2);
}

void GameManager::BeginContact(b2Contact* contact) {
	Entity* a = dynamic_cast<Entity*>((Node*)contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* b = dynamic_cast<Entity*>((Node*)contact->GetFixtureB()->GetBody()->GetUserData());
	if (a && b) {
		a->BeginContact(b, contact);
		b->BeginContact(a, contact);
	}
}

void GameManager::EndContact(b2Contact* contact) {
	Entity* a = dynamic_cast<Entity*>((Node*)contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* b = dynamic_cast<Entity*>((Node*)contact->GetFixtureB()->GetBody()->GetUserData());
	if (a && b) {
		a->EndContact(b, contact);
		b->EndContact(a, contact);
	}
}


void GameManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	Entity* a = dynamic_cast<Entity*>((Node*)contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* b = dynamic_cast<Entity*>((Node*)contact->GetFixtureB()->GetBody()->GetUserData());
	if (a && b) {
		a->PreSolve(b, contact, oldManifold);
		b->PreSolve(a, contact, oldManifold);
	}
}


void GameManager::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	Entity* a = dynamic_cast<Entity*>((Node*)contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* b = dynamic_cast<Entity*>((Node*)contact->GetFixtureB()->GetBody()->GetUserData());
	if (a && b) {
		a->PostSolve(b, contact, impulse);
		b->PostSolve(a, contact, impulse);
	}
}


void GameManager::clear()
{
	if (_map) {
		_map->clearMap();
	}
	for (auto entity : _entities) {
		entity->autoRemoveBody();
	}
	for (auto bullet : _bullets) {
		bullet->autoRemoveBody();
	}
	for (auto item : _items) {
		item->autoRemoveBody();
	}
	_entities.clear();
	_bullets.clear();
	_items.clear();
	if (_pointer) {
		_pointer->removeFromParentAndCleanup(false);
		_pointer->setEntityFocus(nullptr);
	}
}
