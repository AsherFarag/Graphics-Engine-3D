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

// --- Engine ---
class World;

#pragma region Macros

#define IS_DEBUG true

#define AIE "./bin/"

#pragma endregion

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

	friend class World;

protected:
	World* m_World;

public:

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	bool LaunchShaders();
	bool LoadShaders(aie::ShaderProgram& a_ShaderToLoad, const char* a_FilePath, std::string a_ErrorName);
};

static GraphicsEngine3DApp* s_Instance;