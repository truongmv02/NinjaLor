#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include "Entity/Stat.h"
USING_NS_CC;

class StatManager :public Ref
{
public:
	inline bool init() { return true; }
	Vector<Stat*> getAllStat();
	Vector<Stat*> findItemStatTemplate(int itemId);
	Vector<Stat*> findItemStat(int itemId);
	Vector<Stat*> findSkillStat(int skillId, int level);
	Stat* findStat(int id);
	CREATE_FUNC(StatManager);
private:
	sqlite3_stmt* stmt;

	Vector<Bonus*> findItemBonus(int itemId);
	Vector<Bonus*> findSkillBonus(int skillId, int level);
	Vector<Bonus*> findItemBonusTemplate(int itemId);

	Stat* getStat(sqlite3_stmt* stmt);
	StatTemplate* getStatTemplate(sqlite3_stmt* stmt);

	Bonus* getBonus(sqlite3_stmt* stmt);
	BonusTemplate* getBonusTemplate(sqlite3_stmt* stmt);
};
