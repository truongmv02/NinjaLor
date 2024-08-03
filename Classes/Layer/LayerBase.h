#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "Definitions/Definitions.h"
#include "Lib/Lib.h" 


USING_NS_CC;
using namespace ui;

class LayerBase : public Layer
{
public:

	bool init();
	bool init(std::string layerName);
	Button* createButton(std::string icon, int tag, Widget::ccWidgetTouchCallback = nullptr);
	void initTitleInfo(std::string title);
	void setButtonActive();
	virtual	void clear() {}
	CC_SYNTHESIZE(ScrollView*, _btnContainer, ButtonContainer);
	CC_SYNTHESIZE(ScrollView*, _infoContainer, InfoContainer);
	CC_SYNTHESIZE(ScrollView*, _mainContainer, MainContainer);
	CC_SYNTHESIZE(Button*, _closeBtn, CloseButton);
	CC_SYNTHESIZE(Label*, _titleInfo, TitleInfo);
	CREATE_FUNC(LayerBase);
protected:
	Sprite* _bgMain;
	Sprite* _bgInfo;
};
