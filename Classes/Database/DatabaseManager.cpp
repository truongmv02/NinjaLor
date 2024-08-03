#include "DatabaseManager.h"
#include "Definitions/Definitions.h"
#include <string>

DatabaseManager* DatabaseManager::_instance = nullptr;

DatabaseManager::DatabaseManager() {
	db = nullptr;
	int result = sqlite3_open(DATABASE_PATH.c_str(), &db);
	createDatabase();
}

DatabaseManager* DatabaseManager::getInstance() {
	_instance = (_instance != nullptr) ? _instance : new DatabaseManager();
	return _instance;
}


bool DatabaseManager::loadData(std::string query, sqlite3_stmt** stmt) {
	int result = sqlite3_prepare_v2(db, query.c_str(), -1, stmt, 0);
	if (result != SQLITE_OK)
		log("%s", sqlite3_errmsg(db));
	return result == SQLITE_OK;
}

bool DatabaseManager::execute(std::string query) {
	int result = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
	if (result != SQLITE_OK)
		log("%s", sqlite3_errmsg(db));
	return result == SQLITE_OK;
}

void DatabaseManager::reset()
{
	execute("delete from character_map;");
	execute("delete from stats;");
	execute("delete from skills;");
	execute("delete from skill_level;");
	execute("delete from skill_bonus;");
	execute("delete from quests;");
	execute("delete from character_quest;");
	execute("delete from quest_enemy;");
	execute("delete from quest_item;");
	execute("delete from quest_know_map;");
	execute("delete from quest_conversation;");
	execute("delete from items_template;");
	execute("delete from items;");
	execute("delete from item_shop;");
	execute("delete from item_bonus_template;");
	execute("delete from item_bonus;");
	execute("delete from equipments;");
	execute("delete from inventory;");
	execute("delete from enemies;");
	execute("delete from characters;");
	execute("delete from character_skill;");
	execute("delete from character_quest_enemy;");
	execute("delete from map;");
	insertData();
}

