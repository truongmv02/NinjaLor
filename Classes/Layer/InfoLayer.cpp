#include "InfoLayer.h"
#include "InfoLayers.h"
#include "ShopLayer.h"
#include "Manager/GameManager.h"

bool InfoLayer::init(std::string layerName)
{
	if (!LayerBase::init(layerName)) return false;

	Vector<Node*> buttons;
	std::string iconName[] = { "bag", "info", "skills", "quest", "setting" };
	for (int i = 0; i < 5; i++)
	{
		auto btn = createButton(PATH_IMAGE_UI + iconName[i] + ".png", i, CC_CALLBACK_2(InfoLayer::changeTabCallback, this));
		buttons.pushBack(btn);
	}
	auto size = Utility::alignItemsVerticallyWithPadding(buttons, 3, _btnContainer->getContentSize());
	_btnContainer->setInnerContainerSize(size);
	return true;
}

void InfoLayer::changeTabCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Button* btn = (Button*)sender;
		auto tag = btn->getTag();
		if (tag == this->getTag())return;
		Layer* layer = nullptr;
		switch (tag)
		{
		case 0:
			layer = InventoryLayer::create();
			break;
		case 1:
		{
			auto inven = dynamic_cast<InventoryLayer*>(this);
			if (inven && inven->getType() == InventoryLayer::SHOP) {
				auto  stype = inven->getShopType();
				layer = ShopLayer::create((ShopLayer::ShopType)stype);
			}
			else
				layer = EquipmentLayer::create();
		}
		break;
		case 2:
			layer = SkillLayer::create();
			break;
		case 3:
			layer = QuestLayer::create();
			break;
		case 4:
			layer = SettingLayer::create();
			break;
		}
		if (layer) {
			this->clear();
			this->removeFromParent();
			GameManager::getInstance()->getHUDLayer()->addChild(layer, 10);
		}
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
