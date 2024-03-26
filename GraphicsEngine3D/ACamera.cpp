#include "ACamera.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- GLM ---
#include "glm/common.hpp"

// --- AIE ---
#include "Shader.h"

// --- Engine ---
#include "GraphicsEngine3DApp.h"
#include "ALight.h"
#include "RenderingManager.h"
#include "UMeshRenderer.h"

ACamera::ACamera()
	: AActor()
{
	m_Name = "Camera";
    m_PostProcessQuad.InitialiseFullScreenQuad();

    m_PostProcessShader = ResourceManager::LoadShader("PostProcess");
    m_PostProcessShader->bindUniform("PostProcess", 0);
    m_PostProcessShader->bindUniform("colourTarget", 0);
    m_PostProcessShader->bindUniform("depthTarget", 1);

    World::GetRenderingManager()->AddRenderCamera(this);

    m_MeshRenderer = AddComponent<UMeshRenderer>(this);
    m_MeshRenderer->SetMesh("FilmCamera");
    m_MeshRenderer->SetMaterial(ResourceManager::GetMaterial("Camera"));

    SetScale(vec3(0.1f));
}

ACamera::~ACamera()
{
}

void ACamera::Update()
{
    m_Rotation.z = clamp(m_Rotation.z, -87.5f, 87.5f);

	if (m_RenderTarget)
		SetAspectRatio(m_RenderTarget->getWidth(), m_RenderTarget->getHeight());
	else
		SetAspectRatio(Engine->getWindowWidth(), Engine->getWindowHeight());

	SetViewMatrix();
}

void ACamera::BeginRender()
{
	if (m_RenderTarget)
	{
		// Bind our Render
		m_RenderTarget->bind();
	}

    // Wipe the gizmos clean for this frame
    Gizmos::clear();

    ClearScreen();
}

