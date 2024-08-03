#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"

USING_NS_CC;

class SettingLayer : public InfoLayer
{
public:
	bool init();
	void clear() override;
	CREATE_FUNC(SettingLayer);

protected:
};
