#include "CharacterStateBoard.h"
#include "Entity/Character/Character.h"

bool CharacterStateBoard::init(Character* owner) {
	this->setTexture(PATH_IMAGE_UI + "state_board.png");
	_owner = owner;
	this->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->setPosition(Vec2(VISIBLE_SIZE_WIDTH * 0.005, VISIBLE_SIZE_HEIGHT * 0.99));
	initHpBar();
	initMpBar();
	initExpBar();
	this->scheduleUpdate();
	return true;
}

void CharacterStateBoard::update(float dt) {
	updateHpBar();
	updateMpBar();
	updateExpBar();
}

void CharacterStateBoard::initHpBar() {

	auto hpText = Sprite::create(PATH_IMAGE_UI + "hp_text.png");
	hpText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	hpText->setPosition(_contentSize.width * 0.06, _contentSize.height * 0.87);
	this->addChild(hpText);

	_hpBar = Utility::createProgressbar(PATH_IMAGE_UI + "hp_progress.png", PATH_IMAGE_UI + "hp_bg.png");
	_hpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_hpBar->setPosition(Vec2(Sprite::create(PATH_IMAGE_UI + "mp_text.png")->getContentSize().width + VISIBLE_SIZE_WIDTH * 0.007, hpText->getContentSize().height / 2));
	hpText->addChild(_hpBar);

	_labelHp = Label::createWithTTF("1000/2000", DEFAULT_FONT, 24);

	_labelHp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_labelHp->setPosition(_hpBar->getContentSize() / 2);
	_hpBar->addChild(_labelHp);
}

void CharacterStateBoard::initMpBar() {
	auto mpText = Sprite::create(PATH_IMAGE_UI + "mp_text.png");
	mpText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	mpText->setPosition(_contentSize.width * 0.06, _contentSize.height * 0.6);
	this->addChild(mpText);

	_mpBar = Utility::createProgressbar(PATH_IMAGE_UI + "mp_progress.png", PATH_IMAGE_UI + "mp_bg.png");
	_mpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_mpBar->setPosition(Vec2(mpText->getContentSize().width + VISIBLE_SIZE_WIDTH * 0.007, mpText->getContentSize().height / 2));
	mpText->addChild(_mpBar);

	_labelMp = Label::createWithTTF("1000/2000", DEFAULT_FONT, 24);

	_labelMp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_labelMp->setPosition(_mpBar->getContentSize() / 2);
	_mpBar->addChild(_labelMp);
}

void CharacterStateBoard::initExpBar()
{
	_expBar = Utility::createProgressbar(PATH_IMAGE_UI + "exp_progress.png", PATH_IMAGE_UI + "exp_bg.png");
	_expBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_expBar->setPosition(Vec2(_contentSize.width * 0.06, _contentSize.height * 0.07));
	this->addChild(_expBar);

	_labelExp = Label::createWithTTF("Level 30 + 20.5%", DEFAULT_FONT, 36);
	_labelExp->setScale(0.75);
	_labelExp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_labelExp->setPosition(Vec2(0, _expBar->getContentSize().height + _contentSize.height * 0.005));
	_expBar->addChild(_labelExp);
}


void CharacterStateBoard::updateHpBar() {
	auto hp = _owner->getStat("Hp")->getValue();
	auto hpMax = _owner->getStat("HpMax")->getCurrentValue();
	auto percent = hp / (float)hpMax * 100;
	_hpBar->setPercent(percent);
	_labelHp->setString(std::to_string(hp) + "/" + std::to_string(hpMax));
}

void CharacterStateBoard::updateMpBar() {
	auto mp = _owner->getStat("Mp")->getValue();
	auto mpMax = _owner->getStat("MpMax")->getCurrentValue();
	auto percent = mp / (float)mpMax * 100;
	_mpBar->setPercent(percent);
	_labelMp->setString(std::to_string(mp) + "/" + std::to_string(mpMax));
}

void CharacterStateBoard::updateExpBar()
{
	auto expMax = _owner->getExpMax();
	auto exp = _owner->getCurrentExp();
	float percent = (exp / (float)expMax);
	auto lableString = "Level. " + std::to_string(_owner->getLevel()) + " + " + Utility::percentString(percent * 10000);
	_labelExp->setString(lableString);
	_expBar->setPercent(percent * 100);
}

