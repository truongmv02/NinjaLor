#include "NPC.h"

void NPC::setName(std::string name)
{
	_name = name;
	_sprite = Sprite::create(PATH_IMAGE_NPC + name + "_idle1.png");
	this->setContentSize(_sprite->getContentSize());
	_sprite->setPosition(_contentSize / 2);

	auto labelName = Label::createWithTTF(name, DEFAULT_FONT, 36);
	labelName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	labelName->setPosition(_contentSize.width / 2, _contentSize.height + 20);
	labelName->setTextColor(Color4B::YELLOW);
	labelName->enableOutline(Color4B::RED, 2);
	this->addChild(labelName);

	initBoxBody(_contentSize.width, _contentSize.height, Vec2(_contentSize.width / 2, _contentSize.height * 0.005));
	setBodyType(TYPE_NPC);
	setBodyMask(TYPE_WALL);

	this->addChild(_sprite);
	Vector<SpriteFrame*> aniFrames;
	aniFrames.pushBack(SpriteFrame::create(PATH_IMAGE_NPC + name + "_idle1.png",
		Rect(0, 0, _sprite->getContentSize().width, _sprite->getContentSize().height)));
	aniFrames.pushBack(SpriteFrame::create(PATH_IMAGE_NPC + name + "_idle2.png",
		Rect(0, 0, _sprite->getContentSize().width, _sprite->getContentSize().height)));

	Animation* ani = Animation::createWithSpriteFrames(aniFrames, 0.2f, -1);
	_sprite->runAction(Animate::create(ani));
}

