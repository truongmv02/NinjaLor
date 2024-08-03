#pragma once

#include "Entity/LifeEntity.h"

class Character;
class Coin;
class Enemy : public LifeEntity
{
public:
	enum class EnemyType {
		SCARECROW,
		MOVING,
		BOSS,
		PASSIVE
	};
	Enemy();
	bool init();
	bool init(std::string name);
	void setStatValue();
	virtual void setName(std::string name);
	virtual void setLevel(int level) override;
	virtual void update(float dt);
	void move(float dt);
	void dead();
	void checkQuest();
	Coin* createBonus();
	void spawBullet(float dt);
	void createBullet();
	void doAction(EntityState state) override;
	void initSchedule();
	CC_SYNTHESIZE(Vec2, _pStart, PositionStart);
	CC_SYNTHESIZE(EnemyType, _type, Type);
	CC_SYNTHESIZE(LifeEntity*, _eFocus, EntityFocus);
	CC_SYNTHESIZE(Slider*, _hpBar, HpBar);
	CC_SYNTHESIZE(EntityState, _state, State);
	CREATE_FUNC(Enemy);
	CREATE_FUNC1(Enemy, std::string);
private:

};