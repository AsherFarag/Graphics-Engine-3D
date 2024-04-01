#include "World.h"

// --- AIE ---
#include "Input.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- Engine ---
#include "UBaseObject.h"
#include "ACamera.h"
#include "AFlyCamera.h"
#include "AStaticMesh.h"
#include "ALight.h"
#include "RenderingManager.h"
#include "ResourceManager.h"

#if IS_EDITOR

#include "Editor.h"
#include "ImGui_DebugLog.h"

#endif

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

    ResourceManager::LoadShader("PostProcess");

    #pragma region Default Material Set up

    RMaterial* DefaultMaterial = ResourceManager::InstantiateMaterial("Default");
    DefaultMaterial->Ambient = vec3(0.8f);
    DefaultMaterial->SpecularPower = 1.f;

    // textures
    DefaultMaterial->DiffuseTexture.load("Resources/Mesh/Default.tga");
    DefaultMaterial->NormalTexture.load("Resources/Mesh/DefaultNormal.tga");

    RMaterial* SkyMaterial = ResourceManager::InstantiateMaterial("Sky");
    SkyMaterial->Ambient = vec3(1.f);

    // textures
    SkyMaterial->DiffuseTexture.load("Resources/Mesh/Sky.jpeg");

    RMaterial* CameraMaterial = ResourceManager::InstantiateMaterial("Camera");
    CameraMaterial->Ambient = vec3(0.1f, 0.3f, 1.f);

    // textures
    CameraMaterial->DiffuseTexture.load("Resources/Mesh/Default.tga");


    #pragma endregion

    // Load Primitive Meshes
    ResourceManager::LoadMesh("Primitives/Box", DefaultMaterial);
    ResourceManager::LoadMesh("Primitives/Cone", DefaultMaterial);
    ResourceManager::LoadMesh("Primitives/Cylinder", DefaultMaterial);
    ResourceManager::LoadMesh("Primitives/Pyramid", DefaultMaterial);
    ResourceManager::LoadMesh("Primitives/Sphere", DefaultMaterial);

    ResourceManager::LoadMesh("Robot/Robot", nullptr, true, true);

    // Default Light Set up
    m_RenderingManager = new RenderingManager();
    ALight* AmbientLight = new ALight();
    AmbientLight->SetName("Ambient Light");
    AmbientLight->SetAmbient(true);
    m_RenderingManager->SetAmbientLight(AmbientLight);

    // Camera Set Up
    m_MainCamera = new AFlyCamera();
    m_MainCamera->SetPosition(vec3(-10, 1, 0));

    // Create SoulSpear
    for (int i = 0; i < 64; i++)
    {
        int Rows = 8;
        int Cols = 8;
        AStaticMesh* StaticMesh;
        // Static Mesh Set Up
        StaticMesh = new AStaticMesh("soulspear/soulspear", true, true);

        StaticMesh->SetPosition(vec3(((i % Rows) * 2.f - i * 2) * 0.5f, 0, (i % Cols) * 2.f));
    }

    AStaticMesh* Plane = new AStaticMesh("Primitives/Box");
    Plane->SetName("Plane");
    Plane->SetScale({ 20.f, 0.1f, 20.f });
    Plane->SetPosition({ 0.f, -0.051f, 0.f });

    AStaticMesh* Sky = new AStaticMesh("Primitives/Sphere");
    Sky->GetMesh()->SetMaterial(SkyMaterial);
    Sky->SetName("Sky");
    Sky->SetScale({ 1000.f, 1000.f, -1000.f });
    
#if IS_EDITOR

    Editor::SetEditorStyle(Editor::ES_MIDNIGHT);

    m_DebugLog = Debug::ImGui_DebugLog();
    m_DebugLog.PrintMessage(Debug::DebugMessage(false, "===== Begin World =====", Debug::Default, ImVec4(0, 1, 0, 1)));

#endif // IS_EDITOR




    #pragma endregion

    for (auto Actor : m_Actors)
    {
        Actor->Begin();
    }

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

    // Get the Rendering Manager to draw URenderers
    m_RenderingManager->Draw();
#if IS_EDITOR
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
    if (ImGui::BeginListBox("Actors", ImGui::GetContentRegionAvail()))
    {
        if (ImGui::Button("Destroy All Actors"))
        {
            for (auto Actor : m_Actors)
            {
                if (Actor == m_MainCamera)
                    continue;

                Actor->Destroy();
            }
        }

        for (auto Actor : m_Actors)
        {
            bool IsSelected = (m_InspectedActor == nullptr ? false : Actor->GetId() == m_InspectedActor->GetId());
            if (ImGui::Selectable((std::to_string(ActorCount) + ": " + Actor->GetName()).c_str(), IsSelected))
                m_InspectedActor = Actor;

            ActorCount++;
        }
        ImGui::EndListBox();
    }

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
                SpawnPos = m_MainCamera->GetPosition() + m_MainCamera->GetForward() * 5.f;
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

            if (ImGui::MenuItem("Camera"))
            {
                auto C = new ACamera();
                C->Begin();
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
        World::DebugLog(Debug::DebugMessage(true, " --- Reloaded Shaders ---"));
    }

    ImGui::End();



#pragma endregion

#pragma region Debug Log

    m_DebugLog.Draw();

#pragma endregion

    m_ResourceManagerView.Draw();

    m_RenderingManager->DrawViewports();


#pragma endregion
#endif // IS_EDITOR
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


void World::DebugLog(Debug::DebugMessage a_Log)
{
    GetWorld()->m_DebugLog.PrintMessage(a_Log);
}

#pragma region Object Management

bool World::AddObject(UBaseObject* a_Object)
{
    if (std::find(m_Objects.begin(), m_Objects.end(), a_Object) == m_Objects.end())
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