//#include "CharacterState.h"
//#include "Entity/Character/Character.h"
//CharacterJumpState::CharacterJumpState(Character* owner) :CharacterState(owner) {
//	_stateId = LifeEntity::EntityState::JUMP;
//	_jumpCount = 0; _jumpTime = 0;
//}
//
//CharacterState* CharacterJumpState::handleInput(InputHandler::Input input) {
//
//	return nullptr;
//}
//
//void CharacterJumpState::update(float dt) {
//	if (InputHandler::getInstance()->isKeyPress(InputHandler::JUMP)) {
//		_jumpTime += dt;
//	}
//	else {
//		_jumpTime = 0;
//	}
//	auto body = _owner->getBody();
//	auto v = body->GetLinearVelocity();
//
//	if (v.y <= 19) {
//		if (!_owner->getOnGround()) {
//			_owner->doAction(LifeEntity::EntityState::ROTATE);
//		}
//	}
//
//	CharacterState::update(dt);
//
//}
//
//void CharacterJumpState::enter() {
//	_owner->doAction(LifeEntity::EntityState::JUMP);
//}