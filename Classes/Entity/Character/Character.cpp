#include "Character.h"
#include "Database/DataManager.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"
#include "Lib/Lib.h"
#include "Scene/GameScene.h"

Character::Character() {
	_moveTarget = false;
}

bool Character::init() {
	if (!LifeEntity::init()) return false;
	_cm = CharacterManager::create();
	_cm->retain();

	_head = Sprite::createWithSpriteFrameName("character/head_idle1");
	_cloth = Sprite::createWithSpriteFrameName("character/Duffel shirt_idle1");
	_pants = Sprite::createWithSpriteFrameName("character/Duffel pants_idle1");
	_sword = Sprite::createWithSpriteFrameName("character/sword1_idle1");
	this->setContentSize(_head->getContentSize());

	_head->setPosition(_contentSize / 2);
	_cloth->setPosition(_contentSize / 2);
	_pants->setPosition(_contentSize / 2);
	_sword->setPosition(_contentSize / 2);
	_sword->setVisible(false);

	auto bodySize = Size(_head->getContentSize().width * 0.6, _head->getContentSize().height * 0.9);
	initBoxBody(bodySize.width, bodySize.height, Vec2(_contentSize.width / 2, _contentSize.height * 0.02));
	setBodyType(TYPE_HERO);
	setBodyMask(TYPE_WALL);

	this->addChild(_sword, 1);
	this->addChild(_pants, 1);
	this->addChild(_cloth, 2);
	this->addChild(_head, 1);

	_skillChoose = 0;
	auto listener = EventListenerKeyboard::create();
	_stateMachine->pushState(new CharacterIdleState(this));
	listener->onKeyPressed = [&](EventKeyboard::KeyCode key, Event* e) {
		switch (key)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			_dir.x = -1;
			InputHandler::getInstance()->onKeyPress(InputHandler::Input::MOVE_LEFT);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			InputHandler::getInstance()->onKeyPress(InputHandler::Input::MOVE_RIGHT);
			_dir.x = 1;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			InputHandler::getInstance()->onKeyPress(InputHandler::Input::JUMP);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		{
			auto entity = GameManager::getInstance()->getPointer()->getEntityFocus();
			if (dynamic_cast<Enemy*> (entity)) {
				useSkill((Enemy*)entity);
			}
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_1:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_2:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_3:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_4:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_5:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_0:
			break;

		}
	};
	listener->onKeyReleased = [&](EventKeyboard::KeyCode key, Event* e) {
		switch (key)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			InputHandler::getInstance()->onKeyReleased(InputHandler::Input::JUMP);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			InputHandler::getInstance()->onKeyReleased(InputHandler::Input::MOVE_LEFT);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			InputHandler::getInstance()->onKeyReleased(InputHandler::Input::MOVE_RIGHT);
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	initSchedule();
	initAction();
	return true;
}

void Character::update(float dt) {
	auto hudLayer = GameManager::getInstance()->getHUDLayer();
	if (getStat("Hp")->getValue() <= 0 && !hudLayer->getChildByName("Go Back")) {
		_stateMachine->changeState(new CharacterDeadState(this));
		_moveTarget = false;
		this->unschedule("Move");

		auto v = _body->GetLinearVelocity();
		_body->SetLinearVelocity(b2Vec2(0, v.y));
		hudLayer->getControlLayer()->setVisible(false);

		auto button = Utility::createButton("Go back");
		button->addTouchEventListener(CC_CALLBACK_2(Character::goBackCallback, this));
		button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		button->setPosition(Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.05));
		button->setName("Go Back");
		hudLayer->addChild(button, 100);

	}
	if (getStat("Hp")->getValue() > 0) {
		LifeEntity::update(dt);
		auto v = _body->GetLinearVelocity();
		if (abs(v.y) > 0.1f) {
			_onGround = false;
		}
		updateState(dt);
		setFlippedX(_dir.x <= 0);

		_cm->updateHp(getStat("Hp")->getValue());
		_cm->updateMp(getStat("Mp")->getValue());
		_cm->updateExp(_currentExp);
		_cm->updatePosition(_position);
	}

}

