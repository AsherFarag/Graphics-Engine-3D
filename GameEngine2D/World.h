#pragma once

// Managers
#include "PhysicsManager.h"
#include "RenderingManager.h"
#include "Renderer2D.h"

#include "Transform.h"
#include "AActor.h"

#include <vector>

class World
{
protected:
	World();
	~World();
public:

	static void Update(float DeltaTime);

public:
	World(World& Other) = delete;
	void operator=(const World&) = delete;

	static World* GetWorld();

	static PhysicsManager* GetPhysicsManager();
	static RenderingManager* GetRenderer();
	static aie::Renderer2D* GetRenderer2D();

	static void ResetWorld();

	static void AddTransform(Transform* a_Transform) { GetWorld()->m_Transforms.push_back(a_Transform); }
	static void RemoveTransform(Transform* a_Transform) { GetWorld()->m_Transforms.erase(std::find(GetWorld()->m_Transforms.begin(), GetWorld()->m_Transforms.end(), a_Transform)); }

	static void AddActor(AActor* a_Actor) { GetWorld()->m_Actors.push_back(a_Actor); }
	static void RemoveActor(AActor* a_Actor) { GetWorld()->m_Actors.erase(std::find(GetWorld()->m_Actors.begin(), GetWorld()->m_Actors.end(), a_Actor)); }

	static void TestScene(vec2 a_WindowSize, float a_Spacing = 0.f);
	static void TestBallScene(vec2 a_WindowSize, int a_BallCount);
	static void TestBoxScene(vec2 a_WindowSize, int a_BoxCount);
	static void TestRopeScene(vec2 a_WindowSize, int a_JointCount);

	static Transform* GetHitTransform(const vec2 a_Point);

protected:
	PhysicsManager* m_Physics;
	RenderingManager* m_Renderer;

	std::vector<Transform*> m_Transforms;
	std::vector<AActor*> m_Actors;
};

static World* m_CurrentWorld;

