#include "Card.h"
#include "Lib/Utility.h"


Card* Card::create(float width, float height, std::string title) {
	Card* card = new (std::nothrow) Card();
	if (card && card->init(width, height, title)) {

		card->autorelease();
		return card;
	}

	CC_SAFE_DELETE(card);
	return nullptr;
}

bool Card::init(float width, float height, std::string title) {
	this->initWithFile(PATH_IMAGE_UI + "bg_white.png", Rect(0, 0, width, height));
	auto cwidth = _contentSize.width - 40;
	auto cheight = _contentSize.height - 120;
	_bgMain = Sprite::create(PATH_IMAGE_UI + "bg_grey.png", Rect(0, 0, cwidth, cheight));
	_bgMain->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_bgMain->setPosition(Vec2(20, 20));
	this->addChild(_bgMain);

	if (!title.empty()) {
		_title = Label::createWithTTF(title, DEFAULT_FONT, 48);
		_title->setTextColor(Color4B::BLACK);
		_title->setPosition(Vec2(_contentSize.width / 2, _contentSize.height - 54));
		this->addChild(_title);
	}

	initBorder();
	return true;
}

void Card::initBorder()
{
	auto borderTop = Utility::createBorderHorizontal(_contentSize.width);
	borderTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	borderTop->setPosition(Vec2(0, _contentSize.height));
	this->addChild(borderTop);

	auto borderBot = Utility::createBorderHorizontal(_contentSize.width);
	borderBot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	borderBot->setPosition(Vec2(0, 0));
	this->addChild(borderBot);

	auto borderLeft = Utility::createBorderVertical(_contentSize.height);
	borderLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	borderLeft->setPosition(Vec2(0, 0));
	this->addChild(borderLeft);

	auto borderRight = Utility::createBorderVertical(_contentSize.height);
	borderRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	borderRight->setPosition(Vec2(_contentSize.width, 0));
	this->addChild(borderRight);

	auto cornerTopLeft = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerTopLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cornerTopLeft->setPosition(Vec2(0, _contentSize.height));
	this->addChild(cornerTopLeft);

	auto cornerTopRight = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerTopRight->setFlippedX(true);
	cornerTopRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	cornerTopRight->setPosition(Vec2(_contentSize.width, _contentSize.height));
	this->addChild(cornerTopRight);

	auto cornerBotLeft = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerBotLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	cornerBotLeft->setFlippedY(true);
	this->addChild(cornerBotLeft);

	auto cornerBotRight = Sprite::create(PATH_IMAGE_UI + "corner_border.png");
	cornerBotRight->setFlippedX(true);
	cornerBotRight->setFlippedY(true);
	cornerBotRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	cornerBotRight->setPosition(Vec2(_contentSize.width, 0));
	this->addChild(cornerBotRight);
}
