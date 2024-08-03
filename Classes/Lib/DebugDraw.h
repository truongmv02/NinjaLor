#pragma once

#include "Box2D/Box2D.h"
#include "cocos2d.h"

class CcxBox2dDebugDraw : public b2Draw
{
public:
	CcxBox2dDebugDraw();
	CcxBox2dDebugDraw(float32 ratio);
	virtual ~CcxBox2dDebugDraw();

	void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

	void setRatio(float32 ratio) { _ratio = ratio; }

	cocos2d::DrawNode* GetDrawNode() const { return _drawNode; }

protected:
	float32 _ratio;

	cocos2d::DrawNode* _drawNode = nullptr;
};