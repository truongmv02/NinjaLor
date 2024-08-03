#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"

USING_NS_CC;
class Quest;
class QuestLayer : public InfoLayer
{
public:
	bool init();
	void initData();
	void initQuestInfo();
	Label* createLabel(std::string data, int font = 30);
	void clear() override;
	CREATE_FUNC(QuestLayer);

protected:
	Quest* _quest;
};