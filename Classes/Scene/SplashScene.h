#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class SplashScene : public Scene
{
public:
	bool init();
	void loadResources();
	void loadingCallBack(Texture2D* texture);
	void update(float dt);
	CREATE_FUNC(SplashScene);
private:
	int _total;
	int _count;
	Slider* sliderLoading;
};
