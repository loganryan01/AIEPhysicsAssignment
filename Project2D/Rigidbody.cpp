#include "Rigidbody.h"
#include <iostream>
#include "PhysicsScene.h"

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ANGULAR_THRESHOLD 0.01f

Rigidbody::Rigidbody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, 
	float a_orientation, float a_mass, float a_angularVelocity) : 
	PhysicsObject(a_shapeID), m_position(a_position), m_velocity(a_velocity), 
	m_orientation(a_orientation), m_mass(a_mass), m_angularVelocity(a_angularVelocity)
{
	// Range for drag is 0 to 1
	m_linearDrag = 0.3f;
	m_angularDrag = 0.3f;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	m_velocity -= m_velocity * m_linearDrag * a_timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * a_timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (glm::abs(m_angularVelocity) < MIN_LINEAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
	
	m_position += m_velocity * a_timeStep;
	ApplyForce(a_gravity * m_mass * a_timeStep, glm::vec2(0));

	m_orientation += m_angularVelocity * a_timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 a_force, glm::vec2 a_position)
{
	m_velocity += a_force / GetMass();
	m_angularVelocity += (a_force.y * a_position.x - a_force.x * a_position.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* a_actor2, glm::vec2 a_contact, 
	glm::vec2* a_collisionNormal, float a_pen)
{
	// Find the vector between their centres, or use the provided direction 
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(a_collisionNormal ? *a_collisionNormal :
	a_actor2->m_position - m_position);
	// Get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// Determine the total velocity of the contact points for the two objects,
	// for both linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(a_contact - m_position, -perp);
	float r2 = glm::dot(a_contact - a_actor2->m_position, perp);
	// Velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// Velocity of contact point from actor2
	float v2 = glm::dot(a_actor2->m_velocity, normal) + 
		r2 * a_actor2->m_angularVelocity;

	if (v1 > v2) // They're moving closer together
	{
		// Calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / a_actor2->m_mass + (r2 * r2) / a_actor2->m_moment);

		float elasticity = (GetElasticity() + a_actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		ApplyForce(-force, a_contact - m_position);
		a_actor2->ApplyForce(force, a_contact - a_actor2->m_position);

		if (a_pen > 0)
		{
			PhysicsScene::ApplyContactForces(this, a_actor2, normal, a_pen);
		}
	}
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + 
		m_moment * m_angularVelocity * m_angularVelocity);
}
