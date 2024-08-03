#include "ControlLayer.h"
#include "Definitions/Definitions.h"
#include "Manager/GameManager.h"
#include "Lib/InputHandler.h"
#include "Lib/Pointer.h"
#include "InfoLayers.h"
#include "HUDLayer.h"
#include "Entity/Entities.h"

bool ControlLayer::init() {
	initLeftControl();
	initRightControl();
	initSkillButton();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ControlLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ControlLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void ControlLayer::update(float dt)
{
	auto cha = GameManager::getInstance()->getCharacter();
	auto skills = cha->getSelectedSkill();
	for (int i = 0; i < 5; i++)
	{
		if (skills[i] && _skillButtons[i]) {
			auto scale = 1 - (float)skills[i]->getElapsedTime() / skills[i]->getCooldown();
			((Sprite*)_skillButtons[i]->getChildByName("DelayImage"))->setScaleY(scale);
		}
	}
}


void ControlLayer::initLeftControl() {

	_moveLeftBtn = Button::create(PATH_IMAGE_UI + "move_left_btn.png", "", PATH_IMAGE_UI + "move_left_btn_press.png");
	_moveRightBtn = Button::create(PATH_IMAGE_UI + "move_right_btn.png", "", PATH_IMAGE_UI + "move_right_btn_press.png");

	auto btnSize = _moveLeftBtn->getContentSize();

	_moveLeftBtn->setTouchEnabled(false);
	_moveRightBtn->setTouchEnabled(false);

	auto pos = Vec2(
		VISIBLE_SIZE.width * 0.04 + _moveLeftBtn->getContentSize().width / 2,
		VISIBLE_SIZE.height * 0.05 + _moveLeftBtn->getContentSize().height / 2);
	_moveLeftBtn->setPosition(pos);

	this->addChild(_moveLeftBtn, 2);

	_moveRightBtn->setPosition(Vec2(pos.x + btnSize.width + VISIBLE_SIZE_WIDTH * 0.05, pos.y));
	this->addChild(_moveRightBtn, 2);

	_openMenuBtn = Button::create();
	_openMenuBtn->ignoreContentAdaptWithSize(false);
	_openMenuBtn->setContentSize(Sprite::create(PATH_IMAGE_UI + "state_board.png")->getContentSize());
	_openMenuBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_openMenuBtn->setPosition(Vec2(VISIBLE_SIZE_WIDTH * 0.005, VISIBLE_SIZE_HEIGHT * 0.99));
	this->addChild(_openMenuBtn);
	_openMenuBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto layer = InventoryLayer::create();
			auto hudLayer = GameManager::getInstance()->getHUDLayer();
			hudLayer->addChild(layer, 10);
			hudLayer->getControlLayer()->setVisible(false);
			hudLayer->getControlLayer()->pause();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

}

void ControlLayer::initRightControl()
{
	_attackBtn = Button::create(PATH_IMAGE_UI + "attack_btn.png", PATH_IMAGE_UI + "attack_btn_press.png");
	_attackBtn->addTouchEventListener(CC_CALLBACK_2(ControlLayer::attackButtonCallBack, this));
	_attackBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(_attackBtn);

	_jumpBtn = Button::create(PATH_IMAGE_UI + "jump_btn.png", PATH_IMAGE_UI + "jump_btn_press.png");
	_jumpBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_jumpBtn->addTouchEventListener(CC_CALLBACK_2(ControlLayer::jumpBtnCallBack, this));
	this->addChild(_jumpBtn, 2);

	auto pos = Vec2(VISIBLE_SIZE_WIDTH * 0.96, VISIBLE_SIZE_HEIGHT * 0.05);
	auto attackBtnSize = _attackBtn->getContentSize();

	_hpBtn = Button::create(PATH_IMAGE_UI + "btn_circle_small.png", PATH_IMAGE_UI + "btn_circle_small_press.png");
	_hpBtn->ignoreContentAdaptWithSize(false);
	_hpBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_hpBtn->setName("UseHp");
	_hpBtn->addTouchEventListener(CC_CALLBACK_2(ControlLayer::usePotionCallback, this));

	auto sprite = Sprite::create(PATH_IMAGE_UI + "hp_btn.png");
	sprite->setName("Sprite");
	sprite->setPosition(_hpBtn->getContentSize() / 2);
	_hpBtn->addChild(sprite);

	this->addChild(_hpBtn);

	_mpBtn = (Button*)_hpBtn->clone();

	_mpBtn->setName("UseMp");
	_mpBtn->addTouchEventListener(CC_CALLBACK_2(ControlLayer::usePotionCallback, this));
	sprite = Sprite::create(PATH_IMAGE_UI + "mp_btn.png");
	sprite->setName("Sprite");
	sprite->setPosition(_hpBtn->getContentSize() / 2);
	_mpBtn->addChild(sprite);

	this->addChild(_mpBtn);

	_switchBtn = Button::create(PATH_IMAGE_UI + "switch_btn.png", PATH_IMAGE_UI + "switch_btn_press.png");
	_switchBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_switchBtn->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			GameManager::getInstance()->getPointer()->changeTarget();
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
	this->addChild(_switchBtn);

	// Set Position
	_switchBtn->setPosition(Vec2(pos.x - attackBtnSize.width - VISIBLE_SIZE_WIDTH * 0.04, pos.y));
	_attackBtn->setPosition(pos);
	_jumpBtn->setPosition(Vec2(pos.x, pos.y + attackBtnSize.height + VISIBLE_SIZE_HEIGHT * 0.15));

	auto hpBtnPos = _switchBtn->getPosition() +
		Vec2(VISIBLE_SIZE_WIDTH * 0.025, _switchBtn->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.03);
	_hpBtn->setPosition(hpBtnPos);

	auto mpBtnPos = pos + Vec2(-_attackBtn->getContentSize().width, _attackBtn->getContentSize().height) +
		Vec2(VISIBLE_SIZE_WIDTH * 0.03, VISIBLE_SIZE_HEIGHT * 0.04);
	_mpBtn->setPosition(mpBtnPos);

	auto cha = GameManager::getInstance()->getCharacter();

	auto hpPotionCount = cha->getItemCount(Item::HP_POTION);
	auto labelHpQty = Label::createWithTTF(std::to_string(hpPotionCount), DEFAULT_FONT_MEDIUM, 30);
	labelHpQty->setName("Quantity");
	labelHpQty->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	labelHpQty->setPosition(Vec2(_hpBtn->getContentSize().width / 2, 10));
	_hpBtn->addChild(labelHpQty);

	auto mpPotionCount = cha->getItemCount(Item::MP_POTION);
	auto labelMpQty = Label::createWithTTF(std::to_string(mpPotionCount), DEFAULT_FONT_MEDIUM, 30);
	labelMpQty->setName("Quantity");
	labelMpQty->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	labelMpQty->setPosition(Vec2(_mpBtn->getContentSize().width / 2, 10));
	_mpBtn->addChild(labelMpQty);


}

void ControlLayer::initSkillButton()
{
	this->unscheduleUpdate();
	this->removeChildByName("SkillContainer");
	auto cha = GameManager::getInstance()->getCharacter();
	auto skills = cha->getSelectedSkill();
	Vector<Node*> buttons;
	auto layout = Layout::create();
	layout->setName("SkillContainer");
	for (int i = 0; i < 5; i++)
	{
		auto button = Button::create(PATH_IMAGE_UI + "skill_btn.png");
		button->setZoomScale(0);
		button->setTag(i);
		if (skills[i]) {

			button->addTouchEventListener([&, cha](Ref* sender, Widget::TouchEventType type) {
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					break;
				case cocos2d::ui::Widget::TouchEventType::MOVED:
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
				{
					auto btn = (Button*)sender;
					auto tag = btn->getTag();
					if (tag == cha->getSkillChoose()) {
						auto enemy = GameManager::getInstance()->getEnemyFocus();
						if (dynamic_cast<Enemy*> (enemy)) {
							//cha->useSkill((Enemy*)enemy);
							attackButtonCallBack(_attackBtn, Widget::TouchEventType::BEGAN);
						}
						return;
					}
					_selectecdSkill->removeChildByName("Border");
					_selectecdSkill = btn;
					cha->setSkillChoose(tag);
					auto border = Utility::createGird(1, 1, _selectecdSkill->getContentSize().width, _selectecdSkill->getContentSize().height, Color4F::WHITE, 5);
					border->setName("Border");
					border->setPosition(btn->getContentSize() / 2);
					btn->addChild(border);
				}
				break;
				case cocos2d::ui::Widget::TouchEventType::CANCELED:
					break;
				default:
					break;
				}
				});

			button->ignoreContentAdaptWithSize(false);
			auto skillImg = Sprite::create(PATH_IMAGE_UI + skills[i]->getName() + ".png");
			skillImg->setPosition(button->getContentSize() / 2);
			button->addChild(skillImg);

			auto delayImg = Sprite::create(PATH_IMAGE_UI + "delay_skill" + ".png");
			delayImg->setScaleY(0);
			delayImg->setName("DelayImage");
			delayImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			delayImg->setPosition(skillImg->getPosition().x, skillImg->getPosition().y - skillImg->getContentSize().height / 2);
			button->addChild(delayImg);

		}

		_skillButtons[i] = button;
		layout->addChild(button);
		buttons.pushBack(button);
	}
	_selectecdSkill = _skillButtons[cha->getSkillChoose()];
	auto border = Utility::createGird(1, 1, _selectecdSkill->getContentSize().width, _selectecdSkill->getContentSize().height, Color4F::WHITE, 5);
	border->setName("Border");
	border->setPosition(_selectecdSkill->getContentSize() / 2);
	_selectecdSkill->addChild(border);

	auto size = Utility::alignItemsHorizontallyWithPadding(buttons, 20);
	layout->setContentSize(size);
	layout->setIgnoreAnchorPointForPosition(false);
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	layout->setPosition(Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.06));
	this->addChild(layout);
	this->scheduleUpdate();
}

