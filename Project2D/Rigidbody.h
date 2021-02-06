#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType a_shapeID, glm::vec2 a_position,
		glm::vec2 a_velocity, float a_orientation, float a_mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	void ApplyForce(glm::vec2 a_force);
	void ApplyForceToActor(Rigidbody* a_actor2, glm::vec2 a_force);

	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; //2D so we only need a single float to represent our orientation
};
