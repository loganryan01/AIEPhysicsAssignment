#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

// Function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
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

void PhysicsScene::ApplyContactForces(Rigidbody* a_body1, Rigidbody* a_body2, 
	glm::vec2 a_norm, float a_pen)
{
	float body2Mass = a_body2 ? a_body2->GetMass() : INT_MAX;

	float body1Factor = body2Mass / (a_body1->GetMass() + body2Mass);

	a_body1->SetPosition(a_body1->GetPosition() - body1Factor * a_norm * a_pen);
	if (a_body2)
		a_body2->SetPosition(a_body2->GetPosition() + (1 - body1Factor) * a_norm * a_pen);
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	return Sphere2Plane(a_obj2, a_obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	Plane* plane = dynamic_cast<Plane*>(a_obj1);
	Box* box = dynamic_cast<Box*>(a_obj2);

	// If we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		// Get a representative point on a plane
		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		// Check all four corners to see if we've hit the plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// Get the position of the corner in world space
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				// This is the total velocity of the point in the world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() *
					glm::vec2(-displacement.y, displacement.x);
				// and this is the component of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

				// and moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		// We've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}
	
	return false;
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
		glm::vec2 contact = sphere->GetPosition() + (collisionNormal * -sphere->GetRadius());

		float intersection = sphere->GetRadius() - sphereToPlane;
		float velocityOutOfPlane
			= glm::dot(sphere->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			plane->ResolveCollision(sphere, contact);
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
			float penetration = sphere1->GetRadius() + sphere2->GetRadius() - glm::length(dist);
			if (penetration > 0)
			{
				sphere1->ResolveCollision(sphere2, (sphere1->GetPosition() +
					sphere2->GetPosition()) * 0.5f, nullptr, penetration);
			}
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	return Box2Sphere(a_obj2, a_obj1);
}

bool PhysicsScene::Box2Plane(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	return Plane2Box(a_obj2, a_obj1);
}

bool PhysicsScene::Box2Sphere(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	Box* box = dynamic_cast<Box*>(a_obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(a_obj2);

	if (box != nullptr && sphere != nullptr)
	{
		// Transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()),
			glm::dot(circlePosWorld, box->GetLocalY()));

		// Find the closest point to the circle centre on the box by clamping the
		// coordinates in box-space to the box's extent
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();
		if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
		if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
		if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;
		// and convert back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x *
			box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
		glm::vec2 circleToBox = sphere->GetPosition() - closestPointOnBoxWorld;
		float penetration = sphere->GetRadius() - glm::length(circleToBox);
		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(sphere, contact, &direction);
		}
	}
	
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* a_obj1, PhysicsObject* a_obj2)
{
	Box* box1 = dynamic_cast<Box*>(a_obj1);
	Box* box2 = dynamic_cast<Box*>(a_obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm;
		}
		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	
	return false;
}