void Character::regenerateStat(float dt)
{
	auto hpMax = getStat("HpMax")->getCurrentValue();
	auto mpMax = getStat("MpMax")->getCurrentValue();

	auto hp = getStat("Hp");
	auto mp = getStat("Mp");
	if (hp->getValue() <= 0)return;

	if (hp->getCurrentValue() < hpMax) {
		auto rHp = getStat("Regenerate Hp")->getCurrentValue();
		auto hpBonus = hpMax * rHp / 10000.0f;
		setHp(hp->getValue() + hpBonus);
	}
	if (mp->getCurrentValue() < mpMax) {
		auto rMp = getStat("Regenerate Mp")->getCurrentValue();
		auto mpBonus = mpMax * rMp / 10000.0f;
		setMp(mp->getValue() + mpBonus);
	}
}

void Character::useSkill(LifeEntity* target)
{
	auto skill = _selectedSkill[_skillChoose];
	auto mp = getStat("Mp");

	if (mp->getValue() < skill->getMpUse()) {
		auto message = Message::create("Not enough MP to use");
		GameManager::getInstance()->getHUDLayer()->addMessage(message);

		return;
	}

	if (_stateMachine->getCurrentState()->getStateID() != EntityState::ATTACK) {
		if (_selectedSkill[_skillChoose]->use(target)) {
			_stateMachine->pushState(new CharacterAttackState(this));
			mp->setValue(mp->getValue() - _selectedSkill[_skillChoose]->getMpUse());

		}
	}
}

void Character::initStat()
{
	for (auto e : _equipments) {
		addItemStat(e);
	}
	//LifeEntity::updateStat();
}

void Character::setStatValue()
{
	int hpVal = 200;
	int mp = 100;
	int crit = 100;
	int attack = 10;
	int defense = 1;
	for (int i = 1; i <= _level; i++)
	{
		hpVal += 20;
		mp += 10;
		crit += 15;
		attack += i;
		defense += 3;
	}
	getStat("Attack")->setValue(attack);
	getStat("HpMax")->setValue(hpVal);
	getStat("MpMax")->setValue(mp);
	getStat("Crit")->setValue(crit);
	getStat("Defense")->setValue(defense);
	getStat("Speed")->setValue(20);
	getStat("Regenerate Hp")->setValue(300);
	getStat("Regenerate Mp")->setValue(300);
}



void Character::setEquipments(Vector<Item*> items)
{
	_equipments = items;
	for (auto item : items) {
		addItemStat(item);
	}
	initAction();
}

bool Character::useItem(Item* e)
{
	if (e->isEquipment()) {
		return useEquipment(e);
	}
	else if (e->getType() == Item::HP_POTION || e->getType() == Item::MP_POTION) {
		return usePotion(e);
	}
	return true;

}

bool Character::usePotion(Item* e)
{
	auto result = false;
	Message* message = nullptr;
	if (e->getType() == Item::HP_POTION) {
		if (getStat("Hp")->getValue() >= getStat("HpMax")->getCurrentValue()) {
			message = Message::create("Hp is full");
		}
	}
	else {
		if (getStat("Mp")->getValue() >= getStat("MpMax")->getCurrentValue()) {
			message = Message::create("Mp is full");
		}
	}
	if (message) {
		GameManager::getInstance()->getHUDLayer()->addMessage(message);
		return false;
	}
	addItemStat(e);
	if (e->getQuantity() == 1) {
		ItemManager::create()->removeItem(e->getId());
		removeItemFromBag(e);
	}
	else {
		e->setQuantity(e->getQuantity() - 1);
		ItemManager::create()->updateItem(e->getId(), e->getQuantity());
	}
	return true;
}