void ACamera::Render(ALight* a_AmbientLight, vector<ALight*>* a_Lights, list< UMeshRenderer* >* a_Meshes)
{
	// Calculate the Projected View Matrix
	mat4 ProjectedView = GetProjectionMatrix() * GetViewMatrix();

    const int NumOfLights = a_Lights->size();
    vector<vec3>  LightPositions;
    vector<vec3>  LightColours;
    vector<float> LightFallOffs;

    for (auto Light : *a_Lights)
    {
        LightPositions.push_back(Light->GetPosition());
        LightColours.push_back(Light->GetColour());
        LightFallOffs.push_back(Light->m_FallOff);
    }
    

    // Draw Renderers
    for (auto MeshRenderer = a_Meshes->begin(); MeshRenderer != a_Meshes->end(); ++MeshRenderer)
    {
        if (*MeshRenderer == m_MeshRenderer)
            continue;

        #pragma message("TODO: Currently each draw call rebinds the material. Set-Up Batch Rendering")

        #pragma region Bind Material

        auto Material = (*MeshRenderer)->GetMaterial();
        Material->m_Shader->bind();
        Material->Bind();
        Material->m_Shader->bindUniform("CameraPosition", vec4(GetPosition(), 1));

        if (a_AmbientLight && a_AmbientLight->IsEnabled())
            Material->m_Shader->bindUniform("AmbientLight", a_AmbientLight->GetColour());
        else
            Material->m_Shader->bindUniform("AmbientLight", vec3());

        Material->m_Shader->bindUniform("NumOfLights", NumOfLights);
        if (NumOfLights > 0)
        {
            Material->m_Shader->bindUniform("PointLightColors", NumOfLights, &LightColours[0]);
            Material->m_Shader->bindUniform("PointLightPositions", NumOfLights, &LightPositions[0]);
            Material->m_Shader->bindUniform("PointLightFallOffs", NumOfLights, &LightFallOffs[0]);
        }

        #pragma endregion 

        (*MeshRenderer)->Draw(ProjectedView);
    }

    #pragma region Legacy



    //if (a_Meshes->size() > 0)
    //{
    //    // Get Draw Order and Material buffers for batch rendering
    //    list<URenderer*> DrawOrderBuffer;
    //    list<RMaterial*> MaterialBuffer;
    //    CalculateDrawOrder(DrawOrderBuffer, MaterialBuffer);

    //    // ########################################################
    //    //
    //    // TODO: Fix up the batching because this doesnt even work
    //    // 
    //    // ########################################################

    //    // TEMP
    //    auto FirstMat = DrawOrderBuffer.front()->GetMaterial();
    //    FirstMat->m_Shader->bind();
    //    FirstMat->Bind();
    //    FirstMat->m_Shader->bindUniform("CameraPosition", vec4(m_RenderCamera->GetPosition(), 1));

    //    if (m_AmbientLight && m_AmbientLight->IsEnabled())
    //    {
    //        FirstMat->m_Shader->bindUniform("AmbientLight", m_AmbientLight->GetColour());
    //    }
    //    else
    //    {
    //        FirstMat->m_Shader->bindUniform("AmbientLight", vec3());
    //    }

    //    FirstMat->m_Shader->bindUniform("NumOfLights", ActiveNumOfLights);
    //    if (ActiveNumOfLights > 0)
    //    {
    //        FirstMat->m_Shader->bindUniform("PointLightColors", ActiveNumOfLights, &PointLightColours[0]);
    //        FirstMat->m_Shader->bindUniform("PointLightPositions", ActiveNumOfLights, &PointLightPositions[0]);
    //        FirstMat->m_Shader->bindUniform("PointLightFallOffs", ActiveNumOfLights, &PointLightFallOffs[0]);
    //    }



    //    URenderer* PreviousRenderer = nullptr;
    //    for (auto Renderer : DrawOrderBuffer)
    //    {
    //        if (PreviousRenderer && Renderer->GetMesh() != PreviousRenderer->GetMesh())
    //        {
    //            auto Material = Renderer->GetMaterial();
    //            Material->m_Shader->bind();
    //            Material->Bind();
    //            Material->m_Shader->bindUniform("CameraPosition", vec4(m_RenderCamera->GetPosition(), 1));

    //            if (m_AmbientLight && m_AmbientLight->IsEnabled())
    //            {
    //                Material->m_Shader->bindUniform("AmbientLight", m_AmbientLight->GetColour());
    //                //Material->m_Shader.bindUniform("AmbientLightDirection", normalize(m_AmbientLight->GetRotation()));
    //            }
    //            else
    //            {
    //                Material->m_Shader->bindUniform("AmbientLight", vec3());
    //            }

    //            Material->m_Shader->bindUniform("NumOfLights", ActiveNumOfLights);
    //            if (ActiveNumOfLights > 0)
    //            {
    //                Material->m_Shader->bindUniform("PointLightColors", ActiveNumOfLights, &PointLightColours[0]);
    //                Material->m_Shader->bindUniform("PointLightPositions", ActiveNumOfLights, &PointLightPositions[0]);
    //                Material->m_Shader->bindUniform("PointLightFallOffs", ActiveNumOfLights, &PointLightFallOffs[0]);
    //            }
    //        }

    //        Renderer->Draw(ProjectedView);

    //        PreviousRenderer = Renderer;
    //    }
    //}

    #pragma endregion

#pragma region Gizmos

    if (a_AmbientLight && a_AmbientLight->IsEnabled())
        aie::Gizmos::addSphere(a_AmbientLight->GetPosition(), a_AmbientLight->GetScale().x * 0.5f, 7, 7, vec4(a_AmbientLight->m_Colour, 0.75f));

    for (int i = 0; i < NumOfLights; i++)
    {
        aie::Gizmos::addSphere((*a_Lights)[i]->GetPosition(), (*a_Lights)[i]->GetScale().x * 0.5f, 7, 7, vec4((*a_Lights)[i]->m_Colour, 0.75f));
    }

    // draw a simple grid with gizmos
    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    for (int i = 0; i < 21; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, 0, 10),
            vec3(-10 + i, 0, -10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(10, 0, -10 + i),
            vec3(-10, 0, -10 + i),
            i == 10 ? white : black);
    }

    // Draw Directional Light
    //Gizmos::addSphere(m_DirectionalLight.m_Direction * 100.f, 10.f, 10, 10, vec4((m_DirectionalLight.m_Colour * 0.5f) * m_DirectionalLight.m_Intensity, 1));

    Gizmos::draw(ProjectedView);

#pragma endregion
}

void ACamera::EndRender()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->unbind();
	}

    ClearScreen();
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
    GetRenderTarget()->getTarget(0).bind(0);

    m_PostProcessQuad.Draw();

    GetRenderTarget()->unbind();
}

void ACamera::SetViewMatrix()
{	
	m_ViewTransform = lookAt(m_Position, m_Position + GetForward(), vec3(0, 1, 0));
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_AspectRatio, float a_Near, float a_Far)
{
	m_FOVDegrees = a_FOVDegrees;
	m_AspectRatio = a_AspectRatio;
	m_Near = a_Near;
	m_Far = a_Far;

	m_ProjectionViewTransform = perspective(radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_Width, float a_Height, float a_Near, float a_Far)
{
	SetProjectionMatrix(a_FOVDegrees, a_Width / a_Height, a_Near, a_Far);
}

void ACamera::UpdateProjectionMatrix()
{
    m_ProjectionViewTransform = perspective(radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
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

vec3 ACamera::GetForward()
{
	float ThetaR = radians(m_Rotation.y);
	float PhiR = radians(m_Rotation.z);
	return vec3(cos(PhiR) * cos(-ThetaR), sin(PhiR), cos(PhiR) * sin(-ThetaR));
}

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

        ImGui::Checkbox("Use Post-Process", &m_UsePostProcessing);
        ImGui::TreePop();
    }
}
