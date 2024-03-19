#pragma once

// --- AIE ---
#include "Gizmos.h"

// --- STD ---
#include <List>
using std::list;

// --- Engine ---
#include "GraphicsEngine3DApp.h"
#include "Light.h"
class ALight;
class UBaseObject;
class AActor;
class ACamera;
class AFlyCamera;
class AStaticMesh;
class RMaterial;
class RenderingManager;


// A World is a scene object that stores and handles all the objects inside of it.
// It handles the destruction and creation of objects.
class World
{
public:
	World();
	~World();

	friend class GraphicsEngine3DApp;

protected:
	virtual bool Begin();
	virtual void Update();
	virtual void Draw();
	virtual void End();

	list<UBaseObject*> m_Objects;
	list<AActor*> m_Actors;

	// Clears and deletes all objects inside every update
	list<UBaseObject*> m_ObjectsPendingDestroy;

	ACamera* m_MainCamera = nullptr;
	std::shared_ptr< AActor> m_InspectedActor = nullptr;

	float m_DeltaTime;

	// - Managers -
	RenderingManager* m_RenderingManager;

public:

	static float const DeltaTime() { return GetWorld()->m_DeltaTime; }

	static World* GetWorld() { return GraphicsEngine3DApp::GetInstance()->m_World; }
	static RenderingManager* GetRenderingManager() { return GraphicsEngine3DApp::GetInstance()->m_World->m_RenderingManager; }

#pragma region Object Functions

public:
	bool AddObject(UBaseObject* a_Object);
	bool RemoveObject(UBaseObject* a_Object);

	bool AddActor(AActor* a_Actor);


	template <typename T, typename... Args>
	T* InstatiateObject(Args&... a_Args)
	{
		T* Object = new T(std::forward< Args >(a_Args)...);

		AddObject(Object);

		return Object;
	}

	void DestroyObject(UBaseObject* a_Object);

protected:
	// Called at the start of every update
	void DestroyPendingObjects();

#pragma endregion


	// --- SCENE DATA ---
	// Probably temporary idk
protected:
	ALight* m_AmbientLight;

	RMaterial* m_SoulspearMat;
	AStaticMesh* m_StaticMesh;

	AFlyCamera* m_FlyCamera;
};

