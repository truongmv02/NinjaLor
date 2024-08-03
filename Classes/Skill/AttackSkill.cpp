#include "RangeSkill.h"
#include "Entity/Entities.h"
#include "AttackSkill.h"
#include "Manager/GameManager.h"

bool AttackSkill::init()
{

	return true;
}

void AttackSkill::update(float dt)
{
	Skill::update(dt);
}

bool AttackSkill::use(LifeEntity* target)
{
	if (_elapsedTime < _cooldown) {
		return false;
	}
	auto pos = _owner->getPosition();
	auto targetPos = target->getPosition();

	auto distance = (pos - targetPos).getLength();

	if (distance > _range) {
		auto dt = Director::getInstance()->getAnimationInterval();
		auto cha = dynamic_cast<Character*> (_owner);
		if (cha) {
			cha->setMoveTarget(true);
			cha->unschedule("Move");
			cha->schedule(CC_CALLBACK_1(Character::move, cha), dt, "Move");
			CallbackTimeCounter* timeCounter = CallbackTimeCounter::create();
			timeCounter->initWithCondition([=]() {
				if (cha->isScheduled("Move")) {
					cha->unschedule("Move");
					cha->setMoveTarget(false);
				}
				timeCounter->unscheduleUpdate();
				timeCounter->removeFromParent();
				}, [&, target, cha]() {
					if (!cha->getMoveTarget()) return true;
					auto ownerPos = _owner->getPosition();
					auto tarPos = target->getPosition();
					auto length = (ownerPos - tarPos).getLength();

					if (length <= _range) {
						return true;
					}

					if (abs(ownerPos.x - tarPos.x) < 50) {
						return true;
					}

					return false;
				});
			this->addChild(timeCounter);
		}
		return false;
	}


	return true;
}

bool AttackSkill::attack(LifeEntity* target)
{
	int dame = 0;
	bool crit = false;
	auto enemy = dynamic_cast<Enemy*>(target);
	auto character = dynamic_cast<Character*> (_owner);
	Item* weapon = nullptr;

	if (character) {
		auto weaponType = dynamic_cast<MelleSkill*>(this) ? Item::MELEE_WEAPON : Item::RANGE_WEAPON;
		weapon = Item::findItem(character->getEquipments(), weaponType);
		if (!weapon) {
			auto message = Message::create("Inappropriated weapon");
			GameManager::getInstance()->getHUDLayer()->addMessage(message);
			return false;
		}
	}

	if (enemy->getType() == Enemy::EnemyType::SCARECROW) {
		dame = 10;
	}
	else {
		auto attack = _owner->getStat("Attack");
		auto skillBonus = attack->getValue() * Stat::getStat("Attack", _stats)->getCurrentValue() / 10000.0f;
		dame = attack->getCurrentValue() + skillBonus;//attack->getValue() * Stat::getStat("Attack", _stats)->getCurrentValue() / 10000.0f;
		if (character) {
			int rand = RandomHelper::random_int(0, 10000);
			if (rand <= character->getStat("Crit")->getCurrentValue()) {
				dame *= 2;
				crit = true;
			}
		}
		if (weapon)
			dame += Stat::getStat("Attack", weapon->getStats())->getCurrentValue();
		auto hp = target->getStat("Hp")->getValue();
		dame = dame <= hp ? dame : hp;
	}

	if (character && enemy->getType() != Enemy::EnemyType::BOSS) {
		int exp = dame / 10.0f;
		auto level = target->getLevel() - _owner->getLevel();
		if (level < 0) {
			exp = exp - (exp * abs(level) / 25.0f);
		}

		exp = exp >= 1 ? exp : 1;
		character->setCurrentExp(character->getCurrentExp() + exp, true);
	}

	auto hp = target->getStat("Hp");
	target->setHp(hp->getValue() - dame);
	Utility::createLabelDame(dame, target, crit);
	if (enemy) {
		enemy->setEntityFocus((Character*)_owner);
		enemy->doAction(LifeEntity::HURT);
	}

	return true;
}

void AttackSkill::initEffect()
{

}

void AttackSkill::createEffect()
{
}
