#pragma once
#include "cocos2d.h"
#include "InfoLayer.h"
#include "Entity/Item/Item.h"

USING_NS_CC;

class EquipmentLayer : public InfoLayer
{
public:
	EquipmentLayer() { _btnSelected = nullptr; }
	bool init();
	void initData();
	void initStatInfo();
	void initEquipment();
	CREATE_FUNC(EquipmentLayer);
protected:
	void createGrid();
	void touchItemCallback(Ref* sender, Widget::TouchEventType type);
	void initInfo();
	void initItemOptions();

	Vector<Item*> _items;
	Button* _btnSelected;
	Node* _grid;
	ScrollView* _statContainer;
};