bool DatabaseManager::createDatabase() {
	std::string sql;
	sql = std::string("\
CREATE TABLE character_map (map_id INTEGER PRIMARY KEY REFERENCES map (map_id) ON DELETE CASCADE, status INTEGER); \
CREATE TABLE character_quest (quest_id INTEGER, status INTEGER); \
CREATE TABLE character_quest_enemy (enemy_id INTEGER PRIMARY KEY, kill INTEGER); \
CREATE TABLE character_skill (skill_id INTEGER REFERENCES skills (skill_id), level INTEGER, elapsed_time INTEGER, skill_index INTEGER, PRIMARY KEY (skill_id, level)) WITHOUT ROWID; \
CREATE TABLE characters (hp INTEGER, mp INTEGER, current_exp INTEGER, coin INTEGER, level INTEGER, pos_x DOUBLE, pos_y DOUBLE, potential_point INTEGER, skill_point INTEGER, item_max INTEGER, skill_choose INTEGER, map INTEGER); \
CREATE TABLE enemies (enemy_id INTEGER PRIMARY KEY AUTOINCREMENT, enemy_name VARCHAR (30) UNIQUE, level INTEGER, type INTEGER DEFAULT (1)); \
CREATE TABLE equipments (item_id PRIMARY KEY REFERENCES items (item_id) ON DELETE CASCADE); \
CREATE TABLE inventory (item_id PRIMARY KEY REFERENCES items (item_id) ON DELETE CASCADE, quantity INTEGER); \
CREATE TABLE item_bonus (item_id INTEGER REFERENCES items (item_id) ON DELETE CASCADE, stat_id INTEGER REFERENCES stats (stat_id), value_type INTEGER, gap_time INTEGER, duration INTEGER, value INTEGER, PRIMARY KEY (item_id, stat_id, value_type)); \
CREATE TABLE item_bonus_template (item_id INTEGER REFERENCES items_template (item_id), stat_id INTEGER REFERENCES stats (stat_id), value_type INTEGER, min_value INTEGER, max_value INTEGER, PRIMARY KEY (item_id, stat_id, value_type)); \
CREATE TABLE item_shop (item_id INTEGER PRIMARY KEY REFERENCES items (item_id) ON DELETE CASCADE, type_shop INTEGER); \
CREATE TABLE items (item_id INTEGER PRIMARY KEY, template_id INTEGER REFERENCES items_template (item_id), price_sell INTEGER DEFAULT (0)); \
CREATE TABLE items_template (item_id INTEGER PRIMARY KEY AUTOINCREMENT, item_name VARCHAR (30) UNIQUE, type INTEGER, level_require INTEGER, description TEXT, price_buy INTEGER, price_sell INTEGER, lock INTEGER DEFAULT (0)); \
CREATE TABLE map (map_id INTEGER PRIMARY KEY AUTOINCREMENT, map_name VARCHAR); \
CREATE TABLE quest_conversation (quest_id INTEGER REFERENCES quests (quest_id) ON DELETE CASCADE, conversation_type INTEGER, conversation_index INTEGER, conversation VARCHAR, PRIMARY KEY (quest_id, conversation_type, conversation_index)); \
CREATE TABLE quest_enemy (quest_id INTEGER REFERENCES quests (quest_id) ON DELETE CASCADE, enemy_id INTEGER REFERENCES enemies (enemy_id) ON DELETE CASCADE, quantity INTEGER, map_id INTEGER REFERENCES map (map_id) ON DELETE CASCADE); \
CREATE TABLE quest_item (quest_id INTEGER REFERENCES quests (quest_id) ON DELETE SET DEFAULT, item_id INTEGER, quantity INTEGER DEFAULT (1)); \
CREATE TABLE quest_know_map (quest_id INTEGER, map_id INTEGER REFERENCES map (map_id) ON DELETE CASCADE); \
CREATE TABLE quests (quest_id INTEGER PRIMARY KEY AUTOINCREMENT, quest_name VARCHAR, level_require INTEGER, exp_reward INTEGER, coin_reward INTEGER, description VARCHAR); \
CREATE TABLE skill_bonus (skill_id INTEGER REFERENCES skills (skill_id), level INTEGER, stat_id INTEGER REFERENCES stats (stat_id), bonus_type INTEGER, value_type INTEGER, value INTEGER, gap_time INTEGER, duration INTEGER, PRIMARY KEY (skill_id, level, stat_id, bonus_type, value_type)); \
CREATE TABLE skill_level (skill_id INTEGER REFERENCES skills (skill_id), level INTEGER, level_require INTEGER, cooldown INTEGER, mp_use INTEGER, PRIMARY KEY (skill_id, level)); \
CREATE TABLE skills (skill_id INTEGER PRIMARY KEY AUTOINCREMENT, skill_name VARCHAR (50), type INTEGER, ""range"" float, description VARCHAR (200), level_max INTEGER); \
CREATE TABLE stats (stat_id INTEGER PRIMARY KEY AUTOINCREMENT, stat_name VARCHAR, description TEXT); \
");
	bool result = execute(sql);
	if (result)
		insertData();
	return result;
}