bool Character::useEquipment(Item* e) {
	if (e->getLevelRequire() > _level) {
		auto dialog = Dialog::create("Your level is too low!");
		GameManager::getInstance()->getHUDLayer()->addChild(dialog, 1000);
		return false;
	}

	if (e->isEquipment()) {
		auto equipType = e->getType();

		auto equipBody = Item::findItem(_equipments, e->getType());

		if (equipBody) {
			if (!storeEquipment(equipBody)) return false;
			_itemUsed.eraseObject(equipBody);
		}

		_itemBag.eraseObject(e);
		_equipments.pushBack(e);

		addItemStat(e);
	}
	//updateStat();
	ItemManager::create()->addItemToEquipments(e->getId());
	ItemManager::create()->removeItemFromInventory(e->getId());
	if (e->getType() == Item::CLOTH || e->getType() == Item::PANTS) {
		initAction();
		for (auto skill : _skills)
		{
			skill->initEffect();
		}
	}
	if (e->getType() == Item::MELEE_WEAPON) {
		_sword->setVisible(true);
	}
	return true;
}

void Character::addItemStat(Item* e)
{
	if (e->getType() == Item::MELEE_WEAPON || e->getType() == Item::RANGE_WEAPON) return;
	for (auto eStat : e->getStats()) {
		Stat* stat = getStat(eStat->getName());

		if (e->getType() == Item::HP_POTION || e->getType() == Item::MP_POTION) {
			auto statClone = eStat->clone();
			stat->AddBonuses(statClone->getBonuses());
			CC_SAFE_DELETE(statClone);
		}
		else
			stat->AddBonuses(eStat->getBonuses());

	}
}

bool Character::storeEquipment(Item* e)
{
	if (!addItemToBag(e)) {
		return false;
	}
	if (e->getType() != Item::RANGE_WEAPON && e->getType() != Item::MELEE_WEAPON) {
		for (auto eStat : e->getStats()) {
			Stat* stat = getStat(eStat->getName());
			for (auto eBonus : eStat->getBonuses()) {
				stat->removeBonus(eBonus);
			}
		}
	}
	_equipments.eraseObject(e);
	//updateStat();
	ItemManager::create()->removeItemFromEquipments(e->getId());
	if (e->getType() == Item::CLOTH || e->getType() == Item::PANTS) {
		initAction();
		for (auto skill : _skills)
		{
			skill->initEffect();
		}
	}
	if (e->getType() == Item::MELEE_WEAPON) {
		_sword->setVisible(false);
	}

	return true;
}

bool Character::addItemToBag(Item* item)
{
	if (_itemBag.size() >= _itemMax) {
		auto dialog = Dialog::create("Your luggage doesn't have enough space!");
		GameManager::getInstance()->getHUDLayer()->addChild(dialog, 1000);
		return false;
	}
	_itemBag.pushBack(item);
	ItemManager::create()->addItemToInventory(item->getId());
	return true;
}

void Character::removeItemFromBag(Item* item)
{
	_itemBag.eraseObject(item);
	item->clear();
	CC_SAFE_DELETE(item);

}

void Character::handleInput(InputHandler::Input input) {
	auto state = _stateMachine->handleInput(input);
	if (state) {
		_stateMachine->changeState(state);
	}
}

