#include "ALight.h"

// --- Engine ---
#include "RenderingManager.h"

ALight::ALight(vec3 a_Colour, float a_Intensity)
{
	m_Name = "Light";
	m_Colour = a_Colour;
	m_Intensity = a_Intensity;

	World::GetRenderingManager()->AddLight(this);
}

void ALight::OnDraw_ImGui()
{
	AActor::OnDraw_ImGui();

	ImGui::ColorEdit3("Colour", &m_Colour[0]);
	ImGui::DragFloat("Intensity", &m_Intensity);
}
