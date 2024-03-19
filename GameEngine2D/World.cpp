#include "World.h"

#include "Transform.h"

#include "BoxRB.h"
#include "CircleRB.h"
#include "PlaneRB.h"
#include "SpringRB.h"

#include "Renderer.h"

#include "glm/ext.hpp"

#include <random>
#include "iostream"

World::World()
{
	m_Physics = new PhysicsManager();
	m_Physics->Init();
	m_Renderer = new RenderingManager();
	m_Renderer->Init();
}

World::~World()
{
}

void World::Update(float DeltaTime)
{
	World* CurrentWorld = GetWorld();

	CurrentWorld->m_Physics->Update(DeltaTime);
	CurrentWorld->m_Renderer->Update(DeltaTime);
}

World* World::GetWorld()
{
	if (m_CurrentWorld == nullptr)
		m_CurrentWorld = new World();

	return m_CurrentWorld;
}

PhysicsManager* World::GetPhysicsManager()
{
	return GetWorld()->m_Physics;
}

RenderingManager* World::GetRenderer()
{
	return GetWorld()->m_Renderer;
}

aie::Renderer2D* World::GetRenderer2D()
{
	return GetWorld()->GetRenderer()->GetRenderer2D();
}

void World::ResetWorld()
{
	delete m_CurrentWorld;
	m_CurrentWorld = new World();
}

void World::TestScene(vec2 a_WindowSize, float a_Spacing)
{
	Transform* Plane = new Transform();
	Plane->m_Pos = vec2(0, a_Spacing);
	Plane->m_Rot = pi<float>() * 0.0f;
	Plane->AddComponent<PlaneRB>(Plane, RigidBody::STATIC, 5000);

	Transform* Plane1 = new Transform();
	Plane1->m_Pos = vec2(a_Spacing, 0);
	Plane1->m_Rot = pi<float>() * 0.5f;
	Plane1->AddComponent<PlaneRB>(Plane1, RigidBody::STATIC, 5000);

	Transform* Plane2 = new Transform();
	Plane2->m_Pos = vec2(a_WindowSize.x - a_Spacing + 1, 0);
	Plane2->m_Rot = pi<float>() * -0.5f;
	Plane2->AddComponent<PlaneRB>(Plane2, RigidBody::STATIC, 5000);

	Transform* Plane3 = new Transform();
	Plane3->m_Pos = vec2(0, a_WindowSize.y - a_Spacing + 1);
	Plane3->m_Rot = pi<float>() * 1.f;
	Plane3->AddComponent<PlaneRB>(Plane3, RigidBody::STATIC, 5000);
}

void World::TestBallScene(vec2 a_WindowSize, int a_BallCount)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < a_BallCount; ++i)
	{
		float Radius = 3 + rand() % 50;
		float Mass = pi<float>() * Radius * Radius;
		Transform* Circle1 = new Transform();
		Circle1->m_Pos = vec2(Radius + rand() % (int)(a_WindowSize.x - Radius), Radius + rand() % (int)(a_WindowSize.y - Radius - 1));
		CircleRB* Circle1RB = Circle1->AddComponent<CircleRB>(Circle1, RigidBody::DYNAMIC, Radius, vec4(1, 0, 1, 1.f), Mass, 0.5f);
		Circle1RB->CollisionCallBack = [=](PhysicsObject* other)
			{
				std::cout << "Howzat!!?" << std::endl;
				return;
			};
	}
}

void World::TestBoxScene(vec2 a_WindowSize, int a_BoxCount)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < a_BoxCount; ++i)
	{
		float ExtentX = 1 + rand() % 50;
		float ExtentY = 1 + rand() % 50;
		float Mass = ExtentX * ExtentY;

		Transform* Box = new Transform();
		Box->m_Pos = vec2(ExtentX + rand() % (int)(a_WindowSize.x - ExtentX), ExtentY + rand() % (int)(a_WindowSize.y - ExtentY));
		Box->m_Rot = rand() % 7;
		Box->AddComponent<BoxRB>(Box, RigidBody::DYNAMIC, vec2(ExtentX, ExtentY), vec4(1,0,0,1), Mass);
	}
}

void World::TestRopeScene(vec2 a_WindowSize, int a_JointCount)
{
	GetPhysicsManager()->SetGravity(DEFAULT_GRAVITY);

	CircleRB* Prev = nullptr;

	for (int i = 0; i < a_JointCount; i++)
	{
		// spawn a circle to the right and below the previous one, so that the whole rope acts under gravity and swings
		Transform* T = new Transform();
		T->SetPosition(vec2(6 * i + a_WindowSize.x * 0.5f, /*i * 30 + */a_WindowSize.y * 0.5f));
		CircleRB* circle = new CircleRB(T, RigidBody::DYNAMIC, 3);
		if (i == 0)
			circle->SetCollisionType(RigidBody::STATIC);

		if (Prev)
			new SpringRB(circle->GetTransform(), circle, Prev, 300, 1, vec4(0,1,1,1), 5);
		Prev = circle;
	}

	// Add a kinematic box at an angle for the rope to drape over
	Transform* T = new Transform();
	T->SetPosition(vec2(a_WindowSize.x * 0.5f, a_WindowSize.y * 0.5f - 150));
	BoxRB* box = new BoxRB(T, RigidBody::STATIC, vec4(15));
}

Transform* World::GetHitTransform(const vec2 a_Point)
{
	std::vector<Transform*> HitTransforms;
	for (auto RB : GetPhysicsManager()->m_RigidBodies)
	{
		if (RB->IsInside(a_Point))
			HitTransforms.push_back(RB->GetTransform());
	}

	if (HitTransforms.size() == 0)
		return nullptr;

	Transform* ClosestTransform = HitTransforms[0];
	float ClosestDistance = distance(ClosestTransform->GetPosition(), a_Point);
	for (int i = 1; i < HitTransforms.size(); i++)
	{
		float Distance = distance(HitTransforms[i]->GetPosition(), a_Point);
		if (Distance < ClosestDistance)
		{
			ClosestTransform = HitTransforms[i];
			ClosestDistance = Distance;
		}
	}

	return ClosestTransform;
}
