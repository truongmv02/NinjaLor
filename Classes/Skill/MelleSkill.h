#pragma once

#include "cocos2d.h"
#include "AttackSkill.h"

USING_NS_CC;

class MelleSkill : public AttackSkill
{
public:
	bool init();
	virtual void update(float dt) override;
	virtual bool use(LifeEntity* target) override;
	virtual void initEffect() override;
	virtual void createEffect() override;
	void createSkillEffect(std::string name, std::string* skillFrames, std::string* charFrames, int frameCount, float delay, int loop = 1);
	CREATE_FUNC_RETAIN(MelleSkill);
private:
};