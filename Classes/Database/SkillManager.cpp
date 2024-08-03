#include "SkillManager.h"
#include "DatabaseManager.h"
#include "StatManager.h"

Vector<Skill*> SkillManager::getAllSkill() {
	Vector<Skill*> skills;
	std::string query =
		//			0				1		2		3			4				5			6		7				8			
		"select s.skill_id, s.skill_name, s.type, s.range, s.description, s.level_max, cs.level, cs.elapsed_time, cs.skill_index, \
		ifnull( sl.level_require, 0) as level_require , ifnull( sl.mp_use, 0) as mp_use, ifnull( sl.cooldown, 0) as cooldown from skills s inner join character_skill cs on cs.skill_id = s.skill_id left join skill_level sl on sl.skill_id = s.skill_id and sl.level = cs.level;";

	DatabaseManager::getInstance()->loadData(query, &_stmt);
	while (sqlite3_step(_stmt) == SQLITE_ROW)
	{
		auto skill = getSkill(_stmt);
		if (skill->getLevel() != 0) {
			skill->setStats(StatManager::create()->findSkillStat(skill->getId(), skill->getLevel()));
		}
		skills.pushBack(skill);
	}
	return skills;
}

Skill* SkillManager::findSkill(int id, int level)
{
	std::string query =
		//			 0			1			2		3			4				5			6			7			  8				9					10			11
		"select s.skill_id, s.skill_name, s.type, s.range, s.description, s.level_max, sl.level, cs.elapsed_time, cs.skill_index, sl.level_require, sl.mp_use, sl.cooldown \
		from skills s inner join skill_level sl on sl.skill_id = s.skill_id inner join character_skill cs on cs.skill_id = s.skill_id where s.skill_id = " + std::to_string(id) + " and sl.level = " + std::to_string(level) + ";";
	Skill* skill = nullptr;

	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		skill = getSkill(_stmt);
		skill->setStats(StatManager::create()->findSkillStat(skill->getId(), skill->getLevel()));
	}
	return skill;
}

bool SkillManager::upgradeSkill(int id, int level)
{
	std::string query = "update character_skill set level = " + std::to_string(level) + " where skill_id = " + std::to_string(id) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

bool SkillManager::setIndex(int id, int index)
{
	std::string query = "select skill_id from character_skill where skill_index = " + std::to_string(index) + ";";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt)) {
		int skillID = sqlite3_column_int(_stmt, 0);
		query = "update character_skill set skill_index = -1 where skill_id = " + std::to_string(skillID) + ";";
		DatabaseManager::getInstance()->execute(query);
	}
	query = "update character_skill set skill_index = " + std::to_string(index) + " where skill_id = " + std::to_string(id) + ";";

	return DatabaseManager::getInstance()->execute(query);
}

bool SkillManager::setSkillChoose(int choose)
{
	std::string query = "update characters set skill_choose = " + std::to_string(choose) + ";";

	return DatabaseManager::getInstance()->execute(query);
}


int SkillManager::getSkillPoint()
{
	std::string query = "select skill_point from characters;";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	int point = 0;
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		point = sqlite3_column_int(_stmt, 0);
	}
	return point;
}

bool SkillManager::updateSkillPoint(int value)
{
	std::string query = "update characters set skill_point = " + std::to_string(value) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}


Skill* SkillManager::getSkill(sqlite3_stmt* stmt) {
	auto type = sqlite3_column_int(stmt, 2);
	Skill* skill = nullptr;
	switch (type)
	{
	case 1:
		skill = MelleSkill::create();
		break;
	case 2:
		skill = RangeSkill::create();
		break;
	}
	skill->setId(sqlite3_column_int(stmt, 0));
	skill->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
	skill->setRange(sqlite3_column_int(stmt, 3));
	skill->setDesc(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
	skill->setLevelMax(sqlite3_column_int(stmt, 5));
	skill->setLevel(sqlite3_column_int(stmt, 6));
	skill->setElapsedTime(sqlite3_column_int(stmt, 7));
	skill->setIndex(sqlite3_column_int(stmt, 8));
	skill->setLevelRequire(sqlite3_column_int(stmt, 9));
	skill->setMpUse(sqlite3_column_int(stmt, 10));
	skill->setCooldown(sqlite3_column_int(stmt, 11));
	skill->setElapsedTime(sqlite3_column_int(stmt, 11));

	return skill;
}
