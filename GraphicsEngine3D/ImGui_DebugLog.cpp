#include "ImGui_DebugLog.h"

Debug::ImGui_DebugLog::ImGui_DebugLog()
{
	m_WindowName = "Debug Log";
}

Debug::ImGui_DebugLog::~ImGui_DebugLog()
{
}

void Debug::ImGui_DebugLog::PrintMessage(DebugMessage a_DebugMessage)
{
	m_DebugMessages.push_back(a_DebugMessage);
}

void Debug::ImGui_DebugLog::BeginDraw()
{
	ImGui::Begin(m_WindowName);
}

void Debug::ImGui_DebugLog::OnDraw()
{
	for (auto M : m_DebugMessages)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, M.m_Color);
		ImGui::Text(M.m_Message.c_str());
		ImGui::PopStyleColor();
	}
}

void Debug::ImGui_DebugLog::EndDraw()
{
	ImGui::End();
}
