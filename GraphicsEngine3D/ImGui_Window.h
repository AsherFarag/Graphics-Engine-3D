#pragma once

// --- STD ---
#include <string>
using std::string;

// --- ImGui ---
#include "imgui.h"

class ImGui_Window
{
public:
	ImGui_Window();
	virtual ~ImGui_Window();

	void Draw();

	void SetName(const string& a_WindowName) { m_WindowName = a_WindowName; }

protected:
	size_t m_ID;

	string m_WindowName = "New Window";

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

