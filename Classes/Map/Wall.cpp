#include "Wall.h"
#include "Entity/Character/Character.h"


Wall* Wall::create(float width, float height, int type) {
	Wall* wall = new (std::nothrow) Wall();
	if (wall && wall->init(width, height, type)) {

		wall->autorelease();
		return wall;
	}
	CC_SAFE_DELETE(wall);
	return nullptr;
}

bool Wall::init(float width, float height, int type) {
	_name = "Wall";
	_type = type;
	this->setContentSize(Size(width, height));
	initBoxBody(width, height);
	_body->SetType(b2_staticBody);
	setBodyType(TYPE_WALL);
	setBodyMask(TYPE_ENEMY | TYPE_HERO | TYPE_NPC | TYPE_ITEM | TYPE_COIN);
	return true;
}



void Wall::PreSolve(Entity* entity, b2Contact* contact, const b2Manifold* manifold) {
	auto entitySize = entity->getBodySize();
	auto entityPos = entity->getPosition();
	auto bodyOffset = entity->getBodyOffset();

	if (_type == 1) {
		float otherX, x, otherY, y;
		otherY = entityPos.y + bodyOffset.y;
		y = _contentSize.height + _position.y;
		if (entity->getDir().x < 0) {
			otherX = entityPos.x - bodyOffset.x;
			x = _position.x + _bodyOffset.x;
			if (x + 0.5f > otherX && otherX >= _position.x && otherY < y) {
				auto v = entity->getBody()->GetLinearVelocity();
				v.x = 0;
				entity->getBody()->SetLinearVelocity(v);
			}
		}
		else {
			otherX = entityPos.x + bodyOffset.x;
			x = _position.x - _bodyOffset.x;
			if (otherX + 0.5f > x && otherX <= _position.x && otherY < y) {
				auto v = entity->getBody()->GetLinearVelocity();
				v.x = 0;
				entity->getBody()->SetLinearVelocity(v);
			}
		}
	}
	else if (_type == 2) {
		float y = _position.y + _contentSize.height;
		float otherY = entityPos.y + bodyOffset.y;
		if (otherY < y)
			contact->SetEnabled(false);
	}

	float y = _position.y + _contentSize.height;
	float otherY = entityPos.y + bodyOffset.y;
	auto character = dynamic_cast<Character*> (entity);
	if (character) {
		if (otherY >= y && otherY - y < 1.0f)
			character->setOnGround(true);
	}
}

