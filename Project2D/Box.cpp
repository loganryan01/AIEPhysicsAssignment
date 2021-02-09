#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_width,
	float a_height, glm::vec4 a_colour) :
	Rigidbody(BOX, a_position, a_velocity, 0, a_mass, 0, 1.0f / 12.0f * a_mass * a_width * a_height), 
	m_colour(a_colour), m_extents(glm::vec2(a_width, a_height)), m_width(a_width), 
	m_height(a_height)
{
}

Box::~Box()
{
}

void Box::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	Rigidbody::FixedUpdate(a_gravity, a_timeStep);

	// Store the local axes
	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::Draw()
{
	// Draw using local axes
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

bool Box::CheckBoxCorners(const Box& a_box, glm::vec2 a_contact, int& a_numContacts,
	float& a_pen, glm::vec2& a_edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = a_box.GetExtents().x * 2;
	float boxH = a_box.GetExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	// Loop over all corners of the other box
	for (float x = -a_box.GetExtents().x; x < boxW; x += boxW)
	{
		for (float y = -a_box.GetExtents().y; y < boxH; y += boxH)
		{
			// Get the position in the worldspace
			glm::vec2 p = a_box.GetPosition() + x * a_box.m_localX + y * a_box.m_localY;
			// Get the position in our box's square
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));

			// Update the extents in each cardinal direction in our box's space
			// (ie extents along the seperating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// If this corner is inside the box, add it to the list of contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact = p0;
			}
			first = false;
		}
	}

	// if we lie entirely to one side of the box along one axis, we've found a separating
	// axis, and we can exit
	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	a_contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
		(float)numLocalContacts;
	a_numContacts++;

	// Find the minimum penetration vector as a penetration amount and normal
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localX;
		a_pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = -m_localX;
		a_pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localY;
		a_pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = -m_localY;
		a_pen = pen0;
		res = true;
	}
	
	return res;
}