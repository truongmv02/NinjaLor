#pragma once

#include "sqlite3.h"
#include "cocos2d.h"
USING_NS_CC;

class AManager : public Ref
{
public:
	bool init() { return true; }
	int getMapId(std::string mapName);
	std::string getMapName(int id);
	CREATE_FUNC(AManager);
private:
	sqlite3_stmt* stmt;
	std::string query;
};
