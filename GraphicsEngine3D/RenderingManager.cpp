#include "RenderingManager.h"

// --- Engine ---
#include "URenderer.h"
#include "RMaterial.h"
#include "ACamera.h"
#include "ALight.h"

RenderingManager::RenderingManager()
{

}

RenderingManager::~RenderingManager()
{

}

bool RenderingManager::StartUp()
{
    return true;
}       

void RenderingManager::Draw()
{
    if (m_RenderCamera == nullptr)
        return;

    // Begin Render
    m_RenderCamera->BeginRender();

    // Calculate the Projected View Matrix of the Render Camera
    mat4 ProjectedView = m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix();

    // Draw Renderers
    if (m_Renderers.size() > 0)
    {
        // Get Draw Order and Material buffers for batch rendering
        std::list<URenderer*> DrawOrderBuffer;
        std::list<RMaterial*> MaterialBuffer;
        CalculateDrawOrder(DrawOrderBuffer, MaterialBuffer);

        // Get arrays of the lights values
        int ActiveNumOfLights = GetActiveNumOfLights();
        vector<vec3> PointLightColours = GetPointLightColours();
        vector<vec3> PointLightPositions = GetPointLightPositions();
        vector<float> PointLightFallOffs = GetPointLightFallOffs();

        // ########################################################
        //
        // TODO: Fix up the batching because this doesnt even work
        // 
        // ########################################################

        // TEMP
        auto FirstMat = DrawOrderBuffer.front()->GetMaterial();
        FirstMat->m_Shader->bind();
        FirstMat->Bind();
        FirstMat->m_Shader->bindUniform("CameraPosition", vec4(m_RenderCamera->GetPosition(), 1));

        if (m_AmbientLight && m_AmbientLight->IsEnabled())
        {
            FirstMat->m_Shader->bindUniform("AmbientLight", m_AmbientLight->GetColour());
        }
        else
        {
            FirstMat->m_Shader->bindUniform("AmbientLight", vec3());
        }

        FirstMat->m_Shader->bindUniform("NumOfLights", ActiveNumOfLights);
        if (ActiveNumOfLights > 0)
        {
            FirstMat->m_Shader->bindUniform("PointLightColors", ActiveNumOfLights, &PointLightColours[0]);
            FirstMat->m_Shader->bindUniform("PointLightPositions", ActiveNumOfLights, &PointLightPositions[0]);
            FirstMat->m_Shader->bindUniform("PointLightFallOffs", ActiveNumOfLights, &PointLightFallOffs[0]);
        }



        URenderer* PreviousRenderer = nullptr;
        for (auto Renderer : DrawOrderBuffer)
        {
            if (PreviousRenderer && Renderer->GetMesh() != PreviousRenderer->GetMesh())
            {
                auto Material = Renderer->GetMaterial();
                Material->m_Shader->bind();
                Material->Bind();
                Material->m_Shader->bindUniform("CameraPosition", vec4(m_RenderCamera->GetPosition(), 1));

                if (m_AmbientLight && m_AmbientLight->IsEnabled())
                {
                    Material->m_Shader->bindUniform("AmbientLight", m_AmbientLight->GetColour());
                    //Material->m_Shader.bindUniform("AmbientLightDirection", normalize(m_AmbientLight->GetRotation()));
                }
                else
                {
                    Material->m_Shader->bindUniform("AmbientLight", vec3());
                }

                Material->m_Shader->bindUniform("NumOfLights", ActiveNumOfLights);
                if (ActiveNumOfLights > 0)
                {
                    Material->m_Shader->bindUniform("PointLightColors", ActiveNumOfLights, &PointLightColours[0]);
                    Material->m_Shader->bindUniform("PointLightPositions", ActiveNumOfLights, &PointLightPositions[0]);
                    Material->m_Shader->bindUniform("PointLightFallOffs", ActiveNumOfLights, &PointLightFallOffs[0]);
                }
            }

            Renderer->Draw(ProjectedView);

            PreviousRenderer = Renderer;
        }
    }

#pragma region Gizmos

    if (m_AmbientLight && m_AmbientLight->IsEnabled())
        aie::Gizmos::addSphere(m_AmbientLight->GetPosition(), m_AmbientLight->GetScale().x * 0.5f, 7, 7, vec4(m_AmbientLight->m_Colour, 0.75f));

    for (int i = 0; i < m_NumOfLights; i++)
    {
        if (m_Lights[i]->IsEnabled())
            aie::Gizmos::addSphere(m_Lights[i]->GetPosition(), m_Lights[i]->GetScale().x * 0.5f, 7, 7, vec4(m_Lights[i]->m_Colour, 0.75f));
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

    // Finish Render
    m_RenderCamera->EndRender(); 
}

void RenderingManager::CalculateDrawOrder(std::list<URenderer*>& OutDrawBuffer, std::list<RMaterial*>& OutMaterialBuffer)
{
    for (auto R : m_Renderers)
    {
        OutDrawBuffer.push_front(R);
        auto M = std::find(OutMaterialBuffer.begin(), OutMaterialBuffer.end(), R->GetMaterial());
        if (M == OutMaterialBuffer.end())
        {
            OutMaterialBuffer.push_front(R->GetMaterial());
        }
    }

    //OutDrawBuffer.sort([&](URenderer* a, URenderer* b) -> bool { return a->GetMaterial() != b->GetMaterial(); });
}

int RenderingManager::GetActiveNumOfLights()
{
    int i = 0;
    for (auto L : m_Lights)
        if (L->IsEnabled())
            i++;

    return i;
}

vector<vec3> RenderingManager::GetPointLightPositions()
{
    vector<vec3> PointLightPositions;
    for (auto L : m_Lights)
        if (L->IsEnabled())
         PointLightPositions.push_back(L->m_Position);

    return PointLightPositions;
}

vector<vec3> RenderingManager::GetPointLightColours()
{
    vector<vec3> PointLightColours;
    for (auto L : m_Lights)
        if (L->IsEnabled())
            PointLightColours.push_back(L->GetColour());

    return PointLightColours;
}

vector<float> RenderingManager::GetPointLightFallOffs()
{
    vector<float> PointLightFallOffs;
    for (auto L : m_Lights)
        if (L->IsEnabled())
            PointLightFallOffs.push_back(L->m_FallOff);

    return PointLightFallOffs;
}

bool RenderingManager::AddRenderer(URenderer* a_Renderer)
{
    // If this renderer already exists inside of m_Renderers, or is nullptr
    if (std::find(m_Renderers.begin(), m_Renderers.end(), a_Renderer) != m_Renderers.end())
        return false;

    m_Renderers.push_back(a_Renderer);
    return true;
}

bool RenderingManager::RemoveRenderer(URenderer* a_Renderer)
{
    auto Iterator = std::find(m_Renderers.begin(), m_Renderers.end(), a_Renderer);
    if (Iterator != m_Renderers.end())
    {
        m_Renderers.erase(Iterator);
        return true;
    }

    return false;
}

void RenderingManager::SetRenderCamera(ACamera* a_NewRenderCamera)
{
    if (a_NewRenderCamera == nullptr)
        return;

    m_RenderCamera = a_NewRenderCamera;
}

bool RenderingManager::AddLight(ALight* a_Light)
{
    if (m_NumOfLights < MAX_LIGHTS)
    {
        m_NumOfLights++;
        m_Lights.push_back(a_Light);
        return true;
    }

    a_Light->SetActive(false);

    World::LogMessage(Debug::DebugMessage(true, "Light Limit Reached. Can not instantiate new light", Debug::Error));
    
    return false;
}

bool RenderingManager::RemoveLight(ALight* a_Light)
{
    if (m_NumOfLights > 0)
    {
        auto i = std::find(m_Lights.begin(), m_Lights.end(), a_Light);
        if (i != m_Lights.end())
        {
            m_NumOfLights--;
            m_Lights.erase(i);
            return true;
        }
    }
    return false;
}

void RenderingManager::SetAmbientLight(ALight* a_AmbientLight)
{    
    if (m_AmbientLight)
    {
        // Add old ambient light to pool of normal lights
        m_AmbientLight->SetAmbient(false);
        AddLight(m_AmbientLight);
    }

    if (a_AmbientLight == nullptr)
    {
        m_AmbientLight = nullptr;
        return;
    }

    // Remove new ambient light from pool of normal lights
    RemoveLight(a_AmbientLight);
    m_AmbientLight = a_AmbientLight;
}
