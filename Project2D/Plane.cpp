#include "Plane.h"
#include <Gizmos.h>
#include <iostream>

Plane::Plane(glm::vec2 a_normal, float a_distance) 
	: PhysicsObject(PLANE), m_normal(a_normal), m_distanceToOrigin(a_distance)
{
	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 a_gravity, float a_timestep)
{
}

void Plane::Draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// Easy to rotate normal through 90 degress around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour)
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour,
		colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f,
		m_colour, colourFade, colourFade);
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(Rigidbody* a_actor2, glm::vec2 a_contact)
{
	// The position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = a_contact - a_actor2->GetPosition();
	
	// The plane isn't moving, so the relative velocity is just actor2's velocity
	// at the contact point
	glm::vec2 vRel = a_actor2->GetVelocity() + a_actor2->GetAngularVelocity() * 
		glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);

	// Perfectly elasticity collisions for now
	float e = (GetElasticity() + a_actor2->GetElasticity());

	// This is the perpendicular distance we apply the force at relative to the COM, so 
	// Torque = F*r
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	// Work out the "effective mass" - this is a combination of moment of inertia and mass,
	// and tells us how much the contact point velocity will change with the force we're
	// applying.
	float mass0 = 1.0f / (1.0f / a_actor2->GetMass() + (r * r) / a_actor2->GetMoment());

	float j = -(1 + e) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float kePre = a_actor2->GetKineticEnergy();

	a_actor2->ApplyForce(force, a_contact - a_actor2->GetPosition());

	float kePost = a_actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}
