#pragma once
// --- GLM ---
#include "glm/glm.hpp"

using namespace glm;

struct Light
{
	Light(vec3 a_Position = vec3(0.f), vec3 a_Direction = vec3(0.f), vec3 a_Colour = vec3(1.f), float a_Intensity = 1.f)
	{
		m_Position = a_Position;
		m_Direction = a_Direction;
		m_Colour = a_Colour;
		m_Intensity = a_Intensity;
	}

	vec3 m_Position;
	vec3 m_Direction;
	vec3 m_Colour;
	float m_Intensity;

	vec3 GetColour() { return m_Colour * m_Intensity; }
};

