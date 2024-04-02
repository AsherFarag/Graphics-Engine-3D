#include "UMeshRenderer.h"

// --- Engine ---
#include "RenderingManager.h"
#include "AActor.h"
#include "RMesh.h"
#include "ResourceManager.h"

UMeshRenderer::UMeshRenderer(AActor* a_Owner)
	: URenderer(a_Owner)
{
	m_RenderingManager->AddMeshRenderer(this);
}

UMeshRenderer::~UMeshRenderer()
{

}

void UMeshRenderer::Draw(mat4 a_ProjectionViewMatrix)
{
	// bind transforms for lighting
	m_Material->m_Shader->bindUniform("ProjectionViewModel", a_ProjectionViewMatrix * *m_Owner->GetTransform());
	m_Material->m_Shader->bindUniform("ModelMatrix", *m_Owner->GetTransform());

	m_Mesh->draw(false);
}

void UMeshRenderer::OnEnabled()
{
	if (m_RenderingManager)
		m_RenderingManager->AddMeshRenderer(this);
}

void UMeshRenderer::OnDisabled()
{
	if (m_RenderingManager)
		m_RenderingManager->RemoveMeshRenderer(this);
}

bool UMeshRenderer::SetMesh(const char* a_MeshName, bool a_LoadTextures, bool a_FlipTextureV)
{
	m_Mesh = ResourceManager::LoadOBJMesh(a_MeshName, nullptr, a_LoadTextures, a_FlipTextureV);
	SetMaterial(m_Mesh->getMaterial(0));
	return m_Mesh != nullptr;
}


void UMeshRenderer::Draw_ImGui()
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
		if (m_Mesh)
		{
			ImGui::Text("Mesh: [%s]", m_Mesh->GetResourceName().c_str() );

			if (ImGui::BeginMenu("Set Mesh"))
			{
				auto& LoadedMeshes = ResourceManager::GetLoadedOBJMeshes();
				for (auto i : LoadedMeshes)
				{
					if (i.second == GetMesh())
						continue;

					// If Pressed, set the Mesh to this
					if (ImGui::MenuItem(i.second->GetResourceName().c_str()))
						SetMesh(i.first.c_str());
				}

				ImGui::EndMenu();
			}
		}

		ImGui::TreePop();
	}
}

