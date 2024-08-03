#include "InventoryLayer.h"
#include "Manager/GameManager.h"
#include "Entity/Character/Character.h"

InventoryLayer* InventoryLayer::create(Type type, int shopType)
{
	auto inven = new (std::nothrow) InventoryLayer();
	if (inven && inven->init(type, shopType)) {
		inven->autorelease();
		return inven;
	}
	CC_SAFE_DELETE(inven);
	return nullptr;
}

bool InventoryLayer::init(Type type, int shopType)
{
	if (!InfoLayer::init("Inventory")) return false;
	auto  mainSprite = _bgMain->getChildByName("Container");
	_mainContainer->setContentSize(Size(_mainContainer->getContentSize().width, 590));
	_mainContainer->setPosition(Vec2(0, mainSprite->getContentSize().height));

	if (type == SHOP) {
		_btnContainer->removeAllChildren();
		Vector<Node*> btns;
		auto invenBtn = createButton(PATH_IMAGE_UI + "bag.png", 0);
		btns.pushBack(invenBtn);
		auto shopBtn = createButton(PATH_IMAGE_UI + "shop.png", 1, CC_CALLBACK_2(InventoryLayer::changeTabCallback, this));
		btns.pushBack(shopBtn);
		auto size = Utility::alignItemsVerticallyWithPadding(btns, 3, _btnContainer->getContentSize());
		_btnContainer->setInnerContainerSize(size);
	}

	this->setTag(0);
	setButtonActive();
	_type = type;
	_shopType = shopType;
	_im = ItemManager::create();
	_im->retain();
	initData();
	return true;
}

void InventoryLayer::initData(bool removeInfo)
{
	_mainContainer->removeAllChildren();
	if (removeInfo) {
		_infoContainer->removeAllChildren();
		_bgInfo->removeChildByName("UseButton");
		_bgInfo->removeChildByName("DropButton");
		_bgInfo->removeChildByName("SellButton");
		initTitleInfo("");
	}
	createGrid();
	_items = GameManager::getInstance()->getCharacter()->getItemBag();

	Vector<Node*> nodes;
	auto tag = 0;
	for (auto item : _items) {
		auto btn = item->createButton(_grid, CC_CALLBACK_2(InventoryLayer::touchItemCallback, this), tag++);
		nodes.pushBack(btn);
	}
	Utility::alignItemsWidthColumn(nodes, 7, _grid->getContentSize(), -3, -3);

	auto  mainSprite = _bgMain->getChildByName("Container");
	mainSprite->removeChildByName("Coin");
	int coin = GameManager::getInstance()->getCharacter()->getCoin();

	auto labelCoin = Label::createWithTTF("Coin: " + std::to_string(coin), PATH_FONT + "Mitr-Bold.ttf", 36);
	labelCoin->setName("Coin");
	labelCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	labelCoin->setPosition(mainSprite->getContentSize().width / 2, mainSprite->getContentSize().height * 0.01);
	mainSprite->addChild(labelCoin);
}

void InventoryLayer::clear()
{
	CC_SAFE_DELETE(_im);
}


void InventoryLayer::createGrid()
{
	_grid = Utility::createGird(6, 7, 100, 100);
	_grid->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_grid->setPosition(Vec2(0, _mainContainer->getContentSize().height));
	_mainContainer->setInnerContainerSize(_grid->getContentSize());
	_mainContainer->addChild(_grid);
}

void InventoryLayer::initInfo()
{
	_infoContainer->removeAllChildren();
	_bgInfo->removeChildByName("UseButton");
	_bgInfo->removeChildByName("DropButton");
	_bgInfo->removeChildByName("SellButton");

	auto item = _items.at(_btnSelected->getTag());
	initTitleInfo(item->getName());
	auto containerSize = _infoContainer->getContentSize();

	Vector<Node*> nodes;

	nodes.pushBack(item->createLabelInfo(_infoContainer, containerSize.width * 0.9));
	int price = item->getPriceBuy() * 0.25;
	auto labelPriceSell = Label::createWithTTF("Price: " + std::to_string(price), DEFAULT_FONT, 30);
	labelPriceSell->setColor(TEXT_YELLOW);
	_infoContainer->addChild(labelPriceSell);
	nodes.pushBack(labelPriceSell);

	auto size = Utility::alignItemsVerticallyWithPadding(nodes, containerSize.height * 0.02, containerSize, 0, containerSize.width * 0.05);
	_infoContainer->setInnerContainerSize(size);

	initItemOptions();

}

