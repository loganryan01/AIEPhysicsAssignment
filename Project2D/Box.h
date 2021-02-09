#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_width, 
		float a_height, glm::vec4 a_colour);
	~Box();

	void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	void Draw();

	bool CheckBoxCorners(const Box& a_box, glm::vec2 a_contact, int& a_numContacts,
		float& a_pen, glm::vec2& a_edgeNormal);

	glm::vec2 GetExtents() { return m_extents; }
	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }

protected:
	glm::vec2 m_extents;	// The halfedge lengths
	glm::vec4 m_colour;

	// Store the local x, y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;

	float m_width;
	float m_height;
};

