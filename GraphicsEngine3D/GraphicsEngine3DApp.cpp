#include "GraphicsEngine3DApp.h"

// --- AIE ---
#include "Gizmos.h"
#include "Input.h"
#include "gl_core_4_4.h"

// --- GLM ---
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/fwd.hpp>

// --- Engine ---
#include "AFlyCamera.h"
#include "RResource.h"
#include "AStaticMesh.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "RMaterial.h"
#include "World.h"
#include "Utilities.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;
using aie::Gizmos;

GraphicsEngine3DApp::GraphicsEngine3DApp()
{
}

GraphicsEngine3DApp::~GraphicsEngine3DApp()
{
	if ( m_World )
		delete m_World;
}

GraphicsEngine3DApp* GraphicsEngine3DApp::GetInstance()
{
	if ( s_Instance == nullptr )
		s_Instance = new GraphicsEngine3DApp();

	return s_Instance;
}

bool GraphicsEngine3DApp::startup()
{
	#if IS_EDITOR

	Editor::SetEditorStyle( ES_MIDNIGHT );

	m_DebugLog = Debug::ImGui_DebugLog();
	m_DebugLog.PrintMessage( Debug::DebugMessage( false, "===== Begin World =====", Debug::Default, ImVec4( 0, 1, 0, 1 ) ) );

	#endif // IS_EDITOR

	setBackgroundColour( 0.15f, 0.15f, 0.15f );

	// initialise gizmo primitive counts
	Gizmos::create( 10000, 10000, 10000, 10000 );

	// Initialise Loaders
	MeshLoader::GetInstance();
	ShaderLoader::GetInstance();
	MaterialLoader::GetInstance();
	TextureLoader::GetInstance();

	m_RenderManager = new RenderManager();
	m_World = new World();
	
	Assimp::Importer importer;
	importer.SetPropertyBool( AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false );
	importer.SetPropertyFloat( AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f );

	//unsigned int propertyFlags = aiProcess_GlobalScale | aiProcess_OptimizeMeshes | aiProcess_RemoveRedundantMaterials | aiProcess_PopulateArmatureData | aiProcess_Triangulate;

	unsigned int propertyFlags = aiProcess_GlobalScale |
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		//aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		//aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		0;

	const aiScene* Scene = importer.ReadFile( "Content/Mesh/TheBoss.fbx", propertyFlags );
	auto mesh = MeshLoader::GetInstance()->LoadMesh( "Content/Mesh/TheBoss.fbx", "HipHop_Mesh", Scene );
	//skellie = MeshLoader::GetInstance()->LoadSkeleton( "Content/Mesh/TheBoss.fbx", "HipHop_Skeleton", Scene->mRootNode->FindNode("mixamorig:Hips"));
	
	//AnimationHandle Anim = AnimationLoader::GetInstance()->LoadAnimation( "Content/Mesh/TheBoss.fbx", "SomeAnim", Scene, 0 );
	

	if ( !m_World->Begin() )
		return false;
	
	DebugPrint( Scene->mRootNode );

	return true;
}

void GraphicsEngine3DApp::shutdown()
{
	if ( m_World )
		m_World->End();

	delete m_RenderManager;

	Gizmos::destroy();
}

void GraphicsEngine3DApp::update( float deltaTime )
{
	DEBUG_TIME_DECLARE;
	DEBUG_TIME_BEGIN;

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if ( input->isKeyDown( aie::INPUT_KEY_ESCAPE ) )
		quit();

	if ( m_World )
	{
		m_World->m_DeltaTime = deltaTime;
		m_World->Update();
	}

	// Get the time it took to complete this function
#if IS_EDITOR
	m_UpdateTime = DEBUG_TIME_END( 1000.f );
#endif // IS_EDITOR
}

