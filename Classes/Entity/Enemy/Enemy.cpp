#include "Enemy.h"
#include "Database/DataManager.h"
#include "Entity/Entities.h"
#include "Manager/GameManager.h"
#include "Map/GameMap.h"

Enemy::Enemy()
{
	_type = EnemyType::SCARECROW;
	_eFocus = nullptr;
	_state = IDLE;
}

bool Enemy::init()
{
	if (!LifeEntity::init()) return false;
	addStats(StatManager::create()->getAllStat());
	_dir.x = RandomHelper::random_int(0, 1) == 0 ? -1 : 1;
	//updateStat();
	auto hp = Stat::create("Hp", 0);
	auto hpMax = Stat::create("HpMax", 0);
	auto attack = Stat::create("Attack", 0);

	_stats.pushBack(hp);
	_stats.pushBack(hpMax);
	_stats.pushBack(attack);

	_hpBar = Utility::createProgressbar(PATH_IMAGE_UI + "hp_enemy.png", PATH_IMAGE_UI + "hp_enemy_bg.png");
	this->addChild(_hpBar);
	initSchedule();
	return true;
}

bool Enemy::init(std::string name)
{
	if (!init()) return false;
	setName(name);
	return true;
}


void Enemy::setStatValue()
{
	auto hpMax = getStat("HpMax");
	auto attack = getStat("Attack");

	if (_type == EnemyType::SCARECROW) {

		hpMax->setValue(100);
		setHp(100);
	}
	else {
		auto hpVal = 200;
		auto attackVal = 10;
		for (int i = 1; i <= _level; i++)
		{
			hpVal += 50;
			attackVal += 4;
		}
		if (_type == EnemyType::BOSS) {
			hpVal *= 4;
			attackVal *= 4;
		}
		hpMax->setValue(hpVal);
		setHp(hpVal);
		attack->setValue(attackVal);
	}
}

void Enemy::setName(std::string name)
{
	_name = name;
	auto sprite = Sprite::createWithSpriteFrameName("enemy/" + name + "_1");
	setSprite(sprite);
	initBoxBody(_contentSize.width * 0.7, _contentSize.height, Vec2(_contentSize.width / 2, 0));
	setBodyType(TYPE_ENEMY);
	setBodyMask(TYPE_WALL);
	_hpBar->setPosition(Vec2(0, _contentSize.height + VISIBLE_SIZE_HEIGHT * 0.02));
	_hpBar->setVisible(false);
}

void Enemy::setLevel(int level)
{
	_level = level;
	setStatValue();
}

void Enemy::update(float dt)
{
	if (getStat("Hp")->getValue() <= 0) {
		dead();
	}

	LifeEntity::update(dt);
	if (_level >= 10) {
		auto cha = GameManager::getInstance()->getCharacter();
		auto length = Vec2(cha->getPosition() - _position).getLength();
		auto lengthAttack = _type == EnemyType::MOVING ? 300 : 600;
		if (length < lengthAttack && cha->getStat("Hp")->getValue() >0) {
			_eFocus = cha;
		}

	}

	if (_eFocus && _type != EnemyType::SCARECROW) {
		auto length = Vec2(_eFocus->getPosition() - _position).getLength();
		auto lengthAttack = _type == EnemyType::MOVING ? 600 : 1000;
		if (length < lengthAttack && getStat("Hp")->getValue() > 0) {
			if (!isScheduled("SpawBullet")) {
				this->schedule(CC_CALLBACK_1(Enemy::spawBullet, this), 1.5f, "SpawBullet");
			}
		}
		else {
			if (length > 1200) {
				unschedule("SpawBullet");
			}
		}

	}
	setFlippedX(_dir.x <= 0);
}

void Enemy::move(float dt)
{
	if (_type == EnemyType::SCARECROW || !_body) return;
	if (abs(_position.x - _pStart.x) > 150) {
		if (_position.x > _pStart.x) {
			setPosition(_pStart.x + 149, _position.y);
		}
		else {
			setPosition(_pStart.x - 149, _position.y);
		}
		_dir.x = -_dir.x;
	}
	if (_state != DEAD && getStat("Hp")->getValue() > 0) {
		auto v = _body->GetLinearVelocity();
		_body->SetLinearVelocity(b2Vec2(_dir.x * 3.5, v.y));
	}
	if (!_sprite->getActionByTag(1)) {
		doAction(MOVE);
	}
}

void Enemy::dead()
{
	this->unschedule("SpawBullet");
	this->unscheduleUpdate();
	checkQuest();
	if (_type == EnemyType::MOVING) {
		if (RandomHelper::random_int(0, 100) >= 60)
			createBonus();
	}
	if (_type == EnemyType::BOSS) {
		for (int i = 0; i < 10; i++)
		{
			auto coin = createBonus();
			coin->setPosition(this->getPosition() + Vec2((i - 5) * 20, _contentSize.height / 2));
		}
		return;
	}
	this->scheduleOnce([&](float dt) {
		this->setVisible(true);
		this->setEntityFocus(nullptr);
		this->scheduleUpdate();
		this->setHp(this->getStat("HpMax")->getCurrentValue());
		}, 15.0f, "Update");

}

