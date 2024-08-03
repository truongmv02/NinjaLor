#include "CharacterState.h"
#include "Entity/Entities.h"

CharacterState::CharacterState(Character* owner) :LifeEntityState(owner)
{
}

void CharacterState::update(float dt) {
	Character* owner = (Character*)_owner;
	auto body = _owner->getBody();
	auto v = body->GetLinearVelocity();
	if (owner->getStateMachine()->getCurrentState()->getStateID() == LifeEntity::EntityState::ATTACK) return;
	if (owner->getStateMachine()->getCurrentState()->getStateID() == LifeEntity::EntityState::DEAD) return;

	if (v.y < 0) {
		if (!owner->_head->getActionByTag(LifeEntity::EntityState::ROTATE))
			owner->getStateMachine()->changeState(new CharacterFallState(owner));
	}
	else if (owner->getOnGround() && abs(v.x) < 0.01f) {
		owner->getStateMachine()->changeState(new CharacterIdleState(owner));
	}
	else if (owner->getOnGround() && abs(v.x) > 0.5f) {
		owner->getStateMachine()->changeState(new CharacterRunState(owner));
	}
}

void CharacterState::enter()
{
	_owner->doAction((LifeEntity::EntityState)_stateId);
}

//ATTACK
void CharacterAttackState::update(float dt)
{

	if (((Character*)_owner)->getOnGround()) {
		auto v = _owner->getBody()->GetLinearVelocity();
		v.x = 0;
		_owner->getBody()->SetLinearVelocity(v);
	}
}

CharacterAttackState::CharacterAttackState(Character* owner) :CharacterState(owner) {
	_stateId = LifeEntity::EntityState::ATTACK;
}


// IDLE
CharacterIdleState::CharacterIdleState(Character* owner) :CharacterState(owner) {
	_stateId = LifeEntity::EntityState::IDLE;
}

LifeEntityState* CharacterIdleState::handleInput(InputHandler::Input input) {
	if (input == InputHandler::JUMP) {
		return new CharacterJumpState((Character*)_owner);
	}
	if (input == InputHandler::MOVE_LEFT || input == InputHandler::MOVE_RIGHT) {
		return new CharacterRunState((Character*)_owner);
	}
	return nullptr;
}



//RUN
CharacterRunState::CharacterRunState(Character* owner) :CharacterState(owner) {
	_stateId = LifeEntity::EntityState::MOVE;
}

LifeEntityState* CharacterRunState::handleInput(InputHandler::Input input) {
	if (input == InputHandler::JUMP) {
		return new CharacterJumpState((Character*)_owner);
	}
	return nullptr;
}


// JUMP
CharacterJumpState::CharacterJumpState(Character* owner) :CharacterState(owner) {
	_stateId = LifeEntity::EntityState::JUMP;
	_jumpCount = 0; _jumpTime = 0;
}

void CharacterJumpState::update(float dt) {
	if (InputHandler::getInstance()->isKeyPress(InputHandler::JUMP)) {
		_jumpTime += dt;
	}
	else {
		_jumpTime = 0;
	}
	auto body = _owner->getBody();
	auto v = body->GetLinearVelocity();

	if (v.y <= 19) {
		if (!((Character*)_owner)->getOnGround()) {
			_owner->doAction(LifeEntity::EntityState::ROTATE);
		}
	}

	CharacterState::update(dt);

}


// FALL
CharacterFallState::CharacterFallState(Character* owner) :CharacterState(owner) {
	_stateId = LifeEntity::EntityState::FALL;
}

CharacterDeadState::CharacterDeadState(Character* owner) :CharacterState(owner)
{
	_stateId = LifeEntity::EntityState::DEAD;
}
