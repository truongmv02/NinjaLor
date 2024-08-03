#include "ShopLayer.h"
#include "Manager/GameManager.h"
#include "Layer/InventoryLayer.h";
#include "Layer/ShopLayer.h";
#include <regex>

bool ShopLayer::init(ShopType type)
{
	if (!LayerBase::init("Shop")) return false;

	_type = type;
	Vector<Node*> btns;
	auto invenBtn = createButton(PATH_IMAGE_UI + "bag.png", 0, CC_CALLBACK_2(ShopLayer::changeTabCallback, this));
	btns.pushBack(invenBtn);
	auto shopBtn = createButton(PATH_IMAGE_UI + "shop.png", 1, CC_CALLBACK_2(ShopLayer::changeTabCallback, this));
	btns.pushBack(shopBtn);
	auto size = Utility::alignItemsVerticallyWithPadding(btns, 3, _btnContainer->getContentSize());
	_btnContainer->setInnerContainerSize(size);

	this->setTag(1);
	setButtonActive();

	_im = ItemManager::create();
	_im->retain();

	initData();

	return true;
}

void ShopLayer::initData()
{
	_mainContainer->removeAllChildren();
	_infoContainer->removeAllChildren();
	createGrid();
	_items = _im->findItemShop(_type);

	Vector<Node*> nodes;
	auto tag = 0;
	for (auto item : _items) {
		auto btn = item->createButton(_grid, CC_CALLBACK_2(ShopLayer::touchItemCallback, this), tag++);
		nodes.pushBack(btn);
	}
	Utility::alignItemsWidthColumn(nodes, 7, _grid->getContentSize(), -3, -3);

	auto  mainSprite = _bgMain->getChildByName("Container");
	mainSprite->removeChildByName("Coin");
	int coin = GameManager::getInstance()->getCharacter()->getCoin();

	_labelCoin = Label::createWithTTF("Coin: " + std::to_string(coin), PATH_FONT + "Mitr-Bold.ttf", 36);
	_labelCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_labelCoin->setName("Coin");
	_labelCoin->setPosition(mainSprite->getContentSize().width / 2, mainSprite->getContentSize().height * 0.01);
	mainSprite->addChild(_labelCoin);
}

void ShopLayer::clear()
{
	for (auto item : _items) {
		item->clear();
		CC_SAFE_DELETE(item);
	}
	CC_SAFE_DELETE(_im);
}


void ShopLayer::changeTabCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Button* btn = (Button*)sender;
		auto tag = btn->getTag();
		if (tag == this->getTag())return;
		Layer* layer;
		if (tag == 0) {
			layer = InventoryLayer::create(InventoryLayer::SHOP, (int)_type);
		}
		this->clear();
		this->removeFromParent();
		GameManager::getInstance()->getHUDLayer()->addChild(layer, 10);
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ShopLayer::buyItem(Item* item, int quantity)
{

	auto cha = GameManager::getInstance()->getCharacter();

	if (cha->getCoin() < item->getPriceBuy() * quantity) {
		auto dialog = Dialog::create("I don't have e enough money");
		this->addChild(dialog, 10);
		return;
	}

	auto newItem = _im->createItem(item->getTemplateId(), quantity);
	Item* itemInBag = nullptr;
	if (newItem->getType() == Item::HP_POTION || newItem->getType() == Item::MP_POTION) {
		initInfo();
		itemInBag = Item::findItem(cha->getItemBag(), newItem->getId());
		if (itemInBag) {
			itemInBag->setQuantity(newItem->getQuantity() + itemInBag->getQuantity());
			newItem->clear();
			CC_SAFE_DELETE(newItem);
		}
	}
	if (!itemInBag) {
		if (!cha->addItemToBag(newItem)) {
			_im->removeItem(newItem->getId());
			newItem->clear();
			CC_SAFE_DELETE(newItem);
			return;
		}
		itemInBag = newItem;
	}
	auto quantityString = quantity > 1 ? std::to_string(quantity) + " " : "";
	auto dialog = Dialog::create("You got " + quantityString + itemInBag->getName());
	this->addChild(dialog, 10);
	auto coin = item->getPriceBuy() * quantity;
	cha->setCoin(cha->getCoin() - coin);

	CharacterManager::create()->updateCoin(cha->getCoin());
	_labelCoin->setString("Coin: " + std::to_string(cha->getCoin()));
	Label* label = nullptr;
	int qty = 0;

	if (itemInBag->getType() == Item::HP_POTION) {
		qty = cha->getItemCount(Item::HP_POTION);
		label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getHpButton()->getChildByName("Quantity");
	}
	else if (itemInBag->getType() == Item::MP_POTION) {
		qty = cha->getItemCount(Item::MP_POTION);
		label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getMpButton()->getChildByName("Quantity");
	}
	if (label) {
		label->setString(std::to_string(qty));
	}
}

