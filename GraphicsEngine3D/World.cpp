#include "World.h"



// --- Engine ---
#include "UBaseObject.h"
#include "ACamera.h"
#include "AFlyCamera.h"
#include "AStaticMesh.h"
#include "ALight.h"
#include "RenderingManager.h"
#include "ResourceManager.h"

World::World()
{
    // - Set Up Managers -
    m_RenderingManager = new RenderingManager();

    m_Objects = std::list<UBaseObject*>();
    m_Actors = std::list<AActor*>();
}

World::~World()
{

}

bool World::Begin()
{
    ResourceManager::SetMainShader( ResourceManager::LoadShader("Phong") );

    m_PostProcess = ResourceManager::LoadShader("PostProcess");
    m_PostProcess->bindUniform("PostProcess", 0);
    m_PostProcess->bindUniform("colourTarget", 0);

    // create a fullscreen quad
    m_FullScreenQuad.InitialiseFullScreenQuad();

    #pragma region Default Material Set up

    RMaterial* DefaultMaterial = ResourceManager::InstantiateMaterial("Default");
    DefaultMaterial->Ambient = vec3(1.f);
    DefaultMaterial->SpecularPower = 20.f;

    // textures
    DefaultMaterial->DiffuseTexture.load("./bin/Resources/Mesh/Default.tga");
    DefaultMaterial->NormalTexture.load("./bin/Resources/Mesh/DefaultNormal.tga");

    #pragma endregion

    ResourceManager::LoadMesh("Primitives/Box", DefaultMaterial);
    ResourceManager::LoadMesh("Robot/Robot", nullptr, true, true);

    // Default Light Set up
    m_RenderingManager = new RenderingManager();
    ALight* AmbientLight = new ALight();
    AmbientLight->SetName("Ambient Light");
    AmbientLight->SetAmbient(true);
    m_RenderingManager->SetAmbientLight(AmbientLight);

    // Camera Set Up
    m_FlyCamera = new AFlyCamera();
    m_MainCamera = m_FlyCamera;
    m_MainCamera->SetAspectRatio(GraphicsEngine3DApp::GetInstance()->getWindowWidth(), GraphicsEngine3DApp::GetInstance()->getWindowWidth());
    m_MainCamera->SetPosition(vec3(-10, 1, 0));
    m_RenderingManager->SetRenderCamera(m_MainCamera);

    // Create SoulSpear
    for (int i = 0; i < 1; i++)
    {
        int Rows = 8;
        int Cols = 8;
        AStaticMesh* StaticMesh;
        // Static Mesh Set Up
        StaticMesh = new AStaticMesh("soulspear/soulspear", true, true);

        StaticMesh->SetPosition(vec3(((i % Rows) * 2.f - i * 2) * 0.5f, 0, (i % Cols) * 2.f));
    }

    AStaticMesh* Plane = new AStaticMesh("Primitives/Box");
    Plane->SetScale({ 20.f, 0.1f, 20.f });

    #pragma region ImGui

    #pragma region Style

    ImGui::GetStyle().WindowRounding = 0;

    #pragma endregion

    m_DebugLog = Debug::ImGui_DebugLog();
    m_DebugLog.PrintMessage(Debug::DebugMessage(false, "===== Begin World =====", Debug::Default, ImVec4(0,1,0,1)));

    #pragma endregion

    return true;
}

void World::Update()
{
    DestroyPendingObjects();

    for (auto Actor : m_Actors)
    {
        Actor->Update();
    }
}

