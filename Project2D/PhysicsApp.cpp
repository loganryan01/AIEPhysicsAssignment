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
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));
	m_physicsScene->SetTimeStep(0.01f);

	SphereTest();
	//BoxTest();
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
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 0);
	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::BoxTest()
{
	Box* box = new Box(glm::vec2(10, 0), glm::vec2(1), 0, 4, 2, 2, 0, glm::vec4(1, 0, 0, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -30);

	box->SetElasticity(0.3f);
	plane->SetElasticity(0);

	m_physicsScene->AddActor(box);
	m_physicsScene->AddActor(plane);
}

void PhysicsApp::SphereTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(1), 0, 4, 0, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(10, 0), glm::vec2(1), 0, 4, 0, 4,  glm::vec4(0, 1, 0, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -30);

	ball1->SetElasticity(0.3f);
	ball2->SetElasticity(0.3f);
	plane->SetElasticity(0);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane);
}
