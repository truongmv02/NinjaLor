#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Utility.h"

USING_NS_CC;

class Entity;
class Pointer :public Sprite
{
public:
	static Pointer* create();
	bool init();
	void update(float dt);
	void changeTarget();
	void setInfo(Entity* entity);
	void setEntitiesFocus();
	void setEntitiesFocus(Vector<Entity*> entities);
	void removeEntityFocus(Entity* entity);
	CC_SYNTHESIZE(Entity*, _entityFocus, EntityFocus);
	CC_SYNTHESIZE(Sprite*, _pointerSprite, PointerSprite);
protected:
	Vector<Entity*> _entities;
	Vector<Entity*> _entitiesFocus;

};


