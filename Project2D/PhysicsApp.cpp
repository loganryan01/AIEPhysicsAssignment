#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

PhysicsApp::PhysicsApp() 
{
	
}

PhysicsApp::~PhysicsApp() 
{

}

bool PhysicsApp::startup() 
{
	// Increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);

	// initialize the physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, -20));
	m_physicsScene->SetTimeStep(0.01f);

	PachinkoScene();
	return true;
}

void PhysicsApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsApp::update(float deltaTime) 
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (input->isMouseButtonDown(0) && m_availableSpheres > 0)
	{
		m_availableSpheres--;
		
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		Sphere* ball = new Sphere(worldPos, glm::vec2(0), 1, 4, glm::vec4(0, 1, 0, 1));
		ball->SetElasticity(0.f);
		m_physicsScene->AddActor(ball);
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 0);
	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the center of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// Scale this according to the extents
	worldPos.x *= 2.f * m_extents / getWindowWidth();
	worldPos.y *= 2.f * m_extents / getWindowHeight() / m_aspectRatio;

	return worldPos;
}

void PhysicsApp::PachinkoScene()
{
	m_availableSpheres = 5;
	
	Plane* plane1 = new Plane(glm::vec2(0, 1), -125, glm::vec4(0.59, 0.29, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(0, -1), -125, glm::vec4(0.59, 0.29, 0, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -90, glm::vec4(0.59, 0.29, 0, 1));
	Plane* plane4 = new Plane(glm::vec2(-1, 0), -90, glm::vec4(0.59, 0.29, 0, 1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	Sphere* ball = new Sphere(glm::vec2(10, 100), glm::vec2(0), 1, 4, glm::vec4(0, 1, 0, 1));
	ball->SetElasticity(0.f);
	m_physicsScene->AddActor(ball);

	for (int i = 0; i < 5; i++)
	{
		Box* box = new Box(glm::vec2(-68 + (i * 32), -110), glm::vec2(0), 0, 4, 4, 15);
		box->SetKinematic(true);
		m_physicsScene->AddActor(box);
	}

	int rows = 4;
	int columns = 11;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			Sphere* sphere = new Sphere(glm::vec2((16 * x) - 76, (-40 * y) + 50), glm::vec2(0), 1, 4, glm::vec4(1, 0, 0, 1));
			sphere->SetElasticity(1);
			sphere->SetKinematic(true);
			m_physicsScene->AddActor(sphere);
		}
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			Sphere* sphere = new Sphere(glm::vec2((16 * x) - 84, (-40 * y) + 30), glm::vec2(0), 1, 4, glm::vec4(1, 0, 0, 1));
			sphere->SetElasticity(1);
			sphere->SetKinematic(true);
			m_physicsScene->AddActor(sphere);
		}
	}
}
