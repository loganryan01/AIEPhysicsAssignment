#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID), m_elasticity(1.0f) {}

public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }
	float GetElasticity() { return m_elasticity; }

	void SetElasticity(float a_elasticity) { m_elasticity = a_elasticity; }

protected:
	ShapeType m_shapeID;
	float m_elasticity;
};

