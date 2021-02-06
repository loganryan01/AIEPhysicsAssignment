#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 a_normal, float a_distance);
	~Plane();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timestep);
	virtual void Draw();
	virtual void ResetPosition();

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;
};

