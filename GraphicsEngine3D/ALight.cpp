#include "ALight.h"

// --- Engine ---
#include "RenderingManager.h"

ALight::ALight(vec3 a_Colour, float a_Intensity)
{
	m_Name = "Light";
	m_Colour = a_Colour;
	m_Intensity = a_Intensity;

	if (World::GetRenderingManager()->AddLight(this) == false)
	{
		Destroy();
	}
}

void ALight::SetAmbient(bool a_IsAmbient)
{
	// If the new state is the same as the old state,
	// Do nothing
	//if (a_IsAmbient == m_IsAmbient)
	//	return;

	m_IsAmbient = a_IsAmbient;
	//if (m_IsAmbient)
	//{
	//	World::GetRenderingManager()->SetAmbientLight(this);
	//}
}

void ALight::OnDestroyed()
{
	if (IsAmbient())
		World::GetRenderingManager()->SetAmbientLight(nullptr);
	else
		World::GetRenderingManager()->RemoveLight(this);
}

void ALight::OnDraw_ImGui()
{
	AActor::OnDraw_ImGui();

	ImGui::ColorEdit3("Colour", &m_Colour[0]);
	ImGui::DragFloat("Intensity", &m_Intensity, 0.01f);
	m_Intensity = glm::max(0.f, m_Intensity);

	ImGui::SliderFloat("Fall Off", &m_FallOff, 1.f, 5.f);

	bool NewAmbient = m_IsAmbient;
	if (ImGui::Checkbox("Is Ambient", &NewAmbient))
	{
		SetAmbient(NewAmbient);
		NewAmbient ? World::GetRenderingManager()->SetAmbientLight(this) : World::GetRenderingManager()->SetAmbientLight(nullptr);
	}
}
