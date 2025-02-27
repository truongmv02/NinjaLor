﻿#include "DebugDraw.h"
#include "cocos2d.h"

USING_NS_CC;

CcxBox2dDebugDraw::CcxBox2dDebugDraw()
	: CcxBox2dDebugDraw(1.0f)
{
}

CcxBox2dDebugDraw::CcxBox2dDebugDraw(float32 ratio)
	: _ratio(ratio)
{
	_drawNode = DrawNode::create();
	_drawNode->retain();
	_drawNode->setGlobalZOrder(10000);
}

CcxBox2dDebugDraw::~CcxBox2dDebugDraw()
{
	_drawNode->removeFromParent();
	CC_SAFE_RELEASE(_drawNode);
}

void CcxBox2dDebugDraw::DrawPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
	const auto vertices = new Vec2[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i] = Vec2((old_vertices[i].x * _ratio), (old_vertices[i].y * _ratio));
	}
	_drawNode->drawPoly(vertices, vertexCount, true, Color4F(color.r, color.g, color.b, color.a));
	delete[] vertices;
}

void CcxBox2dDebugDraw::DrawSolidPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
	const auto vertices = new Vec2[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i] = Vec2((old_vertices[i].x * _ratio), (old_vertices[i].y * _ratio));
	}
	_drawNode->drawSolidPoly(vertices, vertexCount, Color4F(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 0.2f));
	_drawNode->drawPoly(vertices, vertexCount, true, Color4F(color.r, color.g, color.b, 1));
	delete[] vertices;
}

void CcxBox2dDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const auto pos = Vec2((center.x * _ratio), (center.y * _ratio));
	_drawNode->drawCircle(pos, radius * _ratio, 0, 16, false, Color4F(color.r, color.g, color.b, 1));
}

void CcxBox2dDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const auto pos = Vec2((center.x * _ratio), (center.y * _ratio));
	// Draw filled circle
	_drawNode->drawSolidCircle(pos, radius * _ratio, 0, 16, Color4F(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 0.2f));
	// Draw edge of circle
	_drawNode->drawCircle(pos, radius * _ratio, 0, 16, false, Color4F(color.r, color.g, color.b, 1));

	// Draw the axis line
	DrawSegment(center, center + radius * axis, color);
}

void CcxBox2dDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const auto pos1 = Vec2((p1.x * _ratio), (p1.y * _ratio));
	const auto pos2 = Vec2((p2.x * _ratio), (p2.y * _ratio));

	_drawNode->drawSegment(pos1, pos2, 1, Color4F(color.r, color.g, color.b, 1));
}

void CcxBox2dDebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}

void CcxBox2dDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	const auto pos1 = Vec2((p.x * _ratio), (p.y * _ratio));
	_drawNode->drawPoint(pos1, 1.f, Color4F(color.r, color.g, color.b, 1));
}
