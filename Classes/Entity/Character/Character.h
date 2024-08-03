#pragma once
#include "cocos2d.h"
#include "Lib/Lib.h"
#include "Entity/LifeEntity.h"
#include "StateMachine/CharacterState.h"
#include "Skill/Skills.h"

class Quest;
class CharacterManager;
USING_NS_CC;
class Character : public  LifeEntity
{
public:
	Character();

	bool init();
	void update(float dt) override;
	void regenerateStat(float dt);
	void useSkill(LifeEntity* target);
	void initStat();
	void setStatValue();
	void setEquipments(Vector<Item*> items);

	bool useItem(Item* e);
	bool usePotion(Item* e);
	bool useEquipment(Item* e);
	void addItemStat(Item* e);
	bool storeEquipment(Item* e);
	bool addItemToBag(Item* item);
	void removeItemFromBag(Item* item);

	void handleInput(InputHandler::Input input);
	void initAction();
	void doAction(EntityState state) override;
	void updateState(float dt);
	void move(float dt);
	void jump();
	void stop();
	int getItemCount(Item::ItemType type);

	void pickEntity(Entity* entity);

	void initSchedule();
	void removeSchedule();

	void setLevel(int level) override;
	void setCurrentExp(int exp, bool createLable = false);
	void setCoin(int coin, bool createLable = false);
	void setSkillSelected(Skill* skill, int index);
	void setSkills(Vector<Skill*> skills);
	Skill** getSelectedSkill() { return _selectedSkill; }

	void levelUp();
	bool knowMap(int mapId);
	void pushKnowMap(int mapId);
	void pushKnowMaps(std::vector<int> maps);
	void clear();
	virtual void setFlippedX(bool flip) override;
	CC_SYNTHESIZE(bool, _onGround, OnGround);
	CC_SYNTHESIZE(bool, _moveTarget, MoveTarget);
	CC_SYNTHESIZE(int, _itemMax, ItemMax);
	CC_SYNTHESIZE(int, _skillPoint, SkillPoint);
	CC_SYNTHESIZE(int, _expMax, ExpMax);
	CC_SYNTHESIZE(int, _skillChoose, SkillChoose);
	CC_SYNTHESIZE(Quest*, _quest, Quest);
	CC_SYNTHESIZE_READONLY(int, _coin, Coin);
	CC_SYNTHESIZE_READONLY(int, _currentExp, CurrentExp);
	CC_SYNTHESIZE_READONLY(Vector<Skill*>, _skills, Skills);
	CC_SYNTHESIZE(Vector<Item*>, _itemBag, ItemBag);
	CC_SYNTHESIZE_READONLY(Vector<Item*>, _equipments, Equipments);
	CC_SYNTHESIZE(Vector<Item*>, _itemUsed, ItemUsed);
	CC_SYNTHESIZE(std::vector<int>, _knowMaps, KnowMaps);
	CREATE_FUNC_RETAIN(Character);
public:
	Sprite* _head;
	Sprite* _cloth;
	Sprite* _pants;
	Sprite* _sword;
	Skill* _selectedSkill[5] = { nullptr };
protected:
	void goBackCallback(Ref* sender, Widget::TouchEventType type);
	CharacterManager* _cm;
};
