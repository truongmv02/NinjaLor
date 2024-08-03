#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
USING_NS_CC;

class LifeEntity;
class Bonus;
class Stat :public Ref
{
public:
	enum class ValueType
	{
		NUMBER,
		PERCENT
	};
	Stat();
	static Stat* create(std::string name, int value);
	static  Stat* getStat(std::string statName, Vector<Stat*> stats);
	static Stat* getStat(int id, Vector<Stat*> stats);
	void setId(int id);
	static Vector<Bonus*> getBonuses(std::string statName, Vector<Stat*> stats);
	inline	bool init() { return true; }
	bool init(std::string name, int value);
	void AddBonus(Bonus* bonus);
	void AddBonuses(Vector<Bonus*> bonuses);
	void removeBonus(Bonus* bonus);
	void update(float dt);
	int getCurrentValue();
	Stat* clone();
	void clear();
	CC_SYNTHESIZE_READONLY(int, _id, Id);
	CC_SYNTHESIZE(std::string, _name, Name);
	CC_SYNTHESIZE(ValueType, _valueType, ValueType);
	CC_SYNTHESIZE(std::string, _description, Description);
	CC_SYNTHESIZE(int, _value, Value);
	CC_SYNTHESIZE(Vector<Bonus*>, _bonuses, Bonuses);
	CC_SYNTHESIZE(LifeEntity*, _owner, Owner);
	CREATE_FUNC_RETAIN(Stat);
private:
};

class StatTemplate : public Stat {
public:
	StatTemplate();
	static StatTemplate* getStat(int id, Vector<StatTemplate*> stats);
	CC_SYNTHESIZE(int, _minValue, MinValue);
	CC_SYNTHESIZE(int, _maxValue, MaxValue);
	CREATE_FUNC(StatTemplate);
};

class Bonus :public Ref
{
public:

	Bonus();
	bool init() { return true; }
	Bonus* clone();
	CC_SYNTHESIZE(int, _statId, StatId);
	CC_SYNTHESIZE(Stat::ValueType, _valueType, ValueType);
	CC_SYNTHESIZE(int, _value, Value);
	CC_SYNTHESIZE(int, _duration, Duration);
	CC_SYNTHESIZE(int, _timeStarted, TimeStarted);
	CC_SYNTHESIZE(int, _elapsedTime, ElapsedTime);
	CC_SYNTHESIZE(int, _timeGap, TimeGap);
	friend class Stat;
	CREATE_FUNC_RETAIN(Bonus);
};

class BonusTemplate : public Bonus {

	bool init() { return true; }
	CC_SYNTHESIZE(int, _minValue, MinValue);
	CC_SYNTHESIZE(int, _maxValue, MaxValue);
	CREATE_FUNC(BonusTemplate);
};


