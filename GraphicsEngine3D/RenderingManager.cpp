#include "RenderingManager.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- Engine ---
#include "URenderer.h"
#include "RMaterial.h"
#include "ACamera.h"
#include "ALight.h"

//RenderingManager::RenderingManager()
//{
//
//}
//
//RenderingManager::~RenderingManager()
//{
//
//}
//
//bool RenderingManager::StartUp()
//{
//    return true;
//}       
//
//void RenderingManager::Draw()
//{
//    for (auto Camera : m_RenderCameras)
//    {
//        if (Camera->IsEnabled() == false)
//            continue;
//
//        // Begin Render
//        Camera->BeginRender();
//
//        Render(Camera);
//
//        // Finish Render
//        Camera->EndRender();
//
//        if (Camera->IsUsingPostProcessing())
//            Camera->ApplyPostProcessing();
//    }
//}
//
//void RenderingManager::Reset()
//{
//    m_Renderers.clear();
//    m_MeshRenderers.clear();
//    m_RenderCameras.clear();
//}
//
//#if IS_EDITOR
//
//void RenderingManager::DrawViewports()
//{
//    for (auto Camera : m_RenderCameras)
//        Camera->m_Viewport.Draw();
//}
//
//#endif // IS_EDITOR
//
//void RenderingManager::Render(ACamera* Camera)
//{
//    // Calculate the Projected View Matrix
//    mat4 ProjectedView = Camera->GetProjectionMatrix() * Camera->GetViewMatrix();
//
//    const int NumOfLights = m_Lights.size();
//    vector<vec3>  LightPositions;
//    vector<vec3>  LightColours;
//    vector<float> LightFallOffs;
//
//    for (auto Light : m_Lights)
//    {
//        if (Light->IsEnabled())
//        {
//            LightPositions.push_back(Light->GetActorPosition());
//            LightColours.push_back(Light->GetColour());
//            LightFallOffs.push_back(Light->m_FallOff);
//        }
//    }
//
//    // Draw Renderers
//    for (auto MeshRenderer = m_MeshRenderers.begin(); MeshRenderer != m_MeshRenderers.end(); ++MeshRenderer)
//    {
//        if (*MeshRenderer == Camera->m_MeshRenderer)
//            continue;
//
//        #pragma message("TODO: Currently each draw call rebinds the material. Set-Up Batch Rendering")
//
//        #pragma region Bind Material
//
//        auto Material = (*MeshRenderer)->GetMaterial();
//        if (Material != nullptr)
//        {
//            //Material->m_Shader->bind();
//            //Material->Bind();
//            //Material->m_Shader->bindUniform("CameraPosition", vec4(Camera->GetActorPosition(), 1));
//
//            //if (m_AmbientLight && m_AmbientLight->IsEnabled())
//            //    Material->m_Shader->bindUniform("AmbientLight", m_AmbientLight->GetColour());
//            //else
//            //    Material->m_Shader->bindUniform("AmbientLight", vec3());
//
//            //Material->m_Shader->bindUniform("NumOfLights", NumOfLights);
//            //if (NumOfLights > 0)
//            //{
//            //    Material->m_Shader->bindUniform("PointLightColors", NumOfLights, &LightColours[0]);
//            //    Material->m_Shader->bindUniform("PointLightPositions", NumOfLights, &LightPositions[0]);
//            //    Material->m_Shader->bindUniform("PointLightFallOffs", NumOfLights, &LightFallOffs[0]);
//            //}
//        }
//
//        #pragma endregion 
//
//        (*MeshRenderer)->Draw(ProjectedView);
//    }
//
//#pragma region Gizmos
//
//    if (m_AmbientLight && m_AmbientLight->IsEnabled())
//        aie::Gizmos::addSphere(m_AmbientLight->GetActorPosition(), m_AmbientLight->GetActorScale().x * 0.5f, 7, 7, vec4(m_AmbientLight->m_Colour, 0.75f));
//
//    for (int i = 0; i < NumOfLights; i++)
//    {
//        aie::Gizmos::addSphere((m_Lights)[i]->GetActorPosition(), (m_Lights)[i]->GetActorScale().x * 0.5f, 7, 7, vec4((m_Lights)[i]->m_Colour, 0.75f));
//    }
//
//    //// draw a simple grid with gizmos
//    //vec4 white(1);
//    //vec4 black(0, 0, 0, 1);
//    //for (int i = 0; i < 21; ++i)
//    //{
//    //    Gizmos::addLine(vec3(-10 + i, 0, 10),
//    //        vec3(-10 + i, 0, -10),
//    //        i == 10 ? white : black);
//    //    Gizmos::addLine(vec3(10, 0, -10 + i),
//    //        vec3(-10, 0, -10 + i),
//    //        i == 10 ? white : black);
//    //}
//
//    // Draw Directional Light
//    //Gizmos::addSphere(m_DirectionalLight.m_Direction * 100.f, 10.f, 10, 10, vec4((m_DirectionalLight.m_Colour * 0.5f) * m_DirectionalLight.m_Intensity, 1));
//
//    Gizmos::draw(ProjectedView);
//
//#pragma endregion
//}
//
//void RenderingManager::DrawShadows(ACamera* Camera)
//{
//    unsigned int DepthMapFBO;
//    glGenFramebuffers(1, &DepthMapFBO);
//
//    unsigned int DepthMap;
//    glGenTextures(1, &DepthMap);
//    glBindTexture(GL_TEXTURE_2D, DepthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
//        m_AmbientLight->m_ShadowResolution.x, m_AmbientLight->m_ShadowResolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    glViewport(0, 0, m_AmbientLight->m_ShadowResolution.x, m_AmbientLight->m_ShadowResolution.y);
//    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
//    glClear(GL_DEPTH_BUFFER_BIT);
//    //ConfigureShaderAndMatrices();
//    Render(Camera);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void RenderingManager::CalculateDrawOrder(std::list<URenderer*>& OutDrawBuffer, std::list<RMaterial*>& OutMaterialBuffer)
//{
//    for (auto R : m_Renderers)
//    {
//        OutDrawBuffer.push_front(R);
//        auto M = std::find(OutMaterialBuffer.begin(), OutMaterialBuffer.end(), R->GetMaterial());
//        if (M == OutMaterialBuffer.end())
//        {
//            //OutMaterialBuffer.push_front(R->GetMaterial());
//        }
//    }
//
//    //OutDrawBuffer.sort([&](URenderer* a, URenderer* b) -> bool { return a->GetMaterial() != b->GetMaterial(); });
//}
//
//int RenderingManager::GetActiveNumOfLights()
//{
//    int i = 0;
//    for (auto L : m_Lights)
//        if (L->IsEnabled())
//            i++;
//
//    return i;
//}
//
//vector<vec3> RenderingManager::GetPointLightPositions()
//{
//    vector<vec3> PointLightPositions;
//    for (auto L : m_Lights)
//        if (L->IsEnabled())
//         PointLightPositions.push_back(L->GetActorPosition());
//
//    return PointLightPositions;
//}
//
//vector<vec3> RenderingManager::GetPointLightColours()
//{
//    vector<vec3> PointLightColours;
//    for (auto L : m_Lights)
//        if (L->IsEnabled())
//            PointLightColours.push_back(L->GetColour());
//
//    return PointLightColours;
//}
//
//vector<float> RenderingManager::GetPointLightFallOffs()
//{
//    vector<float> PointLightFallOffs;
//    for (auto L : m_Lights)
//        if (L->IsEnabled())
//            PointLightFallOffs.push_back(L->m_FallOff);
//
//    return PointLightFallOffs;
//}
//
//bool RenderingManager::AddRenderer(URenderer* a_Renderer)
//{
//    // If this renderer already exists inside of m_Renderers, or is nullptr
//    if (std::find(m_Renderers.begin(), m_Renderers.end(), a_Renderer) != m_Renderers.end())
//        return false;
//
//    m_Renderers.push_back(a_Renderer);
//    return true;
//}
//
//bool RenderingManager::RemoveRenderer(URenderer* a_Renderer)
//{
//    auto Iterator = std::find(m_Renderers.begin(), m_Renderers.end(), a_Renderer);
//    if (Iterator != m_Renderers.end())
//    {
//        m_Renderers.erase(Iterator);
//        return true;
//    }
//
//    return false;
//}
//
//bool RenderingManager::AddMeshRenderer(UMeshRenderer* a_Renderer)
//{
//    if (AddRenderer(a_Renderer))
//    {
//        m_MeshRenderers.push_back(a_Renderer);
//        return true;
//    }
//
//    return false;
//}
//
//bool RenderingManager::RemoveMeshRenderer(UMeshRenderer* a_Renderer)
//{
//    if (RemoveRenderer(a_Renderer))
//    {
//        m_MeshRenderers.erase(std::find(m_MeshRenderers.begin(), m_MeshRenderers.end(), a_Renderer));
//        return true;
//    }
//
//    return false;
//}
//
//bool RenderingManager::AddRenderCamera(ACamera* a_NewRenderCamera)
//{
//    if (std::find(m_RenderCameras.begin(), m_RenderCameras.end(), a_NewRenderCamera) != m_RenderCameras.end())
//        return false;
//
//    m_RenderCameras.push_back(a_NewRenderCamera);
//    return true;
//}
//
//bool RenderingManager::RemoveRenderCamera(ACamera* a_NewRenderCamera)
//{
//    auto i = std::find(m_RenderCameras.begin(), m_RenderCameras.end(), a_NewRenderCamera);
//    if (i != m_RenderCameras.end())
//    {
//        m_RenderCameras.erase(i);
//        return true;
//    }
//
//    return false;
//}
//
//bool RenderingManager::AddLight(ALight* a_Light)
//{
//    if (m_NumOfLights < MAX_LIGHTS)
//    {
//        m_NumOfLights++;
//        m_Lights.push_back(a_Light);
//        return true;
//    }
//
//    a_Light->SetActive(false);
//
//    //LOG( Error, true, "Light Limit Reached. Can not instantiate new light" );
//    
//    return false;
//}
//
//bool RenderingManager::RemoveLight(ALight* a_Light)
//{
//    if (m_NumOfLights > 0)
//    {
//        auto i = std::find(m_Lights.begin(), m_Lights.end(), a_Light);
//        if (i != m_Lights.end())
//        {
//            m_NumOfLights--;
//            m_Lights.erase(i);
//            return true;
//        }
//    }
//    return false;
//}
//
//void RenderingManager::SetAmbientLight(ALight* a_AmbientLight)
//{    
//    if (m_AmbientLight)
//    {
//        // Add old ambient light to pool of normal lights
//        m_AmbientLight->SetAmbient(false);
//        AddLight(m_AmbientLight);
//    }
//
//    if (a_AmbientLight == nullptr)
//    {
//        m_AmbientLight = nullptr;
//        return;
//    }
//
//    // Remove new ambient light from pool of normal lights
//    RemoveLight(a_AmbientLight);
//    m_AmbientLight = a_AmbientLight;
//}
