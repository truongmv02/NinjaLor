#pragma once
#include "sqlite3.h"
#include "cocos2d.h"

USING_NS_CC;
#define DATABASE_PATH std::string(FileUtils::getInstance()->getWritablePath() +"datab.sql")

class DatabaseManager
{
public:
	static DatabaseManager* getInstance();
	bool loadData(std::string query, sqlite3_stmt** stmt);
	bool execute(std::string query);
	void reset();
protected:
	DatabaseManager();
	bool createDatabase();
	bool insertData();
private:
	static DatabaseManager* _instance;
	sqlite3* db;
};
