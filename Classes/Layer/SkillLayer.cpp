#include "SkillLayer.h"
#include <regex>
#include "Lib/Dialog.h"
#include "Lib/CardMenu.h"
#include "Entity/Character/Character.h"
#include "Manager/GameManager.h"

bool SkillLayer::init()
{
	if (!InfoLayer::init("Skills")) return false;
	this->setTag(2);
	setButtonActive();

	auto  mainSprite = _bgMain->getChildByName("Container");
	_mainContainer->setContentSize(Size(mainSprite->getContentSize().width, mainSprite->getContentSize().height * 0.8));
	_mainContainer->setPosition(Vec2(0, mainSprite->getContentSize().height * 0.86));

	_sm = SkillManager::create();

	_sm->retain();

	_character = GameManager::getInstance()->getCharacter();
	initData();
	return true;
}

void SkillLayer::initData()
{
	_mainContainer->removeAllChildren();
	_infoContainer->removeAllChildren();

	auto mainSprite = _bgMain->getChildByName("Container");
	mainSprite->removeChildByName("SkillPoint");
	auto containerSize = _mainContainer->getContentSize();

	auto labelSkillPoint = Label::createWithTTF("Skill points: " + std::to_string(_character->getSkillPoint()), DEFAULT_FONT, 30);
	labelSkillPoint->setName("SkillPoint");
	labelSkillPoint->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelSkillPoint->setPosition(Vec2(containerSize.width * 0.05, mainSprite->getContentSize().height * 0.95));
	mainSprite->addChild(labelSkillPoint);

	Vector<Node*> btns;
	int tag = 0;
	for (auto skill : _character->getSkills()) {
		auto button = Button::create();
		button->addTouchEventListener(CC_CALLBACK_2(SkillLayer::touchSkillCallback, this));
		button->ignoreContentAdaptWithSize(false);
		button->setContentSize(Size(containerSize.width * 0.9, 100));
		button->setTag(tag++);

		auto sprite = Sprite::create(PATH_IMAGE_UI + skill->getName() + ".png");
		sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		sprite->setPosition(Vec2(containerSize.width * 0.05, 50));
		button->addChild(sprite);

		auto label = Label::createWithTTF(skill->getName(), DEFAULT_FONT, 30);
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setPosition(Vec2(sprite->getContentSize().width + containerSize.width * 0.05, sprite->getContentSize().height / 2));
		sprite->addChild(label);

		if (skill->getLevel() != 0) {
			auto labelLevel = Label::createWithTTF(std::string("Lv: ") + std::to_string(skill->getLevel()), DEFAULT_FONT, 36);
			labelLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			labelLevel->setPosition(Vec2(button->getContentSize().width * 0.95, button->getContentSize().height / 2));
			button->addChild(labelLevel);
		}

		auto grid = Utility::createGird(1, 1, button->getContentSize().width, button->getContentSize().height, Color4F::WHITE);
		grid->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		grid->setName("Border");
		button->addChild(grid);

		btns.pushBack(button);
		_mainContainer->addChild(button);
	}

	auto size = Utility::alignItemsVerticallyWithPadding(
		btns, containerSize.height * 0.05, containerSize, 0, containerSize.width * 0.05);
	_mainContainer->setInnerContainerSize(size);
	if (_btnSelected != -1) {
		touchSkillCallback(_mainContainer->getChildByTag(_btnSelected), Widget::TouchEventType::ENDED);
	}

}

void SkillLayer::initInfo()
{
	_bgInfo->removeChildByName("SetKeyBtn");
	_bgInfo->removeChildByName("AddPointBtn");
	auto containerSize = _infoContainer->getContentSize();
	_infoContainer->setContentSize(Size(_bgInfo->getContentSize().width, _bgInfo->getContentSize().height * 0.7));

	_infoContainer->removeAllChildren();
	auto skill = _character->getSkills().at(_btnSelected);
	initTitleInfo("Skill " + skill->getName());
	Vector<Node*> labels;
	if (skill->getLevel() == skill->getLevelMax()) {
		labels.pushBack(createLabels(skill));
	}
	else {

		auto skillNextLevel = _sm->findSkill(skill->getId(), skill->getLevel() + 1);
		labels.pushBack(createLabels(skill, skillNextLevel));
		labels.pushBack(createLabels(skillNextLevel, nullptr, false));
		skillNextLevel->clear();
		CC_SAFE_DELETE(skillNextLevel);
	}

	auto infoSize = Utility::alignItemsVerticallyWithPadding(labels, containerSize.height * 0.02, containerSize, 0, containerSize.width * 0.05);
	_infoContainer->setInnerContainerSize(infoSize);

	initButton();
}

