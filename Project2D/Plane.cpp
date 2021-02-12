#include "Plane.h"
#include "PhysicsScene.h"
#include <Gizmos.h>

Plane::Plane(glm::vec2 a_normal, float a_distance, glm::vec4 a_color) : PhysicsObject(PLANE)
{
	m_normal = glm::normalize(a_normal);
	m_distanceToOrigin = a_distance;
	m_color = a_color;
	m_isKinematic = true;
	m_elasticity = 1;
}

Plane::Plane() : PhysicsObject(PLANE)
{
	m_normal = glm::normalize(glm::vec2(0, 1));
	m_distanceToOrigin = 0;
	m_color = glm::vec4(1, 1, 1, 1);
	m_isKinematic = true;
	m_elasticity = 1;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300.f;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colorFade = m_color;
	colorFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	/*aie::Gizmos::add2DLine(start, end, GetColor());*/
	aie::Gizmos::add2DTri(start, end, start - GetNormal() * 10.f, GetColor(), GetColor(),
		colorFade);
	aie::Gizmos::add2DTri(end, end - GetNormal() * 10.f, start - GetNormal() * 10.f,
		GetColor(), colorFade, colorFade);
}

void Plane::ResolveCollision(Rigidbody* a_otherActor, glm::vec2 a_contact)
{
	// The position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = a_contact - a_otherActor->GetPosition();

	// The plane isn't moving, so the relative velocity is just actor2's velocity
	// at the contact point
	glm::vec2 vRel = a_otherActor->GetVelocity() + a_otherActor->GetAngularVelocity() *
		glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, GetNormal());

	// Perfectly elasticity collisions for now
	float e = (GetElasticity() + a_otherActor->GetElasticity());

	// This is the perpendicular distance we apply the force at relative to the COM, so 
	// Torque = F*r
	float r = glm::dot(localContact, glm::vec2(GetNormal().y, -GetNormal().x));

	// Work out the "effective mass" - this is a combination of moment of inertia and mass,
	// and tells us how much the contact point velocity will change with the force we're
	// applying.
	float mass0 = 1.f / (1.f / a_otherActor->GetMass() + (r * r) / a_otherActor->GetMoment());

	// The plane does not move (Static) so we only use the other actor's velocity
	float j = -(1.f + e) * velocityIntoPlane * mass0;

	glm::vec2 force = GetNormal() * j;

	a_otherActor->ApplyForce(force, a_contact - a_otherActor->GetPosition());

	if (a_otherActor->m_collisionCallback)
	{
		a_otherActor->m_collisionCallback(this);
	}

	float pen = glm::dot(a_contact, GetNormal()) - GetDistance();
	PhysicsScene::ApplyContactForces(a_otherActor, nullptr, GetNormal(), pen);
}