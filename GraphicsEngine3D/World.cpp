#include "World.h"

// --- AIE ---
#include "Input.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- Engine ---
#include "UBaseObject.h"
#include "AActor.h"
#include "ACamera.h"
#include "AFlyCamera.h"
#include "AStaticMesh.h"
#include "ALight.h"
#include "RenderManager.h"
#include "ResourceManager.h"



World::World()
{
    m_Objects = std::list<UBaseObject*>();
    m_Actors = std::list<AActor*>();
}

World::~World()
{
}

bool World::Begin()
{
    m_MainCamera = new AFlyCamera();
    //auto mesh = new AStaticMesh();
    //mesh->GetMesh()->SetMesh( Resource::LoadMesh( "Box.obj", false ) );
    //mesh->GetMesh()->SetMaterial( Resource::GetMaterialInstance( "Default" ) );


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
    auto renderManager = GraphicsEngine3DApp::GetInstance()->m_RenderManager;

    renderManager->RenderPreProcess( m_MainCamera );

    // Draw whatever you want here
    // ======================================================
    auto& skellie = GraphicsEngine3DApp::GetInstance()->skellie;
    skellie->Draw();

    auto& anim = AnimationLoader::GetInstance()->GetAnimation( "SomeAnim" );
    //skellie->EvaluatePose( anim, 0, skellie->m_Pose );
    timestep += m_DeltaTime;
    if ( timestep >= (1.f / anim->GetTickRate() ) )
    {
        animTime += m_DeltaTime;
        skellie->EvaluatePose( anim, animTime, skellie->m_Pose );
        if ( animTime >= anim->GetPlayLength() )
        {
            animTime = 0.f;
        }
        timestep = 0.f;
    }

    Gizmos::addSphere( vec3( 0 ), 0.01f, 10, 10, vec4( 0,1,0,1 ) );

    // ======================================================

    renderManager->Render( m_MainCamera );

    // Currently does nothing
    renderManager->RenderPostProcess( nullptr );

    m_MainCamera->m_Viewport.Draw();
}

void World::End()
{
    // - Clean up all objects -
    for (auto Object : m_Objects)
    {
        Object->Destroy();
    }

    DestroyPendingObjects();
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