#include "PhysicsNode.h"
#include "Manager/GameManager.h"

PhysicsNode::PhysicsNode() {
	_bodyOffset = Vec2::ZERO;
	_body = nullptr;
	_fixture = nullptr;
	_anchorPoint = Vec2::ANCHOR_MIDDLE_BOTTOM;
}

bool PhysicsNode::init() {

	_anchorPoint = Vec2::ANCHOR_MIDDLE_BOTTOM;
	return Node::init();
}


void PhysicsNode::initBoxBody(float width, float height, Vec2 offset, float x, float y) {
	_bodySize = Size(width, height);
	if (offset.equals(Vec2::ZERO)) {
		offset.x = width / 2;
	}
	_bodyOffset = offset;
	auto world = GameManager::getInstance()->getWorld();
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2PolygonShape shape;

	bodyDef.position.Set(
		(x + _bodyOffset.x) / PTM_RATIO,
		(y + height / 2 + _bodyOffset.y) / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;
	_body = world->CreateBody(&bodyDef);

	shape.SetAsBox(width / 2 / PTM_RATIO, height / 2 / PTM_RATIO);
	fixtureDef.shape = &shape;
	_fixture = _body->CreateFixture(&fixtureDef);
	_body->SetUserData(this);
}

void PhysicsNode::initCircleBody(float radius, Vec2 offset, float x, float y) {
	_bodySize = Size(radius * 2, radius * 2);
	_bodyOffset = offset;
	auto world = GameManager::getInstance()->getWorld();
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2CircleShape shape;

	bodyDef.position.Set(
		(x + _bodyOffset.x) / PTM_RATIO,
		(y + radius + _bodyOffset.y) / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;
	_body = world->CreateBody(&bodyDef);

	shape.m_radius = radius / PTM_RATIO;
	fixtureDef.shape = &shape;
	_fixture = _body->CreateFixture(&fixtureDef);

	_body->SetUserData(this);

}

void PhysicsNode::initPolygonBody(b2Vec2* points, int count, Vec2 offset, float x, float y) {
	_bodyOffset = offset;
	auto world = GameManager::getInstance()->getWorld();
	b2BodyDef bodyDef;
	bodyDef.position.Set(
		(x + _bodyOffset.x) / PTM_RATIO,
		(y + _contentSize.height / 2 + _bodyOffset.y) / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;
	_body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.Set(points, count);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	_fixture = _body->CreateFixture(&fixtureDef);

	this->setUserData(this);
}


void PhysicsNode::setBodyType(uint16 type) {
	b2Filter filter = _fixture->GetFilterData();
	filter.categoryBits = type;
	_fixture->SetFilterData(filter);
}

void PhysicsNode::setBodyMask(uint16 mask) {
	b2Filter filter = _fixture->GetFilterData();
	filter.maskBits = mask;
	_fixture->SetFilterData(filter);
}

uint16 PhysicsNode::getBodyMask() {
	return _fixture->GetFilterData().maskBits;
}

uint16 PhysicsNode::getBodyType() {
	return _fixture->GetFilterData().categoryBits;
}

void PhysicsNode::setPosition(float x, float y) {
	Node::setPosition(x, y);
	setBodyPosition(x, y);
}

void PhysicsNode::setBodyPosition(float x, float y) {
	setBodyPosition(Vec2(x, y));
}


void PhysicsNode::setPosition(Vec2 position) {
	_position = position;
	setBodyPosition(position);
}

void PhysicsNode::setBodyPosition(Vec2 position) {
	if (_body)
		_body->SetTransform(b2Vec2(
			(position.x + _bodyOffset.x - _contentSize.width / 2) / PTM_RATIO,
			(position.y + _bodySize.height / 2 + _bodyOffset.y) / PTM_RATIO), 0);
}

void PhysicsNode::remove() {
	if (_body)
		removeBody();
	this->removeFromParent();
}

void PhysicsNode::removeBody() {
	if (_body) {
		GameManager::getInstance()->getWorld()->DestroyBody(_body);
		_body = nullptr;
	}
}

void PhysicsNode::autoRemoveBody()
{
	GameManager::getInstance()->pushDeleteBody(_body);
}