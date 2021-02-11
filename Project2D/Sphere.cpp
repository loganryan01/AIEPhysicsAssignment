#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 a_position, glm::vec2 a_velocity, float a_orientation, 
	float a_mass, float a_angularVelocity, float a_radius, glm::vec4 a_colour) :
	Rigidbody(SPHERE, a_position, a_velocity, a_orientation, a_mass, a_angularVelocity)
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_moment = (2.0f / 3.0f) * m_mass * m_radius * m_radius;
}

Sphere::~Sphere()
{
}

void Sphere::Draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * 
		m_radius;
	
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}