void Enemy::checkQuest()
{
	auto cha = GameManager::getInstance()->getCharacter();
	int index = 0;
	auto enemies = cha->getQuest()->getEnemies();
	for (auto& eq : cha->getQuest()->getEnemies()) {

		if (eq.EnemyId == id && eq.Kill < eq.Quantity) {
			if (index == 0 || enemies.at(index - 1).Kill >= enemies.at(index - 1).Quantity) {
				cha->getQuest()->setEnemyQuantity(id);
				++eq.Kill;
				if (index == enemies.size() - 1 && eq.Kill >= eq.Quantity) {
					cha->getQuest()->setStatus(Quest::Status::COMPLETE);
					auto message = Message::create("Mission completed!");
					GameManager::getInstance()->getHUDLayer()->addMessage(message);
				}
				else {
					auto s = std::string("Kill ") + eq.EnemyName;
					GameManager::getInstance()->getHUDLayer()->addMessage(
						Message::create(s + "(" + std::to_string(eq.Kill) + "/" + std::to_string(eq.Quantity) + ")"), s.size());
				}
			}
		}
		index++;
	}
}

Coin* Enemy::createBonus()
{
	int min = _level * 40 * 2;
	int max = (_level + 3) * 80 * 2;
	auto coinValue = RandomHelper::random_int(min, max);
	auto coin = Coin::create(coinValue);
	this->getParent()->addChild(coin, 10);
	coin->setPosition(this->getPosition() + Vec2(0, _contentSize.height / 2));
	GameManager::getInstance()->pushItem(coin);
	return coin;
}

void Enemy::spawBullet(float dt)
{
	auto length = Vec2(_eFocus->getPosition() - _position).getLength();
	auto lengthAttack = _type == EnemyType::MOVING ? 600 : 1000;
	if (length < lengthAttack && _eFocus->getStat("Hp")->getValue() >0) {
		doAction(ATTACK);
	}
	else {
		this->unschedule("SpawBullet");
	}
}

void Enemy::createBullet()
{
	auto bullet = Bullet::create(_eFocus);
	bullet->setColisionCallback([&]() {
		auto hpStat = _eFocus->getStat("Hp");
		if (hpStat->getValue() > 0) {
			auto dame = getStat("Attack")->getValue() - _eFocus->getStat("Defense")->getCurrentValue();
			dame = dame > 0 ? dame : 1;
			auto hp = hpStat->getValue() - dame;
			Utility::createLabelDame(dame, _eFocus);
			_eFocus->setHp(hp);
		}
		if (hpStat->getValue() <= 0) {
			_eFocus = nullptr;
			this->unschedule("SpawBullet");
			schedule(CC_CALLBACK_1(Enemy::move, this), Director::getInstance()->getAnimationInterval(), "Move");
		}
		});
	auto pos = Vec2(_position.x, _position.y + _contentSize.height / 2);
	bullet->setPosition(pos);
	GameManager::getInstance()->getMap()->addChild(bullet, 5);
}


void Enemy::doAction(EntityState state)
{
	auto dir = _dir;
	if (_eFocus && state == ATTACK) {
		_dir = _eFocus->getPositionX() < _position.x ? Vec2(-1, 0) : Vec2(1, 0);
	}

	auto finisAction = CallFunc::create([&, state, dir]() {
		if (state == ATTACK) {
			createBullet();
			this->setDir(dir);
		}
		if (state == HURT) {
			if (getStat("Hp")->getValue() <= 0) {
				this->setVisible(false);
			}
		}
		doAction(MOVE);
		schedule(CC_CALLBACK_1(Enemy::move, this), Director::getInstance()->getAnimationInterval(), "Move");
		});
	switch (state)
	{
	case LifeEntity::IDLE:
	case LifeEntity::MOVE:
	{
		auto ani = Utility::createAnimation("enemy/" + _name + "_", 2, 0.1, -1);
		auto action = Animate::create(ani);
		action->setTag(1);
		_sprite->stopAllActions();
		_sprite->runAction(action);
	}
	break;
	case LifeEntity::ATTACK:
	{
		std::string farms[] = { "_1", "_2", "_4" };
		auto ani = Utility::createAnimation(farms, 3, 0.12, "enemy/" + _name);
		_sprite->stopAllActions();
		_sprite->runAction(Sequence::create(Animate::create(ani), finisAction, nullptr));
		this->unschedule("Move");
	}
	break;
	case LifeEntity::HURT:
	{
		std::string farms[] = { "_2", "_3" };
		auto animation = Utility::createAnimation(farms, 2, 0.1, "enemy/" + _name);
		_sprite->stopAllActions();
		_sprite->runAction(Sequence::create(Animate::create(animation), finisAction, nullptr));
		this->unschedule("Move");

		auto healthEffect = Sprite::create();
		_sprite->addChild(healthEffect);
		auto dameEffect = Sprite::createWithSpriteFrameName("damage_effect/effect_hurt");
		dameEffect->setPosition(_sprite->getContentSize() / 2);
		_sprite->addChild(dameEffect);

		auto dir = _eFocus->getPositionX() < _position.x ? -1 : 1;
		healthEffect->setPosition(Vec2(_contentSize.width * (dir <= 0 ? 1.2 : -0.2), _contentSize.height * 0.7));

		healthEffect->setFlippedX(dir >= 0);
		dameEffect->setFlippedX(dir >= 0);

		auto ani = Utility::createAnimation("damage_effect/health", 0.08);
		auto action = Animate::create(ani);
		auto finishEffect = CallFunc::create([=] {
			healthEffect->removeFromParent();
			dameEffect->removeFromParent();
			});

		healthEffect->runAction(Sequence::create(action, finishEffect, nullptr));
		break;
	}
	case LifeEntity::DEAD: {

		break;
	}

	}
}

void Enemy::initSchedule()
{
	this->scheduleUpdate();
	schedule(CC_CALLBACK_1(Enemy::move, this), Director::getInstance()->getAnimationInterval(), "Move");
}

