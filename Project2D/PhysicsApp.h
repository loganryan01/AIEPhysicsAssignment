#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class PhysicsApp : public aie::Application 
{
public:
	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

	void PachinkoScene();

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;

	const float m_aspectRatio = 3.f / 4.f;
	const float m_extents = 100;

	int m_availableSpheres;
	int m_playerScore = 0;
	int m_scores[6] = { 10, 0, 2, 5, 1, 4 };
};