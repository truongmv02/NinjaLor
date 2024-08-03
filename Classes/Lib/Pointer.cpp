#include "Pointer.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"

Pointer* Pointer::create()
{
	Pointer* pointer = new (std::nothrow)Pointer();
	if (pointer && pointer->init()) {

		pointer->retain();
		return pointer;
	}
	CC_SAFE_DELETE(pointer);
	return nullptr;
}

bool Pointer::init()
{
	if (!Sprite::create()) return false;
	_entityFocus = nullptr;
	this->initWithSpriteFrameName("pointer1");
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	//doAction();
	this->scheduleUpdate();
	return true;
}

void Pointer::update(float dt)
{
	auto player = GameManager::getInstance()->getCharacter();
	auto camPos = Director::getInstance()->getRunningScene()->getDefaultCamera()->getPosition();
	setEntitiesFocus();
	if (_entityFocus != nullptr && _entityFocus->isVisible()) {
		auto entityPos = _entityFocus->getPosition();
		//auto result1 = Vec2(player->getPosition() - entityPos).getLength() < 500;
		auto visibleRect = Rect(camPos.x - VISIBLE_SIZE_WIDTH / 2,
			camPos.y - VISIBLE_SIZE_HEIGHT / 2, VISIBLE_SIZE_WIDTH, VISIBLE_SIZE_HEIGHT);

		if (visibleRect.containsPoint(entityPos)) {
			setInfo(_entityFocus);
			return;
		}
	}

	int index = -1;
	float minLeght = 100000;

	for (int i = 0; i < _entities.size(); i++) {

		auto entity = _entities.at(i);
		if (!entity->isVisible()) continue;
		float length = Vec2(player->getPosition() - entity->getPosition()).getLength();

		if (length < minLeght) {
			index = i;
			minLeght = length;
		}
	}

	if (index == -1) {
		_entityFocus = nullptr;
		this->setVisible(false);
	}

	else {
		_entityFocus = _entities.at(index);
		_entitiesFocus.pushBack(_entityFocus);
		this->setPosition(
			_entityFocus->getPosition() +
			Vec2(0, _entityFocus->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.015));
		this->setVisible(true);

	}

}

void Pointer::changeTarget()
{
	auto player = GameManager::getInstance()->getCharacter();
	auto camPos = Director::getInstance()->getRunningScene()->getDefaultCamera()->getPosition();
	int index = -1;
	if (_entitiesFocus.size() >= _entities.size()) {
		_entitiesFocus.clear();
	}
	for (int i = 0; i < _entities.size(); i++) {
		auto entity = _entities.at(i);
		if (!entity->isVisible() || _entitiesFocus.getIndex(entity) != -1) continue;
		index = i;
		break;
	}
	if (index == -1) return;
	_entityFocus = _entities.at(index);
	_entitiesFocus.pushBack(_entityFocus);
	this->setPosition(
		_entityFocus->getPosition() +
		Vec2(0, _entityFocus->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.015));
	this->setVisible(true);
}

void Pointer::setInfo(Entity* entity)
{
	std::string labelString = "";
	if (dynamic_cast<NPC*>(entity) || dynamic_cast<Item*>(entity) || dynamic_cast<Coin*>(entity)) {
		this->initWithSpriteFrameName("pointer2");

	}
	else {
		auto enemy = dynamic_cast<Enemy*> (entity);
		if (enemy) {
			auto currentHp = enemy->getStat("Hp")->getCurrentValue();
			auto hpMax = enemy->getStat("HpMax")->getCurrentValue();
			int value = 11 - (int)((currentHp / (float)hpMax) * 10);
			std::string poiterName = std::string("pointer") + std::to_string(value != 11 ? value : 10);
			this->initWithSpriteFrameName(poiterName);
		}
	}
	this->setPosition(
		_entityFocus->getPosition() +
		Vec2(0, _entityFocus->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.015));
}

void Pointer::setEntitiesFocus()
{
	_entities.clear();
	setEntitiesFocus(GameManager::getInstance()->getItems());
	setEntitiesFocus(GameManager::getInstance()->getEntities());
}

void Pointer::setEntitiesFocus(Vector<Entity*> entities)
{
	auto camPos = Director::getInstance()->getRunningScene()->getDefaultCamera()->getPosition();
	auto visibleRect = Rect(camPos.x - VISIBLE_SIZE_WIDTH / 2,
		camPos.y - VISIBLE_SIZE_HEIGHT / 2, VISIBLE_SIZE_WIDTH, VISIBLE_SIZE_HEIGHT);
	for (auto e : entities) {

		if (e->isVisible() && visibleRect.containsPoint(e->getPosition())) {
			_entities.pushBack(e);
		}
		else {
			if (_entitiesFocus.getIndex(e) != -1) {
				_entitiesFocus.eraseObject(e, true);
			}
		}
	}
}

void Pointer::removeEntityFocus(Entity* entity)
{
	if (entity == _entityFocus) {
		_entityFocus = nullptr;
	}
}

//void Pointer::doAction()
//{
//	auto ani = Utility::createAnimation("pointer", 2, 0.2, -1);
//
//	this->stopAllActions();
//	this->runAction((Animate::create(ani)));
//}
