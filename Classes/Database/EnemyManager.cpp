#include "EnemyManager.h"
#include "DatabaseManager.h"
#include "Entity/Entities.h"

Enemy* EnemyManager::getEnemy(std::string name)
{
	Enemy* enemy = nullptr;
	std::string query = "select  enemy_id, enemy_name, level, type from enemies where enemy_name = '" + name + "' ;";
	DatabaseManager::getInstance()->loadData(query, &_stmt);
	if (sqlite3_step(_stmt) == SQLITE_ROW) {
		enemy = Enemy::create();
		enemy->setId(sqlite3_column_int(_stmt, 0));
		enemy->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(_stmt, 1))));
		enemy->setType((Enemy::EnemyType)sqlite3_column_int(_stmt, 3));
		enemy->setLevel(sqlite3_column_int(_stmt, 2));
	}
	return enemy;
}
