#pragma once

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "Layer/HUDLayer.h"
#include "Lib/DebugDraw.h"
#include "Entity/Entity.h"
#include "Entity/Bullet/Bullet.h"
#include "Definitions/Definitions.h"


USING_NS_CC;

class Character;
class GameMap;
class Pointer;
//class Entity;

class GameManager : public b2ContactListener, public Ref
{
public:
	static GameManager* getInstance();

	void step(float dt);
	void updateMap(float dt);
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void pause(Node* node);
	void pause();
	void resume();
	void resume(Node* node);

	void clear();
	Pointer* getPointer();
	HUDLayer* getHUDLayer();

	void pushDeleteBody(b2Body* body);
	void pushEntity(Entity* entity);
	void removeEntity(Entity* entity);

	void pushItem(Entity* item);
	void removeItem(Entity* item);

	void pushBullet(Bullet* bullet);
	void removeBullet(Bullet* bullet);
	void changeMap(std::string map, int door);
	Enemy* getEnemyFocus();
	void reset();
	CREATE_SET(HUDLayer*, _HUDLayer, HUDLayer);
	CC_SYNTHESIZE(Character*, _character, Character);
	CREATE_SET(Pointer*, _pointer, Pointer);
	CC_SYNTHESIZE(b2World*, _world, World);
	CC_SYNTHESIZE(GameMap*, _map, Map);
	CC_SYNTHESIZE(Vector<Entity*>, _entities, Entities);
	CC_SYNTHESIZE(Vector<Entity*>, _items, Items);
	CC_SYNTHESIZE(Vector<Bullet*>, _bullets, Bullets);
	CC_SYNTHESIZE_READONLY(bool, _isPause, Pause);
	CC_SYNTHESIZE(Layer*, _gameLayer, GameLayer);
	//std::unique_ptr<CcxBox2dDebugDraw> _debugDrawInst = nullptr;

private:
	GameManager();
	void initPhysicsWorld(float gravity);
private:
	static GameManager* _instance;
	std::vector<b2Body*> _deleteBody;
};