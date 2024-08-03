#include "StatManager.h"
#include "DatabaseManager.h"
#include "StatManager.h"


Vector<Stat*> StatManager::getAllStat()
{
	Vector<Stat*> stats;
	std::string query = "select stat_id, stat_name, ifnull(description, '') as description from stats;";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		auto stat = Stat::create();
		stat->setId(sqlite3_column_int(stmt, 0));
		stat->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
		stat->setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
		stats.pushBack(stat);
	}

	return stats;
}

Vector<Stat*> StatManager::findItemStatTemplate(int itemId)
{
	Vector<Stat*> stats;

	auto bonues = findItemBonusTemplate(itemId);
	for (auto bonus : bonues) {
		auto stat = Stat::getStat(bonus->getStatId(), stats);
		if (stat) {
			stat->AddBonus(bonus);
		}
		else {
			auto s = findStat(bonus->getStatId());
			s->AddBonus(bonus);
			stats.pushBack(s);
		}
	}
	return stats;

}

Vector<Stat*> StatManager::findItemStat(int itemId)
{
	Vector<Stat*> stats;
	auto bonues = findItemBonus(itemId);
	for (auto bonus : bonues) {
		auto stat = Stat::getStat(bonus->getStatId(), stats);
		if (stat) {
			stat->AddBonus(bonus);
		}
		else {
			auto s = findStat(bonus->getStatId());
			s->AddBonus(bonus);
			stats.pushBack(s);
		}
	}

	return stats;
}

Vector<Stat*> StatManager::findSkillStat(int skillId, int level)
{
	stmt = nullptr;
	Vector<Stat*> stats;

	auto bonues = findSkillBonus(skillId, level);
	for (auto bonus : bonues) {
		auto stat = Stat::getStat(bonus->getStatId(), stats);
		if (stat) {
			stat->AddBonus(bonus);
		}
		else {
			auto s = findStat(bonus->getStatId());
			s->AddBonus(bonus);
			stats.pushBack(s);
		}
	}

	return stats;
}


Vector<Bonus*> StatManager::findItemBonusTemplate(int itemId)
{
	stmt = nullptr;
	Vector<Bonus*> bonuses;
	std::string query =
		//			0		1			  2			3		  4			 5			
		"select stat_id, value_type, min_value, max_value \
		from item_bonus_template where item_id = " + std::to_string(itemId) + ";";

	DatabaseManager::getInstance()->loadData(query, &stmt);

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		bonuses.pushBack(getBonusTemplate(stmt));
	}
	return bonuses;
}

Vector<Bonus*> StatManager::findItemBonus(int itemId)
{
	stmt = nullptr;
	Vector<Bonus*> bonuses;
	std::string query =
		//			0		1			  2					3		  4			 5	
		"select b.stat_id, b.value_type, b.gap_time, b.duration, b.value \
		from item_bonus b where b.item_id = " + std::to_string(itemId) + " group by b.stat_id;";

	DatabaseManager::getInstance()->loadData(query, &stmt);

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		bonuses.pushBack(getBonus(stmt));
	}
	return bonuses;
}

Vector<Bonus*> StatManager::findSkillBonus(int skillId, int level)
{
	Vector<Bonus*> bonuses;
	std::string query =
		//			0		1			2			3			4		5
		"select stat_id, bonus_type, gap_time, duration, value \
		from skill_bonus where skill_id = " + std::to_string(skillId) + " and level = " + std::to_string(level) + "; ";

	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		bonuses.pushBack(getBonus(stmt));
	}
	return bonuses;
}

Stat* StatManager::findStat(int id)
{
	stmt = nullptr;
	Stat* stat = nullptr;
	std::string query = "select stat_id, stat_name, description from stats where stat_id = " + std::to_string(id) + ";";

	DatabaseManager::getInstance()->loadData(query, &stmt);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		stat = Stat::create();
		stat->setId(sqlite3_column_int(stmt, 0));
		stat->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
		auto desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		stat->setDescription(desc != nullptr ? desc : "");
	}
	return stat;
}

Stat* StatManager::getStat(sqlite3_stmt* stmt)
{
	Stat* stat = Stat::create();
	stat->setId(sqlite3_column_int(stmt, 0));
	stat->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
	auto desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
	stat->setDescription(desc != nullptr ? desc : "");
	stat->setValue(sqlite3_column_int(stmt, 3));
	return stat;
}

Bonus* StatManager::getBonus(sqlite3_stmt* stmt)
{
	Bonus* bonus = Bonus::create();
	bonus->setStatId(sqlite3_column_int(stmt, 0));
	bonus->setValueType((Stat::ValueType)sqlite3_column_int(stmt, 1));
	bonus->setTimeGap(sqlite3_column_int(stmt, 2));
	bonus->setDuration(sqlite3_column_int(stmt, 3));
	bonus->setValue(sqlite3_column_int(stmt, 4));
	return bonus;
}

BonusTemplate* StatManager::getBonusTemplate(sqlite3_stmt* stmt)
{
	BonusTemplate* bonus = BonusTemplate::create();
	bonus->setStatId(sqlite3_column_int(stmt, 0));
	bonus->setValueType((Stat::ValueType)sqlite3_column_int(stmt, 1));
	bonus->setMinValue(sqlite3_column_int(stmt, 2));
	bonus->setMaxValue(sqlite3_column_int(stmt, 3));
	return bonus;
}

StatTemplate* StatManager::getStatTemplate(sqlite3_stmt* stmt)
{
	StatTemplate* stat = StatTemplate::create();
	stat->setId(sqlite3_column_int(stmt, 0));
	stat->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
	auto desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
	stat->setDescription(desc != nullptr ? desc : "");
	stat->setMinValue(sqlite3_column_int(stmt, 3));
	stat->setMaxValue(sqlite3_column_int(stmt, 4));
	return stat;
}
