#include "MiniMap.h"
#include "GameMap.h"
#include "Definitions/Definitions.h"
#include "Entity/Entities.h"
#include "Manager/GameManager.h"
#include "Database/DataManager.h"

void MiniMap::initMaps() {
	auto objects = _map->getObjectGroup("maps");
	if (!objects) return;
	ValueVector maps = objects->getObjects();
	for (int i = 0; i < maps.size(); i++)
	{
		ValueMap value = maps.at(i).asValueMap();

		std::string name = value.at("name").asString();

		int x = value.at("x").asInt();
		int y = value.at("y").asInt();

		auto btn = Button::create();
		btn->addTouchEventListener(CC_CALLBACK_2(MiniMap::touchMapCallback, this));
		btn->ignoreContentAdaptWithSize(false);
		btn->setContentSize(Size(50, 50));
		btn->setName(name);
		btn->setPosition(Vec2(x, y));
		btn->setSwallowTouches(true);
		_map->addChild(btn);
	}
}

bool MiniMap::init()
{
	if (!initWithColor(Color4B(0, 0, 0, 255))) return false;
	_mapSelected = nullptr;
	_map = TMXTiledMap::create(PATH_MAP + "Map.tmx");
	_map->setIgnoreAnchorPointForPosition(false);
	_map->setPosition(_contentSize / 2);
	_map->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_map);
	initMaps();

	auto touchEvent = EventListenerTouchOneByOne::create();
	touchEvent->setSwallowTouches(true);
	touchEvent->onTouchBegan = [&](Touch* touch, Event* event) {
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);
	_tick = Sprite::create(PATH_IMAGE_UI + "160.png");
	_tick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_map->addChild(_tick, 10);
	_tick->setPosition(Vec2(-1000, -1000));

	_mapName = Label::createWithTTF("", DEFAULT_FONT, 30);
	_mapName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_mapName->setTextColor(Color4B::YELLOW);
	_mapName->enableOutline(Color4B::ORANGE, 2);
	_map->addChild(_mapName);

	auto closeBtn = Utility::createButton("Close", 0.7);
	closeBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			this->removeFromParent();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

	closeBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	closeBtn->setPosition(Vec2(_contentSize.width - _contentSize.height * 0.02, _contentSize.height * 0.02));
	this->addChild(closeBtn);

	auto teleBtn = Utility::createButton("Teleport(1000 coin)", 0.7);
	teleBtn->addTouchEventListener(CC_CALLBACK_2(MiniMap::teleCallback, this));
	teleBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	teleBtn->setPosition(Vec2(_contentSize.height * 0.02, _contentSize.height * 0.02));
	teleBtn->setVisible(false);
	teleBtn->setName("TeleBtn");
	this->addChild(teleBtn);
	return true;
}

void MiniMap::touchMapCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto btn = (Button*)sender;
		if (btn == _mapSelected) return;
		if (!_mapSelected) {
			this->getChildByName("TeleBtn")->setVisible(true);
		}
		_mapSelected = btn;
		_mapName->setString(btn->getName());
		_mapName->setPosition(btn->getPosition() + Vec2(0, 15));
		_tick->setPosition(btn->getPosition());
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void MiniMap::teleCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		if (GameManager::getInstance()->getMap()->getName() == _mapSelected->getName()) return;

		auto cha = GameManager::getInstance()->getCharacter();
		if (cha->getCoin() < 1000) {
			auto dialog = Dialog::create("I don't have e enough money");
			this->addChild(dialog);
			return;
		}

		auto cm = CharacterManager::create();

		if (!cm->canTeleMap(_mapSelected->getName())) {
			auto dl = Dialog::create("You can't go to this area yet");
			this->addChild(dl);
			return;
		}

		cha->setCoin(cha->getCoin() - 1000);
		cm->updateCoin(cha->getCoin());
		GameManager::getInstance()->changeMap(_mapSelected->getName(), 1);
		this->getParent()->removeFromParent();
		GameManager::getInstance()->resume();

	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

