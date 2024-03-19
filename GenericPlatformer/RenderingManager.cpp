#include "RenderingManager.h"
#include "Renderer.h"

void RenderingManager::Init()
{
	m_Renderer2D = new aie::Renderer2D();
}

void RenderingManager::Update(float DeltaTime)
{
}

void RenderingManager::Draw(float a_Aplha)
{
	for (auto Renderer : m_Renderers)
	{
		Renderer->Draw(a_Aplha);
	}
}

void RenderingManager::AddRenderer(Renderer* a_Component)
{
	m_Renderers.push_back(a_Component);
}

void RenderingManager::RemoveRenderer(Renderer* a_Component)
{
	m_Renderers.erase(std::find(m_Renderers.begin(), m_Renderers.end(), a_Component));
}
