#pragma once

class GameEngine2DApp;

// Managers
#include "PhysicsManager.h"
#include "RenderingManager.h"
#include "Renderer2D.h"

#include "Transform.h"
#include "AActor.h"
class APlayer;
class Tile;

#include <vector>
#include <string>

class World
{
protected:
	World();
	~World();
public:

	static void Update(float DeltaTime);
	static void Draw(float a_Aplha = 1.f);

public:
	World(World& Other) = delete;
	void operator=(const World&) = delete;

	static GameEngine2DApp* GetApplication() { GetWorld()->m_Application; }
	static void SetApplication(GameEngine2DApp* a_App) { GetWorld()->m_Application = a_App; }
	static World* GetWorld();

	static PhysicsManager* GetPhysicsManager();
	static RenderingManager* GetRenderer();
	static aie::Renderer2D* GetRenderer2D();

	static vec2 GetWindowSize() { return GetWorld()->m_WindowSize; }
	static void SetWindowSize(vec2 a_WindowSize) { GetWorld()->m_WindowSize = a_WindowSize; }

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

	GameEngine2DApp* m_Application;

	// Game Members
	static void BuildLevel(vec2 a_WindowSize, const std::vector<std::string>& a_Tiles);
	void ReachedGoal();
public:

	int m_CurrentLevel = 0;
	void ResetLevel();
	void ProgressLevel();

	float m_LevelBannerTimeAlive = 0.f;
	float m_LevelBannerLifeTime = 3.f;
	void  DrawLevelBanner();

#pragma region LevelMaps

	const std::vector<std::vector<std::string>> m_Levels
	{
		{
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..S...........#.............G.."),
		},
		{
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("..............#................"),
			("......G.......#................"),
			("####################..........."),
			("...................D..........."),
			("...................D..........."),
			("...................D..........."),
			("...................#..........."),
			("...................#..........."),
			(".S.................#..........."),

		},
		{
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("................M.............."),
			("..............................."),
			("................R.............."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
			("..............................."),
		},
	};

#pragma endregion


protected:
	vec2 m_WindowSize;

	PhysicsManager* m_Physics;
	RenderingManager* m_Renderer;

	std::vector<Transform*> m_Transforms;
	std::vector<AActor*> m_Actors;

	APlayer* m_Player = nullptr;
};

static World* m_CurrentWorld;

