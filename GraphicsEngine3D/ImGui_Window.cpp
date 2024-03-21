#include "ImGui_Window.h"

ImGui_Window::ImGui_Window()
{
}

ImGui_Window::~ImGui_Window()
{
}

void ImGui_Window::Draw()
{
	BeginDraw();
	OnDraw();
	EndDraw();
}

void ImGui_Window::BeginDraw()
{
	ImGui::Begin(m_WindowName);
}

void ImGui_Window::OnDraw()
{
}

void ImGui_Window::EndDraw()
{
	ImGui::End();
}
