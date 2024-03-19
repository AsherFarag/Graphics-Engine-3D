#include "URenderer.h"

// --- Engine ---
#include "RenderingManager.h"
#include "World.h"

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

void URenderer::SetRenderingManager(RenderingManager* a_RenderingManager)
{
	m_RenderingManager = a_RenderingManager;
}
