#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include "Entity/Item/Item.h"
#include "Layer/ShopLayer.h"
USING_NS_CC;

class ItemManager :public Ref
{
public:
	bool init() { return true; }

	Vector<Item*> getInventory();
	Vector<Item*> getCharacterEquipments();

	Item* createItem(int templateId, int quantity = 1);
	//Item* createItem(int type, int level, int quantity = 1);
	Item* createItem(Item* itemTemplate, int quantity = 1);

	Vector<Item*> findItemShop(int type);
	Item* findItemTemplate(int templateId);
	Item* findItemTemplate(int type, int levelRequire);

	bool addItemToInventory(int itemId, int quantity = 1);
	bool addItemToEquipments(int itemId);
	bool updateItem(int itemId, int quantity);
	int addItem(Item* item);
	bool removeItem(int itemId);
	bool removeItemFromInventory(int itemId);
	bool removeItemFromEquipments(int itemId);
	CREATE_FUNC(ItemManager);
private:
	sqlite3_stmt* stmt;
	Item* getItem(sqlite3_stmt* stmt);
	Item* getItemTemplate(sqlite3_stmt* stmt);
};
