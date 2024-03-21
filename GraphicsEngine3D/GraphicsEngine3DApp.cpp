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

	return true;
}

void GraphicsEngine3DApp::shutdown()
{
	if (m_World)
		m_World->End();

	Gizmos::destroy();
}

void GraphicsEngine3DApp::update(float deltaTime)
{
	if (m_World)
	{
		m_World->m_DeltaTime = deltaTime;
		m_World->Update();
	}

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsEngine3DApp::draw()
{
	// Bind our Render
	m_RenderTarget.bind();

	// Wipe the screen to the background colour
	clearScreen();

	if (m_World)
		m_World->Draw();

	m_RenderTarget.unbind();

	// Wipe the screen to the background colour
	clearScreen();

	#pragma region ImGui
	
	#pragma region Stats

	ImGui::Begin("Stats");

	ImGui::Text("FPS: %i", m_fps);

	ImGui::End();

	#pragma endregion

	m_RenderTarget.getTarget(0).bind(0);
	ImGui::Begin("Viewport");

	// we access the ImGui window size
	const float window_width = ImGui::GetContentRegionAvail().x;
	const float window_height = ImGui::GetContentRegionAvail().y;

	//m_RenderTarget.initialise(1, window_width, window_height);
	m_RenderTarget.rescaleFrameBuffer(0, window_width, window_height);

	// we rescale the framebuffer to the actual window size here and reset the glViewport 
	//rescale_framebuffer(window_width, window_height);
	glViewport(0, 0, window_width, window_height);

	// we get the screen position of the window
	ImVec2 pos = ImGui::GetCursorScreenPos();

	// and here we can add our created texture as image to ImGui
	// unfortunately we need to use the cast to void* or I didn't find another way tbh
	ImGui::Image(
		(ImTextureID)m_RenderTarget.getTarget(0).getHandle(),
		ImGui::GetContentRegionAvail(),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
	
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
