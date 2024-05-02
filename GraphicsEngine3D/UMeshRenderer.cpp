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

MaterialHandle UMeshRenderer::GetMaterial()
{
	return m_OverrideMaterial == nullptr ? m_Mesh->GetMaterial() : m_OverrideMaterial;
}

void UMeshRenderer::SetMaterial( MaterialHandle a_Material )
{
	m_OverrideMaterial = a_Material;
}

MeshHandle UMeshRenderer::GetMesh()
{
	return m_Mesh;
}

void UMeshRenderer::SetMesh( MeshHandle a_Mesh )
{
	m_Mesh = a_Mesh;
}

void UMeshRenderer::Draw_ImGui()
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
	}
}

