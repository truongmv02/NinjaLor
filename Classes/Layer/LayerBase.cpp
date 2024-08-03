#include "LayerBase.h"
#include "Lib/Utility.h"
#include "Manager/GameManager.h"

bool LayerBase::init() {
	if (!Layer::init()) return false;
	auto bg = Sprite::create(PATH_IMAGE_UI + "bg_white.png");
	bg->setPosition(VISIBLE_SIZE / 2);
	bg->setScaleX(VISIBLE_SIZE_WIDTH / bg->getContentSize().width);
	this->addChild(bg);

	_bgMain = Sprite::create(PATH_IMAGE_UI + "bg_main.png");
	auto mainSprite = Sprite::create(PATH_IMAGE_UI + "main_container.png");
	mainSprite->setName("Container");
	_bgInfo = Sprite::create(PATH_IMAGE_UI + "bg_info.png");

	_bgMain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_bgInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	mainSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	auto posX = VISIBLE_SIZE_WIDTH / 2 - (_bgMain->getContentSize().width + VISIBLE_SIZE_WIDTH * 0.01 + _bgInfo->getContentSize().width) / 2;
	auto posY = VISIBLE_SIZE_HEIGHT / 2;

	_bgMain->setPosition(Vec2(posX, posY));
	mainSprite->setPosition(
		Vec2(_bgMain->getContentSize().width - 22,
			_bgMain->getContentSize().height - 93));

	_bgInfo->setPosition(_bgMain->getContentSize().width + VISIBLE_SIZE_WIDTH * 0.01, _bgMain->getContentSize().height / 2);

	this->addChild(_bgMain);
	_bgMain->addChild(mainSprite);
	_bgMain->addChild(_bgInfo);

	_closeBtn = Button::create(PATH_IMAGE_UI + "close_btn.png", PATH_IMAGE_UI + "close_btn_press.png");
	_closeBtn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeBtn->setPosition(Vec2(bg->getContentSize().width * 0.98, bg->getContentSize().height - bg->getContentSize().width * 0.02));
	this->addChild(_closeBtn, 1);

	_closeBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto controlLayer = GameManager::getInstance()->getHUDLayer()->getControlLayer();
			controlLayer->setVisible(true);
			controlLayer->resume();
			clear();
			this->removeFromParent();
			GameManager::getInstance()->resume();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
		});

	_btnContainer = Utility::createScrollView(Size(120, mainSprite->getContentSize().height), ScrollView::Direction::VERTICAL);
	_btnContainer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_btnContainer->setPosition(Vec2(_bgMain->getContentSize().width * 0.011, _bgMain->getContentSize().height - 93));
	_bgMain->addChild(_btnContainer);
	_btnContainer->setBounceEnabled(false);

	_mainContainer = Utility::createScrollView(mainSprite->getContentSize(), ScrollView::Direction::VERTICAL);
	_mainContainer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_mainContainer->setPosition(Size(0, mainSprite->getContentSize().height));
	mainSprite->addChild(_mainContainer);

	_infoContainer = Utility::createScrollView(Size(_bgInfo->getContentSize().width, _bgInfo->getContentSize().height - 93 - _bgInfo->getContentSize().height * 0.05), ScrollView::Direction::VERTICAL);
	_infoContainer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_infoContainer->setPosition(Vec2(0, _bgInfo->getContentSize().height - 93 - _bgInfo->getContentSize().height * 0.02));
	_bgInfo->addChild(_infoContainer);

	GameManager::getInstance()->pause();

	return true;
}

bool LayerBase::init(std::string layerName) {

	if (!LayerBase::init()) return false;
	_name = layerName;
	_titleInfo = Label::createWithTTF("", PATH_FONT + "Mitr-Medium.ttf", 48);
	_bgInfo->addChild(_titleInfo);
	auto labelTitle = Label::createWithTTF(layerName, PATH_FONT + "Mitr-Bold.ttf", 48);
	labelTitle->setPosition(Vec2(_mainContainer->getContentSize().width / 2, _mainContainer->getContentSize().height + 93.0f / 2));
	_bgMain->getChildByName("Container")->addChild(labelTitle);

	return true;
}

Button* LayerBase::createButton(std::string icon, int tag, Widget::ccWidgetTouchCallback callback)
{
	Button* btn = Button::create(PATH_IMAGE_UI + "btn_bg.png");
	btn->addTouchEventListener(callback);
	btn->ignoreContentAdaptWithSize(false);
	btn->setZoomScale(0.0f);
	btn->setTag(tag);
	_btnContainer->addChild(btn);
	auto i = Sprite::create(icon);
	i->setName("icon");
	i->setPosition(btn->getContentSize() / 2);
	btn->addChild(i);
	return btn;
}

void LayerBase::initTitleInfo(std::string title)
{
	_titleInfo->setString(title);
	_titleInfo->setPosition(Vec2(_bgInfo->getContentSize().width / 2, _bgInfo->getContentSize().height - 93 / 2.0f));

}

void LayerBase::setButtonActive()
{
	auto btn = _btnContainer->getChildByTag(this->getTag());
	btn->getChildByName("icon")->runAction(Utility::createAnimationWaggle(10, 4));
	btn->setOpacity(0);
	((Button*)btn)->setEnabled(false);
}
