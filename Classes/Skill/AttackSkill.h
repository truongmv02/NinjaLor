#pragma once

#include "cocos2d.h"
#include "Skill.h"

USING_NS_CC;

class AttackSkill : public Skill
{
public:
	bool init();
	virtual void update(float dt) override;
	virtual bool use(LifeEntity* target) override;
	bool attack(LifeEntity* target);
	virtual void initEffect() override;
	virtual void createEffect() override;
	CREATE_FUNC_RETAIN(AttackSkill);
private:
};