void World::Draw()
{
    auto EngineInstance = GraphicsEngine3DApp::GetInstance();

    // Wipe the gizmos clean for this frame
    Gizmos::clear();

    // Get the Rendering Manager to draw URenderers
    m_RenderingManager->Draw();

    if (m_UsePostProcess)
    {
        m_PostProcess->bind();

        m_PostProcessPercent += 10.f * DeltaTime();

        m_PostProcess->bindUniform("ProgressPercent", m_PostProcessPercent);


        m_MainCamera->GetRenderTarget()->bind();
        m_MainCamera->GetRenderTarget()->getTarget(0).bind(0);

        m_FullScreenQuad.Draw();

        m_MainCamera->GetRenderTarget()->unbind();
    }

    #pragma region ImGui

    ImGuiWindowFlags Window_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(EngineInstance->getWindowWidth(), EngineInstance->getWindowHeight()));
    if (ImGui::Begin("Editor", nullptr, Window_Flags))
    {
        ImGuiID idWind = ImGui::GetID("Editor");

        ImGui::DockSpace(idWind, ImVec2(0, 0));
    }
    ImGui::End();

    #pragma region Scene Hierarchy

    // Create Scene Hierarchy Window
    ImGui::Begin("Scene Hierarchy", nullptr);

    // Print num of Actors
    ImGui::Text("Number of Actors: %i", m_Actors.size());

    int ActorCount = 1;
    // Create a List of Actors
    ImGui::BeginListBox("Actors", ImGui::GetContentRegionAvail());
    for (auto Actor : m_Actors)
    {
        bool IsSelected = (m_InspectedActor == nullptr ? false : Actor->GetId() == m_InspectedActor->GetId());
        if (ImGui::Selectable((std::to_string(ActorCount) + ": " + Actor->GetName()).c_str(), IsSelected))
            m_InspectedActor = Actor;

        ActorCount++;
    }
    ImGui::EndListBox();

    ImGui::End();

    #pragma endregion

    #pragma region Inspector

    ImGui::Begin("Inspector");
    if (m_InspectedActor)
    {
        m_InspectedActor->Draw_ImGui();
        ImVec4 OldColour = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.15f, 0.25f, 1);
        if (ImGui::Button("DESTROY"))
        {
            m_InspectedActor->Destroy();
            m_InspectedActor = nullptr;
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = OldColour;
    }
    ImGui::End();

    #pragma endregion

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Add Actor"))
        {
            AActor* NewActor = nullptr;
            vec3 SpawnPos = vec3(0);
            if (m_MainCamera)
            {
                SpawnPos = m_MainCamera->GetPosition() + m_MainCamera->GetForward() * 10.f;
            }

            if (ImGui::MenuItem("Light"))
                NewActor = new ALight();

            if (ImGui::MenuItem("Static Mesh"))
            {
                AStaticMesh* StaticMesh;
                // Static Mesh Set Up
                StaticMesh = new AStaticMesh();
                //StaticMesh->GetMesh()->SetMaterial(m_SoulspearMat);

                NewActor = StaticMesh;
            }

            if (NewActor)
            {
                NewActor->SetPosition(SpawnPos);
                m_InspectedActor = NewActor;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    #pragma region Shaders

    ImGui::Begin("Shader");

    if (ImGui::Button("Reload Shaders"))
    {
        ResourceManager::ReloadShaders();

        m_PostProcess->loadShader(aie::eShaderStage::VERTEX, "./bin/shaders/PostProcess.vert");
        m_PostProcess->loadShader(aie::eShaderStage::FRAGMENT, "./bin/shaders/PostProcess.frag");

        LogMessage(Debug::DebugMessage(true, " --- Reloaded Shaders ---"));
    }

    static int PostProcessIndex = 0;
    if (ImGui::SliderInt("Post Process", &PostProcessIndex, 0, 7))
    {
        m_PostProcess->bindUniform("PostProcess", PostProcessIndex);
    }

    // Toon
    if (PostProcessIndex == 4)
    {
        static int ToonScale = 1;
        if (ImGui::SliderInt("ToonScale", &ToonScale, 0, 10))
            m_PostProcess->bindUniform("ToonScale", ToonScale);
    }

    ImGui::Checkbox("Use Post-Process", &m_UsePostProcess);


    ImGui::End();



    #pragma endregion

    #pragma region Debug Log

    m_DebugLog.Draw();

    #pragma endregion

    m_ResourceManagerView.Draw();


    #pragma endregion
}

void World::End()
{
    // - Clean up all objects -
    for (auto Object : m_Objects)
    {
        Object->Destroy();
    }
    DestroyPendingObjects();

    delete m_RenderingManager;
}

#pragma region Object Management

void World::LogMessage(Debug::DebugMessage a_Message)
{
    GetWorld()->m_DebugLog.PrintMessage(a_Message);
}

bool World::AddObject(UBaseObject* a_Object)
{
    if (a_Object)
    {
        a_Object->SetWorld(this);
        a_Object->SetEnabled(true);
        m_Objects.push_back(a_Object);
  
        return true;
    }

    return false;
}

bool World::RemoveObject(UBaseObject* a_Object)
{
    // Check if this object is valid
    if (a_Object)
    {
        // Attempt to find this object in m_Objects
        auto Iterator = std::find(m_Objects.begin(), m_Objects.end(), a_Object);
        if (Iterator != m_Objects.end())
        {
            // If this object is an actor,
            // Remove it from the list of actors.
            auto Actor = dynamic_cast<AActor*>(a_Object);
            if (Actor)
                m_Actors.erase(std::find(m_Actors.begin(), m_Actors.end(), Actor));

            m_Objects.erase(Iterator);
            return true;
        }
    }

    return false;
}

bool World::AddActor(AActor* a_Actor)
{
    if (a_Actor)
    {
        a_Actor->SetActive(true);
        m_Actors.push_back(a_Actor);
        return true;
    }

    return false;
}

void World::DestroyObject(UBaseObject* a_Object)
{
    m_ObjectsPendingDestroy.push_back(a_Object);
}

void World::DestroyPendingObjects()
{
    while (m_ObjectsPendingDestroy.begin() != m_ObjectsPendingDestroy.end())
    {
        RemoveObject(m_ObjectsPendingDestroy.front());
        delete m_ObjectsPendingDestroy.front();
        m_ObjectsPendingDestroy.front() = nullptr;
        m_ObjectsPendingDestroy.pop_front();
    }
}

#pragma endregion