void ControlLayer::attackButtonCallBack(Ref* sender, Widget::TouchEventType type)
{
	auto pointer = GameManager::getInstance()->getPointer();
	auto entityFocus = pointer->getEntityFocus();
	auto cha = GameManager::getInstance()->getCharacter();
	if (!entityFocus) return;
	auto entityPos = entityFocus->getPosition();
	auto chaPos = cha->getPosition();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		auto dir = cha->getDir();
		dir.x = chaPos.x <= entityPos.x ? 1 : -1;
		cha->setDir(dir);

		auto enemy = dynamic_cast<Enemy*>(entityFocus);

		if (enemy) {
			GameManager::getInstance()->getCharacter()->useSkill(enemy);
			return;
		}

		auto distance = abs(chaPos.x - entityPos.x);
		if (distance > 200) {
			auto dt = Director::getInstance()->getAnimationInterval();
			cha->setMoveTarget(true);
			cha->schedule(CC_CALLBACK_1(Character::move, cha), dt, "Move");
			CallbackTimeCounter* timeCounter = CallbackTimeCounter::create();
			timeCounter->initWithCondition([=]() {
				if (cha->isScheduled("Move")) {
					cha->unschedule("Move");
					cha->setMoveTarget(false);
				}
				timeCounter->unscheduleUpdate();
				timeCounter->removeFromParent();
				}, [&, entityFocus, cha]() {
					auto ownerPos = cha->getPosition();
					auto tarPos = entityFocus->getPosition();
					auto length = (ownerPos - tarPos).getLength();

					if (length < 200) {
						return true;
					}

					if (abs(ownerPos.x - tarPos.x) < 50) {
						return true;
					}

					return false;
				});
			this->addChild(timeCounter);

			return;
		}

		auto npc = dynamic_cast<NPC*>(entityFocus);
		if (npc) {
			npc->setFlippedX(npc->getPositionX() > cha->getPositionX());
			npc->onClick();
			return;
		}
		auto coin = dynamic_cast<Coin*>(entityFocus);
		if (dynamic_cast<Item*>(entityFocus) || dynamic_cast<Coin*>(entityFocus)) {
			cha->pickEntity(entityFocus);
		}


	}
	break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ControlLayer::jumpBtnCallBack(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		InputHandler::getInstance()->onKeyPress(InputHandler::JUMP);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		InputHandler::getInstance()->onKeyReleased(InputHandler::JUMP);
		break;
	default:
		break;
	}
}

