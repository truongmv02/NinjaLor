#pragma once

#include "cocos2d.h"
#include "Entity/Entity.h"
#include "Entity/Stat.h"
#include "ui/CocosGUI.h"

using namespace ui;
USING_NS_CC;

class Item :public Entity
{
public:
	enum  ItemType
	{
		MELEE_WEAPON,
		RANGE_WEAPON,
		CLOTH,
		PANTS,
		GLOVES,
		SHOE,
		GEMS,
		NECKLACE,
		HP_POTION = 100,
		MP_POTION
	};
	Item();
	static Item* findItem(Vector<Item*> items, ItemType type);
	static Item* findItem(Vector<Item*> items, int id);
	bool init();
	bool isEquipment();
	Button* createButton(Node* parent, Widget::ccWidgetTouchCallback callback = nullptr, int tag = 0);
	Vector<Node*> createLabelInfo(Node* parent, float dimensions);
	void clear();
	void autoRemove();
	CC_SYNTHESIZE(ItemType, _type, Type);
	CC_SYNTHESIZE(std::string, _desc, Description);
	CC_SYNTHESIZE(int, _templateId, TemplateId);
	CC_SYNTHESIZE(int, _levelRequire, LevelRequire);
	CC_SYNTHESIZE(int, _quantity, Quantity);
	CC_SYNTHESIZE(int, _priceBuy, PriceBuy);
	CC_SYNTHESIZE(int, _priceSell, PriceSell);
	CC_SYNTHESIZE(bool, _islock, IsLock);
	CC_SYNTHESIZE(bool, _onGround, OnGround);
	CC_SYNTHESIZE(Vector<Stat*>, _stats, Stats);

	CREATE_FUNC_RETAIN(Item);
};
