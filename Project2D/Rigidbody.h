#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, 
		float a_orientation, float a_mass, float a_angularVelocity);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	void ApplyForce(glm::vec2 a_force, glm::vec2 a_position);

	void ResolveCollision(Rigidbody* a_actor2, glm::vec2 a_contact, glm::vec2*
	a_collisionNormal = nullptr);

	glm::vec2 GetPosition() const { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMoment() { return m_moment; }
	float GetKineticEnergy();

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;

	float m_orientation; //2D so we only need a single float to represent our orientation
	float m_angularVelocity;
	float m_moment;

	float m_linearDrag;
	float m_angularDrag;
};