void Character::initAction() {
	//run
	auto cloth = Item::findItem(_equipments, Item::CLOTH);
	auto pants = Item::findItem(_equipments, Item::PANTS);
	std::string clothString = "character/" + (cloth != nullptr ? cloth->getName() : "Duffel shirt");
	std::string pantsString = "character/" + (pants != nullptr ? pants->getName() : "Duffel pants");

	_cloth->initWithSpriteFrameName(clothString + "_idle1");
	_pants->initWithSpriteFrameName(pantsString + "_idle1");
	_sword->initWithFile("character/sword1_idle1");

	auto headRun = Utility::createAnimation("character/head_run", 0.1, -1);
	AnimationCache::getInstance()->removeAnimation("head_move");
	AnimationCache::getInstance()->addAnimation(headRun, "head_move");

	auto clothRun = Utility::createAnimation(clothString + "_run", 0.1, -1);
	AnimationCache::getInstance()->removeAnimation("cloth_move");
	AnimationCache::getInstance()->addAnimation(clothRun, "cloth_move");

	auto swordRun = Utility::createAnimation("character/sword1_run", 0.1, -1);
	AnimationCache::getInstance()->removeAnimation("sword_move");
	AnimationCache::getInstance()->addAnimation(swordRun, "sword_move");

	auto pantsRun = Utility::createAnimation(pantsString + "_run", 0.1, -1);
	AnimationCache::getInstance()->removeAnimation("pants_run");
	AnimationCache::getInstance()->addAnimation(pantsRun, "pants_move");

	auto headIdle = Utility::createAnimation("character/head_idle", 0.2, -1);
	AnimationCache::getInstance()->removeAnimation("head_idle");
	AnimationCache::getInstance()->addAnimation(headIdle, "head_idle");

	auto clothIdle = Utility::createAnimation(clothString + "_idle", 0.2, -1);
	AnimationCache::getInstance()->removeAnimation("cloth_idle");
	AnimationCache::getInstance()->addAnimation(clothIdle, "cloth_idle");

	auto pantsIdle = Utility::createAnimation(pantsString + "_idle", 0.2, -1);
	AnimationCache::getInstance()->removeAnimation("pants_idle");
	AnimationCache::getInstance()->addAnimation(pantsIdle, "pants_idle");

	auto swordIdle = Utility::createAnimation("character/sword1_idle", 0.2, -1);
	AnimationCache::getInstance()->removeAnimation("sword_idle");
	AnimationCache::getInstance()->addAnimation(swordIdle, "sword_idle");

	auto rotate = Utility::createAnimation("character/rotate", 0.12, 1);
	AnimationCache::getInstance()->removeAnimation("rotate");
	AnimationCache::getInstance()->addAnimation(rotate, "rotate");

	_head->stopAllActions();
	doAction(IDLE);
}

void Character::doAction(EntityState state)
{
	auto sword = Item::findItem(_equipments, Item::MELEE_WEAPON);
	_sword->setVisible(sword != nullptr);
	std::string attackAni = "";
	if (_selectedSkill[_skillChoose]) {
		attackAni = _selectedSkill[_skillChoose]->getName();
	}
	if (_head->getActionByTag(state) != nullptr)return;
	std::string stateString = state == EntityState::IDLE ? "idle" :
		state == EntityState::MOVE ? "move" :
		state == EntityState::ATTACK ? attackAni :
		state == EntityState::JUMP ? "jump" :
		state == EntityState::FALL ? "fall" :
		state == EntityState::ROTATE ? "rotate" :
		state == EntityState::HURT ? "hurt" :
		state == EntityState::DEAD ? "dead" : "";
	_cloth->setVisible(true);
	_pants->setVisible(true);
	_head->stopAllActions();
	_cloth->stopAllActions();
	_pants->stopAllActions();
	_sword->stopAllActions();
	switch (state)
	{
	case EntityState::IDLE:
	case EntityState::MOVE:
	case EntityState::ATTACK:
		Utility::doAction(_head, "head_" + stateString, state);
		Utility::doAction(_sword, "sword_" + stateString, state);
		Utility::doAction(_cloth, "cloth_" + stateString, state);
		Utility::doAction(_pants, "pants_" + stateString, state);

		if (state == EntityState::ATTACK) {
			_sword->setVisible(false);
		}
		break;
	case EntityState::JUMP:
	case EntityState::FALL:
	{
		auto cloth = Item::findItem(_equipments, Item::CLOTH);
		auto pants = Item::findItem(_equipments, Item::PANTS);
		std::string clothString = "character/" + (cloth != nullptr ? cloth->getName() : "Duffel shirt");
		std::string pantsString = "character/" + (pants != nullptr ? pants->getName() : "Duffel pants");

		_head->initWithSpriteFrameName("character/head_" + stateString);
		_cloth->initWithSpriteFrameName(clothString + "_" + stateString);
		_pants->initWithSpriteFrameName(pantsString + "_" + stateString);
		_sword->initWithSpriteFrameName("character/sword1_" + stateString);
	}
	break;
	case EntityState::ROTATE:
	{
		auto finishAction = CallFunc::create([&] {
			_stateMachine->changeState(new CharacterFallState(this));
			});
		_cloth->setVisible(false);
		_sword->setVisible(false);
		_pants->setVisible(false);
		Utility::doAction(_head, "rotate", finishAction, state);
	}
	break;
	case EntityState::DEAD:
	{
		_cloth->setVisible(false);
		_pants->setVisible(false);
		_sword->setVisible(false);
		_head->initWithSpriteFrameName("character/dead");
	}
	default:
		break;
	}
}