void ShopLayer::createGrid()
{
	_grid = Utility::createGird(6, 7, 100, 100);
	_grid->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_grid->setPosition(Vec2(0, _mainContainer->getContentSize().height));
	_mainContainer->setInnerContainerSize(_grid->getContentSize());
	_mainContainer->addChild(_grid);
}

void ShopLayer::initInfo()
{
	_infoContainer->removeAllChildren();
	_bgInfo->removeChildByName("BuyButton");

	auto item = _items.at(_btnSelected->getTag());
	initTitleInfo(item->getName());
	auto containerSize = _infoContainer->getContentSize();

	Vector<Node*> nodes;

	nodes.pushBack(item->createLabelInfo(_infoContainer, containerSize.width * 0.9));
	auto labelPriceSell = Label::createWithTTF("Price: " + std::to_string(item->getPriceBuy()), DEFAULT_FONT, 30);
	labelPriceSell->setColor(TEXT_YELLOW);
	_infoContainer->addChild(labelPriceSell);
	nodes.pushBack(labelPriceSell);

	auto size = Utility::alignItemsVerticallyWithPadding(nodes, containerSize.height * 0.02, containerSize, 0, containerSize.width * 0.05);
	_infoContainer->setInnerContainerSize(size);

	initItemOptions();

}

void ShopLayer::initItemOptions()
{
	auto contentSize = _infoContainer->getContentSize();

	auto buyBtn = Utility::createButton("Buy", 0.8);
	buyBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::buyItemCallback, this));
	buyBtn->setName("BuyButton");
	buyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	buyBtn->setPosition(Vec2(contentSize.width / 2, _contentSize.height * 0.02));
	_bgInfo->addChild(buyBtn);
}

void ShopLayer::buyItemCallback(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto item = _items.at(_btnSelected->getTag());
		auto cha = GameManager::getInstance()->getCharacter();

		if (item->getType() == Item::HP_POTION || item->getType() == Item::MP_POTION) {
			_bgInfo->removeChildByName("BuyButton");
			auto cardInput = CardInput::create("Quantity", 800, 400);
			auto notiLabel = Label::createWithTTF("Input quantity you want to buy:", DEFAULT_FONT, 36);
			auto inputContainer = cardInput->getContainer();
			auto size = inputContainer->getContentSize();
			notiLabel->setDimensions(size.width * 0.8, notiLabel->getContentSize().height);
			notiLabel->setOverflow(Label::Overflow::RESIZE_HEIGHT);
			notiLabel->setHorizontalAlignment(TextHAlignment::CENTER);
			notiLabel->setPosition(Vec2(size.width / 2, size.height * 0.6));
			inputContainer->addChild(notiLabel);
			cardInput->initButtonConfirm("Buy", [&, cardInput, item](Ref* sender1, Widget::TouchEventType type1) {
				switch (type1)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					break;
				case cocos2d::ui::Widget::TouchEventType::MOVED:
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
				{
					auto valueString = cardInput->getValue();
					cardInput->removeFromParent();
					bool isValidNum = false;
					int value = 0;
					if (std::regex_match(valueString, std::regex("^[0-9]+$"))) {
						value = std::atoi(valueString.c_str());
						isValidNum = true;
					};
					if (isValidNum) {
						buyItem(item, value);
					}
					else {
						auto dialog = Dialog::create("Invalid quantity");
						this->addChild(dialog);
					}
				}
				break;
				case cocos2d::ui::Widget::TouchEventType::CANCELED:
					break;
				default:
					break;
				}
				});
			this->addChild(cardInput);
		}
		else {
			buyItem(item);
		}
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ShopLayer::touchItemCallback(Ref* sender, Widget::TouchEventType type)
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
