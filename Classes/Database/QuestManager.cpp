#include "QuestManager.h"

Quest* QuestManager::getCurrentQuest()
{
	Quest* quest = nullptr;
	query =
		//				0		1				2				3			 4				5
		"select cq.quest_id, q.quest_name, q.level_require, q.exp_reward, q.coin_reward, cq.status from character_quest cq inner join quests q on cq.quest_id = q.quest_id;";

	DatabaseManager::getInstance()->loadData(query, &stmt);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		quest = getQuest(stmt);
		quest->setStatus((Quest::Status)sqlite3_column_int(stmt, 5));
		quest->setEnemies(getQuestEnemies(quest->getId()));
		quest->setItems(getQuestItems(quest->getId()));
		quest->setConversation1(getConversation(quest->getId(), Quest::Status::UNRECEIVED));
		quest->setConversation2(getConversation(quest->getId(), Quest::Status::UNFINISHED));
	}

	return quest;
}

Quest* QuestManager::getQuest(int id)
{
	Quest* quest = nullptr;
	query =
		//				0		1				2				3			4				
		"select q.quest_id, q.quest_name, q.level_require, q.exp_reward, q.coin_reward from quests q where q.quest_id = " + std::to_string(id) + ";";

	DatabaseManager::getInstance()->loadData(query, &stmt);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		quest = getQuest(stmt);
		quest->setEnemies(getQuestEnemies(quest->getId()));
		quest->setItems(getQuestItems(quest->getId()));
		quest->setConversation1(getConversation(quest->getId(), Quest::Status::UNRECEIVED));
		quest->setConversation2(getConversation(quest->getId(), Quest::Status::UNFINISHED));
	}

	return quest;
}

bool QuestManager::updateQuest(Quest* quest)
{
	query = "delete from character_quest_enemy; ";
	DatabaseManager::getInstance()->execute(query);
	for (auto e : quest->getEnemies()) {
		query = "INSERT INTO character_quest_enemy (enemy_id, kill) VALUES (" + std::to_string(e.EnemyId) + ",0);";
		DatabaseManager::getInstance()->execute(query);
	}
	query = "update character_quest set quest_id =" + std::to_string(quest->getId()) + ";";
	DatabaseManager::getInstance()->execute(query);
	query = "update character_quest set status =1;";
	DatabaseManager::getInstance()->execute(query);

	auto knowMap = getKnowMaps(quest->getId());
	for (auto map : knowMap) {
		query = "INSERT INTO character_map(map_id, status) VALUES(" + std::to_string(map) + ", 0); ";
		DatabaseManager::getInstance()->execute(query);
	}

	return true;
}


std::vector<Quest::EnemyQuest> QuestManager::getQuestEnemies(int questId)
{
	std::vector<Quest::EnemyQuest> enemies;
	query = "select qe.enemy_id, e.enemy_name, qe.quantity, ifnull(cqe.kill,0) as kill from quest_enemy qe inner join enemies e on qe.enemy_id = e.enemy_id left join character_quest_enemy cqe on qe.enemy_id = cqe.enemy_id  where quest_id =" + std::to_string(questId) + " order by qe.enemy_id;";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Quest::EnemyQuest enemy;
		enemy.EnemyId = sqlite3_column_int(stmt, 0);
		enemy.EnemyName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		enemy.Quantity = sqlite3_column_int(stmt, 2);
		enemy.Kill = sqlite3_column_int(stmt, 3);
		enemies.push_back(enemy);
	}
	return enemies;
}

std::vector<Quest::ItemQuest> QuestManager::getQuestItems(int questId)
{
	std::vector<Quest::ItemQuest> items;
	query = "select item_id, quantity from quest_item where quest_id = " + std::to_string(questId) + ";";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Quest::ItemQuest item;
		item.ItemId = sqlite3_column_int(stmt, 0);
		item.Quantity = sqlite3_column_int(stmt, 1);
		items.push_back(item);
	}
	return items;
}

std::vector<std::string> QuestManager::getConversation(int questId, Quest::Status status)
{
	std::vector<std::string> reponses;
	query = "select conversation from quest_conversation where quest_id = " + std::to_string(questId) + " and conversation_type = " + std::to_string(status) + " order by conversation_index;";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string reponse;
		reponse = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
		reponses.push_back(reponse);
	}
	return reponses;
}

std::vector<int> QuestManager::getKnowMaps(int questId)
{
	std::vector<int> maps;
	query = "select ifnull(map_id,'') as map_name from quest_know_map where quest_id = " + std::to_string(questId) + "; ";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		int map = (sqlite3_column_int(stmt, 0));
		maps.push_back(map);
	}
	return maps;
}



bool QuestManager::updateStatus(Quest::Status status)
{
	query = "update character_quest set status = " + std::to_string((int)status) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

bool QuestManager::updateEnemies(int id, int kill)
{
	query = "update character_quest_enemy set kill = " + std::to_string(kill) + " where enemy_id = " + std::to_string(id) + ";";
	return DatabaseManager::getInstance()->execute(query);
}


Quest* QuestManager::getQuest(sqlite3_stmt* stmt)
{
	auto quest = Quest::create();
	quest->setId(sqlite3_column_int(stmt, 0));
	quest->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
	quest->setLevelRequire(sqlite3_column_int(stmt, 2));
	quest->setExp(sqlite3_column_int(stmt, 3));
	quest->setCoin(sqlite3_column_int(stmt, 4));
	return quest;
}
