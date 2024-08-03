#pragma once

#include "cocos2d.h"
#include "Definitions/Definitions.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

using namespace ui;

class Utility {
public:
	// Animation
	static Animation* createAnimation(std::string frameName, int frameCount, float delay, int loop = 1);
	static Animation* createAnimation(std::string frameName, float delay, int loop = 1);
	static Animation* createAnimation(std::string frameNames[], int frameCount, float delay, std::string prefix = "", int loop = 1);
	static Animate* getAnimate(std::string animateName, int frameCount, float delay, int loop = 1);
	static Animate* getAnimate(std::string animateName, float delay, int loop = 1);
	static Animate* getAnimate(std::string animateName);
	static Action* createActionReverse(Animate* animate, bool repeatForever = true);
	static Action* getAction(Animate* animate1, Animate* animate2, bool repeatForever = true);
	static void doActionAndRemove(Node* node, FiniteTimeAction* action);
	static void doAction(Node* node, std::string actionName, int tag = 0);
	static void doAction(Node* node, std::string actionName, CallFunc* func, int tag = 0);

	// Color
	static Color4F color3BToColor4F(Color3B color, float alpha = 1.0f);
	static Color4B color3BToColor4B(Color3B color, uint8_t alpha = 255);

	//Menu
	static void setMenu(Menu* feature, std::string featureName, std::string lableName = "", ccMenuCallback callBack = nullptr);
	static MenuItemImage* createMenuItemSmall(std::string title, std::string font = DEFAULT_FONT, float fontSize = 48, Color3B titleColor = TEXT_YELLOW);
	static MenuItemImage* createMenuItemLarget(std::string title, std::string font = DEFAULT_FONT, float fontSize = 48, Color3B titleColor = TEXT_YELLOW);
	static Size getSizeVetical(Vector<Node*> nodes, float padding);
	static Size getSizeHorizontal(Vector<Node*> nodes, float padding);
	static MenuItemImage* createItemImage(std::string normal = "", std::string selected = "", std::string disable = "", Size itemSize = Size::ZERO, Button::TextureResType type = Button::TextureResType::LOCAL);
	static Label* addText(Node* node, std::string text, std::string font = DEFAULT_FONT, float fontSize = 48, Color3B textColor = TEXT_YELLOW);

	static Button* createButton(std::string title, float scale = 1.0f, std::string font = DEFAULT_FONT, float fontSize = 48, Color3B titleColor = Color3B::WHITE);
	// Align Item0
	static Size alignItemsVerticallyWithPadding(Vector<Node*> nodes, float padding = 0, Size size = Size::ZERO, float marginTop = 0.0f, float marginLeft = 0.0f);
	static Size alignItemsHorizontallyWithPadding(Vector<Node*> nodes, float padding = 0);
	static Size alignItemsWidthRow(Vector<Node*> nodes, int row, float paddingX = 0, float paddingY = 0);
	static Size alignItemsWidthColumn(Vector<Node*> nodes, int column, Size size = Size::ZERO, float paddingX = 0, float paddingY = 0, float marginLeft = 0, float magrinTop = 0);

	// Grid
	static DrawNode* createGird(int row, int col, float width, float height,
		Color4F color = color3BToColor4F(BORDER_BROWN), float borderWidth = 3);

	//View
	static ScrollView* createScrollView(Size contentSize, ScrollView::Direction direction = ScrollView::Direction::HORIZONTAL);
	static ListView* createListView(Size contentSize, ScrollView::Direction direction = ScrollView::Direction::HORIZONTAL);

	static Slider* createProgressbar(std::string progress, std::string background);

	static Label* createLabelRegenerateHp(int hp, Node* owner);
	static Label* createLabelRegenerateMp(int mp, Node* owner);
	static Label* createLabelExp(int exp, Node* owner);
	static Label* createLabelCoin(int coin, Node* owner);
	static Label* createLabelDame(int dame, Node* owner, bool crit = false);

	static Action* createAnimationWaggle(float range, int loop = -1);

	static Vec2 reduceFraction(Vec2 fraction);

	static Sprite* createBorderHorizontal(float length);
	static Sprite* createBorderVertical(float length);

	static float convertToPercentValue(int value);
	static std::string percentString(int value);

};