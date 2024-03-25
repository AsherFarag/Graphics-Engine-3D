#include "UMeshRenderer.h"

// --- Engine ---
#include "AActor.h"
#include "RMesh.h"
#include "ResourceManager.h"

UMeshRenderer::UMeshRenderer(AActor* a_Owner)
	: URenderer(a_Owner)
{

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



void UMeshRenderer::Draw_ImGui()
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
		if (m_Mesh)
		{
			ImGui::Text("Mesh: [%s]", m_Mesh->GetResourceName().c_str() );

			if (ImGui::BeginMenu("Set Mesh"))
			{
				auto& LoadedMeshes = ResourceManager::GetLoadedMeshes();
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
