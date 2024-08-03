#pragma once

#include "cocos2d.h"
#include "Lib/InputHandler.h"
#include "LifeEntityState.h"


USING_NS_CC;
class Character;
class CharacterState :public LifeEntityState {
public:
	CharacterState(Character* owner);
	virtual LifeEntityState* handleInput(InputHandler::Input input) { return nullptr; }
	virtual void update(float dt);
	virtual void enter();
	virtual int getStateID() { return _stateId; }
protected:
};


class CharacterIdleState : public CharacterState
{
public:
	CharacterIdleState(Character* owner);
	virtual LifeEntityState* handleInput(InputHandler::Input input) override;
private:
};


class CharacterRunState : public CharacterState
{
public:
	CharacterRunState(Character* owner);
	virtual LifeEntityState* handleInput(InputHandler::Input input) override;

private:
};


class CharacterJumpState : public CharacterState
{
public:
	CharacterJumpState(Character* owner);
	//virtual LifeEntityState* handleInput(InputHandler::Input input) override;
	virtual void update(float dt) override;
	CC_SYNTHESIZE(int, _jumpCount, JumpCount);
	CC_SYNTHESIZE(float, _jumpTime, JumpTime);

private:
};


class CharacterFallState : public CharacterState
{
public:
	CharacterFallState(Character* owner);
private:
};

class CharacterAttackState : public CharacterState
{
public:
	CharacterAttackState(Character* owner);
	virtual void update(float dt) override;
private:
};


class CharacterDeadState : public CharacterState
{
public:
	CharacterDeadState(Character* owner);

private:
};
