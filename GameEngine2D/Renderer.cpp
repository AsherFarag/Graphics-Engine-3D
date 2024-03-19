#include "Renderer.h"

Renderer::Renderer(Transform* a_Owner, const char* a_TextureFilePath, vec2 a_Size, vec2 a_Origin, float a_Depth)
	: Component(a_Owner)
{
	World::GetRenderer()->AddRenderer(this);

	m_Texture = new aie::Texture(a_TextureFilePath);
	m_Size = a_Size;
	m_Origin = a_Origin;
	m_Depth = a_Depth;
}

Renderer::~Renderer()
{
	World::GetRenderer()->RemoveRenderer(this);

	delete m_Texture;
}

void Renderer::Draw(float a_Alpha)
{
	if (m_Texture)
	{
		World::GetRenderer2D()->setUVRect(0, 0, 1, 1);
		World::GetRenderer2D()->drawSprite(
			m_Texture,
			GetPosition().x, GetPosition().y,
			m_Size.x, m_Size.y,
			GetRotation(),
			m_Depth,
			m_Origin.x, m_Origin.y);
	}
}
