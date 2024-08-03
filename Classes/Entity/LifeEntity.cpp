#include "LifeEntity.h"
#include "Entity/Character/Character.h"

LifeEntity::LifeEntity() {
	//_state = IDLE;
	_stateMachine = new StateMachine();

}

bool LifeEntity::init()
{
	if (!Entity::init()) return false;
	return true;
}

Stat* LifeEntity::getStat(std::string name)
{
	return Stat::getStat(name, _stats);
}

void LifeEntity::addStats(Vector<Stat*> stats) {
	_stats.pushBack(stats);
	for (auto stat : stats) {
		stat->setOwner(this);
	}
}


void LifeEntity::update(float dt) {
	for (auto stat : _stats) {
		stat->update(dt);
	}
}

void LifeEntity::setHp(int hp)
{
	auto hpMax = getStat("HpMax");
	auto hpStat = getStat("Hp");
	auto currentHp = hpStat->getValue();

	auto hpReal = hp < hpMax->getCurrentValue() ? hp : hpMax->getCurrentValue();
	hpStat->setValue(hpReal >= 0 ? hpReal : 0);

	if (hpReal - currentHp > 0 && dynamic_cast<Character*>(this)) {
		Utility::createLabelRegenerateHp(hpReal - currentHp, this);
	}


}

void LifeEntity::setMp(int mp)
{
	auto mpMax = getStat("MpMax");
	auto mpStat = getStat("Mp");
	auto currentMp = mpStat->getValue();
	auto mpReal = mp <= mpMax->getCurrentValue() ? mp : mpMax->getCurrentValue();
	mpStat->setValue(mpReal >= 0 ? mpReal : 0);
	if (mpReal - currentMp > 0 && dynamic_cast<Character*>(this)) {
		Utility::createLabelRegenerateMp(mpReal - currentMp, this);
	}


}

void LifeEntity::setStats(Vector<Stat*> stats)
{
	_stats = stats;
	for (auto stat : _stats) {
		stat->setOwner(this);
	}
}

