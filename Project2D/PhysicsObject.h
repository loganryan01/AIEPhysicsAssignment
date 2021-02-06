#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	PhysicsObject() {}

public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

protected:
	ShapeType m_shapeID;
};