void SkillLayer::clear()
{
	InfoLayer::clear();
	CC_SAFE_DELETE(_sm);
}

void SkillLayer::clearInfo()
{
	_infoContainer->removeAllChildren();
}

void SkillLayer::initButton()
{
	auto skill = _character->getSkills().at(_btnSelected);

	auto addPointBtn = Utility::createButton("Upgrade", 0.8);
	auto setKeyBtn = Utility::createButton("Set key", 0.8);

	addPointBtn->setName("AddPointBtn");
	setKeyBtn->setName("SetKeyBtn");

	addPointBtn->addTouchEventListener(CC_CALLBACK_2(SkillLayer::touchAddPointButtonCallback, this));
	setKeyBtn->addTouchEventListener(CC_CALLBACK_2(SkillLayer::touchSetKeyButtonCallback, this));

	auto containerSize = _bgInfo->getContentSize();

	addPointBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	setKeyBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	addPointBtn->setPosition(Vec2(containerSize.width * 0.05, containerSize.height * 0.01));
	setKeyBtn->setPosition(Vec2(containerSize.width * 0.95, containerSize.height * 0.01));

	if (skill->getLevel() == skill->getLevelMax() || _character->getSkillPoint() == 0) {
		addPointBtn->setEnabled(false);
	}
	else {
		auto nextLevel = _sm->findSkill(skill->getId(), skill->getLevel() + 1);
		if (skill->getLevel() == 0) {
			setKeyBtn->setEnabled(false);
			if (_character->getLevel() < nextLevel->getLevelRequire()) {
				addPointBtn->setEnabled(false);
			}
		}
		else if (_character->getLevel() < nextLevel->getLevelRequire()) {
			addPointBtn->setEnabled(false);
		}
		nextLevel->clear();
		CC_SAFE_DELETE(nextLevel);
	}

	_bgInfo->addChild(addPointBtn);
	_bgInfo->addChild(setKeyBtn);
}

Vector<Node*> SkillLayer::createLabels(Skill* skill, Skill* nextLevel, bool isCurrentLevel)
{
	auto containerSize = _infoContainer->getContentSize();
	Vector<Node*> labels;

	if (isCurrentLevel) {

		Label* labelLevelRequire;
		if (skill->getLevel() == 0) {
			labelLevelRequire = Label::createWithTTF("Require: level " + std::to_string(nextLevel->getLevelRequire()), DEFAULT_FONT, 30);
		}
		else if (skill->getLevel() == skill->getLevelMax()) {
			labelLevelRequire = Label::createWithTTF("Reached maximum level", DEFAULT_FONT, 30);
			labelLevelRequire->setColor(Color3B::GREEN);
		}
		else {
			labelLevelRequire = Label::createWithTTF("Upgrade require: level " + std::to_string(nextLevel->getLevelRequire()), DEFAULT_FONT, 30);

		}

		_infoContainer->addChild(labelLevelRequire);
		labels.pushBack(labelLevelRequire);

		auto labelDesc = Label::createWithTTF(skill->getDesc(), DEFAULT_FONT, 30);
		labelDesc->setDimensions(containerSize.width * 0.9, labelDesc->getContentSize().height);
		labelDesc->setOverflow(Label::Overflow::RESIZE_HEIGHT);
		_infoContainer->addChild(labelDesc);
		labels.pushBack(labelDesc);

		auto labelLevelMax = Label::createWithTTF("Maximum level: " + std::to_string(skill->getLevelMax()), DEFAULT_FONT, 30);
		_infoContainer->addChild(labelLevelMax);
		labels.pushBack(labelLevelMax);

	}
	else {
		auto labelNextLevel = Label::createWithTTF("Next level: " + std::to_string(skill->getLevel()), DEFAULT_FONT, 30);
		_infoContainer->addChild(labelNextLevel);
		labels.pushBack(labelNextLevel);
		labelNextLevel->setColor(Color3B::RED);

		auto labelLevelRequire = Label::createWithTTF("Require: level " + std::to_string(skill->getLevelRequire()), DEFAULT_FONT, 30);
		_infoContainer->addChild(labelLevelRequire);
		labels.pushBack(labelLevelRequire);
		labelLevelRequire->setColor(Color3B::RED);

	}
	if (skill->getLevel() != 0) {
		if (skill->getMpUse() > 0) {
			auto labelMpUse = Label::createWithTTF("Mp use: " + std::to_string(skill->getMpUse()), DEFAULT_FONT, 30);
			_infoContainer->addChild(labelMpUse);
			labels.pushBack(labelMpUse);

		}
		if (skill->getCooldown() > 0) {
			auto labelCoolDown = Label::createWithTTF("Cooldown: " + std::to_string(skill->getCooldown()) + " milisecond",
				DEFAULT_FONT, 30);
			_infoContainer->addChild(labelCoolDown);
			labels.pushBack(labelCoolDown);

		}
		if (skill->getRange() > 0) {
			auto labelRange = Label::createWithTTF("Range: " + std::to_string(skill->getRange()), DEFAULT_FONT, 30);
			_infoContainer->addChild(labelRange);
			labels.pushBack(labelRange);
		}
		for (auto stat : skill->getStats()) {
			if (stat->getValue() != 0) {
				auto labelStat = Label::createWithTTF(stat->getName() + ": +" + std::to_string(stat->getValue()), DEFAULT_FONT, 30);
				_infoContainer->addChild(labelStat);
				labels.pushBack(labelStat);
			}
			for (auto bonus : stat->getBonuses()) {
				auto valueString = bonus->getValueType() == Stat::ValueType::NUMBER ? std::to_string(bonus->getValue()) :
					Utility::percentString(bonus->getValue());
				auto labelBonus = Label::createWithTTF(stat->getName() + ": +" + valueString, DEFAULT_FONT, 30);
				_infoContainer->addChild(labelBonus);
				labels.pushBack(labelBonus);
			}
		}
	}

	return labels;
}

