#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "Definitions/Definitions.h"
class Character;

USING_NS_CC;
using namespace ui;

class CharacterStateBoard : public Sprite
{
public:
	bool init(Character* owner);
	void initHpBar();
	void initMpBar();
	void initExpBar();
	void updateHpBar();
	void updateMpBar();
	void updateExpBar();
	void update(float dt);
	CREATE_FUNC1(CharacterStateBoard, Character*);
private:

	Slider* _hpBar;
	Slider* _mpBar;
	Slider* _expBar;

	Label* _labelHp;
	Label* _labelMp;
	Label* _labelExp;

	Character* _owner;
};
