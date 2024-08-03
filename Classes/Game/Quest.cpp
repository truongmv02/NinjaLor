#include "Quest.h"
#include "Database/QuestManager.h"

bool Quest::init()
{
	return true;
}

void Quest::setEnemyQuantity(int id)
{
	for (auto& e : _enemies) {
		if (e.EnemyId == id) {
			e.Kill++;
			QuestManager::create()->updateEnemies(id, e.Kill);
		}
	}
}

void Quest::setStatus(Status status)
{
	_status = status;
	QuestManager::create()->updateStatus(status);
}

