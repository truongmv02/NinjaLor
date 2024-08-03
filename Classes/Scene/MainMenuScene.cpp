#include "MainMenuScene.h"
#include "GameScene.h"
#include "Lib/Utility.h"
#include "Definitions/Definitions.h"
#include "Entity/Entities.h"
#include "Database/CharacterManager.h"
#include "Database/DatabaseManager.h"
#include "Manager/GameManager.h"

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}
	auto backgound = Sprite::create(PATH_IMAGE_UI + "background.png");
	backgound->setPosition(VISIBLE_SIZE / 2);
	backgound->setScaleX(VISIBLE_SIZE_WIDTH / backgound->getContentSize().width);
	this->addChild(backgound);
	auto logo = Sprite::create(PATH_IMAGE_UI + "logo.png");
	logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	logo->setPosition(Vec2(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.92));
	this->addChild(logo);

	auto menuItem1 = createMenuItem("Resume");
	auto menuItem2 = createMenuItem("New Game");
	auto menuItem3 = createMenuItem("Exit");

	menuItem1->setCallback(CC_CALLBACK_0(MainMenuScene::goToGameScene, this));
	menuItem2->setCallback(CC_CALLBACK_0(MainMenuScene::newGame, this));

	menuItem3->setCallback([](Ref* sender) {
		Director::getInstance()->end();
		});
	auto pos = CharacterManager::create()->getCharacterPosition();

	Menu* menu;
	if (pos.equals(Vec2(-1, -1))) {
		menu = Menu::create(menuItem2, menuItem3, nullptr);
	}
	else {
		menu = Menu::create(menuItem1, menuItem2, menuItem3, nullptr);
	}

	menu->alignItemsVerticallyWithPadding(20);
	menu->setPosition(VISIBLE_SIZE_WIDTH / 2, VISIBLE_SIZE_HEIGHT * 0.35);
	this->addChild(menu);
	return true;
}

MenuItemImage* MainMenuScene::createMenuItem(std::string title)
{
	auto menuItem = MenuItemImage::create(PATH_IMAGE_UI + "menu_button.png",
		PATH_IMAGE_UI + "menu_button_press.png");
	auto label = Label::createWithTTF(title, DEFAULT_FONT, 48);
	label->setPosition(menuItem->getContentSize() / 2);
	menuItem->addChild(label);
	return menuItem;
}

void MainMenuScene::goToGameScene() {
	auto map = CharacterManager::create()->getMap();
	auto scene = GameScene::create(map, 0, false);
	Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::newGame()
{
	DatabaseManager::getInstance()->reset();
	GameManager::getInstance()->reset();
	goToGameScene();
}
