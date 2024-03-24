#include "URenderer.h"

// --- Engine ---
#include "RenderingManager.h"
#include "World.h"
#include "ResourceManager.h"

URenderer::URenderer(AActor* a_Owner)
	: UBaseComponent(a_Owner)
{
	SetRenderingManager(World::GetRenderingManager());
	m_RenderingManager->AddRenderer(this);
}

URenderer::~URenderer()
{
}

void URenderer::OnEnabled()
{
	if (m_RenderingManager)
		m_RenderingManager->AddRenderer(this);
}

void URenderer::OnDisabled()
{
	if (m_RenderingManager)
		m_RenderingManager->RemoveRenderer(this);
}

bool URenderer::SetMesh(const char* a_MeshName, bool a_LoadTextures, bool a_FlipTextureV)
{
	m_Mesh = ResourceManager::LoadMesh(a_MeshName, nullptr, a_LoadTextures, a_FlipTextureV);
	SetMaterial(m_Mesh->getMaterial(0));
	return m_Mesh != nullptr;
}

void URenderer::SetRenderingManager(RenderingManager* a_RenderingManager)
{
	m_RenderingManager = a_RenderingManager;
}
