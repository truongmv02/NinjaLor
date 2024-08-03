#pragma once
#include "cocos2d.h"
#include "Card.h"

class CardInput : public Layer
{
public:
	CardInput() { _closeCallback = nullptr;  _updateCallback = nullptr; }
	static CardInput* create(std::string title, float width = 1000.0f, float height = 500.0f);
	bool init(std::string title, float width = 1000.0f, float height = 500.0f);
	inline std::string getValue() { return _textField->getString(); }
	void setCardPosition(Vec2 position);
	void update(float dt) override;
	void initButtonConfirm(std::string name, Widget::ccWidgetTouchCallback callback);
	inline void setCloseCallback(std::function<void()> closeCallback) { _closeCallback = closeCallback; }
	inline void setUpdateCallback(std::function<void()> callback) { _updateCallback = callback; }
	CC_SYNTHESIZE(Button*, _closeBtn, CloseButton);
	CC_SYNTHESIZE(Button*, _confirmBtn, ConfirmButon);
	CC_SYNTHESIZE(Card*, _card, Container);
protected:
	void initTextField();
	Sprite* createTextFieldBackground(std::string path);
protected:
	float _textFieldWidth;

	Sprite* _textFieldBg;
	Sprite* _textFieldBgFocus;

	TextField* _textField;

	std::function<void()> _closeCallback;
	std::function<void()> _updateCallback;
};


