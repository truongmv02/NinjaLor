#pragma once

#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Entity.h"
#include "Stat.h"
#include "Item/Item.h"
#include "StateMachine/StateMachine.h"


USING_NS_CC;
class LifeEntity :public Entity {
public:
	enum EntityState
	{
		IDLE,
		ATTACK,
		MOVE,
		JUMP,
		ROTATE,
		FALL,
		HURT,
		DEAD
	};
	LifeEntity();
	bool init();
	Stat* getStat(std::string name);
	void addStats(Vector<Stat*> stats);
	virtual	void doAction(EntityState state) {}
	void update(float dt);
	void setHp(int hp);
	void setMp(int mp);
	void setStats(Vector<Stat*> stats);
	CC_SYNTHESIZE(StateMachine*, _stateMachine, StateMachine);
	CC_SYNTHESIZE(int, _level, Level);
	CC_SYNTHESIZE_READONLY(Vector<Stat*>, _stats, Stats);

	CREATE_FUNC(LifeEntity);
protected:

};