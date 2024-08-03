#pragma once
#include "cocos2d.h"
#include "LayerBase.h"

USING_NS_CC;

class InfoLayer : public LayerBase
{
public:
	bool init(std::string layerName);
	CREATE_FUNC1(InfoLayer, std::string);
protected:
	void changeTabCallback(Ref* sender, Widget::TouchEventType type);
};