void SkillLayer::touchSkillCallback(Ref* sender, Widget::TouchEventType type)
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

		if (_btnSelected != -1) {

			_mainContainer->getChildByTag(_btnSelected)->removeChildByName("Border");
			auto border = Utility::createGird(1, 1, btn->getContentSize().width, btn->getContentSize().height, Color4F::WHITE);
			border->setName("Border");
			border->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			_mainContainer->getChildByTag(_btnSelected)->addChild(border);
		}

		_btnSelected = btn->getTag();
		auto border = Utility::createGird(1, 1, btn->getContentSize().width, btn->getContentSize().height, Utility::color3BToColor4F(BORDER_YELLOW));
		border->setName("Border");
		border->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		_mainContainer->getChildByTag(_btnSelected)->addChild(border);
		initInfo();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SkillLayer::touchAddPointButtonCallback(Ref* sender, Widget::TouchEventType type)
{
	auto btn = (Button*)sender;
	auto skill = _character->getSkills().at(_btnSelected);
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto skillPoint = _character->getSkillPoint();
		_sm->upgradeSkill(skill->getId(), skill->getLevel() + 1);
		_sm->updateSkillPoint(skillPoint - 1);
		_character->setSkillPoint(skillPoint - 1);
		skill->setLevel(skill->getLevel() + 1);
		auto skillNext = _sm->findSkill(skill->getId(), skill->getLevel());
		skill->setCooldown(skillNext->getCooldown());
		skill->setMpUse(skillNext->getMpUse());
		skill->setLevelRequire(skillNext->getLevelRequire());
		skill->setStats(skillNext->getStats());
		CC_SAFE_DELETE(skillNext);
		initData();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SkillLayer::touchSetKeyButtonCallback(Ref* sender, Widget::TouchEventType type)
{
	auto skill = _character->getSkills().at(_btnSelected);

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto cardMenu = CardMenu::create(600, 700);
		cardMenu->setPosition(VISIBLE_SIZE / 2);
		this->addChild(cardMenu);
		Vector<Node*> btns;
		auto container = cardMenu->getContainer();
		for (int i = 0; i < 5; i++) {
			auto btn = cardMenu->createButton("KEY " + std::to_string(i + 1));
			btn->setTag(i);
			btn->addTouchEventListener([&, skill, cardMenu](Ref* button, Widget::TouchEventType t) {
				switch (t)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					break;
				case cocos2d::ui::Widget::TouchEventType::MOVED:
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
				{
					auto tag = ((Button*)button)->getTag();
					auto skills = _character->getSelectedSkill();
					auto sm = SkillManager::create();
					for (int i = 0; i < 5; i++)
					{
						if (skills[i] && skills[i]->getId() == skill->getId()) {
							skills[i] = nullptr;
							if (_character->getSkillChoose() == i) {
								_character->setSkillChoose(tag);
								sm->setSkillChoose(tag);
							}
						}
					}
					skills[tag] = skill;
					sm->setIndex(skill->getId(), tag);
					GameManager::getInstance()->getHUDLayer()->getControlLayer()->initSkillButton();
					cardMenu->removeFromParent();
				}
				break;
				case cocos2d::ui::Widget::TouchEventType::CANCELED:
					break;
				default:
					break;
				}
				});
			btns.pushBack(btn);
		}
		auto menuSize = Utility::alignItemsVerticallyWithPadding(btns, 0,
			container->getContentSize(), 0, container->getContentSize().width * 0.05);
		container->setInnerContainerSize(menuSize);
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}