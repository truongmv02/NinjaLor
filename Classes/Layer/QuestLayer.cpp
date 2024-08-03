#include "QuestLayer.h"
#include "Game/Quest.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"
#include "Map/MiniMap.h"

bool QuestLayer::init()
{
	if (!InfoLayer::init("Quest")) return false;
	this->setTag(3);
	setButtonActive();
	auto mainSprite = _bgMain->getChildByName("Container");
	_mainContainer->setContentSize(Size(mainSprite->getContentSize().width, mainSprite->getContentSize().height * 0.7));
	_mainContainer->setPosition(Vec2(0, mainSprite->getContentSize().height * 0.85));

	initData();
	return true;
}

void QuestLayer::initData()
{
	_quest = GameManager::getInstance()->getCharacter()->getQuest();
	auto mainSprite = _bgMain->getChildByName("Container");

	if (_quest->getStatus() == Quest::UNRECEIVED) {
		auto label = createLabel("Come to Neko Sennin at Uchiha Clan to get the quest.", 36);
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		label->setPosition(Vec2(mainSprite->getContentSize().width / 2, mainSprite->getContentSize().height * 0.98));
		label->removeFromParentAndCleanup(false);
		mainSprite->addChild(label);
	}
	else {
		initQuestInfo();
	}
	auto mapBtn = Utility::createButton("Map", 0.6);
	mapBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	mapBtn->setPosition(Size(mainSprite->getContentSize().width / 2, mainSprite->getContentSize().height * 0.025));
	mainSprite->addChild(mapBtn);

	mapBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto map = MiniMap::create();
			this->addChild(map, 100);
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
}

void QuestLayer::initQuestInfo()
{
	auto mainSprite = _bgMain->getChildByName("Container");
	mainSprite->removeChildByName("QuestName");

	auto labelName = Label::createWithTTF(_quest->getName(), DEFAULT_FONT_MEDIUM, 40);
	labelName->setDimensions(_mainContainer->getContentSize().width, labelName->getContentSize().height);
	labelName->setAlignment(TextHAlignment::CENTER);
	labelName->setName("QuestName");
	labelName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	labelName->setPosition(Vec2(mainSprite->getContentSize().width / 2, mainSprite->getContentSize().height * 0.98));
	labelName->setTextColor(Color4B::YELLOW);
	labelName->enableOutline(Color4B::RED, 2);
	mainSprite->addChild(labelName);

	Vector<Node*> labels;
	Label* label;
	if (!_quest->getDescription().empty()) {
		label = createLabel(_quest->getDescription());
		labels.pushBack(label);
	}

	for (auto data : _quest->getEnemies()) {
		auto dataString = "Kill " + data.EnemyName + " (" + std::to_string(data.Kill) + "/" + std::to_string(data.Quantity) + ")";
		label = createLabel(dataString);
		labels.pushBack(label);
	}

	label = createLabel("Report to Neko Sennin.");
	labels.pushBack(label);

	label = createLabel("Reward:", 36);
	label->setColor(Color3B::YELLOW);
	labels.pushBack(label);

	if (_quest->getCoin() > 0) {
		label = createLabel("+ " + std::to_string(_quest->getCoin()) + " Coin");
		labels.pushBack(label);
	}
	if (_quest->getExp() > 0) {
		label = createLabel("+ " + std::to_string(_quest->getExp()) + " Exp");
		labels.pushBack(label);
	}

	auto size = Utility::alignItemsVerticallyWithPadding(labels, _mainContainer->getContentSize().height * 0.01, _mainContainer->getContentSize(), 0, _mainContainer->getContentSize().height * 0.05);
	_mainContainer->setInnerContainerSize(size);
}

Label* QuestLayer::createLabel(std::string data, int font)
{
	auto label = Label::createWithTTF(data, DEFAULT_FONT, font);
	_mainContainer->addChild(label);
	label->setDimensions(_mainContainer->getContentSize().width * 0.9, label->getContentSize().height);
	label->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	return label;
}

void QuestLayer::clear()
{

}
