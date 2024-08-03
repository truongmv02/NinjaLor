#include "MelleSkill.h"
#include "Entity/Entities.h"

bool MelleSkill::init() {
	if (!AttackSkill::init()) return false;

	return true;
}

void MelleSkill::update(float dt)
{
	AttackSkill::update(dt);
}

bool MelleSkill::use(LifeEntity* target)
{
	if (!AttackSkill::use(target))  return false;

	auto pos = _owner->getPosition();
	auto targetPos = target->getPosition();

	if (!attack(target)) {
		return false;
	}

	createEffect();

	_elapsedTime = 0;

	return true;
}

void MelleSkill::initEffect()
{
	std::string efSkill[] = { "1", "2", "3", "4", "6" };
	std::string charAction[] = { "attack1","attack2", "attack3", "attack4","attack1" };
	std::string efCharacter[] = { "attack1","attack2", "attack3", "attack4","attack1" };
	createSkillEffect(_name, efSkill, charAction, 5, 0.3 / 5);
	/*switch (_level)
	{
	case 1:
	{
		std::string efSkill[] = { "1", "2", "3", "4", "6" };
		std::string charAction[] = { "attack1","attack2", "attack3", "attack4","attack1" };
		std::string efCharacter[] = { "attack1","attack2", "attack3", "attack4","attack1" };
		createSkillEffect(_name, efSkill, charAction, 5, 0.3 / 5);
	}
	break;
	case 2:
	{
		std::string efSkill[] = { "1", "2", "21", "22", "6" };
		std::string charAction[] = { "attack1","attack2", "attack3", "attack4","attack1" };
		createSkillEffect(_name, efSkill, charAction, 5, 0.3 / 5);
	}
	break;
	case 3:
	{
		std::string efSkill[] = { "1", "2", "25" ,"26", "27", "28" };
		std::string charAction[] = { "attack1","attack2", "attack3", "attack4", "idle1", "idle1" };
		createSkillEffect(_name, efSkill, charAction, 6, 0.3 / 6);
	}
	break;
	case 4:
	{
		std::string efSkill[] = { "7", "8", "23" ,"24" ,"11" };
		std::string charAction[] = { "attack5","attack6", "attack7", "attack8", "attack8" };
		createSkillEffect(_name, efSkill, charAction, 5, 0.5 / 10, 2);
	}
	break;
	case 5:
	{
		std::string efSkill[] = { "7", "8", "13" ,"29", "30", "31", "32" };
		std::string charAction[] = { "attack5","attack6", "attack7", "attack6", "attack8", "idle1", "idle1" };
		createSkillEffect(_name, efSkill, charAction, 7, 0.4 / 7);
	}
	break;
	case 6:
		std::string efSkill[] = { "1", "33", "34" ,"35", "36", "37", "38","39","40" };
		std::string charAction[] = { "attack1","attack1", "attack2", "attack2", "attack3", "attack3", "attack4", "idle4", "idle1" };
		createSkillEffect(_name, efSkill, charAction, 9, 0.6 / 9);
		break;
	}*/

}

void MelleSkill::createEffect()
{
	auto finish = CallFuncN::create([&](Node* node) {

		auto cha = dynamic_cast<Character*> (_owner);
		if (cha) {
			cha->getStateMachine()->popState();
		}
		node->removeFromParent();
		});

	auto size = _owner->getContentSize();
	auto efSkill = Sprite::createWithSpriteFrameName("effect_skill/1");
	_owner->addChild(efSkill, 10);
	Utility::doAction(efSkill, "Effect Skill" + _name, finish);
	efSkill->setPosition(Vec2(size.width / 2, size.height / 2));

	//switch (_level)
	//{
	//case 1:
	//case 2:
	//case 3:
	//case 4:
	//case 6:
	//{
	//	efSkill->setPosition(Vec2(size.width / 2, size.height / 2));
	//}
	//break;
	//case 5:
	//{
	//	efSkill->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	//	efSkill->setPosition(Vec2(size.width / 2, 0));
	//}
	//break;
//}
}


void MelleSkill::createSkillEffect(std::string name, std::string* skillFrames, std::string* charFrames, int frameCount, float delay, int loop)
{
	auto cha = (Character*)_owner;
	auto cloth = Item::findItem(cha->getEquipments(), Item::CLOTH);
	auto pants = Item::findItem(cha->getEquipments(), Item::PANTS);
	std::string clothString = "character/" + (cloth != nullptr ? cloth->getName() : "Duffel shirt");
	std::string pantsString = "character/" + (pants != nullptr ? pants->getName() : "Duffel pants");
	auto ef = Utility::createAnimation(skillFrames, frameCount, delay, "effect_skill/", loop);
	AnimationCache::getInstance()->addAnimation(ef, "Effect Skill" + name);
	ef = Utility::createAnimation(charFrames, frameCount, delay, "character/head_", loop);
	AnimationCache::getInstance()->addAnimation(ef, "head_" + name);
	ef = Utility::createAnimation(charFrames, frameCount, delay, clothString + "_", loop);
	AnimationCache::getInstance()->addAnimation(ef, "cloth_" + name);
	ef = Utility::createAnimation(charFrames, frameCount, delay, pantsString + "_", loop);
	AnimationCache::getInstance()->addAnimation(ef, "pants_" + name);
}
