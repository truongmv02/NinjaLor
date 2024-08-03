#include "EnemyStateBoard.h"
#include "Manager/GameManager.h"
#include "Lib/Utility.h"
#include "Entity/Entities.h"

bool EnemyStateBoard::init() {
	this->setTexture(PATH_IMAGE_UI + "state_board_small.png");
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->setPosition(Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.99));
	initHpBar();
	_labelInfo = Label::createWithTTF("Blind zombie Lv20", DEFAULT_FONT, 30);
	_labelInfo->enableBold();
	_labelInfo->setTextColor(Utility::color3BToColor4B(TEXT_BLUE));
	_labelInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_labelInfo->setPosition(Vec2(_contentSize.width / 2, _contentSize.height * 0.85));
	this->addChild(_labelInfo);

	this->scheduleUpdate();
	return true;
}

void EnemyStateBoard::update(float dt) {
	auto enemy = GameManager::getInstance()->getEnemyFocus();
	if (enemy == nullptr) {
		this->setVisible(false);
		return;
	}
	this->setVisible(true);
	_labelInfo->setString(enemy->getName() + " Lv" + std::to_string(enemy->getLevel()));

	auto hp = enemy->getStat("Hp")->getValue();
	auto hpMax = enemy->getStat("HpMax")->getCurrentValue();
	auto percent = hp / (float)hpMax * 100;
	_hpBar->setPercent(percent);
	_labelHp->setString(std::to_string(hp) + "/" + std::to_string(hpMax));

}

void EnemyStateBoard::initHpBar() {

	_hpBar = Utility::createProgressbar(PATH_IMAGE_UI + "hp_progress.png", PATH_IMAGE_UI + "hp_bg.png");
	_hpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_hpBar->setPosition(Vec2(_contentSize.width / 2, _contentSize.height * 0.15));
	this->addChild(_hpBar);

	_labelHp = Label::createWithTTF("1000/2000", DEFAULT_FONT, 24);

	_labelHp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_labelHp->setPosition(_hpBar->getContentSize() / 2);
	_hpBar->addChild(_labelHp);
}



