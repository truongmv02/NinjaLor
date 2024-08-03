#include "Stat.h"
#include "LifeEntity.h"

Bonus::Bonus() {
	_value = 0;
	_duration = 0;
	_elapsedTime = 0;
	_timeStarted = 0;
	_timeGap = 0;
	_valueType = Stat::ValueType::NUMBER;
}

Bonus* Bonus::clone()
{
	Bonus* bonus = Bonus::create();
	bonus->setValue(_value);
	bonus->setValueType(_valueType);
	bonus->setDuration(_duration);
	bonus->setElapsedTime(_elapsedTime);
	bonus->setTimeGap(_timeGap);
	bonus->setTimeStarted(_timeStarted);
	bonus->setStatId(_statId);
	return bonus;
}


Stat::Stat() {
	_value = 0;
	_owner = nullptr;
	_valueType = ValueType::NUMBER;
}


Stat* Stat::create(std::string name, int value) {
	Stat* stat = new (std::nothrow) Stat();
	if (stat && stat->init(name, value)) {
		stat->retain();
		return stat;
	}
	CC_SAFE_DELETE(stat);
	return stat;
}

bool Stat::init(std::string name, int value) {
	_name = name;
	_value = value;
	return true;
}

Stat* Stat::getStat(std::string statName, Vector<Stat*>stats) {
	for (auto stat : stats) {
		if (stat->getName() == statName) {
			return stat;
		}
	}
	return nullptr;
}

Stat* Stat::getStat(int id, Vector<Stat*> stats)
{
	for (auto stat : stats) {
		if (stat->getId() == id) {
			return stat;
		}
	}
	return nullptr;
}

void Stat::setId(int id)
{
	_id = id;
	if (id == 7 || id == 9 || id == 10) {
		_valueType = ValueType::PERCENT;
	}
}

Vector<Bonus*> Stat::getBonuses(std::string statName, Vector<Stat*>stats) {
	Vector<Bonus*> bonuses;
	for (auto stat : stats) {
		if (stat->getName() == statName) {
			bonuses.pushBack(stat->getBonuses());
		}
	}
	return bonuses;
}


int Stat::getCurrentValue() {
	int val = _value;
	if (_bonuses.size() > 0) {
		for (auto bonus : _bonuses) {
			if (bonus->_valueType == Stat::ValueType::NUMBER) {
				val += bonus->_value;
			}
			else {
				val += _value * bonus->_value / 10000.0f;
			}
		}
	}
	return val;
}

Stat* Stat::clone()
{
	Stat* stat = Stat::create();
	stat->setId(_id);
	stat->setName(_name);
	stat->setDescription(_description);
	stat->setOwner(_owner);
	stat->setValue(_value);
	Vector<Bonus*> bonuses;
	for (auto bonus : _bonuses) {
		bonuses.pushBack(bonus->clone());
	}
	stat->setBonuses(bonuses);
	return stat;
}

void Stat::clear()
{
	for (auto bonus : _bonuses) {
		CC_SAFE_DELETE(bonus);
	}
}

void Stat::update(float dt) {

	if (_bonuses.size() == 0)return;
	for (int i = _bonuses.size() - 1; i >= 0; i--)
	{
		auto bonus = _bonuses.at(i);
		if ((_name == "Hp" || _name == "Mp") && _owner) {
			auto currentStat = _owner->getStat(_name);
			if (_name == "Hp")
				_owner->setHp(currentStat->getValue() + bonus->getValue());
			else
				_owner->setMp(currentStat->getValue() + bonus->getValue());

			_bonuses.eraseObject(bonus);
			CC_SAFE_DELETE(bonus);
			continue;
		}
		bonus->_elapsedTime += dt * 1000;

		if (bonus->_duration == 0) {
			bonus->_elapsedTime = 0;
			continue;
		}
		if (bonus->_elapsedTime >= bonus->_duration) {
			_bonuses.erase(i);
		}
	}


}

void Stat::AddBonus(Bonus* bonus) {
	_bonuses.pushBack(bonus);
}

void Stat::AddBonuses(Vector<Bonus*> bonuses) {
	_bonuses.pushBack(bonuses);
}

void Stat::removeBonus(Bonus* bonus)
{
	_bonuses.eraseObject(bonus);
}

StatTemplate::StatTemplate()
{
	_minValue = 0;
	_maxValue = 0;
}

StatTemplate* StatTemplate::getStat(int id, Vector<StatTemplate*> stats)
{
	for (auto stat : stats) {
		if (stat->getId() == id) {
			return stat;
		}
	}
	return nullptr;
}
