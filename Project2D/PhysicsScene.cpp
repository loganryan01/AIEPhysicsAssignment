#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"

// Function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere,
};

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

		// Check for collisions
		CheckForCollision();
	}
}

void PhysicsScene::UpdateGizmos()
{
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
		pActor->Draw();
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();

	// Need to check for collisions agianst all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			// Using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// Did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	return Sphere2Plane(a_obj2, a_obj1);
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(a_obj1);
	Plane* plane = dynamic_cast<Plane*>(a_obj2);

	// If we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), plane->GetNormal()) -
			plane->GetDistance();

		float intersection = sphere->GetRadius() - sphereToPlane;
		float velocityOutOfPlane
			= glm::dot(sphere->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			// Set sphere velocity to zero here
			sphere->ApplyForce(-sphere->GetVelocity() * sphere->GetMass());
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	// Try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(a_obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(a_obj2);
	
	// If we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		glm::vec2 dist = sphere1->GetPosition() - sphere2->GetPosition();
		float sumOfRadii = sphere1->GetRadius() + sphere2->GetRadius();

		if (glm::length(dist) < sumOfRadii)
		{
			sphere1->ResolveCollision(sphere2);
			return true;
		}
	}

	return false;
}