void Character::updateState(float dt) {

	_stateMachine->update(dt);
	if (_stateMachine->getCurrentState()->getStateID() != EntityState::ATTACK) {
		if (InputHandler::getInstance()->isKeyPress(InputHandler::MOVE_LEFT)) {
			handleInput(InputHandler::MOVE_LEFT);
			_dir.x = -1;
			_moveTarget = false;
			if (!isScheduled("Move"))
				schedule(CC_CALLBACK_1(Character::move, this), Director::getInstance()->getAnimationInterval(), "Move");
		}
		else if (InputHandler::getInstance()->isKeyPress(InputHandler::MOVE_RIGHT)) {
			handleInput(InputHandler::MOVE_RIGHT);
			_dir.x = 1;
			_moveTarget = false;
			if (!isScheduled("Move"))
				schedule(CC_CALLBACK_1(Character::move, this), Director::getInstance()->getAnimationInterval(), "Move");
		}
		else if (!_moveTarget) {
			stop();
		}
	}
	if (InputHandler::getInstance()->isKeyPress(InputHandler::JUMP)) {
		_moveTarget = false;
		handleInput(InputHandler::JUMP);
		jump();
	}
}

void Character::move(float dt) {
	if (_stateMachine->getCurrentState()->getStateID() == LifeEntity::DEAD) return;
	auto v = _body->GetLinearVelocity();
	_body->SetLinearVelocity(b2Vec2(_dir.x * Stat::getStat("Speed", _stats)->getCurrentValue(), v.y));
}

void Character::jump() {
	auto state = dynamic_cast<CharacterJumpState*>(_stateMachine->getCurrentState());
	if (!state) return;
	auto v = _body->GetLinearVelocity();
	auto mass = _body->GetMass();
	auto jumpCount = state->getJumpCount();
	auto jumpTime = state->getJumpTime();
	if (jumpTime >= 0.21f && jumpCount == 1) {
		state->setJumpCount(jumpCount + 1);
		_body->ApplyLinearImpulse(b2Vec2(0, mass * 18), _body->GetWorldCenter(), true);
		return;
	}
	if (jumpCount > 0 || abs(v.y) > 0.01f) return;
	state->setJumpCount(jumpCount + 1);
	state->setJumpTime(0);
	_body->ApplyLinearImpulse(b2Vec2(0, mass * 40), _body->GetWorldCenter(), true);
}

void Character::stop() {
	unschedule("Move");
	auto v = _body->GetLinearVelocity();
	if (_onGround)
		_body->SetLinearVelocity(b2Vec2(0, v.y));
}

int Character::getItemCount(Item::ItemType type)
{
	int count = 0;
	for (auto item : _itemBag) {
		if (item->getType() == type) {
			count += item->getQuantity();
		}
	}
	return count;
}

void Character::pickEntity(Entity* entity)
{
	auto coin = dynamic_cast<Coin*> (entity);

	auto timeCounter = CallbackTimeCounter::create();
	this->addChild(timeCounter);

	timeCounter->initWithCondition([&, entity, timeCounter]() {
		entity->autoRemoveBody();
		entity->removeFromParent();
		timeCounter->unscheduleUpdate();
		timeCounter->removeFromParent();
		},
		[&, entity]() {
			auto chaPos = this->getPosition() + Vec2(0, _contentSize.height / 4);
			auto distance = Vec2(entity->getPosition() - chaPos).getLength();
			if (distance < 20) return true;
			auto dir = chaPos - entity->getPosition();
			dir.normalize();
			entity->getBody()->SetLinearVelocity(b2Vec2(dir.x * 50, dir.y * 50));
			return false;
		}
		);

	if (coin) {
		GameManager::getInstance()->removeItem(entity);
		coin->setOnGround(false);
		setCoin(_coin + coin->getValue(), true);
		_cm->updateCoin(_coin);
		return;
	}

}

