#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include "Entity/Character/Character.h"

USING_NS_CC;

class CharacterManager :public Ref
{
public:
	bool init() { return true; }
	Character* getCharacter();

	bool updateCoin(int coin);
	bool updateExp(int exp);
	bool updateLevel(int level);
	bool updateHp(int hp);
	bool updateMp(int mp);
	bool updateMap(int id);
	bool addTeleMap(int id);
	bool addKnowMap(int id);
	std::string getMap();
	bool updatePosition(Vec2 position);
	std::vector<int> getKnowMaps();
	int getExp(int level);
	bool canTeleMap(std::string mapName);
	Vec2 getCharacterPosition();
	CREATE_FUNC(CharacterManager);
private:
	sqlite3_stmt* _stmt;
	Character* _character;
	std::string query;
};
