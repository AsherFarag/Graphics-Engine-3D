#include "World.h"
#include "GameEngine2DApp.h"

#include "Transform.h"

#include "BoxRB.h"
#include "CircleRB.h"
#include "PlaneRB.h"
#include "SpringRB.h"
#include "Tile.h"
#include "GoalPoint.h"
#include "APlayer.h"
#include "ASoftBody.h"

#include "Renderer.h"
#include "Font.h"

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

	CurrentWorld->m_LevelBannerTimeAlive += DeltaTime;

	CurrentWorld->m_Physics->Update(DeltaTime);
	CurrentWorld->m_Renderer->Update(DeltaTime);

	if (CurrentWorld->m_Player)
	{
		CurrentWorld->m_Player->Update(DeltaTime);
	}
}

void World::Draw(float a_Aplha)
{
	World* CurrentWorld = GetWorld();

	//CurrentWorld->m_Physics->Draw(a_Aplha);
	CurrentWorld->m_Renderer->Draw(a_Aplha);

	if (CurrentWorld->m_Player)
	{
		CurrentWorld->m_Player->Draw();
		//CircleRB* PlayerRB = CurrentWorld->m_Player->GetComponent<CircleRB>();
		//if (PlayerRB)
			//std::cout << length(PlayerRB->GetVelocity()) << std::endl;
	}

	CurrentWorld->DrawLevelBanner();
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
	GetPhysicsManager()->SetGravity(3.f * DEFAULT_GRAVITY);

	CircleRB* Prev = nullptr;

	for (int i = 0; i < a_JointCount; i++)
	{
		// spawn a circle to the right and below the previous one, so that the whole rope acts under gravity and swings
		Transform* T = new Transform();
		T->SetPosition(vec2(30 * i + a_WindowSize.x * 0.5f,a_WindowSize.y * 0.5f));
		CircleRB* circle = T->AddComponent<CircleRB>(T, RigidBody::DYNAMIC, 5, vec4(0,1,0,1));
		if (i == 0 || i == a_JointCount - 1)
			circle->SetCollisionType(RigidBody::STATIC);

		circle->SetDynamicFrictionCoefficient(0.f);
		circle->SetStaticFrictionCoefficient(0.f);

		if (Prev)
			T->AddComponent<SpringRB>(T, circle, Prev, 50, 1, vec4(0,1,1,1), 15);
		Prev = circle;
	}

	// Add a kinematic box at an angle for the rope to drape over
	//Transform* T = new Transform();
	//T->SetPosition(vec2(a_WindowSize.x * 0.5f, a_WindowSize.y * 0.5f - 150));
	//T->AddComponent<BoxRB>(T, RigidBody::STATIC, vec4(15));
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

void World::BuildLevel(vec2 a_WindowSize, const std::vector<std::string>& a_Tiles)
{
	World* CurrentWorld = GetWorld();

	const int NumColumns = a_Tiles.size();
	const int NumRows = a_Tiles[0].length();

	vec2 TileHalfSize = vec2(a_WindowSize.x / (float)(NumRows * 2), a_WindowSize.y / (float)(NumColumns * 2));

	// Add Joints
	for (int i = 0; i < NumRows; i++)
	{
		for (int j = 0; j < NumColumns; j++)
		{
			// Wall Tile
			if (a_Tiles[j][i] == '#')
			{
				new Tile(TileHalfSize + vec2(i, NumColumns - j - 1) * (TileHalfSize * 2.f), TileHalfSize, DEFAULT_COLOUR);
			}
			// Player Spawn Tile
			else if (a_Tiles[j][i] == 'S')
			{
				CurrentWorld->m_Player = new APlayer(TileHalfSize + vec2(i, NumColumns - j - 1) * (TileHalfSize * 2.f), 12.5f);
			}
			// Goal Tile
			else if (a_Tiles[j][i] == 'G')
			{
				new GoalPoint(TileHalfSize + vec2(i, NumColumns - j - 1) * (TileHalfSize * 2.f), TileHalfSize);
			}
			// Test Rope Scene
			else if (a_Tiles[j][i] == 'R')
			{
				TestRopeScene(GetWindowSize(), 10);
			}
			// Test Ball Scene
			else if (a_Tiles[j][i] == 'C')
			{
				TestBallScene(GetWindowSize(), 2);
			}
			// Test Box Scene
			else if (a_Tiles[j][i] == 'B')
			{
				TestBoxScene(GetWindowSize(), 3);
			}
			// SoftBody Spawn Tile
			else if (a_Tiles[j][i] == 'M')
			{
				new ASoftBody(ASoftBody::MAGNET, TileHalfSize + vec2(i, NumColumns - j - 1) * (TileHalfSize * 2.f));
			}
			// Goal Tile
			else if (a_Tiles[j][i] == 'D')
			{
				Transform* T = new Transform(TileHalfSize + vec2(i, NumColumns - j - 1) * (TileHalfSize * 2.f));
				T->AddComponent<BoxRB>(T, RigidBody::DYNAMIC, TileHalfSize * 0.7f, vec4(1,0,1,1), TileHalfSize.x * 2.f * TileHalfSize.y * 2.f);
			}
		}
	}
}

void World::ReachedGoal()
{
	ProgressLevel();
	std::cout << "Level " << m_CurrentLevel << " Complete!" << std::endl;
}

void World::ResetLevel()
{
	for (int i = m_Transforms.size() - 1; i >= 0; i--)
		delete m_Transforms[i];

	for (int i = m_Actors.size() - 1; i >= 0; i--)
		delete m_Actors[i];

	m_Transforms.clear();

	m_Player = nullptr;

	TestScene(m_WindowSize);
}

void World::ProgressLevel()
{
	ResetLevel();

	m_CurrentLevel++;

	// Progress to next level
	if (m_CurrentLevel < m_Levels.size())
	{
		// Reset Level Banner so it draws
		m_LevelBannerTimeAlive = 0.f;
		BuildLevel(m_WindowSize, m_Levels[m_CurrentLevel]);
	}
}

void World::DrawLevelBanner()
{
	if (m_LevelBannerTimeAlive <= m_LevelBannerLifeTime)
	{
		if (m_Application->m_font->getTextureHandle() != 0)
		{
			vec2 ScreenCentre = m_WindowSize * 0.5f;

			char LevelBannerText[38];
			if (m_CurrentLevel == m_Levels.size() - 1)
				sprintf_s(LevelBannerText, 38, "Congrats! You have Completed the Demo");
			else
				sprintf_s(LevelBannerText, 38, "Level: %i", m_CurrentLevel + 1);

			float TextSizeX, TextSizeY;
			m_Application->m_font->getStringSize(LevelBannerText, TextSizeX, TextSizeY);

			aie::Gizmos::add2DAABBFilled(ScreenCentre, vec2(TextSizeX, TextSizeY) * 0.75f, vec4(0,0,1,1));
			aie::Gizmos::add2DAABB(ScreenCentre, vec2(TextSizeX, TextSizeY) * 0.75f, vec4(1));
			GetRenderer2D()->drawText(m_Application->m_font, LevelBannerText, ScreenCentre.x - TextSizeX * 0.5f, ScreenCentre.y - TextSizeY * 0.5f);
		}
	}
}
