#pragma once
#include "ImGui_Window.h"

class ImGui_ResourceManager :
    public ImGui_Window
{
public:
	ImGui_ResourceManager();
	virtual ~ImGui_ResourceManager();

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

