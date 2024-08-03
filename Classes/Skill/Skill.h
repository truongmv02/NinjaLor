#pragma once

#include "cocos2d.h"
#include "Lib/Utility.h"
#include "Entity/Stat.h"

USING_NS_CC;
class Character;
class Skill : public Node
{
public:
	Skill();
	bool init();
	static Skill* getSkillByIndex(Vector<Skill*> skills, int index);
	virtual void update(float dt) override;
	virtual	bool use(LifeEntity* target);
	virtual void initEffect() {}
	virtual void createEffect() {}
	virtual	void clear();
	virtual void clearStat();
	CC_SYNTHESIZE(int, _id, Id);
	//CC_SYNTHESIZE(std::string, _name, Name);
	CC_SYNTHESIZE(int, _level, Level);
	CC_SYNTHESIZE(int, _levelRequire, LevelRequire);
	CC_SYNTHESIZE(int, _levelMax, LevelMax);
	CC_SYNTHESIZE(int, _index, Index);
	CC_SYNTHESIZE(int, _elapsedTime, ElapsedTime);
	CC_SYNTHESIZE(int, _cooldown, Cooldown);
	CC_SYNTHESIZE(int, _mpUse, MpUse);
	CC_SYNTHESIZE(int, _range, Range);

	CC_SYNTHESIZE(std::string, _desc, Desc);
	CC_SYNTHESIZE(Vector<Stat*>, _stats, Stats);
	CC_SYNTHESIZE(LifeEntity*, _owner, Owner);

private:
};