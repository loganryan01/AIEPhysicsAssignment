#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::AddActor(PhysicsObject* a_actor)
{
	m_actors.push_back(a_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* a_actor)
{
	auto iter = std::find(m_actors.begin(), m_actors.end(), a_actor);

	if (iter != m_actors.end())
	{
		m_actors.erase(iter);
	}
}

void PhysicsScene::Update(float dt)
{
	// Update physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
		pActor->Draw();
}
