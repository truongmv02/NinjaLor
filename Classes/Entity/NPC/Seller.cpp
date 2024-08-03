#include "Seller.h"
#include "Manager/GameManager.h"
#include "Layer/ShopLayer.h"

bool Seller::init()
{
	if (!NPC::init()) return false;

	return true;
}

void Seller::onClick()
{
	GameManager::getInstance()->pause();

	_menu = CardMenu::create(600, 700);
	_menu->setCloseCallback([]() {
		GameManager::getInstance()->resume();
		});

	Vector<Node*> btns;
	auto container = _menu->getContainer();
	auto weaponBtn = _menu->createButton("WEAPON", CC_CALLBACK_2(Seller::openShopCallback, this));
	weaponBtn->setTag(0);
	btns.pushBack(weaponBtn);

	auto customeBtn = _menu->createButton("CUSTOME", CC_CALLBACK_2(Seller::openShopCallback, this));
	customeBtn->setTag(1);
	btns.pushBack(customeBtn);

	auto accessoryBtn = _menu->createButton("ACCESSORY", CC_CALLBACK_2(Seller::openShopCallback, this));
	accessoryBtn->setTag(2);
	btns.pushBack(accessoryBtn);

	auto potionBtn = _menu->createButton("POTION", CC_CALLBACK_2(Seller::openShopCallback, this));
	potionBtn->setTag(3);
	btns.pushBack(potionBtn);

	auto menuSize = Utility::alignItemsVerticallyWithPadding(btns, 0,
		container->getContentSize(), 0, container->getContentSize().width * 0.05);
	container->setInnerContainerSize(menuSize);

	GameManager::getInstance()->getHUDLayer()->addChild(_menu, 100);
}

void Seller::openShopCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		int tag = ((Button*)sender)->getTag();
		auto layer = ShopLayer::create((ShopLayer::ShopType)tag);
		_menu->removeFromParent();
		GameManager::getInstance()->getHUDLayer()->addChild(layer, 10);
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
