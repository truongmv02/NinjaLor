#include "MapManager.h"
#include "DatabaseManager.h"

int AManager::getMapId(std::string mapName)
{
	query = std::string("select map_id from map where map_name = '") + mapName + "';";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		return sqlite3_column_int(stmt, 0);
	}
	return 1;
}

std::string AManager::getMapName(int id)
{
	query = "select map_name from map where map_id = " + std::to_string(id) + ";";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		return std::string(reinterpret_cast<const char*> (sqlite3_column_text(stmt, 0)));
	}
	return "";
}
