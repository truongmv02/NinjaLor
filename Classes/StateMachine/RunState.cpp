//#include "CharacterState.h"
//#include "Entity/Character/Character.h"
//
//
//CharacterRunState::CharacterRunState(Character* owner) :CharacterState(owner) {
//	_stateId = LifeEntity::EntityState::MOVE;
//}
//
//CharacterState* CharacterRunState::handleInput(InputHandler::Input input) {
//	if (input == InputHandler::JUMP) {
//		return new CharacterJumpState(_owner);
//	}
//	return nullptr;
//}
//
//void CharacterRunState::enter() {
//	_owner->doAction(LifeEntity::EntityState::MOVE);
//}
