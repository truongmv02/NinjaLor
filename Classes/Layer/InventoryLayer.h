#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"
#include "Entity/Item/Item.h"
#include "Database/DataManager.h"

USING_NS_CC;

class InventoryLayer : public InfoLayer
{
public:
	enum Type
	{
		DEFAULT,
		SHOP
	};

	InventoryLayer() { _btnSelected = nullptr; }
	static InventoryLayer* create(Type type = DEFAULT, int shopType = -1);
	bool init(Type type = DEFAULT, int shopType = -1);
	void initData(bool removeInfo = false);
	void clear() override;
	CC_SYNTHESIZE(Type, _type, Type);
	CC_SYNTHESIZE(int, _shopType, ShopType);
protected:
	void createGrid();
	void initInfo();
	void initItemOptions();
	void touchItemCallback(Ref* sender, Widget::TouchEventType type);
	Button* _btnSelected;
	Vector<Item*> _items;
	ItemManager* _im;
	Node* _grid;
};
