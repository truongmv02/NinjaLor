#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
USING_NS_CC;
class Enemy;
class EnemyManager :public Ref
{
public:
	bool init() { return true; }
	Enemy* getEnemy(std::string name);
	CREATE_FUNC(EnemyManager);
private:
	sqlite3_stmt* _stmt;
	Enemy* getEnemy(sqlite3_stmt* stmt);
};
