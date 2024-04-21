#pragma once
#include "Application.h"

// --- STD ---
#include <string>

// --- GLM ---
#include <glm/mat4x4.hpp>

// --- AIE ---
#include "Shader.h"

// --- ImGui ---
#include "imgui.h"
#include "ImGui_ResourceManager.h"

// --- Engine ---
class World;
class RenderManager;
class RSkeleton;
using SkeletonHandle = std::shared_ptr<RSkeleton>;

class GraphicsEngine3DApp
	: public aie::Application
{
protected:

	GraphicsEngine3DApp();
	virtual ~GraphicsEngine3DApp();

public:
	GraphicsEngine3DApp(GraphicsEngine3DApp& Other) = delete;
	void operator=(const GraphicsEngine3DApp&) = delete;

	static GraphicsEngine3DApp* GetInstance();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	static RenderManager* GetRenderManager() { return GetInstance()->m_RenderManager; }

	SkeletonHandle skellie;

protected:
	friend class World;
	World* m_World;
	// Managers
	RenderManager* m_RenderManager;
	// Time taken for the update function
	float m_UpdateTime;

#if IS_EDITOR

public:
	Debug::ImGui_DebugLog m_DebugLog;
	ImGui_ResourceManager m_ResourceManagerView;

#endif // IS_EDITOR
};

static GraphicsEngine3DApp* s_Instance;