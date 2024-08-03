#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include "Game/Quest.h"

USING_NS_CC;
class QuestManager :public Ref
{
public:
	bool init() { return true; }
	Quest* getCurrentQuest();
	std::vector<Quest::EnemyQuest> getQuestEnemies(int questId);
	std::vector<Quest::ItemQuest> getQuestItems(int questId);
	std::vector<std::string> getConversation(int questId, Quest::Status status);
	std::vector<int> getKnowMaps(int questId);
	Quest* getQuest(int id);
	bool updateQuest(Quest* quest);
	bool updateStatus(Quest::Status status);
	bool updateEnemies(int id, int kill);
	CREATE_FUNC(QuestManager);
private:
	std::string query;
	sqlite3_stmt* stmt;
	Quest* getQuest(sqlite3_stmt* stmt);
};
