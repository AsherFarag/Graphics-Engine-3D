#include "GraphicsEngine3DApp.h"

// --- AIE ---
#include "Gizmos.h"
#include "Input.h"
#include "gl_core_4_4.h"

// --- GLM ---
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// --- Engine ---
#include "AFlyCamera.h"
#include "RResource.h"
#include "AStaticMesh.h"
#include "RenderingManager.h"
#include "ResourceManager.h"
#include "RMaterial.h"
#include "World.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsEngine3DApp::GraphicsEngine3DApp()
{

}

GraphicsEngine3DApp::~GraphicsEngine3DApp()
{
	if (m_World)
		delete m_World;
}

GraphicsEngine3DApp* GraphicsEngine3DApp::GetInstance()
{
	if (s_Instance == nullptr)
		s_Instance = new GraphicsEngine3DApp();

	return s_Instance;
}

bool GraphicsEngine3DApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	m_World = new World();
	if (m_World->Begin() == false)
		return false;
	m_World->m_MainCamera->SetRenderTarget(&m_RenderTarget);

	if (m_RenderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
		return false;

	m_Viewport = ImGui_Viewport(&m_RenderTarget);

	return true;
}

void GraphicsEngine3DApp::shutdown()
{
	if (m_World)
		m_World->End();

	delete ResourceManager::GetInstance();

	Gizmos::destroy();
}

void GraphicsEngine3DApp::update(float deltaTime)
{
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (m_World)
	{
		m_World->m_DeltaTime = deltaTime;
		m_World->Update();
	}
}

void GraphicsEngine3DApp::draw()
{
	if (m_World)
		m_World->Draw();

	#pragma region ImGui
	
	#pragma region Stats

	ImGui::Begin("Stats");

	ImGui::Text("FPS: %i", m_fps);

	ImGui::End();

	#pragma endregion

	#pragma region Viewport

	m_Viewport.Draw();

	#pragma endregion
	
	#pragma endregion
}

bool GraphicsEngine3DApp::LaunchShaders()
{
	//if (!LoadShaders(m_SimpleShader, "./shaders/simple.", "Simple Shader"))
	return false;
}

bool GraphicsEngine3DApp::LoadShaders(aie::ShaderProgram& a_ShaderToLoad, const char* a_FilePath, std::string a_ErrorName)
{
	std::string Vert = a_FilePath;
	std::string Frag = a_FilePath;

	a_ShaderToLoad.loadShader(aie::eShaderStage::VERTEX,
		Vert.append("vert").c_str());
	a_ShaderToLoad.loadShader(aie::eShaderStage::FRAGMENT,
		Frag.append("frag").c_str());
	if (a_ShaderToLoad.link() == false)
	{
		printf(a_ErrorName.append(" Shader Error: %s\n").c_str(), a_ShaderToLoad.getLastError());
		return false;
	}

	return true;
}
