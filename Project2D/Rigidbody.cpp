#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_orientation, float a_mass) : 
	PhysicsObject(a_shapeID), m_position(a_position), m_velocity(a_velocity), m_orientation(a_orientation), m_mass(a_mass)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	m_position += m_velocity * a_timeStep;
	ApplyForce(a_gravity * m_mass * a_timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 a_force)
{
	m_velocity += a_force / GetMass();
}

void Rigidbody::ApplyForceToActor(Rigidbody* a_actor2, glm::vec2 a_force)
{
	ApplyForce(-a_force);
	a_actor2->ApplyForce(a_force);
}

void Rigidbody::ResolveCollision(Rigidbody* a_actor2)
{
	glm::vec2 normal = glm::normalize(a_actor2->GetPosition() - m_position);
	glm::vec2 relativeVelocity = a_actor2->GetVelocity() - m_velocity;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		((1 / m_mass) + (1 / a_actor2->GetMass()));

	glm::vec2 force = normal * j;

	ApplyForceToActor(a_actor2, -force);
}
