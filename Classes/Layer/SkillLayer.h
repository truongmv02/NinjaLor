#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"
#include "Database/SkillManager.h"
#include "Skill/Skills.h"

USING_NS_CC;
class Character;
class SkillLayer : public InfoLayer
{
public:
	SkillLayer() { _sm = nullptr; _btnSelected = -1; }
	bool init();
	void initData();
	void initInfo();
	void clear() override;
	void clearInfo();
	void initButton();
	CREATE_FUNC(SkillLayer);

protected:
	Vector<Node*> createLabels(Skill* skill, Skill* nextLevel = nullptr, bool isCurrentLevel = true);
	void touchSkillCallback(Ref* sender, Widget::TouchEventType type);
	void touchAddPointButtonCallback(Ref* sender, Widget::TouchEventType type);
	void touchSetKeyButtonCallback(Ref* sender, Widget::TouchEventType type);

	int _btnSelected;
	SkillManager* _sm;
	CardInput* _cardInput;
	Character* _character;

};
