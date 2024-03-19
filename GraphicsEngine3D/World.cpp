#include "World.h"



// --- Engine ---
#include "ACamera.h"
#include "AFlyCamera.h"
#include "UBaseObject.h"
#include "RenderingManager.h"
#include "AStaticMesh.h"
#include "ALight.h"

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
    m_RenderingManager = new RenderingManager();
    m_AmbientLight = new ALight();
    m_AmbientLight->SetName("Ambient Light");
    m_RenderingManager->m_AmbientLight = m_AmbientLight;

    new ALight();
    new ALight();

    // Camera Set Up
    m_FlyCamera = new AFlyCamera();
    m_MainCamera = m_FlyCamera;
    m_MainCamera->SetAspectRatio(GraphicsEngine3DApp::GetInstance()->getWindowWidth(), GraphicsEngine3DApp::GetInstance()->getWindowWidth());
    m_MainCamera->SetPosition(vec3(-10, 1, 0));

    m_RenderingManager->SetRenderCamera(m_MainCamera);

    // Material Set up
    m_SoulspearMat = new RMaterial();
    m_SoulspearMat->LoadShader("Phong");

    for (int i = 0; i < 4; i++)
    {
        int Rows = 32;
        int Cols = 32;
        AStaticMesh* StaticMesh;
        // Static Mesh Set Up
        StaticMesh = new AStaticMesh("soulspear/soulspear.obj", true, true);
        StaticMesh->GetMesh()->SetMaterial(m_SoulspearMat);

        StaticMesh->SetPosition(vec3(((i % Rows) * 2.f - i * 2) * 0.1f, 0, (i % Cols) * 2.f));

        if (i == 0)
            m_StaticMesh = StaticMesh;
    }

    return true;
}

void World::Update()
{
    for (auto Actor : m_Actors)
    {
        Actor->Update();
    }
    DestroyPendingObjects();
}

void World::Draw()
{
    // wipe the gizmos clean for this frame
    Gizmos::clear();

    m_RenderingManager->Draw();

    #pragma region Gizmos

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

    Gizmos::addTransform(*m_StaticMesh->GetTransform(), 1);

    // Draw Directional Light
    //Gizmos::addSphere(m_DirectionalLight.m_Direction * 100.f, 10.f, 10, 10, vec4((m_DirectionalLight.m_Colour * 0.5f) * m_DirectionalLight.m_Intensity, 1));

    Gizmos::draw(m_MainCamera->GetProjectionMatrix() * m_MainCamera->GetViewMatrix());

    #pragma endregion

    #pragma region ImGui

#pragma region Scene Hierarchy

    ImGui::Begin("Scene Hierarchy");

    ImGui::Text("Number of Actors: %i", m_Actors.size());

    ImGui::SameLine();
    if (ImGui::BeginMenu("Add Actor"))
    {
        if (ImGui::MenuItem("Light"))
            new ALight();

        ImGui::EndMenu();
    }


    int i = 1;
    for (auto Actor : m_Actors)
    {
        ImGui::Text("%i : ", i);
        ImGui::SameLine();
        ImGui::PushID(Actor->GetId());
        if (ImGui::Button(Actor->GetName().c_str()))
            m_InspectedActor = std::shared_ptr<AActor>(Actor);
        ImGui::PopID();

        i++;
    }

    ImGui::End();

#pragma endregion


#pragma region Inspector

    ImGui::Begin("Inspector");

    if (m_InspectedActor)
    {
        m_InspectedActor->Draw_ImGui();
        if (ImGui::Button("DESTROY"))
        {
            m_InspectedActor->Destroy();
            m_InspectedActor = nullptr;
        }
    }

    ImGui::End();

#pragma endregion

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