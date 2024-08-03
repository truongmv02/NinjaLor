#pragma once
#include "NPC.h"
#include "Lib/Lib.h"

class Seller :public NPC {

public:
	bool init();
	void onClick() override;
	void openShopCallback(Ref* sender, Widget::TouchEventType type);
	CREATE_FUNC(Seller);
protected:
	CardMenu* _menu;
};