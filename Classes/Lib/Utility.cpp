#include "Utility.h"
#include "Entity/Character/Character.h"

Animation* Utility::createAnimation(std::string frameName, int frameCount, float delay, int loop) {

	Vector<SpriteFrame*> frameAni;
	for (int i = 1; i <= frameCount; i++) {
		char buffer[20] = { 0 };
		sprintf(buffer, "%d", i);
		std::string str = frameName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		if (frame != NULL)
			frameAni.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(frameAni, delay, loop);
	return ani;
}

Animation* Utility::createAnimation(std::string frameName, float delay, int loop) {

	Vector<SpriteFrame*> frameAni;
	for (int i = 1; ; i++) {
		char buffer[20] = { 0 };
		sprintf(buffer, "%d", i);
		std::string str = frameName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		if (frame == NULL) break;
		frameAni.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(frameAni, delay, loop);
	return ani;
}

Animation* Utility::createAnimation(std::string frameNames[], int frameCount, float delay, std::string prefix, int loop)
{
	Vector<SpriteFrame*> frameAni;
	for (int i = 0; i < frameCount; i++) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(prefix + frameNames[i]);
		if (frame != NULL)
			frameAni.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(frameAni, delay, loop);
	return ani;
}


Animate* Utility::getAnimate(std::string animateName, int frameCount, float delay, int loop) {
	auto animation = createAnimation(animateName, frameCount, delay, loop);
	return  Animate::create(animation);
}

Animate* Utility::getAnimate(std::string animateName, float delay, int loop) {
	auto animation = createAnimation(animateName, delay, loop);
	return  Animate::create(animation);
}

Animate* Utility::getAnimate(std::string animateName) {
	auto animation = AnimationCache::getInstance()->getAnimation(animateName);
	return Animate::create(animation);
}

Action* Utility::getAction(Animate* animate1, Animate* animate2, bool repeatForever) {
	auto action = Sequence::create(animate1, animate2, nullptr);
	if (repeatForever) {
		return RepeatForever::create(action);
	}
	return action;
}

void Utility::doActionAndRemove(Node* node, FiniteTimeAction* action)
{
	auto finishAction = CallFuncN::create([](Node* n) {
		n->removeFromParent();
		});
	node->runAction(Sequence::create(action, finishAction, nullptr));
}

Action* Utility::createActionReverse(Animate* animate, bool repeatForever) {
	auto animateReverse = animate->reverse();
	return getAction(animate, animateReverse, repeatForever);
}

void Utility::doAction(Node* node, std::string name, int tag) {
	if (node->getActionByTag(tag) != nullptr)return;
	auto ani = AnimationCache::getInstance()->getAnimation(name);
	if (ani == nullptr) return;
	auto action = Animate::create(ani);
	action->setTag(tag);
	node->stopAllActions();
	node->runAction(action);
}

void Utility::doAction(Node* node, std::string name, CallFunc* func, int tag) {
	if (node->getActionByTag(tag) != nullptr)return;
	auto ani = AnimationCache::getInstance()->getAnimation(name);
	auto action = Sequence::create(Animate::create(ani), func, nullptr);
	action->setTag(tag);
	node->stopAllActions();
	node->runAction(action);
}

Color4F Utility::color3BToColor4F(Color3B color, float alpha) {
	Color4F color4f;
	color4f.r = color.r / (float)255;
	color4f.g = color.g / (float)255;
	color4f.b = color.b / (float)255;
	color4f.a = alpha;
	return color4f;
}

Color4B Utility::color3BToColor4B(Color3B color, uint8_t alpha) {
	Color4B color4b;
	color4b.r = color.r;
	color4b.g = color.g;
	color4b.b = color.b;
	color4b.a = alpha;
	return color4b;
}


MenuItemImage* Utility::createItemImage(std::string normal, std::string selected, std::string disable, Size itemSize, Button::TextureResType type) {
	MenuItemImage* menuItem;
	switch (type)
	{
	case cocos2d::ui::Widget::TextureResType::LOCAL:
		menuItem = MenuItemImage::create(normal, selected, disable);
		break;
	case cocos2d::ui::Widget::TextureResType::PLIST: {
		menuItem = MenuItemImage::create();
		if (!normal.empty()) {
			menuItem->setNormalImage(Sprite::createWithSpriteFrameName(normal));
		}
		if (!selected.empty()) {
			menuItem->setSelectedImage(Sprite::createWithSpriteFrameName(selected));
		}
		if (!disable.empty()) {
			menuItem->setDisabledImage(Sprite::createWithSpriteFrameName(disable));
		}
		break;
	}
	default:
		break;
	}
	Node* images[3] = { menuItem->getNormalImage() , menuItem->getSelectedImage(),menuItem->getDisabledImage() };
	Size imageSize = Size::ZERO;
	for (int i = 0; i < 3; i++)
	{
		if (images[i] != nullptr) {
			imageSize = images[i]->getContentSize();
			break;
		}
	}
	menuItem->setContentSize(itemSize);
	if (itemSize.equals(Size::ZERO)) {
		menuItem->setContentSize(imageSize);
		itemSize = imageSize;
	}

	for (int i = 0; i < 3; i++)
	{
		if (images[i]) {
			images[i]->setPosition(itemSize / 2);
			images[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		}

	}
	return menuItem;
}

Label* Utility::addText(Node* node, std::string text, std::string font, float fontSize, Color3B textColor) {
	auto label = Label::createWithTTF(text, font, fontSize);
	label->setName("Text");
	label->setPosition(node->getContentSize() / 2);
	label->setTextColor(color3BToColor4B(textColor));
	node->addChild(label);
	return label;
}

Button* Utility::createButton(std::string title, float scale, std::string font, float fontSize, Color3B titleColor)
{
	auto button = Button::create(PATH_IMAGE_UI + "button.png", PATH_IMAGE_UI + "button_press.png");
	auto label = addText(button, title, font, fontSize, titleColor);
	button->setScale(scale);
	button->setSwallowTouches(true);
	auto btnSize = button->getContentSize();
	label->setDimensions(btnSize.width * 0.9, label->getContentSize().height);
	label->setLineHeight(label->getContentSize().height * 1.1);
	label->setOverflow(Label::Overflow::SHRINK);
	label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	return button;
}

MenuItemImage* Utility::createMenuItemSmall(std::string title, std::string font, float fontSize, Color3B titleColor) {
	auto menuItem = createItemImage(PATH_IMAGE_UI + "btn_small.png", PATH_IMAGE_UI + "btn_small_press.png");
	addText(menuItem, title, font, fontSize, titleColor);
	return menuItem;
}


MenuItemImage* Utility::createMenuItemLarget(std::string title, std::string font, float fontSize, Color3B titleColor) {
	auto menuItem = createItemImage(PATH_IMAGE_UI + "btn_larget.png", PATH_IMAGE_UI + "btn_larget_press.png");
	auto label = addText(menuItem, title, font, fontSize, titleColor);

	auto itemSize = menuItem->getContentSize();
	label->setDimensions(itemSize.width * 0.8, label->getContentSize().height);
	label->setLineHeight(label->getContentSize().height * 1.2);
	label->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	return menuItem;
}

Size Utility::getSizeVetical(Vector<Node*> nodes, float padding) {
	float width = 0;
	float height = -padding;
	for (auto item : nodes) {
		height += item->getContentSize().height + padding;
		if (item->getContentSize().width > width) {
			width = item->getContentSize().width;
		}
	}
	return Size(width, height);
}

Size Utility::getSizeHorizontal(Vector<Node*> nodes, float padding) {

	float width = -padding;
	float height = 0;
	for (auto item : nodes) {
		width += item->getContentSize().width + padding;
		if (item->getContentSize().height > height) {
			height = item->getContentSize().height;
		}
	}
	return Size(width, height);
}

Size Utility::alignItemsVerticallyWithPadding(Vector<Node*> nodes, float padding, Size size, float marginTop, float marginLeft) {
	if (nodes.size() == 0) return Size(0, 0);
	auto sizeNodes = getSizeVetical(nodes, padding);
	float height = sizeNodes.height > size.height ? sizeNodes.height : size.height;
	float  width = sizeNodes.width;

	float y = height - marginTop;

	for (const auto& node : nodes) {

		Size s = node->getContentSize();
		node->setPosition(s.width / 2 + marginLeft, y - s.height * node->getScaleY() / 2.0f);
		y -= s.height * node->getScaleY() + padding;
	}
	return Size(width, height);
}

Size Utility::alignItemsHorizontallyWithPadding(Vector<Node*> nodes, float padding) {
	if (nodes.size() == 0) return Size(0, 0);

	auto size = getSizeHorizontal(nodes, padding);
	float width = size.width;
	float height = size.height;

	float x = 0;

	for (const auto& node : nodes) {
		auto s = node->getContentSize();
		node->setPosition(x + s.width * node->getScaleX() / 2.0f, s.height / 2);
		x += s.width * node->getScaleX() + padding;
	}
	return Size(width, height);
}

Size Utility::alignItemsWidthRow(Vector<Node*> nodes, int row, float paddingX, float paddingY) {
	auto count = nodes.size();
	if (count == 0) return Size(0, 0);

	row = count < row ? count : row;
	auto itemSize = nodes.at(0)->getContentSize();

	auto col = count % row == 0 ? count / row : count / row + 1;

	float  width = itemSize.width * col + paddingX * (col - 1);
	float height = itemSize.height * row + paddingY * (row - 1);

	float x = 0, y = height;
	for (int i = 0; i < count; i++)
	{
		auto item = nodes.at(i);
		if ((i + 1) % row == 0) {
			y = height;
			x += itemSize.width * item->getScaleX() + paddingX;
		}
		item->setPosition(
			x + itemSize.width * item->getScaleX() / 2.0f,
			y - itemSize.height * item->getScaleY() / 2.0f);
		y -= itemSize.height * item->getScaleY() + paddingY;
	}

	return Size(width, height);
}

Size Utility::alignItemsWidthColumn(Vector<Node*> nodes, int column, Size size, float paddingLeft, float paddingTop, float marginLeft, float marginTop) {
	auto count = nodes.size();
	if (count == 0) return Size(0, 0);
	auto col = count < column ? count : column;
	auto itemSize = nodes.at(0)->getContentSize();

	auto row = count % column == 0 ? count / column : count / column + 1;

	float width = itemSize.width * col + paddingLeft * (col - 1);
	float  height = itemSize.height * row + paddingTop * (row - 1);

	height = height > size.height ? height : size.height;

	float x = marginLeft, y = height - marginTop;
	for (int i = 0; i < count; i++)
	{
		auto item = nodes.at(i);

		item->setPosition(
			x + itemSize.width * item->getScaleX() / 2.0f,
			y - itemSize.height * item->getScaleY() / 2.0f);
		x += itemSize.width * item->getScaleX() + paddingLeft;
		if ((i + 1) % column == 0) {
			x = 0;
			y -= itemSize.height * item->getScaleY() + paddingTop;
		}
	}
	return Size(width + marginLeft, height + marginTop);
}



DrawNode* Utility::createGird(int row, int col, float width, float height, Color4F color, float borderWidth) {
	auto grid = DrawNode::create(borderWidth);
	grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto a = (col + 1) / 2;
	auto size = Size(width * col, height * row) - Size((col - 1) * borderWidth, (row - 1) * borderWidth);
	grid->setContentSize(size);
	for (int i = 0; i <= row; i++)
	{
		auto h = i * (height - borderWidth) + borderWidth / 2;
		grid->drawLine(Vec2(borderWidth / 2, h),
			Vec2(size.width, h), color);
	}

	for (int i = 0; i <= col; i++)
	{
		auto w = i * (width - borderWidth) + borderWidth / 2;
		grid->drawLine(Vec2(w, borderWidth / 2),
			Vec2(w, size.height), color);
	}

	return grid;
}

void Utility::setMenu(Menu* feature, std::string featureName, std::string lableName, ccMenuCallback callBack) {
	auto item = (MenuItem*)feature->getChildByName(featureName);
	if (item == nullptr) return;
	item->setCallback(callBack);
	auto label = (Label*)item->getChildByName("Text");
	label->setString(lableName);
}



ScrollView* Utility::createScrollView(Size contenSize, ScrollView::Direction direction) {
	auto scrollView = ScrollView::create();

	scrollView->setDirection(direction);
	scrollView->setScrollBarEnabled(false);
	scrollView->setBounceEnabled(true);

	scrollView->setContentSize(contenSize);
	if (direction == ScrollView::Direction::HORIZONTAL) {
		scrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	}
	else if (direction == ScrollView::Direction::VERTICAL) {
		scrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	}

	return scrollView;
}

ListView* Utility::createListView(Size contenSize, ScrollView::Direction direction) {
	auto listView = ListView::create();
	listView->setDirection(direction);
	listView->setSwallowTouches(true);
	listView->setScrollBarEnabled(false);

	listView->setContentSize(contenSize);
	if (direction == ScrollView::Direction::HORIZONTAL) {
		listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	}
	else if (direction == ScrollView::Direction::VERTICAL) {
		listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	}

	return listView;
}

Slider* Utility::createProgressbar(std::string progress, std::string background)
{
	auto progressbar = Slider::create();
	progressbar->loadProgressBarTexture(progress);
	progressbar->loadBarTexture(background);
	progressbar->setPercent(80);
	progressbar->setEnabled(false);
	progressbar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	return progressbar;
}

Label* Utility::createLabelRegenerateHp(int hp, Node* owner)
{
	auto label = Label::createWithTTF("+" + std::to_string(hp), PATH_FONT + "Katibeh.ttf", 34);
	label->setTextColor(Color4B(172, 72, 74, 255));
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label->setPosition(owner->getPosition().x, owner->getPosition().y + owner->getContentSize().height);
	label->enableOutline(Color4B(99, 49, 50, 255), 2);
	auto action = MoveBy::create(0.7, Vec2(0, 120));
	doActionAndRemove(label, action);
	if (owner->getParent())
		owner->getParent()->addChild(label, 10);

	return label;
}

Label* Utility::createLabelRegenerateMp(int mp, Node* owner)
{
	auto label = Label::createWithTTF("+" + std::to_string(mp), PATH_FONT + "Katibeh.ttf", 34);
	label->setTextColor(Color4B(5, 253, 252, 255));
	label->enableOutline(Color4B(11, 98, 92, 255), 2);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label->setPosition(owner->getPosition().x, owner->getPosition().y + owner->getContentSize().height * 0.7);
	auto action = MoveBy::create(0.7, Vec2(0, 120));
	doActionAndRemove(label, action);
	if (owner->getParent())
		owner->getParent()->addChild(label, 10);

	return label;
}

Label* Utility::createLabelExp(int exp, Node* owner)
{
	auto label = Label::createWithTTF("+" + std::to_string(exp) + " exp", DEFAULT_FONT, 30);
	label->setTextColor(Color4B(0, 203, 0, 255));
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label->setPosition(owner->getPosition().x, owner->getPosition().y + owner->getContentSize().height * 0.75);
	label->enableOutline(Color4B(0, 82, 0, 255), 2);
	auto action = MoveBy::create(0.7, Vec2(0, 120));
	doActionAndRemove(label, action);
	if (owner->getParent())
		owner->getParent()->addChild(label, 10);
	return label;
}

Label* Utility::createLabelCoin(int coin, Node* owner)
{
	auto label = Label::createWithTTF("+" + std::to_string(coin), DEFAULT_FONT_BOLD, 36);
	label->setTextColor(Color4B::YELLOW);
	label->enableShadow(Color4B::ORANGE);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label->setPosition(owner->getPosition().x, owner->getPosition().y + owner->getContentSize().height * 0.9);
	auto action = MoveBy::create(0.7, Vec2(0, 120));
	doActionAndRemove(label, action);
	if (owner->getParent())
		owner->getParent()->addChild(label, 10);
	return label;
	return nullptr;
}

Label* Utility::createLabelDame(int dame, Node* owner, bool crit)
{
	int font = (dynamic_cast<Character*>(owner)) ? 36 : crit ? 72 : font = 48;

	auto label = Label::createWithTTF(std::to_string(-dame), PATH_FONT + "Pacifico.ttf", font);
	auto color = crit ? Color4B::ORANGE : Color4B(254, 0, 2, 255);
	auto border = Color4B(111, 0, 0, 255);
	label->setTextColor(color);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label->setPosition(owner->getPosition().x, owner->getPosition().y + owner->getContentSize().height * 0.8);
	label->enableOutline(border, 4);
	auto action = MoveBy::create(0.7, Vec2(0, 120));
	doActionAndRemove(label, action);
	if (owner->getParent()) {
		owner->getParent()->addChild(label, 10);
	}

	return label;
}

Action* Utility::createAnimationWaggle(float range, int loop)
{
	auto action = MoveBy::create(0.12, Vec2(range, 0));
	auto actionReverse = action->reverse();
	if (loop == -1) {
		return RepeatForever::create(Sequence::create(action, actionReverse, nullptr));
	}

	return Repeat::create(Sequence::create(action, actionReverse, nullptr), loop);
}

Vec2 Utility::reduceFraction(Vec2 fraction)
{
	int a, b;
	a = abs(fraction.x);
	b = abs(fraction.y);
	while (a != b) {
		if (a > b) {
			a = a - b;
		}
		else {
			b = b - a;
		}
	}
	fraction.x /= a;
	fraction.y /= a;
	return fraction;
}

Sprite* Utility::createBorderHorizontal(float length)
{
	auto border = Sprite::create(PATH_IMAGE_UI + "border_horizontal.png");
	border->setScaleX(length / border->getContentSize().width);
	return border;
}

Sprite* Utility::createBorderVertical(float length)
{
	auto border = Sprite::create(PATH_IMAGE_UI + "border_vertical.png");
	border->setScaleY(length / border->getContentSize().height);
	return border;
}

float Utility::convertToPercentValue(int value)
{
	return value / 10000.0f;
}

std::string Utility::percentString(int value)
{
	if (value == 0) return "0%";
	std::vector<int> valueVector;
	std::string valueString = value < 0 ? "-" : "";
	value = abs(value);
	while (value > 0)
	{
		valueVector.push_back(value % 10);
		value = value / 10;
	}
	auto count = valueVector.size();
	if (count > 2) {
		for (int i = count - 1; i > 0; i--)
		{
			if (i >= 2) {
				valueString += std::to_string(valueVector.at(i));
				continue;
			}

			auto val1 = valueVector.at(1);
			auto val2 = valueVector.at(0);
			if (val1 != 0 && val2 != 0) {
				valueString += "." + std::to_string(val1) + std::to_string(val2);
			}
			else if (val1 == 0 && val2 != 0) {
				valueString += ".0" + std::to_string(val2);
			}
			else if (val1 != 0 && val2 == 0) {
				valueString += "." + std::to_string(val1);
			}
		}
	}
	else if (count == 2) {
		valueString += "0";
		auto val1 = valueVector.at(1);
		auto val2 = valueVector.at(0);
		if (val1 != 0 && val2 != 0) {
			valueString += "." + std::to_string(val1) + std::to_string(val2);
		}
		else if (val1 == 0 && val2 != 0) {
			valueString += ".0" + std::to_string(val2);
		}
		else if (val1 != 0 && val2 == 0) {
			valueString += "." + std::to_string(val1);
		}
	}
	else {
		valueString += "0.0" + std::to_string(valueVector.at(0));
	}
	return valueString + "%";
}


/*
10.000 -> 100%
1.000 -> 10%
100 -> 1%
10 -> 0.1%
1 -> 0.01%
*/