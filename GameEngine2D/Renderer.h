#pragma once
#include "Component.h"
#include "Texture.h"

class Renderer :
    public Component
{
public:
    //Renderer() {};
    Renderer(Transform* a_Owner, const char* a_TextureFilePath, vec2 a_Size = vec2(500), vec2 a_Origin = vec2(0.5f), float a_Depth = 0.f);
    virtual ~Renderer() override;

protected:
    aie::Texture* m_Texture;
    vec2 m_Size;
    vec2 m_Origin;
    float m_Depth = 0.f;

public:
    virtual void Update(float DeltaTime) override {}
    virtual void Draw(float a_Alpha) override;
};

