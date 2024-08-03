#pragma once
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "Definitions/Definitions.h"
#include "Game/CharacterStateBoard.h"
#include "Game/EnemyStateBoard.h"
#include "ControlLayer.h"

USING_NS_CC;
using namespace ui;
class Message;
class HUDLayer : public Layer
{
public:
	bool init();
	void addMessage(Message* message, int length = 0);
	CC_SYNTHESIZE(CharacterStateBoard*, _charStateBoard, CharacterStateBoard);
	CC_SYNTHESIZE(EnemyStateBoard*, _eStateBoard, EnemyStateBoard);
	CC_SYNTHESIZE(ControlLayer*, _controlLayer, ControlLayer);
	CREATE_FUNC_RETAIN(HUDLayer);
protected:
};
