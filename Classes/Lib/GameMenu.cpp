#include "GameMenu.h"
#include "Definitions/Definitions.h"
#include "Lib/Lib.h"

GameMenu::GameMenu()
{
	_type = Type::DEFAULT;
}


GameMenu* GameMenu::create(Size size, Type type, Direction direction)
{
	GameMenu* menu = new(std::nothrow) GameMenu();

	if (menu && menu->init(size, type, direction)) {

		menu->autorelease();
		return menu;
	}
	CC_SAFE_DELETE(menu);
	return nullptr;
}

bool GameMenu::init()
{
	if (!ListView::init()) return false;

	this->setSwallowTouches(true);
	this->setScrollBarEnabled(false);

	_bounceEnabled = true;
	return true;
}

bool GameMenu::init(Size size, Type type, Direction direction)
{
	if (!GameMenu::init())  return false;

	_type = type;
	this->setContentSize(size);
	_position = Vec2(VISIBLE_SIZE_WIDTH / 2, this->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.05 + 1);

	this->setDirection(direction);
	if (direction == ScrollView::Direction::HORIZONTAL) {
		this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	}
	else if (direction == ScrollView::Direction::VERTICAL) {
		this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	}
	return true;
}

void GameMenu::open()
{
	auto openAction = MoveTo::create(0.12,
		Vec2(VISIBLE_SIZE_WIDTH / 2, this->getContentSize().height + VISIBLE_SIZE_HEIGHT * 0.05 + 1));
	this->runAction(openAction);
}

void GameMenu::close()
{
	auto closeAction = MoveTo::create(0.12, Vec2(VISIBLE_SIZE_WIDTH / 2, -1));
	this->runAction(closeAction);
}


void GameMenu::centerItems()
{
	if (_direction == Direction::HORIZONTAL) {
		auto size = Utility::getSizeHorizontal(_innerContainer->getChildren(), _itemsMargin);
		this->setLeftPadding(_contentSize.width / 2 - size.width / 2);
	}
}

bool GameMenu::onTouchBegan(Touch* touch, Event* event)
{
	if (!ListView::onTouchBegan(touch, event)) {
		if (_type == Type::CLICK_OUTSIDE_CLOSE) {
			close();
		}
		return false;
	}
	return true;
}
