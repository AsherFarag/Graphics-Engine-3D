#include "ACamera.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- GLM ---
#include "glm/common.hpp"
#include "glm/ext.hpp"


// --- AIE ---
#include "Shader.h"

// --- Engine ---
#include "GraphicsEngine3DApp.h"
#include "ALight.h"
#include "RenderingManager.h"
#include "UMeshRenderer.h"
#include "ResourceManager.h"

ACamera::ACamera(aie::AIERenderTarget* a_RenderTarget)
	: AActor()
{
	m_Name = "Camera";
    m_PostProcessQuad.InitialiseFullScreenQuad();

    m_PostProcessShader = ResourceManager::LoadShader("PostProcess");
    m_PostProcessShader->bindUniform("PostProcess", 0);
    m_PostProcessShader->bindUniform("colourTarget", 0);

    World::GetRenderingManager()->AddRenderCamera(this);

    m_MeshRenderer = AddComponent<UMeshRenderer>(this);
    m_MeshRenderer->SetMesh("FilmCamera");
    m_MeshRenderer->SetMaterial(ResourceManager::GetMaterial("Camera"));

    // Set Mesh Size as Camera Model is Huge
    SetActorScale(vec3(0.1f));

#if IS_EDITOR
    m_RenderTarget = a_RenderTarget;
    if (m_RenderTarget == nullptr)
        m_RenderTarget = new aie::AIERenderTarget();

    m_Viewport.SetName((string("Viewport##") + std::to_string(ID)).c_str());
    m_Viewport.SetRenderTarget(m_RenderTarget);

#endif // IS_EDITOR

    m_Forward = vec3(1, 0, 0);
}

ACamera::~ACamera()
{
    delete m_RenderTarget;
}

void ACamera::Begin()
{
#pragma message(" - Fix up rescaling frame buffer in ACamera - ")
    if (m_RenderTarget && !m_RenderTarget->initialise(1, 640, 480, true))
    {
        LOG(Error, true, "Could not initialise camera render target");

        return;
    }
#if IS_EDITOR

    m_Viewport.SetRenderTarget(m_RenderTarget);

#endif // IS_EDITOR
}

void ACamera::Update()
{
    GetActorRotation().z = glm::clamp(GetActorRotation().z, -87.5f, 87.5f);
    m_Transform->Update();

	if (m_RenderTarget)
    {
        #if IS_EDITOR

        SetAspectRatio(m_RenderTarget->getWidth(), m_RenderTarget->getHeight());

        #else

        SetAspectRatio(Engine->getWindowWidth(), Engine->getWindowHeight());

        #endif // IS_EDITOR
    }

	SetViewMatrix();
}

void ACamera::BeginRender()
{
	if (m_RenderTarget)
	{
		// Bind our Render
		m_RenderTarget->bind();

        glViewport(0, 0, m_RenderTarget->getWidth(), m_RenderTarget->getHeight());


        #if !IS_EDITOR
        m_RenderTarget->rescaleFrameBuffer(Engine->getWindowWidth(), Engine->getWindowHeight());
        #endif // !IS_EDITOR
	}

    // Wipe the gizmos clean for this frame
    Gizmos::clear();

    ClearScreen();
}


void ACamera::EndRender()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->unbind();
	}
}

void ACamera::ClearScreen()
{
    // Wipe the screen to the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void ACamera::ApplyPostProcessing()
{
    m_PostProcessPercent += 10.f * m_World->DeltaTime();

    m_PostProcessShader->bind();
    m_PostProcessShader->bindUniform("ProgressPercent", m_PostProcessPercent);


    GetRenderTarget()->bind();
    GetRenderTarget()->bindRead();

    // 

    GLboolean depthMask = GL_TRUE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

    glDepthMask(GL_FALSE);

    glDisable(GL_DEPTH_TEST);

    m_PostProcessQuad.Draw();

    glEnable(GL_DEPTH_TEST);

    glDepthMask(depthMask);

    GetRenderTarget()->unbind();
}

void ACamera::SetViewMatrix()
{	
	m_ViewTransform = lookAt(GetActorPosition(), GetActorPosition() + GetForward(), vec3(0, 1, 0));
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_AspectRatio, float a_Near, float a_Far)
{
	m_FOVDegrees = a_FOVDegrees;
	m_AspectRatio = a_AspectRatio;
	m_Near = a_Near;
	m_Far = a_Far;

	m_ProjectionViewTransform = glm::perspective(glm::radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_Width, float a_Height, float a_Near, float a_Far)
{
	SetProjectionMatrix(a_FOVDegrees, a_Width / a_Height, a_Near, a_Far);
}

void ACamera::UpdateProjectionMatrix()
{
    m_ProjectionViewTransform = glm::perspective(glm::radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
}

void ACamera::SetAspectRatio(float a_AspectRatio)
{
	m_AspectRatio = a_AspectRatio;
	UpdateProjectionMatrix();
}

void ACamera::SetAspectRatio(float a_Width, float a_Height)
{
	SetAspectRatio(a_Width / a_Height);
}

void ACamera::SetRenderTarget(aie::AIERenderTarget* a_RenderTarget)
{
    m_RenderTarget = a_RenderTarget; 
#if IS_EDITOR

    m_Viewport.SetRenderTarget(m_RenderTarget);

#endif // IS_EDITOR

}

//vec3 ACamera::GetForward()
//{
//	float ThetaR = radians(m_Rotation.y);
//	float PhiR = radians(m_Rotation.z);
//	return vec3(cos(PhiR) * cos(-ThetaR), sin(PhiR), cos(PhiR) * sin(-ThetaR));
//}

void ACamera::OnDraw_ImGui()
{
	ImGui::Text("Aspect Ratio: %0.3f", m_AspectRatio);
	float FOV = m_FOVDegrees;
	if (ImGui::SliderFloat("FOV", &FOV, 0.f, 180.f, "%.f"))
		SetFOV(FOV);

    if (ImGui::TreeNode("Post-Processing"))
    {
        static int PostProcessIndex = 0;
        if (ImGui::SliderInt("Post Process", &PostProcessIndex, 0, 7))
        {
            m_PostProcessShader->bindUniform("PostProcess", PostProcessIndex);
        }

        // Toon
        if (PostProcessIndex == 4)
        {
            static int ToonScale = 1;
            ImGui::SliderInt("ToonScale", &ToonScale, 0, 10);
            m_PostProcessShader->bindUniform("ToonScale", ToonScale);
        }

        // Fog
        if (PostProcessIndex == 6)
        {
            static vec4 FogColour = vec4(1);
            ImGui::ColorEdit4("Fog Colour", &FogColour[0]);
            m_PostProcessShader->bindUniform("FogColour", FogColour);
        }

        ImGui::Checkbox("Use Post-Process", &m_UsePostProcessing);
        ImGui::TreePop();
    }
}
