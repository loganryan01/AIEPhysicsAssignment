#pragma once
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 a_position, glm::vec2 a_velocity,
		float a_orientation, float a_mass, float a_angularVelocity, float a_radius, glm::vec4 a_colour);
	~Sphere();

	virtual void Draw();

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};

