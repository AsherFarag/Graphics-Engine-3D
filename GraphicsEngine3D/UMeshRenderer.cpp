#include "UMeshRenderer.h"

// --- Engine ---
#include "RenderManager.h"
#include "AActor.h"
#include "RMesh.h"
#include "ResourceManager.h"

UMeshRenderer::UMeshRenderer(AActor* a_Owner)
	: URenderer(a_Owner)
{
	GraphicsEngine3DApp::GetRenderManager()->AddMeshRenderer( this );
}

UMeshRenderer::~UMeshRenderer()
{
	GraphicsEngine3DApp::GetRenderManager()->RemoveMeshRenderer( this );
}

void UMeshRenderer::Draw()
{
	m_Mesh->Draw();
}

void UMeshRenderer::OnEnabled()
{
	//if (m_RenderingManager)
	//	m_RenderingManager->AddMeshRenderer(this);
}

void UMeshRenderer::OnDisabled()
{
	//if (m_RenderingManager)
	//	m_RenderingManager->RemoveMeshRenderer(this);
}

bool UMeshRenderer::SetMesh(const char* a_MeshName, bool a_LoadTextures, bool a_FlipTextureV)
{
	//m_OldMesh = ResourceManager::LoadOBJMesh(a_MeshName, nullptr, a_LoadTextures, a_FlipTextureV);
	//SetMaterial(m_OldMesh->getMaterial(0));

	//Resource::LoadMesh()
	return m_OldMesh != nullptr;
}

bool UMeshRenderer::SetMesh( const string& a_Name )
{
	m_Mesh = Resource::LoadMesh( a_Name );
	return m_Mesh != nullptr;
}

void UMeshRenderer::SetMesh( MeshHandle a_Mesh )
{
	m_Mesh = a_Mesh;
}


void UMeshRenderer::Draw_ImGui()
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
		if (m_OldMesh)
		{
			ImGui::Text("Mesh: [%s]", m_OldMesh->GetResourceName().c_str() );

			//if (ImGui::BeginMenu("Set Mesh"))
			//{
			//	auto& LoadedMeshes = ResourceManager::GetLoadedOBJMeshes();
			//	for (auto i : LoadedMeshes)
			//	{
			//		if (i.second == GetMesh())
			//			continue;

			//		// If Pressed, set the Mesh to this
			//		if (ImGui::MenuItem(i.second->GetResourceName().c_str()))
			//			SetMesh(i.first.c_str());
			//	}

			//	ImGui::EndMenu();
			//}
		}

		ImGui::TreePop();
	}
}

