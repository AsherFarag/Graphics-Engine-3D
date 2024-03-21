#pragma once
#include "AActor.h"

class ALight :
    public AActor
{
public:
    ALight(vec3 a_Colour = vec3(1.f), float a_Intensity = 1.f);
    virtual ~ALight() {}

	friend class RenderingManager;

    vec3 m_Colour;
    float m_Intensity;
    float m_FallOff = 1.f;

    vec3 GetColour() { return m_Colour * m_Intensity; }

    void SetAmbient(bool a_IsAmbient);
    bool IsAmbient() const { return m_IsAmbient; }

protected:
    bool m_IsAmbient = false;

    virtual void OnDestroyed() override;

#pragma region ImGui

protected:
	virtual void OnDraw_ImGui();

#pragma endregion
};