void InventoryLayer::initItemOptions()
{
	auto contentSize = _infoContainer->getContentSize();

	auto item = _items.at(_btnSelected->getTag());

	if (_type == SHOP) {
		auto sellBtn = Utility::createButton("Sell", 0.8);
		sellBtn->addTouchEventListener([&, item](Ref* sender, Widget::TouchEventType type) {
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			{
				Vector<Button*> btns;
				auto btnConfirm = Utility::createButton("Sell", 0.65);
				btns.pushBack(btnConfirm);
				int price = item->getPriceBuy() * 0.25 * item->getQuantity();
				auto quantityString = item->getQuantity() > 1 ? std::to_string(item->getQuantity()) + " " : "";
				auto dialog = Dialog::create("Do you want to sell " + quantityString + item->getName() + "?  Price: " + std::to_string(price), "", btns);
				this->addChild(dialog);
				btnConfirm->addTouchEventListener([&, item, dialog, price](Ref* sender1, Widget::TouchEventType type) {
					switch (type)
					{
					case cocos2d::ui::Widget::TouchEventType::BEGAN:
						break;
					case cocos2d::ui::Widget::TouchEventType::MOVED:
						break;
					case cocos2d::ui::Widget::TouchEventType::ENDED:
					{
						auto cha = GameManager::getInstance()->getCharacter();
						cha->setCoin(cha->getCoin() + price);
						CharacterManager::create()->updateCoin(cha->getCoin());
						auto itemType = item->getType();
						_im->removeItem(item->getId());
						cha->removeItemFromBag(item);
						dialog->removeFromParent();

						Label* label = nullptr;
						int qty = 0;
						if (itemType == Item::HP_POTION) {
							qty = cha->getItemCount(Item::HP_POTION);
							label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getHpButton()->getChildByName("Quantity");
						}
						else if (itemType == Item::MP_POTION) {
							qty = cha->getItemCount(Item::MP_POTION);
							label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getMpButton()->getChildByName("Quantity");
						}
						if (label) {
							label->setString(std::to_string(qty));
						}

						initData(true);
					}
					break;
					case cocos2d::ui::Widget::TouchEventType::CANCELED:
						break;
					default:
						break;
					}
					});
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
			});

		sellBtn->setName("SellButton");
		sellBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		sellBtn->setPosition(Vec2(contentSize.width / 2, _contentSize.height * 0.02));
		_bgInfo->addChild(sellBtn);
		return;
	}

	auto useBtn = Utility::createButton("Use", 0.8);
	useBtn->setName("UseButton");
	auto dropBtn = Utility::createButton("Drop Item", 0.8);
	dropBtn->setName("DropButton");

	useBtn->addTouchEventListener([&, item](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			if (GameManager::getInstance()->getCharacter()->useItem(item)) {
				_btnSelected = nullptr;
				_infoContainer->removeAllChildren();
				_bgInfo->removeChildByName("UseButton");
				_bgInfo->removeChildByName("DropButton");
				_bgInfo->removeChildByName("SellButton");
				initData();
			}
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});
	useBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	dropBtn->addTouchEventListener([&, item](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			Vector<Button*> btns;
			auto btnConfirm = Utility::createButton("Drop item", 0.65);
			btns.pushBack(btnConfirm);
			auto dialog = Dialog::create("Do you want to drop " + item->getName() + "?", "", btns);
			this->addChild(dialog);
			btnConfirm->addTouchEventListener([&, item, dialog](Ref* sender1, Widget::TouchEventType type) {
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					break;
				case cocos2d::ui::Widget::TouchEventType::MOVED:
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
				{
					dialog->removeFromParent();
					Label* label = nullptr;
					_im->removeItem(item->getId());
					auto itemType = item->getType();
					GameManager::getInstance()->getCharacter()->removeItemFromBag(item);
					_btnSelected = nullptr;
					int qty = 0;
					auto cha = GameManager::getInstance()->getCharacter();
					if (itemType == Item::HP_POTION) {
						qty = cha->getItemCount(Item::HP_POTION);
						label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getHpButton()->getChildByName("Quantity");
					}
					else if (itemType == Item::MP_POTION) {
						qty = cha->getItemCount(Item::MP_POTION);
						label = (Label*)GameManager::getInstance()->getHUDLayer()->getControlLayer()->getMpButton()->getChildByName("Quantity");
					}
					if (label) {
						label->setString(std::to_string(qty));
					}
					initData(true);
				}
				break;
				case cocos2d::ui::Widget::TouchEventType::CANCELED:
					break;
				default:
					break;
				}
				});
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});


	if (item->getType() != Item::HP_POTION && item->getType() != Item::MP_POTION) {
		_bgInfo->addChild(useBtn);
		useBtn->setPosition(Vec2(contentSize.width * 0.05, _contentSize.height * 0.02));
		dropBtn->setPosition(Vec2(contentSize.width * 0.95, _contentSize.height * 0.02));
		dropBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	}
	else {
		dropBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		dropBtn->setPosition(Vec2(contentSize.width / 2, _contentSize.height * 0.02));
	}

	_bgInfo->addChild(dropBtn);
}

void InventoryLayer::touchItemCallback(Ref* sender, Widget::TouchEventType type)
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