bool DatabaseManager::insertData()
{
	auto sql = std::string("\
INSERT INTO character_map(map_id, status) VALUES(1, 1); \
INSERT INTO character_quest(quest_id, status) VALUES(1, 0); \
INSERT INTO character_quest_enemy(enemy_id, kill) VALUES(1, 0); \
INSERT INTO character_skill(skill_id, level, elapsed_time, skill_index) VALUES(1, 1, 0, 0); \
INSERT INTO character_skill(skill_id, level, elapsed_time, skill_index) VALUES(2, 0, 0, -1); \
INSERT INTO characters(hp, mp, current_exp, coin, level, pos_x, pos_y, potential_point, skill_point, item_max, skill_choose, map) VALUES(220, 110, 0, 10000, 1, -1.0, -1.0, 0, 0, 42, 0, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(1, 'Scarecrow', 1, 0); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(2, 'Snail', 2, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(3, 'Green frog', 3, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(4, 'Stone porcupine', 4, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(5, 'Gray rabbit', 5, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(6, 'Cruel mouse', 6, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(7, 'Spotted spider', 7, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(8, 'Mole', 8, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(9, 'Yellow turtle', 9, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(10, 'Wild boar', 10, 2); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(11, 'One eyed demon', 11, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(12, 'Stone scorpion', 12, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(13, 'Giant snail', 13, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(14, 'Field mouse', 14, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(15, 'Green snake', 15, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(16, 'Stone snail', 16, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(17, 'Cyclops snake', 17, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(18, 'Spiky snail', 18, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(19, 'One foot demon', 19, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(20, 'Hell scorpion', 20, 2); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(21, 'Ghost centipede', 21, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(22, 'Strong ant', 22, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(23, 'Helmet bug', 23, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(24, 'Ice devil', 24, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(25, 'Sea crab', 25, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(26, 'Green worm', 26, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(27, 'Ghost umbrella', 27, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(28, 'Purple snake', 28, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(29, 'Three colors insect', 29, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(30, 'Ghost pumpkin', 30, 2); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(31, 'Evil mud', 31, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(32, 'One eyed ghost', 32, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(33, 'Evil bone', 33, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(34, 'Skull devil', 34, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(35, 'Red eye devil', 35, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(36, 'Tiny skeleton', 36, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(37, 'Tapir', 37, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(38, 'Fire scorpion', 38, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(39, 'Lizard', 39, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(40, 'Shadow ninja', 40, 2); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(41, 'Zombie', 41, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(42, 'Big frog', 42, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(43, 'Blue mouse', 43, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(44, 'Crocodile', 44, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(45, 'Green ear turtle', 45, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(46, 'Devil eyes', 46, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(47, 'One legged monster', 47, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(48, 'Green Kappa', 48, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(49, 'Old tree', 49, 1); \
INSERT INTO enemies(enemy_id, enemy_name, level, type) VALUES(50, 'Samurai lord', 50, 1); \
INSERT INTO equipments(item_id) VALUES(59); \
INSERT INTO equipments(item_id) VALUES(60); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(1, 2, 0, 0, 0, 100); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(2, 2, 0, 0, 0, 200); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(3, 2, 0, 0, 0, 300); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(4, 2, 0, 0, 0, 400); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(5, 2, 0, 0, 0, 500); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(6, 4, 0, 0, 0, 50); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(7, 4, 0, 0, 0, 100); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(8, 4, 0, 0, 0, 150); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(9, 4, 0, 0, 0, 200); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(10, 4, 0, 0, 0, 250); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(11, 5, 0, 0, 0, 3); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(12, 5, 0, 0, 0, 10); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(13, 5, 0, 0, 0, 20); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(14, 5, 0, 0, 0, 30); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(15, 5, 0, 0, 0, 40); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(16, 5, 0, 0, 0, 50); ")
+ std::string("\
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(17, 5, 0, 0, 0, 3); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(18, 5, 0, 0, 0, 5); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(19, 5, 0, 0, 0, 10); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(20, 5, 0, 0, 0, 15); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(21, 5, 0, 0, 0, 20); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(22, 5, 0, 0, 0, 25); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(23, 6, 0, 0, 0, 1); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(24, 6, 0, 0, 0, 10); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(25, 6, 0, 0, 0, 20); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(26, 6, 0, 0, 0, 30); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(27, 6, 0, 0, 0, 40); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(28, 6, 0, 0, 0, 50); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(29, 1, 0, 0, 0, 15); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(30, 1, 0, 0, 0, 150); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(31, 1, 0, 0, 0, 300); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(32, 1, 0, 0, 0, 450); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(33, 1, 0, 0, 0, 600); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(34, 1, 0, 0, 0, 750); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(35, 5, 0, 0, 0, 3); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(36, 5, 0, 0, 0, 5); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(37, 5, 0, 0, 0, 10); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(38, 5, 0, 0, 0, 15); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(39, 5, 0, 0, 0, 20); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(40, 5, 0, 0, 0, 25); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(41, 3, 0, 0, 0, 7); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(42, 3, 0, 0, 0, 70); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(43, 3, 0, 0, 0, 140); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(44, 3, 0, 0, 0, 210); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(45, 3, 0, 0, 0, 280); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(46, 3, 0, 0, 0, 350); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(47, 7, 0, 0, 0, 200); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(48, 7, 0, 0, 0, 325); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(49, 7, 0, 0, 0, 450); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(50, 7, 0, 0, 0, 650); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(51, 7, 0, 0, 0, 800); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(52, 7, 0, 0, 0, 1000); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(53, 9, 0, 0, 0, 400); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(53, 10, 0, 0, 0, 400); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(54, 10, 0, 0, 0, 500); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(54, 9, 0, 0, 0, 500); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(55, 10, 0, 0, 0, 600); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(55, 9, 0, 0, 0, 600); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(56, 10, 0, 0, 0, 700); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(56, 9, 0, 0, 0, 700); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(57, 10, 0, 0, 0, 850); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(57, 9, 0, 0, 0, 850); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(58, 10, 0, 0, 0, 1000); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(58, 9, 0, 0, 0, 1000); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(41, 8, 0, 0, 0, 1); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(42, 8, 0, 0, 0, 2); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(43, 8, 0, 0, 0, 3); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(44, 8, 0, 0, 0, 4); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(45, 8, 0, 0, 0, 5); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(46, 8, 0, 0, 0, 6); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(47, 8, 0, 0, 0, 10); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(59, 6, 0, 0, 0, 1); \
INSERT INTO item_bonus(item_id, stat_id, value_type, gap_time, duration, value) VALUES(60, 1, 0, 0, 0, 15); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(11, 5, 0, 5, 3); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(12, 5, 0, 10, 10); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(13, 5, 0, 20, 20); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(14, 5, 0, 30, 30); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(15, 5, 0, 40, 40); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(16, 5, 0, 50, 50); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(17, 5, 0, 1, 3); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(18, 5, 0, 5, 5); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(19, 5, 0, 10, 10); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(20, 5, 0, 15, 15); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(21, 5, 0, 20, 20); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(22, 5, 0, 25, 25); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(23, 6, 0, 1, 1); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(24, 6, 0, 5, 10); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(25, 6, 0, 10, 20); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(26, 6, 0, 15, 30); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(27, 6, 0, 25, 40); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(28, 6, 0, 35, 50); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(29, 1, 0, 5, 15); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(30, 1, 0, 15, 150); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(31, 1, 0, 40, 300); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(32, 1, 0, 70, 450); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(33, 1, 0, 120, 600); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(34, 1, 0, 160, 750); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(35, 5, 0, 3, 3); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(36, 5, 0, 10, 5); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(37, 5, 0, 20, 10); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(38, 5, 0, 30, 15); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(39, 5, 0, 40, 20); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(40, 5, 0, 50, 25); ")
+ std::string("\
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(41, 3, 0, 3, 7); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(42, 3, 0, 5, 70); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(43, 3, 0, 15, 140); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(44, 3, 0, 35, 210); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(46, 3, 0, 85, 350); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(45, 3, 0, 60, 280); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(47, 7, 0, 100, 200); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(48, 7, 0, 200, 325); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(49, 7, 0, 325, 450); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(50, 7, 0, 500, 650); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(51, 7, 0, 650, 800); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(52, 7, 0, 800, 1000); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(53, 9, 0, 300, 400); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(53, 10, 0, 300, 400); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(54, 9, 0, 400, 500); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(54, 10, 0, 400, 500); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(55, 9, 0, 500, 600); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(55, 10, 0, 500, 600); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(56, 9, 0, 600, 700); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(56, 10, 0, 600, 700); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(57, 9, 0, 700, 850); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(57, 10, 0, 700, 850); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(58, 9, 0, 850, 1000); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(58, 10, 0, 850, 1000); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(41, 8, 0, 1, 1); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(42, 8, 0, 2, 2); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(43, 8, 0, 3, 3); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(44, 8, 0, 4, 4); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(45, 8, 0, 6, 5); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(46, 8, 0, 8, 6); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(1, 2, 0, 100, 100); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(2, 2, 0, 200, 200); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(3, 2, 0, 300, 300); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(4, 2, 0, 400, 500); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(5, 2, 0, 100, 1000); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(6, 4, 0, 50, 50); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(7, 4, 0, 100, 100); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(8, 4, 0, 150, 150); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(9, 4, 0, 200, 250); \
INSERT INTO item_bonus_template(item_id, stat_id, value_type, min_value, max_value) VALUES(10, 4, 0, 250, 500); \
INSERT INTO item_shop(item_id, type_shop) VALUES(1, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(2, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(3, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(4, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(5, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(6, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(7, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(8, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(9, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(10, 3); \
INSERT INTO item_shop(item_id, type_shop) VALUES(11, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(12, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(13, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(14, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(15, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(16, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(17, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(18, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(19, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(20, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(21, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(22, 0); \
INSERT INTO item_shop(item_id, type_shop) VALUES(23, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(24, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(25, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(26, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(27, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(28, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(29, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(30, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(31, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(32, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(33, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(34, 1); \
INSERT INTO item_shop(item_id, type_shop) VALUES(35, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(36, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(37, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(38, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(39, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(40, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(41, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(42, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(43, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(44, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(45, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(46, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(47, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(48, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(49, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(50, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(51, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(52, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(53, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(54, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(55, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(56, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(57, 2); \
INSERT INTO item_shop(item_id, type_shop) VALUES(58, 2); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(1, 1, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(2, 2, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(3, 3, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(4, 4, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(5, 5, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(6, 6, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(7, 7, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(8, 8, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(9, 9, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(10, 10, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(11, 11, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(12, 12, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(13, 13, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(14, 14, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(15, 15, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(16, 16, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(17, 17, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(18, 18, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(19, 19, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(20, 20, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(21, 21, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(22, 22, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(23, 23, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(24, 24, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(25, 25, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(26, 25, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(27, 27, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(28, 28, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(29, 29, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(30, 30, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(31, 31, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(32, 32, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(33, 33, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(34, 34, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(35, 35, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(36, 36, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(37, 37, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(38, 38, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(39, 39, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(40, 40, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(41, 41, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(42, 42, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(43, 43, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(44, 44, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(45, 45, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(46, 46, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(47, 47, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(48, 48, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(49, 49, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(50, 50, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(51, 51, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(52, 52, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(53, 53, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(54, 54, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(55, 55, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(56, 56, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(57, 57, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(58, 58, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(59, 23, 10); \
INSERT INTO items(item_id, template_id, price_sell) VALUES(60, 29, 10); ")
+ std::string("\
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(1, 'Mini HP potion', 100, 1, 'Recover 100 Hp', 200, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(2, 'Small HP potion', 100, 10, 'Recover 200 Hp', 500, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(3, 'Average HP potion', 100, 20, 'Recover 300 Hp', 1000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(4, 'Large HP potion', 100, 30, 'Recover 500 Hp', 2000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(5, 'Max HP potion', 100, 40, 'Recover 1000 Hp', 3000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(6, 'Mini MP potion', 101, 1, 'Recover 50 Mp', 200, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(7, 'Small MP potion', 101, 10, 'Recover 100 Mp', 500, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(8, 'Average MP potion', 101, 20, 'Recover 150 Mp', 1000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(9, 'Large MP potion', 101, 30, 'Recover 250 Mp', 2000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(10, 'Max MP potion', 101, 40, 'Recover 500 Mp', 3000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(11, 'White crane sword', 0, 1, NULL, 900, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(12, 'Heavy thunder sword', 0, 10, NULL, 9000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(13, 'Silver sword', 0, 20, NULL, 20000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(14, 'Gold sword', 0, 30, NULL, 45000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(15, 'Phoenix sword', 0, 40, NULL, 90000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(16, 'Dragon sword', 0, 50, NULL, 190000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(17, 'Silver shuriken', 1, 1, NULL, 700, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(18, 'Osun shuriken', 1, 10, NULL, 7000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(19, 'Badass shuriken', 1, 20, NULL, 15000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(20, 'Gold shuriken', 1, 30, NULL, 30000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(21, 'Phoenix shuriken', 1, 40, NULL, 60000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(22, 'Dragon shuriken', 1, 50, NULL, 120000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(23, 'Duffel shirt', 2, 1, NULL, 500, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(24, 'Leather jacket', 2, 10, NULL, 5000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(25, 'Silver shirt', 2, 20, NULL, 10000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(26, 'Gold shirt', 2, 30, NULL, 20000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(27, 'Phoenix shirt', 2, 40, NULL, 50000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(28, 'Dragon shirt', 2, 50, NULL, 100000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(29, 'Duffel pants', 3, 1, NULL, 400, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(30, 'Leather pants', 3, 10, NULL, 4000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(31, 'Silver pants', 3, 20, NULL, 8000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(32, 'Gold pants', 3, 30, NULL, 18000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(33, 'Phoenix pants', 3, 40, NULL, 45000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(34, 'Dragon pants', 3, 50, NULL, 90000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(35, 'Silk gloves', 4, 1, NULL, 700, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(36, 'Leather gloves', 4, 10, NULL, 7000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(37, 'Silver gloves', 4, 20, NULL, 14000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(38, 'Gold gloves', 4, 30, NULL, 30000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(39, 'Phoenix gloves', 4, 40, NULL, 60000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(40, 'Dragon gloves', 4, 50, NULL, 120000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(41, 'Silk shoes', 5, 1, NULL, 300, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(42, 'Leather shoes', 5, 10, NULL, 3000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(43, 'Silver shoes', 5, 20, NULL, 6000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(44, 'Gold shoes', 5, 30, NULL, 15000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(45, 'Phoenix shoes', 5, 40, NULL, 30000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(46, 'Dragon shoes', 5, 50, NULL, 70000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(47, 'Marble gem', 6, 1, NULL, 600, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(48, 'Ivory gem', 6, 10, NULL, 6000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(49, 'Amber gem', 6, 20, NULL, 12000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(50, 'Secret gem', 6, 30, NULL, 24000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(51, 'Phoenix gem', 6, 40, NULL, 55000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(52, 'Dragon gem', 6, 50, NULL, 11000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(53, 'Silver necklace', 7, 1, NULL, 600, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(54, 'Emerald necklace', 7, 10, NULL, 6000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(55, 'Jade necklace', 7, 20, NULL, 15000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(56, 'Gold necklace', 7, 30, NULL, 35000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(57, 'Phoenix necklace', 7, 40, NULL, 75000, NULL, 0); \
INSERT INTO items_template(item_id, item_name, type, level_require, description, price_buy, price_sell, lock) VALUES(58, 'Dragon necklace', 7, 50, NULL, 150000, NULL, 0); \
INSERT INTO map(map_id, map_name) VALUES(1, 'Uchiha Clan'); \
INSERT INTO map(map_id, map_name) VALUES(2, 'Konoha'); \
INSERT INTO map(map_id, map_name) VALUES(3, 'Behind Konoha'); \
INSERT INTO map(map_id, map_name) VALUES(4, 'Strange Hill'); \
INSERT INTO map(map_id, map_name) VALUES(5, 'Lake'); \
INSERT INTO map(map_id, map_name) VALUES(6, 'Strange Mountain Range'); \
INSERT INTO map(map_id, map_name) VALUES(7, 'Green Moss Forest'); \
INSERT INTO map(map_id, map_name) VALUES(8, 'Ancient Ruins'); \
INSERT INTO map(map_id, map_name) VALUES(9, 'Copper Road'); \
INSERT INTO map(map_id, map_name) VALUES(10, 'Moonless Night'); \
INSERT INTO map(map_id, map_name) VALUES(11, 'Stone Cave'); \
INSERT INTO map(map_id, map_name) VALUES(12, 'Snow Mountain'); \
INSERT INTO map(map_id, map_name) VALUES(13, 'Snow Hill'); \
INSERT INTO map(map_id, map_name) VALUES(14, 'Rock Mountain'); \
INSERT INTO map(map_id, map_name) VALUES(15, 'Red Rock Mountain'); \
INSERT INTO map(map_id, map_name) VALUES(16, 'Arena'); ")
+ std::string("\
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 1, 'I am Chief Neko Sennin, honored to welcome you.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 2, 'It is said that the Samurai lord has just been resurrected.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 1, 1, 'Haha, you''re so good, looks like you passed the test brilliantly.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(2, 0, 1, 'Next, go to Konoha to chase the small monsters.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(2, 1, 1, 'Very good.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(3, 1, 1, 'Very good, looks like you''ve gotten used to using Shuriken already!'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(3, 0, 1, 'Aside from melee, i want you to know more about the Shuriken''s ranged attacks.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(3, 0, 2, 'Let''s try combining Sword with Shuriken to destroy monsters for me.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(4, 1, 1, 'Very well, it seems you are quite proficient already!'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(4, 0, 1, 'To improve your skills even more, I want to ask you a favor'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(4, 0, 2, 'go to Behind Konoha to defeat those aggressive monsters.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(8, 1, 1, 'I''m really good at it'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(8, 0, 1, 'Go to Moonless Night to defeat the fourth king Shadow Ninja.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(7, 1, 1, 'I''m really good at it'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(7, 0, 1, 'Go to Stone Cave to defeat the third king Ghost Pumpkin.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(6, 1, 1, 'I''m really good at it.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(6, 0, 1, 'Go to Green Moss Forest to defeat the second king Hell Scorpion.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(5, 1, 1, 'I''m really good at it.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(5, 0, 1, 'To defeat the Samurai lord, you need to defeat the four kings of hell under his command.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(5, 0, 2, 'Go to Strange Hill to defeat the first king Wild Boar.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 3, 'He is trying to organize large scale attacks on our village.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 4, 'With natural talent, more qualified than humans, you will be the chosen one'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 5, 'representing the village to seek and destroy the darkness.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 6, 'Are you ready for the adventure?'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 0, 7, 'First come and destroy 5 straw scarecrow for me.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(1, 1, 2, 'I see from you a great source of strength, try to practice.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(9, 0, 1, 'You are strong enough now'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(9, 0, 2, 'but I still advise you to prepare well before heading to the Arena.'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(9, 0, 3, 'Good luck!'); \
INSERT INTO quest_conversation(quest_id, conversation_type, conversation_index, conversation) VALUES(9, 1, 1, 'acb'); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(1, 1, 5, 1); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(2, 2, 10, 2); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(2, 3, 15, 2); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(3, 5, 25, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(3, 6, 30, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(4, 7, 30, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(4, 8, 30, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(4, 9, 30, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(5, 10, 1, 3); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(6, 20, 1, 4); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(7, 30, 1, 7); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(8, 40, 1, 10); \
INSERT INTO quest_enemy(quest_id, enemy_id, quantity, map_id) VALUES(9, 50, 1, 16); \
INSERT INTO quest_item(quest_id, item_id, quantity) VALUES(1, 11, 1); \
INSERT INTO quest_item(quest_id, item_id, quantity) VALUES(3, 17, 1); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(2, 2); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(5, 4); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(3, 3); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(5, 5); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(5, 6); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(6, 7); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(6, 8); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(6, 9); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(7, 12); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(7, 13); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(7, 11); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(8, 10); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(8, 14); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(8, 15); \
INSERT INTO quest_know_map(quest_id, map_id) VALUES(9, 16); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(1, 'Learn to use a sword', 0, 100, 5000, 'Into the inventory use sword.'); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(2, 'Defeat Monsters', 0, 200, 10000, ''); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(3, 'Learn to use darts', 5, 500, 15000, 'Into the inventory use shuriken.'); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(4, 'Defeat Monsters', 0, 1000, 20000, NULL); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(5, 'Monster leader', 10, 2000, 25000, NULL); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(6, 'Second leader', 20, 4000, 30000, NULL); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(7, 'Third Leader', 30, 8000, 50000, NULL); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(8, 'Fourth leader', 40, 16000, 100000, NULL); \
INSERT INTO quests(quest_id, quest_name, level_require, exp_reward, coin_reward, description) VALUES(9, 'Strongest Demon Lord', 50, 0, 1000000, NULL); ")
+ std::string("\
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 1, 5, 0, 1, 10000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 2, 5, 0, 1, 11000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 3, 5, 0, 1, 12000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 4, 5, 0, 1, 13000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 5, 5, 0, 1, 14000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 6, 5, 0, 1, 15000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 7, 5, 0, 1, 16000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 1, 5, 0, 1, 5000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 2, 5, 0, 1, 6000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 3, 5, 0, 1, 7000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 4, 5, 0, 1, 8000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 5, 5, 0, 1, 9000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 6, 5, 0, 1, 10000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 7, 5, 0, 1, 11000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 8, 5, 0, 1, 17000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 9, 5, 0, 1, 18000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(1, 10, 5, 0, 1, 19000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 8, 5, 0, 1, 12000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 9, 5, 0, 1, 13000, 0, 0); \
INSERT INTO skill_bonus(skill_id, level, stat_id, bonus_type, value_type, value, gap_time, duration) VALUES(2, 10, 5, 0, 1, 14000, 0, 0); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 1, 1, 600, 1); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 2, 2, 600, 1); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 3, 3, 575, 2); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 4, 4, 575, 2); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 5, 5, 550, 3); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 6, 6, 550, 3); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 7, 7, 525, 4); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 1, 10, 800, 3); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 2, 11, 800, 3); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 3, 12, 775, 4); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 4, 14, 775, 4); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 5, 15, 750, 6); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 6, 16, 750, 6); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 7, 17, 725, 7); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 8, 8, 525, 4); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 9, 9, 500, 5); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(1, 10, 10, 450, 5); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 8, 18, 600, 6); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 9, 19, 650, 6); \
INSERT INTO skill_level(skill_id, level, level_require, cooldown, mp_use) VALUES(2, 10, 20, 700, 7); \
INSERT INTO skills(skill_id, skill_name, type, ""range"", description, level_max) VALUES(1, 'Hiyoko', 1, 100.0, 'Basic sword skill to damage target', 10); \
INSERT INTO skills(skill_id, skill_name, type, ""range"", description, level_max) VALUES(2, 'Kasoushuriken', 2, 600.0, 'Entry level shuriken throwing skill to damage a given target', 10); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(1, 'HpMax', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(2, 'Hp', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(3, 'MpMax', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(4, 'Mp', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(5, 'Attack', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(6, 'Defense', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(7, 'Crit', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(8, 'Speed', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(9, 'Regenerate Hp', NULL); \
INSERT INTO stats(stat_id, stat_name, description) VALUES(10, 'Regenerate Mp', NULL); \
");
	return execute(sql);
}
