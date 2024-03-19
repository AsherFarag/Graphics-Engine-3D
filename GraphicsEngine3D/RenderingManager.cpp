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

void RenderingManager::Draw()
{
    if (m_RenderCamera == nullptr)
        return;

    mat4 ProjectedView = m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix();

    std::list<URenderer*> DrawOrderBuffer;
    std::list<RMaterial*> MaterialBuffer;
    CalculateDrawOrder(DrawOrderBuffer, MaterialBuffer);

    vector<vec3> PointLightColours = GetPointLightColours();
    vector<vec3> PointLightPositions = GetPointLightPositions();

    for (auto Material : MaterialBuffer)
    {
        Material->m_Shader.bind();
        Material->m_Shader.bindUniform("CameraPosition", vec4(m_RenderCamera->GetPosition(), 1));

        if (m_AmbientLight)
            Material->m_Shader.bindUniform("AmbientLight",  m_AmbientLight->GetColour());

        if (m_Lights.size() > 0)
        {
            Material->m_Shader.bindUniform("NumOfLights", m_NumOfLights);
            Material->m_Shader.bindUniform("PointLightColors", m_NumOfLights, &PointLightColours[0]);
            Material->m_Shader.bindUniform("PointLightPositions", m_NumOfLights, &PointLightPositions[0]);
        }
    }

    for (auto Renderer : DrawOrderBuffer)
    {
        Renderer->Draw(ProjectedView);
    }

    for (int i = 0; i < m_NumOfLights; i++)
    {
        aie::Gizmos::addSphere(m_Lights[i]->GetPosition(), m_Lights[i]->m_Intensity * 0.1f, 7, 7, vec4(m_Lights[i]->m_Colour * m_Lights[i]->m_Intensity, 1));
    }
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

    OutDrawBuffer.sort([&](URenderer* a, URenderer* b) -> bool { return a->GetMaterial() != b->GetMaterial(); });
}

vector<vec3> RenderingManager::GetPointLightPositions()
{
    vector<vec3> PointLightPositions;
    for (auto L : m_Lights)
        PointLightPositions.push_back(L->m_Position);

    return PointLightPositions;
}

vector<vec3> RenderingManager::GetPointLightColours()
{
    vector<vec3> PointLightColours;
    for (auto L : m_Lights)
        PointLightColours.push_back(L->GetColour());

    return PointLightColours;
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
    }
    return false;
}
