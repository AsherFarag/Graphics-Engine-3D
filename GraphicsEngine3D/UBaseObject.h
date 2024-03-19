#pragma once
// --- GLM ---
#include <glm/mat4x4.hpp>
using namespace glm;

// --- Engine ---
#include "GraphicsEngine3DApp.h"
#include "World.h"

class UBaseObject
{
public:
	UBaseObject();
	virtual ~UBaseObject();

	size_t GetId() const { return ID; }
	bool Destroy(float a_LifeTime = -1.f);

protected:
	// A Reference to the engine instance
	GraphicsEngine3DApp* Engine = nullptr;
	// A reference to the world that owns this object
	World* m_World = nullptr;

	bool m_Enabled = false;
	virtual void OnEnabled() {};
	virtual void OnDisabled() {};

	// An object is valid if it is not pending to be destroyed
	bool m_Valid = true;
	virtual void OnDestroyed() {};

	size_t ID;

public:
	World* GetWorld() const;
	bool SetWorld(World* a_World); 	// Use Cautiously

	bool IsEnabled() const { return m_Enabled; }
	void SetEnabled(bool a_Enabled);

	bool IsValid() const { return m_Valid; }

};

