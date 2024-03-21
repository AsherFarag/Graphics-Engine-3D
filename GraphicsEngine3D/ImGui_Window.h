#pragma once

// --- STD ---
#include <string>

// --- ImGui ---
#include "imgui.h"

class ImGui_Window
{
public:
	ImGui_Window();
	virtual ~ImGui_Window();

	void Draw();

protected:
	size_t m_ID;

	const char* m_WindowName = "New Window";

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

