#include "Item.h"
#include "Definitions/Definitions.h"
#include <string>
#include "Lib/CallbackTimeCounter.h"
#include "Manager/GameManager.h"

Item::Item()
{
	_islock = false;
	_quantity = 1;
	_onGround = false;
}

Item* Item::findItem(Vector<Item*> items, ItemType type)
{
	for (auto item : items) {
		if (item->_type == type) return item;
	}
	return nullptr;
}

Item* Item::findItem(Vector<Item*> items, int id)
{
	for (auto item : items) {
		if (item->getId() == id) return item;
	}
	return nullptr;
}

bool Item::init()
{
	return true;
}

bool Item::isEquipment()
{
	return _type <= NECKLACE;
}

Button* Item::createButton(Node* parent, Widget::ccWidgetTouchCallback callback, int tag)
{
	auto btn = Button::create();
	btn->setTag(tag);
	if (callback) {
		btn->addTouchEventListener(callback);
	}
	if (parent) {
		parent->addChild(btn);
	}
	btn->ignoreContentAdaptWithSize(false);
	btn->setContentSize(Size(100, 100));
	auto sprite = Sprite::createWithSpriteFrameName("item_icon/" + _name);
	sprite->setPosition(btn->getContentSize() / 2);
	btn->addChild(sprite);

	if (_quantity > 1) {
		auto labelQty = Label::createWithTTF(std::to_string(_quantity), DEFAULT_FONT, 24);
		labelQty->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		labelQty->setPosition(btn->getContentSize().width - 8, 2);
		btn->addChild(labelQty);
	}
	return btn;
}

Vector<Node*> Item::createLabelInfo(Node* parent, float dimensions)
{
	Vector<Node*> nodes;
	auto labelLevelRequire = Label::createWithTTF("Require: level " + std::to_string(_levelRequire), DEFAULT_FONT, 30);
	parent->addChild(labelLevelRequire);
	nodes.pushBack(labelLevelRequire);

	if (!_desc.empty()) {
		auto labelDesc = Label::createWithTTF(_desc, DEFAULT_FONT, 30);
		labelDesc->setDimensions(dimensions, labelDesc->getContentSize().height);
		labelDesc->setOverflow(Label::Overflow::RESIZE_HEIGHT);
		parent->addChild(labelDesc);
		nodes.pushBack(labelDesc);
	}
	if (isEquipment())
		for (auto stat : _stats) {
			for (auto bonus : stat->getBonuses()) {
				auto name = stat->getName() == "HpMax" ? "HP" : stat->getName() == "MpMax" ? "MP" : stat->getName();
				auto valueString = bonus->getValueType() == Stat::ValueType::NUMBER && stat->getValueType() == Stat::ValueType::NUMBER ? std::to_string(bonus->getValue()) :
					Utility::percentString(bonus->getValue());
				auto labelBonus = Label::createWithTTF(name + ": +" + valueString, DEFAULT_FONT, 30);
				parent->addChild(labelBonus);
				nodes.pushBack(labelBonus);
			}
		}
	return nodes;
}

void Item::clear()
{
	for (auto stat : _stats) {
		stat->clear();
		CC_SAFE_DELETE(stat);
	}
}

void Item::autoRemove()
{
	auto timeCounter = CallbackTimeCounter::create();
	timeCounter->init(30, [&, timeCounter]() {
		if (this->getOnGround()) {
			this->autoRemoveBody();
			GameManager::getInstance()->removeEntity(this);
			auto p = GameManager::getInstance()->getPointer();
			if (p->getEntityFocus() == this) {
				p->setEntityFocus(nullptr);
			}
			this->clear();
			this->release();
		}
		});
}

