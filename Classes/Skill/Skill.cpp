#include "Skill.h"
#include "Manager/GameManager.h"
Skill::Skill()
{

}

bool Skill::init()
{
	return true;
}

Skill* Skill::getSkillByIndex(Vector<Skill*> skills, int index)
{
	for (auto skill : skills) {
		if (skill->getIndex() == index) return skill;
	}
	return nullptr;
}

void Skill::update(float dt)
{
	if (_elapsedTime < _cooldown) {
		_elapsedTime += dt * 1000;
	}

}

bool Skill::use(LifeEntity* target)
{
	return true;
}

void Skill::clear()
{
	clearStat();
}

void Skill::clearStat()
{
	for (auto stat : _stats) {
		stat->clear();
		CC_SAFE_DELETE(stat);
	}
}
