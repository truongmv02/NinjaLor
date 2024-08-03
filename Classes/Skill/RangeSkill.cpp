#include "RangeSkill.h"
#include "Entity/Entities.h"
#include "Manager/GameManager.h"

bool RangeSkill::init()
{
	if (!Skill::init()) return false;

	return true;
}

void RangeSkill::update(float dt)
{
	AttackSkill::update(dt);
}

bool RangeSkill::use(LifeEntity* target)
{
	if (!AttackSkill::use(target)) return false;
	auto pos = _owner->getPosition();
	auto targetPos = target->getPosition();

	if (!attack(target)) {
		return false;
	}

	auto bullet = Bullet::create(target, PATH_IMAGE_BULLET + "shuriken.png");
	bullet->setSpeed(50);
	bullet->setPosition(Vec2(pos.x, pos.y + _owner->getContentSize().height * 0.3));
	_owner->getParent()->addChild(bullet, 5);

	auto character = dynamic_cast<Character*>(_owner);
	if (character) {
		CallbackTimeCounter* timeCounter = CallbackTimeCounter::create();
		character->addChild(timeCounter);
		timeCounter->init(0.24, [=]() {
			character->getStateMachine()->popState();
			timeCounter->unscheduleUpdate();
			timeCounter->removeFromParent();
			});
	}

	_elapsedTime = 0;
	return true;
}

void RangeSkill::initEffect()
{
	std::string charFrames[] = { "attack1", "attack3", "attack4" };
	float delay = 0.08;
	auto cha = (Character*)_owner;
	auto cloth = Item::findItem(cha->getEquipments(), Item::CLOTH);
	auto pants = Item::findItem(cha->getEquipments(), Item::PANTS);
	std::string clothString = "character/" + (cloth != nullptr ? cloth->getName() : "Duffel shirt");
	std::string pantsString = "character/" + (pants != nullptr ? pants->getName() : "Duffel pants");
	auto ef = Utility::createAnimation(charFrames, 3, delay, "character/head_");
	AnimationCache::getInstance()->addAnimation(ef, "head_" + _name);
	ef = Utility::createAnimation(charFrames, 3, delay, clothString + "_");
	AnimationCache::getInstance()->addAnimation(ef, "cloth_" + _name);
	ef = Utility::createAnimation(charFrames, 3, delay, pantsString + "_");
	AnimationCache::getInstance()->addAnimation(ef, "pants_" + _name);
}

void RangeSkill::createEffect()
{
}
