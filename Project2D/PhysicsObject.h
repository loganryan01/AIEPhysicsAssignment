#pragma once
#include <glm/glm.hpp>

class PhysicsObject
{
protected:
	PhysicsObject() {}

public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};
};

