//#include "CharacterState.h"
//#include "Entity/Character/Character.h"
//
//CharacterAttackState::CharacterAttackState(Character* owner) :CharacterState(owner) {
//	_stateId = LifeEntity::EntityState::ATTACK;
//}
//
//void CharacterAttackState::enter() {
//	_owner->doAction(LifeEntity::EntityState::ATTACK);
//}
//
//void CharacterAttackState::update(float dt)
//{
//
//	if (((Character*)_owner)->getOnGround()) {
//		auto v = _owner->getBody()->GetLinearVelocity();
//		v.x = 0;
//		_owner->getBody()->SetLinearVelocity(v);
//	}
//}
