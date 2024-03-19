#include "Renderer.h"

#include <iostream>

Renderer::Renderer(Transform* a_Owner, const char* a_TextureFilePath, vec2 a_Size, vec4 a_Colour, vec2 a_Origin, float a_Depth)
	: Component(a_Owner)
{
	World::GetRenderer()->AddRenderer(this);

	m_Texture = new aie::Texture(a_TextureFilePath);
	if (m_Texture->getHandle() == 0)
		std::cout << "Could not load Texture at: \"" << a_TextureFilePath << "\"" << std::endl;

	m_Size = a_Size;
	m_Colour = a_Colour;
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
	if (m_Texture->getHandle() != 0)
	{
		World::GetRenderer2D()->setUVRect(0, 0, 1, 1);
		World::GetRenderer2D()->setRenderColour(m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a);
		World::GetRenderer2D()->drawSprite(
			m_Texture,
			GetPosition().x, GetPosition().y,
			m_Size.x, m_Size.y,
			GetRotation(),
			m_Depth,
			m_Origin.x, m_Origin.y);
		World::GetRenderer2D()->setRenderColour(DEFAULT_RENDER_COLOUR);
	}
}
