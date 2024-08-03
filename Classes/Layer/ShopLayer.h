#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"
#include "Entity/Item/Item.h"
#include "Database/DataManager.h"

class ItemManager;
USING_NS_CC;

class ShopLayer : public LayerBase
{
public:
	enum ShopType
	{
		WEAPON,
		CUSTOME,
		ACCESSORY,
		POTION
	};
	ShopLayer() { _btnSelected = nullptr; }
	bool init(ShopType type);
	void initData();
	virtual void clear() override;
	CREATE_FUNC1(ShopLayer, ShopType);
protected:
	void buyItem(Item* item, int quantity = 1);
	void createGrid();
	void initInfo();
	void initItemOptions();
	void buyItemCallback(Ref* sender, Widget::TouchEventType type);
	void touchItemCallback(Ref* sender, Widget::TouchEventType type);
	void changeTabCallback(Ref* sender, Widget::TouchEventType type);
	Button* _btnSelected;
	Vector<Item*> _items;
	ItemManager* _im;
	Node* _grid;
	ShopType _type;
	Label* _labelCoin;
};
