//#include "CharacterState.h"
//#include "Entity/Character/Character.h"
//
//CharacterIdleState::CharacterIdleState(Character* owner) :CharacterState(owner) {
//	_stateId = LifeEntity::EntityState::IDLE;
//}
//
//CharacterState* CharacterIdleState::handleInput(InputHandler::Input input) {
//	if (input == InputHandler::JUMP) {
//		return new CharacterJumpState(_owner);
//	}
//	if (input == InputHandler::MOVE_LEFT || input == InputHandler::MOVE_RIGHT) {
//		return new CharacterRunState(_owner);
//	}
//	return nullptr;
//}
//
//void CharacterIdleState::enter() {
//	_owner->doAction(LifeEntity::EntityState::IDLE);
//}
//
