#include "ItemManager.h"
#include "DatabaseManager.h"
#include "StatManager.h"

Vector<Item*> ItemManager::getInventory() {
	Vector<Item*> items;
	std::string query =
		//							0			1			  2				  3		     4		    	5				6			7		  8		
		std::string("select iv.item_id, it.item_name, it.type, it.level_require, it.description, it.price_buy, i.price_sell, it.lock, i.template_id, iv.quantity  ") +
		"from inventory iv inner join items_template it on i.template_id = it.item_id inner join items i on iv.item_id = i.item_id;";
	DatabaseManager::getInstance()->loadData(query, &stmt);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		auto item = getItem(stmt);

		item->setQuantity(sqlite3_column_int(stmt, 9));
		item->setStats(StatManager::create()->findItemStat(item->getId()));
		items.pushBack(item);
	}
	return items;

}

Vector<Item*> ItemManager::getCharacterEquipments()
{
	Vector<Item*> items;
	std::string query =
		//							0			1			  2				  3		     4		    	5				6			7		  	
		std::string("select eq.item_id, it.item_name, it.type, it.level_require, it.description, it.price_buy, i.price_sell, it.lock ") +
		"from equipments eq inner join items_template it on i.template_id = it.item_id inner join items i on eq.item_id = i.item_id;";
	DatabaseManager::getInstance()->loadData(query, &stmt);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		auto item = getItem(stmt);
		item->setQuantity(1);
		item->setStats(StatManager::create()->findItemStat(item->getId()));
		items.pushBack(item);
	}
	return items;
}

Item* ItemManager::createItem(int templateId, int quantity) {
	auto iTemplate = findItemTemplate(templateId);
	if (iTemplate) {
		return createItem(iTemplate, quantity);
	}
	return nullptr;
}

//Item* ItemManager::createItem(int type, int levelRequire, int quantity)
//{
//	Item* iTemplate = findItemTemplate(type, levelRequire);
//	if (iTemplate) {
//		return createItem(iTemplate, quantity);
//	}
//	return nullptr;
//}

Item* ItemManager::createItem(Item* iTemplate, int quantity) {

	auto item = Item::create();
	item->setName(iTemplate->getName());
	item->setType(iTemplate->getType());
	item->setTemplateId(iTemplate->getId());
	item->setLevelRequire(iTemplate->getLevelRequire());
	item->setDescription(iTemplate->getDescription());
	item->setPriceBuy(iTemplate->getPriceBuy());
	item->setPriceSell(iTemplate->getPriceSell());
	item->setIsLock(iTemplate->getIsLock());
	item->setQuantity(quantity);
	Vector<Stat*> stats;
	for (auto s : iTemplate->getStats()) {
		auto statTemplate = (StatTemplate*)s;
		auto stat = Stat::create();
		stat->setId(statTemplate->getId());
		stat->setName(statTemplate->getName());
		stat->setDescription(statTemplate->getDescription());
		auto statValue = statTemplate->getValue(); //RandomHelper::random_int(statTemplate->getMinValue(), statTemplate->getMaxValue());
		stat->setValue(statValue);

		for (auto b : statTemplate->getBonuses()) {
			auto bonusTemplate = dynamic_cast<BonusTemplate*>(b);
			auto bonus = Bonus::create();
			bonus->setStatId(stat->getId());
			bonus->setValueType(bonusTemplate->getValueType());
			auto bonusValue = bonusTemplate->getMaxValue(); //RandomHelper::random_int(bonusTemplate->getMinValue(), bonusTemplate->getMaxValue());
			bonus->setValue(bonusValue);
			stat->AddBonus(bonus);
		}
		stats.pushBack(stat);
	}
	item->setStats(stats);
	int id = addItem(item);
	item->setId(id);
	return id != 0 ? item : nullptr;
}

bool ItemManager::addItemToInventory(int itemId, int quantity)
{
	std::string query = "INSERT INTO inventory (item_id, quantity) values (" + std::to_string(itemId) + ", " + std::to_string(quantity) + ");";
	return DatabaseManager::getInstance()->execute(query);
}

bool ItemManager::addItemToEquipments(int itemId)
{
	std::string query = "INSERT INTO equipments (item_id) values (" + std::to_string(itemId) + ");";
	return DatabaseManager::getInstance()->execute(query);
}

bool ItemManager::updateItem(int itemId, int quantity)
{
	if (quantity == 0)
		return removeItem(itemId);
	std::string query = "update inventory set quantity = " + std::to_string(quantity) + " where item_id = " + std::to_string(itemId) + " ;";
	return DatabaseManager::getInstance()->execute(query);
}

