#pragma once
#include "NPC.h"

class Instructor :public NPC {

public:
	bool init();
	void setName(std::string name) override;
	void onClick() override;
	void showConversations(std::vector<std::string> conversations, std::function<void()> callback = nullptr);
	void update(float dt) override;
	CREATE_FUNC(Instructor);
private:
	Sprite* _effect;
};