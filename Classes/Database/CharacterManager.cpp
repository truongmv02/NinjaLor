#include "CharacterManager.h"
#include "DatabaseManager.h"
#include "Database/DataManager.h"

Character* CharacterManager::getCharacter()
{
	_character = Character::create();
	_character->addStats(StatManager::create()->getAllStat());
	auto sm = SkillManager::create();
	_character->setSkills(sm->getAllSkill());
	auto quest = QuestManager::create()->getCurrentQuest();
	_character->setQuest(quest);
	for (int i = 0; i < 5; i++)
	{
		_character->setSkillSelected(Skill::getSkillByIndex(_character->getSkills(), i), i);
	}

	query =
		//        0    1		2		 3			 4			5		6		7		     	8	             9								         
		"select c.hp, c.mp, c.level, c.current_exp, c.coin, c.pos_x, c.pos_y, c.item_max,  c.skill_point, c.skill_choose from characters c;";

	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		_character->setLevel(sqlite3_column_int(_stmt, 2));
		_character->setStatValue();
		auto im = ItemManager::create();
		_character->setItemBag(im->getInventory());
		_character->setEquipments(im->getCharacterEquipments());

		_character->setHp(sqlite3_column_int(_stmt, 0));
		_character->setMp(sqlite3_column_int(_stmt, 1));
		_character->setCurrentExp(sqlite3_column_int(_stmt, 3));
		_character->setCoin(sqlite3_column_int(_stmt, 4));
		_character->setPosition(Vec2(sqlite3_column_double(_stmt, 5), sqlite3_column_double(_stmt, 6)));
		_character->setItemMax(sqlite3_column_int(_stmt, 7));
		_character->setSkillPoint(sqlite3_column_int(_stmt, 8));
		_character->setSkillChoose(sqlite3_column_int(_stmt, 9));
		//_character->setExpMax(sqlite3_column_int(_stmt, 4));
	}
	_character->setKnowMaps(getKnowMaps());
	return _character;
}

bool CharacterManager::updateCoin(int coin)
{
	query = "update characters set coin = " + std::to_string(coin) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::updateExp(int exp)
{
	query = "update characters set current_exp = " + std::to_string(exp) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::updateLevel(int level)
{
	query = "update characters set level = " + std::to_string(level) + ";";

	return	DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::updateHp(int hp)
{
	query = "update characters set hp = " + std::to_string(hp) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::updateMp(int mp)
{
	query = "update characters set mp = " + std::to_string(mp) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::updateMap(int id)
{
	query = "update characters set map = " + std::to_string(id) + ";";
	DatabaseManager::getInstance()->execute(query);
	return addTeleMap(id);
}

bool CharacterManager::addTeleMap(int id)
{
	query = "update character_map set status = 1 where map_id = " + std::to_string(id) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}

bool CharacterManager::addKnowMap(int id)
{
	query = "insert into character_map (map_id) values(" + std::to_string(id) + ");";
	return DatabaseManager::getInstance()->execute(query);
}



std::string CharacterManager::getMap()
{
	query = "select m.map_name from characters c inner join map m on m.map_id = c.map;";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		return std::string(reinterpret_cast<const char*> (sqlite3_column_text(_stmt, 0)));
	}
	return "";
}

bool CharacterManager::updatePosition(Vec2 position)
{
	query = "update characters set pos_x = " + std::to_string(position.x) + ";";
	DatabaseManager::getInstance()->execute(query);
	query = "update characters set pos_y = " + std::to_string(position.y) + ";";
	return	DatabaseManager::getInstance()->execute(query);
}

std::vector<int> CharacterManager::getKnowMaps()
{
	std::vector<int> map;
	query = "select map_id from character_map;";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	while (sqlite3_step(_stmt) == SQLITE_ROW)
	{
		auto m = sqlite3_column_int(_stmt, 0);
		map.push_back(m);
	}
	return map;
}

int CharacterManager::getExp(int level)
{
	query = "select exp from character_exp where level = " + std::to_string(level) + ";";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt)) {
		return sqlite3_column_int(_stmt, 0);
	}
	return 0;
}

bool CharacterManager::canTeleMap(std::string mapName)
{
	query = "select ifnull(cm.status,0) as status from map m left join character_map cm on m.map_id = cm.map_id where m.map_name = '" + mapName + "';";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt)) {
		return sqlite3_column_int(_stmt, 0) == 1;
	}

	return false;
}

Vec2 CharacterManager::getCharacterPosition()
{
	query = "select pos_x, pos_y from characters;";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	Vec2 pos = Vec2(-1, -1);
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		pos.x = sqlite3_column_double(_stmt, 0);
		pos.y = sqlite3_column_double(_stmt, 1);
	}
	return pos;
}
