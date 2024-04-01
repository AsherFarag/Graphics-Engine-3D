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
    for (auto Camera : m_RenderCameras)
    {
        if (Camera->IsEnabled() == false)
            continue;

        // Begin Render
        Camera->BeginRender();

        // Get a vector of the active lights
        vector<ALight*> ActiveLights;
        for (auto Light : m_Lights)
        {
            if (Light->IsEnabled())
                ActiveLights.push_back(Light);
        }

        Camera->Render(m_AmbientLight, &ActiveLights, &m_MeshRenderers);

        // Finish Render
        Camera->EndRender();

        if (Camera->IsUsingPostProcessing())
            Camera->ApplyPostProcessing();
    }
}

void RenderingManager::DrawViewports()
{
    for (auto Camera : m_RenderCameras)
        Camera->m_Viewport.Draw();
}

bool RenderingManager::End()
{
    return true;
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

bool RenderingManager::AddMeshRenderer(UMeshRenderer* a_Renderer)
{
    if (AddRenderer(a_Renderer))
    {
        m_MeshRenderers.push_back(a_Renderer);
        return true;
    }

    return false;
}

bool RenderingManager::RemoveMeshRenderer(UMeshRenderer* a_Renderer)
{
    if (RemoveRenderer(a_Renderer))
    {
        m_MeshRenderers.erase(std::find(m_MeshRenderers.begin(), m_MeshRenderers.end(), a_Renderer));
        return true;
    }

    return false;
}

bool RenderingManager::AddRenderCamera(ACamera* a_NewRenderCamera)
{
    if (std::find(m_RenderCameras.begin(), m_RenderCameras.end(), a_NewRenderCamera) != m_RenderCameras.end())
        return false;

    m_RenderCameras.push_back(a_NewRenderCamera);
    return true;
}

bool RenderingManager::RemoveRenderCamera(ACamera* a_NewRenderCamera)
{
    auto i = std::find(m_RenderCameras.begin(), m_RenderCameras.end(), a_NewRenderCamera);
    if (i != m_RenderCameras.end())
    {
        m_RenderCameras.erase(i);
        return true;
    }

    return false;
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

    LOG( Error, true, "Light Limit Reached. Can not instantiate new light" );
    
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
