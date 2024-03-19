#pragma once
#include "AActor.h"

class ALight :
    public AActor
{
public:
    ALight(vec3 a_Colour = vec3(1.f), float a_Intensity = 1.f);

	friend class RenderingManager;

    vec3 m_Colour;
    float m_Intensity;

    vec3 GetColour() { return m_Colour * m_Intensity; }

#pragma region ImGui

protected:
	virtual void OnDraw_ImGui();

#pragma endregion
};

