//#pragma once
//
//#include "cocos2d.h"
//#include "LifeEntityState.h"
//
//USING_NS_CC;
//class Enemy;
//class EnemyState :public LifeEntityState {
//public:
//	EnemyState(Enemy* owner);
//	virtual void update(float dt);
//	virtual void enter() {}
//	virtual int getStateID() { return _stateId; }
//protected:
//	int _stateId;
//	Enemy* _owner;
//};
//
//
//class EnemyIdleState : public EnemyState
//{
//public:
//	EnemyIdleState(Enemy* owner);
//	virtual void enter() override;
//private:
//};
//
//
//class EnemyRunState : public EnemyState
//{
//public:
//	EnemyRunState(Enemy* owner);
//	virtual void enter() override;
//
//private:
//};
//
//class EnemyAttackState : public EnemyState
//{
//public:
//	EnemyAttackState(Enemy* owner);
//	void  enter() override;
//	virtual void update(float dt) override;
//private:
//};