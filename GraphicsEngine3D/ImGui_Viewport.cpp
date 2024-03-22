#include "ImGui_Viewport.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

ImGui_Viewport::ImGui_Viewport()
	: ImGui_Viewport(nullptr)
{
}

ImGui_Viewport::ImGui_Viewport(aie::RenderTarget* a_RenderTarget)
{
	m_WindowName = "Viewport";
	SetRenderTarget(a_RenderTarget);
}

ImGui_Viewport::~ImGui_Viewport()
{
}

void ImGui_Viewport::BeginDraw()
{
	// Bind the Render Target Texture to a slot
	m_RenderTarget->getTarget(0).bind(0);

	// Create a small border around the Image
	ImGui::GetStyle().WindowPadding = ImVec2(3.f, 3.f);

	ImGui::Begin(m_WindowName, nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
}

void ImGui_Viewport::OnDraw()
{
	// Access the ImGui window size
	const float window_width = ImGui::GetContentRegionAvail().x;
	const float window_height = ImGui::GetContentRegionAvail().y;

	// Rescale the framebuffer to the actual window size and reset the glViewport 
	m_RenderTarget->rescaleFrameBuffer(0, window_width, window_height);
	glViewport(0, 0, window_width, window_height);

	ImGui::Image(
		(ImTextureID)m_RenderTarget->getTarget(0).getHandle(),
		ImGui::GetContentRegionAvail(),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	// Window Size Tool Tip
	if (ImGui::IsWindowHovered())
	{
		if (ImGui::BeginTooltip())
		{
			std::string SizeString = std::to_string((int)window_width) + " , " + std::to_string((int)window_height);
			ImGui::Text(SizeString.c_str());
			ImGui::EndTooltip();
		}
	}
}

void ImGui_Viewport::EndDraw()
{
	ImGui::End();
}