void GraphicsEngine3DApp::draw()
{
#if IS_EDITOR

#pragma region ImGui

#pragma region Main Tool Bar

	if ( ImGui::BeginMainMenuBar() )
	{
		if ( ImGui::BeginMenu( "Add Actor" ) )
		{
			AActor* NewActor = nullptr;
			vec3 SpawnPos = vec3( 0 );
			if ( m_World->m_MainCamera )
			{
				SpawnPos = m_World->m_MainCamera->GetActorPosition() + m_World->m_MainCamera->GetForward() * 5.f;
			}

			if ( ImGui::MenuItem( "Light" ) )
			{
				//NewActor = new ALight();
			}

			if ( ImGui::MenuItem( "Static Mesh" ) )
			{
				AStaticMesh* StaticMesh;
				// Static Mesh Set Up
				StaticMesh = new AStaticMesh();
				//StaticMesh->GetMesh()->SetMaterial(m_SoulspearMat);

				NewActor = StaticMesh;
			}

			if ( ImGui::MenuItem( "Camera" ) )
			{
				auto C = new ACamera();
				C->Begin();
			}

			if ( NewActor )
			{
				NewActor->SetActorPosition( SpawnPos );
				m_World->m_InspectedActor = NewActor;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

#pragma endregion

#pragma region Editor

	ImGuiWindowFlags Window_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowPos( ImVec2( 0, 20 ) );
	ImGui::SetNextWindowSize( ImVec2( getWindowWidth(), getWindowHeight() ) );
	if ( ImGui::Begin( "Editor", nullptr, Window_Flags ) )
	{
		ImGuiID idWind = ImGui::GetID( "Editor" );

		ImGui::DockSpace( idWind, ImVec2( 0, 0 ) );
	}
	ImGui::End();

#pragma endregion

#pragma region Scene Hierarchy

	if ( m_World )
	{
	// Create Scene Hierarchy Window
	ImGui::Begin( "Scene Hierarchy", nullptr );

	// Print num of Actors
	ImGui::Text( "Number of Actors: %i", m_World->m_Actors.size() );

	int ActorCount = 1;
	// Create a List of Actors
	if ( ImGui::BeginListBox( "Actors", ImGui::GetContentRegionAvail() ) )
	{
		if ( ImGui::Button( "Destroy All Actors" ) )
		{
			for ( auto Actor : m_World->m_Actors )
			{
				if ( Actor == m_World->m_MainCamera )
					continue;

				Actor->Destroy();
			}
		}

		for ( auto Actor : m_World->m_Actors )
		{
			bool IsSelected = ( m_World->m_InspectedActor == nullptr ? false : Actor->GetId() == m_World->m_InspectedActor->GetId() );
			if ( ImGui::Selectable( ( std::to_string( ActorCount ) + ": " + Actor->GetName() ).c_str(), IsSelected ) )
				m_World->m_InspectedActor = Actor;

			ActorCount++;
		}
		ImGui::EndListBox();
	}

	ImGui::End();

	}

#pragma endregion

#pragma region Inspector

	ImGui::Begin( "Inspector" );
	if ( m_World && m_World->m_InspectedActor )
	{
		m_World->m_InspectedActor->Draw_ImGui();
		ImVec4 OldColour = ImGui::GetStyle().Colors[ ImGuiCol_Button ];
		ImGui::GetStyle().Colors[ ImGuiCol_Button ] = ImVec4( 0.8f, 0.15f, 0.25f, 1 );
		if ( ImGui::Button( "DESTROY" ) )
		{
			m_World->m_InspectedActor->Destroy();
			m_World->m_InspectedActor = nullptr;
		}
		ImGui::GetStyle().Colors[ ImGuiCol_Button ] = OldColour;
	}
	ImGui::End();

#pragma endregion

#pragma region Stats

	ImGui::Begin( "Stats" );

	ImGui::Text( "FPS: %i", m_fps );
	ImGui::Text( "Delta Time: %.4fms", m_frameTime );
	ImGui::Text( "Update Time: %.4fms", m_UpdateTime );

	m_RenderManager->m_RenderStats.PrintStats();

	ImGui::End();

#pragma endregion

#pragma region Shaders

	ImGui::Begin( "Shader" );

	if ( ImGui::Button( "Reload Shaders" ) )
	{
		//ResourceManager::ReloadShaders();
		LOG( Default, " --- Reloaded Shaders ---" );
	}

	ImGui::End();



#pragma endregion

#pragma region Debug Log

	m_DebugLog.Draw();

#pragma endregion

#pragma region Resource Manager
	m_ResourceManagerView.Draw();
#pragma endregion

#pragma endregion

#else

	//m_MainCamera->GetRenderTarget()->bindRead();

#endif // IS_EDITOR

	if ( m_World )
		m_World->Draw();

}