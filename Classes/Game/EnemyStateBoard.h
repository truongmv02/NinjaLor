#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "Definitions/Definitions.h"

class Enemy;

USING_NS_CC;
using namespace ui;

class EnemyStateBoard : public Sprite
{
public:
	bool init();
	void initHpBar();
	void update(float dt);
	CREATE_FUNC(EnemyStateBoard);
private:
	Label* _labelInfo;
	Slider* _hpBar;
	Label* _labelHp;
};
