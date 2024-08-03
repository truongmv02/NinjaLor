#pragma once

#include "cocos2d.h"
#include "AttackSkill.h"

USING_NS_CC;

class RangeSkill : public AttackSkill
{
public:
	bool init();
	virtual void update(float dt) override;
	virtual bool use(LifeEntity* target) override;
	virtual void initEffect() override;
	virtual void createEffect() override;
	CREATE_FUNC_RETAIN(RangeSkill);
private:
};