#include "URenderer.h"

// --- Engine ---
#include "RenderingManager.h"
#include "World.h"
#include "ResourceManager.h"

URenderer::URenderer(AActor* a_Owner)
	: UBaseComponent(a_Owner)
{
	
}

void URenderer::SetRenderingManager(RenderingManager* a_RenderingManager)
{
	m_RenderingManager = a_RenderingManager;
}
