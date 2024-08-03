#pragma once
#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "Database/DatabaseManager.h"
USING_NS_CC;

class Quest : public Ref
{
public:
	typedef struct {
		int EnemyId;
		std::string EnemyName;
		int Quantity;
		int Kill;
	} EnemyQuest;

	typedef struct {
		int ItemId;
		int Quantity;
	} ItemQuest;

	enum Status
	{
		UNRECEIVED,
		UNFINISHED,
		COMPLETE
	};

	bool init();
	inline std::vector<EnemyQuest> getEnemies() { return _enemies; }
	void setEnemyQuantity(int id);
	void setStatus(Status status);
	CC_SYNTHESIZE(int, _id, Id);
	CC_SYNTHESIZE(std::string, _name, Name);
	CC_SYNTHESIZE(int, _coin, Coin);
	CC_SYNTHESIZE(int, _exp, Exp);
	CC_SYNTHESIZE_READONLY(Status, _status, Status);
	CC_SYNTHESIZE(int, _levelRequire, LevelRequire);
	CC_SYNTHESIZE(std::string, _desc, Description);
	CREATE_SET(std::vector<EnemyQuest>, _enemies, Enemies);
	CC_SYNTHESIZE(std::vector<std::string >, _conversation1, Conversation1);
	CC_SYNTHESIZE(std::vector<std::string >, _conversation2, Conversation2);
	CC_SYNTHESIZE(std::vector<ItemQuest>, _items, Items);
	CREATE_FUNC_RETAIN(Quest);
private:

};
