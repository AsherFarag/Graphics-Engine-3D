#pragma once
// --- STD ---
#include <typeinfo>

// --- GLM ---
#include <glm/mat4x4.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;

// --- Engine ---
#include "GraphicsEngine3DApp.h"
#include "World.h"

class UBaseObject
{
	INSPECTABLE;

public:
	UBaseObject();
	virtual ~UBaseObject();

	static constexpr auto& GetParentClass() { return typeid(UBaseObject); }

protected:
	GraphicsEngine3DApp* Engine = nullptr; 	// A Reference to the engine instance
	World* m_World = nullptr;			 	// A reference to the world that owns this object

	unsigned int ID;
	bool m_Enabled = false;
	bool m_Valid = true;

protected:
	virtual void OnEnabled() {};
	virtual void OnDisabled() {};
	virtual void OnDestroyed() {};

public:
	World* GetWorld() const;
	bool SetWorld(World* a_World); 	// Use Cautiously

	bool IsEnabled() const { return m_Enabled; }
	void SetEnabled(bool a_Enabled);

	bool IsValid() const { return m_Valid; }

	size_t GetId() const { return ID; }
	bool Destroy(float a_LifeTime = -1.f);

};

