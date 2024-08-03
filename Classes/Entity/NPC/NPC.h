#pragma once

#include "Entity/Entity.h"

class NPC :public Entity {

public:
	bool init() { return true; }
	virtual void setName(std::string name);
	virtual void onClick() {};
	CREATE_FUNC(NPC);
};