#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, 
	glm::vec4 a_colour) :
	Rigidbody(SPHERE, a_position, a_velocity, 0, a_mass)
{
	m_radius = a_radius;
	m_colour = a_colour;
}

Sphere::~Sphere()
{
}

void Sphere::Draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
}