void ControlLayer::usePotionCallback(Ref* sender, Widget::TouchEventType type)
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
		auto cha = GameManager::getInstance()->getCharacter();
		auto poType = btn->getName() == "UseHp" ? Item::HP_POTION : Item::MP_POTION;
		auto item = Item::findItem(cha->getItemBag(), poType);

		if (item) {
			if (cha->useItem(item)) {
				auto label = (Label*)btn->getChildByName("Quantity");
				label->setString(std::to_string(cha->getItemCount(poType)));
			};
		}
		else {
			auto name = poType == Item::HP_POTION ? "HP" : "MP";
			auto message = Message::create(std::string("Out off ") + name);
			GameManager::getInstance()->getHUDLayer()->addMessage(message);
		}
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}

bool ControlLayer::onTouchBegan(Touch* touch, Event* event) {
	if (_moveLeftBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		InputHandler::getInstance()->onKeyPress(InputHandler::MOVE_LEFT);
		_moveLeftBtn->setEnabled(false);
	}
	else if (_moveRightBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		InputHandler::getInstance()->onKeyPress(InputHandler::MOVE_RIGHT);
		_moveRightBtn->setEnabled(false);
	}
	else
		return false;
	return true;
}

void ControlLayer::onTouchMoved(Touch* touch, Event* event) {
	auto moveLeftContainsPoint = _moveLeftBtn->getBoundingBox().containsPoint(touch->getLocation());
	auto moveRightContainsPoint = _moveRightBtn->getBoundingBox().containsPoint(touch->getLocation());

	if (moveLeftContainsPoint) {
		_moveLeftBtn->setEnabled(false);
		InputHandler::getInstance()->onKeyPress(InputHandler::Input::MOVE_LEFT);
	}
	else
	{
		_moveLeftBtn->setEnabled(true);
		InputHandler::getInstance()->onKeyReleased(InputHandler::Input::MOVE_LEFT);
	}

	if (moveRightContainsPoint) {
		_moveRightBtn->setEnabled(false);
		InputHandler::getInstance()->onKeyPress(InputHandler::Input::MOVE_RIGHT);
	}
	else {
		_moveRightBtn->setEnabled(true);
		InputHandler::getInstance()->onKeyReleased(InputHandler::Input::MOVE_RIGHT);
	}
}


void ControlLayer::onTouchEnded(Touch* touch, Event* event) {
	if (_moveLeftBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		InputHandler::getInstance()->onKeyReleased(InputHandler::MOVE_LEFT);
		_moveLeftBtn->setEnabled(true);
	}

	if (_moveRightBtn->getBoundingBox().containsPoint(touch->getLocation())) {
		InputHandler::getInstance()->onKeyReleased(InputHandler::MOVE_RIGHT);
		_moveRightBtn->setEnabled(true);
	}

}
