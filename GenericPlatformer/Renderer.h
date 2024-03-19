#pragma once
#include "Component.h"
#include "Texture.h"

#define DEFAULT_RENDER_COLOUR 1.f, 1.f, 1.f, 1.f 

class Renderer :
    public Component
{
public:
    //Renderer() {};
    Renderer(Transform* a_Owner, const char* a_TextureFilePath, vec2 a_Size = vec2(500), vec4 a_Colour = vec4(1), vec2 a_Origin = vec2(0.5f), float a_Depth = 0.f);
    virtual ~Renderer() override;

    virtual void Update(float DeltaTime) override {}
    virtual void Draw(float a_Alpha) override;

    vec4 GetColour() { return m_Colour; }
    void SetColour(vec4 a_Colour) { m_Colour = a_Colour; }

protected:
    aie::Texture* m_Texture;
    vec2 m_Size;
    vec4 m_Colour;
    vec2 m_Origin;
    float m_Depth = 0.f;
};

