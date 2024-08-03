#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include "Skill/Skills.h"
USING_NS_CC;

class SkillManager :public Ref
{
public:
	bool init() { return true; }

	Vector<Skill*> getAllSkill();
	Skill* findSkill(int id, int level);

	bool upgradeSkill(int id, int level);

	bool setIndex(int id, int index);
	bool setSkillChoose(int choose);

	int getSkillPoint();
	bool updateSkillPoint(int value);
	CREATE_FUNC(SkillManager);
private:
	sqlite3_stmt* _stmt;
	Skill* getSkill(sqlite3_stmt* stmt);
};