void Character::initSchedule() {
	float time = Director::getInstance()->getAnimationInterval();
	scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(Character::regenerateStat), 6.0f);
}

void Character::removeSchedule() {
	//unscheduleUpdate();
	this->unscheduleAllCallbacks();
}

void Character::setFlippedX(bool flip) {
	for (auto child : _children) {
		auto sprite = dynamic_cast<Sprite*>(child);
		if (sprite) {
			sprite->setFlippedX(flip);
		}
	}
}

void Character::setSkills(Vector<Skill*> skills)
{
	_skills = skills;
	for (auto skill : skills) {
		skill->setOwner(this);
		skill->scheduleUpdate();
		skill->initEffect();
		this->addChild(skill);
	}
}

void Character::goBackCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		setHp(getStat("HpMax")->getCurrentValue());
		setMp(getStat("MpMax")->getCurrentValue());
		_stateMachine->changeState(new CharacterIdleState(this));
		auto btn = (Button*)sender;
		auto scene = (GameScene*)Director::getInstance()->getRunningScene();
		scene->setChangeMap(true);
		scene->setMapName("Uchiha Clan");
		scene->setStartDoor(0);
		btn->removeFromParent();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}

void Character::levelUp()
{
	_cm->updateLevel(_level);
	_skillPoint++;
	SkillManager::create()->updateSkillPoint(_skillPoint);
	setStatValue();
	setHp(getStat("HpMax")->getCurrentValue());
	setMp(getStat("MpMax")->getCurrentValue());

	auto levelUpEf = Sprite::createWithSpriteFrameName("level_up1");
	levelUpEf->setScale(1.5);
	levelUpEf->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	levelUpEf->setPosition(Vec2(_contentSize.width / 2, _contentSize.height * 0.9) + _position);
	auto finishAction = CallFuncN::create([](Node* node) {
		node->removeFromParent();
		});
	auto actionMove = MoveBy::create(0.7, Vec2(0, 130));
	auto animation = Utility::createAnimation("level_up", 0.2, -1);
	levelUpEf->runAction(Spawn::create(Animate::create(animation), Sequence::create(actionMove, finishAction, nullptr), nullptr));

	auto effect = Sprite::createWithSpriteFrameName("effect1");
	effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	effect->setPosition(Vec2(_contentSize.width / 2, _contentSize.height + 50) + _position);
	auto ani = Utility::createAnimation("effect", 0.15);
	Utility::doActionAndRemove(effect, Animate::create(ani));

	if (this->getParent()) {
		this->getParent()->addChild(levelUpEf, 11);
		this->getParent()->addChild(effect);
	}
}

bool Character::knowMap(int mapId)
{
	for (auto map : _knowMaps) {
		if (map == mapId) return true;
	}
	return false;
}

void Character::pushKnowMap(int mapId)
{
	_knowMaps.push_back(mapId);
	_cm->addKnowMap(mapId);
}

void Character::pushKnowMaps(std::vector<int> maps)
{
	for (auto map : maps) {
		knowMap(map);
	}
}


void Character::clear()
{
	for (auto item : _itemBag) {
		item->clear();
		CC_SAFE_DELETE(item);
	}
	for (auto item : _equipments) {
		item->clear();
		CC_SAFE_DELETE(item);
	}
	CC_SAFE_DELETE(_cm);
}

void Character::setLevel(int level)
{
	_level = level;
	_expMax = 10 * 2 * (level * (level + 10));
}

void Character::setCurrentExp(int exp, bool createLabel)
{
	if (createLabel && exp > _currentExp) {
		Utility::createLabelExp(exp - _currentExp, this);
	}
	while (exp >= _expMax)
	{
		exp -= _expMax;
		setLevel(_level + 1);
		levelUp();
	}
	_currentExp = exp;
}

void Character::setCoin(int coin, bool createLbale)
{
	if (createLbale && coin > _coin) {
		Utility::createLabelCoin(coin - _coin, this);
	}
	_coin = coin;
}

void Character::setSkillSelected(Skill* skill, int index)
{
	_selectedSkill[index] = skill;
}
