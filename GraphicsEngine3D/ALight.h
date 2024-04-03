#pragma once
#include "AActor.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

class ALight :
    public AActor
{
    INSPECTABLE;

public:
    ALight(vec3 a_Colour = vec3(1.f), float a_Intensity = 1.f);
    virtual ~ALight() {}

	friend class RenderingManager;

    vec3 GetTrueColour() { return m_Colour; }
    vec3 GetColour() { return m_Colour * m_Intensity; }
    void SetColour(vec3 NewColour) { m_Colour = NewColour; }

    bool IsAmbient() const { return m_IsAmbient; }
    void SetAmbient(bool a_IsAmbient);

    const float GetIntensity() { return m_Intensity; }
    void SetIntensity(float NewIntensity) { m_Intensity = NewIntensity; }

    const float GetFallOff() { return m_FallOff; }
    void SetFallOff(float NewFallOff) { m_FallOff = NewFallOff; }

protected:
    bool m_IsAmbient = false;
    vec3 m_Colour;
    float m_Intensity;
    float m_FallOff = 1.f;

    vec2 m_ShadowResolution;

public:
    GLuint m_DepthMap{ 0 };
    void GenerateDepthMap(unsigned int a_Resolution);

protected:
    virtual void OnDestroyed() override;

#pragma region ImGui

protected:
	virtual void OnDraw_ImGui();

#pragma endregion
};

