#include "PhysicsScene.h"
#include "Sphere.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
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

		// Check for collisions (ideally you'd want to have some sort of
		// scene management in place)
		int actorCount = m_actors.size();

		// Need to check for collisions agianst all objects except this one.
		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = m_actors[outer];
				PhysicsObject* object2 = m_actors[inner];

				// For now we can assume both shapes are spheres,
				// since that is all we've implemented for now.
				Sphere2Sphere(object1, object2);
			}
		}
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
		pActor->Draw();
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	// Try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(a_obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(a_obj2);
	
	// If we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float distance = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
		float sumOfRadii = sphere1->GetRadius() + sphere2->GetRadius();

		if (distance < sumOfRadii)
		{
			sphere1->ApplyForce(-sphere1->GetVelocity());
			sphere2->ApplyForce(-sphere2->GetVelocity());

			return true;
		}
	}

	return false;
}
