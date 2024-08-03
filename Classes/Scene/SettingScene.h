#pragma once
#include "cocos2d.h"
USING_NS_CC;

class SettingScene :public Scene
{
public:
	bool init();
	void goToMainMenuScene();
	CREATE_FUNC(SettingScene);
};
