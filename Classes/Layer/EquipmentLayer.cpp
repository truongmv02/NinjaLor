#include "EquipmentLayer.h"
#include "Manager/GameManager.h"
#include "Entity/Character/Character.h"

bool EquipmentLayer::init()
{
	if (!InfoLayer::init("Character")) return false;
	_mainContainer->setBounceEnabled(false);
	this->setTag(1);
	setButtonActive();
	initData();

	return true;
}

void EquipmentLayer::initData()
{
	_mainContainer->removeAllChildren();
	_infoContainer->removeAllChildren();
	createGrid();
	initStatInfo();
	initEquipment();
}

void EquipmentLayer::createGrid()
{
	auto containerSize = _mainContainer->getContentSize();

	_grid = Utility::createGird(2, 5, 100, 100);
	_grid->setName("Grid");
	_grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_grid->setPosition(Vec2(containerSize.width / 2, containerSize.height * 0.05));
	_mainContainer->addChild(_grid);

	auto grid = Utility::createGird(1, 1, containerSize.width * 0.9,
		containerSize.height - containerSize.height * 0.15 - _grid->getContentSize().height);
	grid->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	grid->setPosition(Vec2(containerSize.width * 0.05, containerSize.height * 0.95));
	grid->setName("GridStat");
	_mainContainer->addChild(grid);
}


void EquipmentLayer::initStatInfo()
{
	auto grid = _mainContainer->getChildByName("GridStat");
	auto gridSize = grid->getContentSize();
	_statContainer = Utility::createScrollView(Size(gridSize.width, gridSize.height * 0.9), ScrollView::Direction::VERTICAL);
	_statContainer->setAnchorPoint(Vec2::ZERO);
	_statContainer->setPosition(Vec2(0, gridSize.height * 0.05));
	grid->addChild(_statContainer);

	auto stats = GameManager::getInstance()->getCharacter()->getStats();
	Vector<Node*> nodes;

	auto labelHpString = "HP: " + std::to_string(Stat::getStat("Hp", stats)->getCurrentValue()) + "/" + std::to_string(Stat::getStat("HpMax", stats)->getCurrentValue());
	auto labelMpString = "MP: " + std::to_string(Stat::getStat("Mp", stats)->getCurrentValue()) + "/" + std::to_string(Stat::getStat("MpMax", stats)->getCurrentValue());

	auto labelHp = Label::createWithTTF(labelHpString, DEFAULT_FONT, 30);
	_statContainer->addChild(labelHp);
	nodes.pushBack(labelHp);

	auto labelMp = Label::createWithTTF(labelMpString, DEFAULT_FONT, 30);
	_statContainer->addChild(labelMp);
	nodes.pushBack(labelMp);

	for (auto stat : stats) {
		if (stat->getName() == "HpMax" || stat->getName() == "Hp" ||
			stat->getName() == "MpMax" || stat->getName() == "Mp") continue;
		auto valueString = stat->getValueType() == Stat::ValueType::NUMBER ? std::to_string(stat->getCurrentValue()) :
			Utility::percentString(stat->getCurrentValue());
		auto label = Label::createWithTTF(stat->getName() + ": " + valueString, DEFAULT_FONT, 30);
		_statContainer->addChild(label);
		nodes.pushBack(label);
	}

	auto size = Utility::alignItemsVerticallyWithPadding(nodes,
		_mainContainer->getContentSize().height * 0.02,
		grid->getContentSize(), 0,
		_mainContainer->getContentSize().width * 0.05);

	_statContainer->setInnerContainerSize(size);
}

void EquipmentLayer::initEquipment()
{
	_items = GameManager::getInstance()->getCharacter()->getEquipments();
	int tag = 0;
	for (auto item : _items)
	{
		auto btn = item->createButton(_grid, CC_CALLBACK_2(EquipmentLayer::touchItemCallback, this), tag++);
		Vec2 pos;
		btn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		auto gridHeight = _grid->getContentSize().height;
		switch (item->getType())
		{
		case 0:
			pos = Vec2(0, gridHeight);
			break;
		case 1:
			pos = Vec2(0, gridHeight - 97);
			break;
		case 2:
			pos = Vec2(100 - 3, gridHeight);
			break;
		case 3:
			pos = Vec2(100 - 3, gridHeight - 97);
			break;
		case 4:
			pos = Vec2(200 - 6, gridHeight);
			break;
		case 5:
			pos = Vec2(200 - 6, gridHeight - 97);
			break;
		case 6:
			pos = Vec2(300 - 9, gridHeight);
			break;
		case 7:
			pos = Vec2(300 - 9, gridHeight - 97);

			break;
		}
		btn->setPosition(pos);
	}
}

void EquipmentLayer::touchItemCallback(Ref* sender, Widget::TouchEventType type)
{
	auto btn = (Button*)sender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		if (btn == _btnSelected) return;
		if (_btnSelected) {
			_btnSelected->removeChildByName("Border");
		}
		_btnSelected = btn;

		auto border = Utility::createGird(1, 1, btn->getContentSize().width, btn->getContentSize().height, Color4F::RED);
		border->setName("Border");
		border->setPosition(btn->getContentSize() / 2);
		_btnSelected->addChild(border);
		initInfo();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void EquipmentLayer::initInfo()
{
	_infoContainer->removeAllChildren();
	_bgInfo->removeChildByName("StoreButton");

	auto item = _items.at(_btnSelected->getTag());
	initTitleInfo(item->getName());
	auto containerSize = _infoContainer->getContentSize();

	Vector<Node*> nodes;

	nodes.pushBack(item->createLabelInfo(_infoContainer, containerSize.width * 0.9));
	int price = item->getPriceBuy() * 0.25;
	auto labelPriceSell = Label::createWithTTF("Price: " + std::to_string(price), DEFAULT_FONT, 30);
	labelPriceSell->setColor(TEXT_YELLOW);
	_infoContainer->addChild(labelPriceSell);
	nodes.pushBack(labelPriceSell);

	auto size = Utility::alignItemsVerticallyWithPadding(nodes, containerSize.height * 0.02, containerSize, 0, containerSize.width * 0.05);
	_infoContainer->setInnerContainerSize(size);

	initItemOptions();
}

void EquipmentLayer::initItemOptions()
{
	auto contentSize = _infoContainer->getContentSize();

	auto item = _items.at(_btnSelected->getTag());
	auto storeBtn = Utility::createButton("Store", 0.8);
	storeBtn->setName("StoreButton");

	storeBtn->addTouchEventListener([&, item](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			if (GameManager::getInstance()->getCharacter()->storeEquipment(item)) {
				_btnSelected = nullptr;
				_infoContainer->removeAllChildren();
				_bgInfo->removeChildByName("StoreButton");
				initData();
			}
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

	storeBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	storeBtn->setPosition(Vec2(contentSize.width / 2, _contentSize.height * 0.02));
	_bgInfo->addChild(storeBtn);
}
