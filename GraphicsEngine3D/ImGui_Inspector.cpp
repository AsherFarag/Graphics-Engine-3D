#include "ImGui_Inspector.h"

ImGui_Inspector::ImGui_Inspector()
{
	m_WindowName = "Inspector##" + std::to_string(m_ID);
}

void ImGui_Inspector::BeginDraw()
{
	ImGui::Begin(m_WindowName.c_str());
}

void ImGui_Inspector::OnDraw()
{
	typeid(InspectedActor)
	m_Inspector.DrawProperty( , );
}

void ImGui_Inspector::EndDraw()
{
	ImGui::End();
}