int ItemManager::addItem(Item* item)
{
	std::string query;
	if (item->getType() == Item::MP_POTION || item->getType() == Item::HP_POTION) {
		query = "select iv.item_id, iv.quantity, i.template_id from items i inner join inventory iv on i.item_id = iv.item_id where i.template_id =" + std::to_string(item->getTemplateId()) + " ;";
		bool result = DatabaseManager::getInstance()->loadData(query, &stmt);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			int qty = sqlite3_column_int(stmt, 1);
			query = "update inventory set quantity = " + std::to_string(qty + item->getQuantity()) + " where item_id = " + std::to_string(id) + " ;";
			DatabaseManager::getInstance()->execute(query);
			return id;
		}

	}
	query =
		"insert into items (template_id, price_sell) values (" +
		std::to_string(item->getTemplateId()) + ", " +
		std::to_string(item->getPriceSell()) + ");";
	bool result = DatabaseManager::getInstance()->execute(query);
	if (!result) return 0;
	query = "select item_id from items order by item_id desc limit 1;";

	DatabaseManager::getInstance()->loadData(query, &stmt);
	sqlite3_step(stmt);
	item->setId(sqlite3_column_int(stmt, 0));
	for (auto stat : item->getStats()) {

		for (auto bonus : stat->getBonuses()) {
			query = "INSERT INTO item_bonus (item_id, stat_id, value_type, value, gap_time, duration) VALUES (" +
				std::to_string(item->getId()) + ", " +
				std::to_string(stat->getId()) + ", " +
				std::to_string((int)bonus->getValueType()) + ", " +
				std::to_string(bonus->getValue()) + ", " +
				std::to_string(bonus->getTimeGap()) + ", " +
				std::to_string(bonus->getDuration()) +
				");";
			DatabaseManager::getInstance()->execute(query);
		}
	}
	return item->getId();
}

bool ItemManager::removeItem(int itemId)
{
	std::string query = "delete from items where item_id = " + std::to_string(itemId) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

bool ItemManager::removeItemFromInventory(int itemId)
{
	std::string query = "delete from inventory where item_id = " + std::to_string(itemId) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

bool ItemManager::removeItemFromEquipments(int itemId)
{
	std::string query = "delete from equipments where item_id = " + std::to_string(itemId) + ";";
	return DatabaseManager::getInstance()->execute(query);
}

Vector<Item*> ItemManager::findItemShop(int type) {
	Vector<Item*> items;

	std::string query =
		//							0			1			  2				  3		     4		    	5				6			7		  8		
		std::string("select its.item_id, it.item_name, it.type, it.level_require, it.description, it.price_buy, i.price_sell, it.lock, i.template_id, its.type_shop ") +
		"from item_shop its inner join items_template it on its.item_id = it.item_id inner join items i on its.item_id = i.item_id "
		+ "where its.type_shop = " + std::to_string((int)type) + ";";
	DatabaseManager::getInstance()->loadData(query, &stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		auto item = getItem(stmt);
		item->setStats(StatManager::create()->findItemStat(item->getId()));
		items.pushBack(item);
	}
	return items;
}

Item* ItemManager::findItemTemplate(int type, int levelRequire)
{
	Item* item = nullptr;
	std::string query =
		//	   		0	    1		 2			3				4		5			6  		  7
		"select item_id, item_name, type, level_require, description, price_buy, price_sell, lock \
		from items_template where type = " + std::to_string(type) + " and level_require = " + std::to_string(levelRequire);

	bool result = DatabaseManager::getInstance()->loadData(query, &stmt);
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		item = getItemTemplate(stmt);
		item->setStats(StatManager::create()->findItemStatTemplate(item->getId()));
	}
	return item;
}

Item* ItemManager::findItemTemplate(int id)
{
	Item* item = nullptr;
	std::string query =
		//	   		0	    1		 2			3				4		5			6  		  7
		"select item_id, item_name, type, level_require, description, price_buy, price_sell, lock \
		from items_template where item_id = " + std::to_string(id) + ";";

	bool result = DatabaseManager::getInstance()->loadData(query, &stmt);
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		item = getItemTemplate(stmt);
		item->setStats(StatManager::create()->findItemStatTemplate(item->getId()));
	}
	return item;
}

Item* ItemManager::getItem(sqlite3_stmt* stmt) {

	auto item = getItemTemplate(stmt);
	item->setTemplateId(sqlite3_column_int(stmt, 8));
	return item;
}

Item* ItemManager::getItemTemplate(sqlite3_stmt* stmt)
{
	Item* item = Item::create();

	item->setId(sqlite3_column_int(stmt, 0));
	item->setName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
	item->setType((Item::ItemType)sqlite3_column_int(stmt, 2));
	item->setLevelRequire(sqlite3_column_int(stmt, 3));
	auto desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
	item->setDescription(desc != nullptr ? desc : "");
	item->setPriceBuy(sqlite3_column_int(stmt, 5));
	item->setPriceSell(sqlite3_column_int(stmt, 6));
	item->setIsLock(sqlite3_column_int(stmt, 7));

	return item;
}

