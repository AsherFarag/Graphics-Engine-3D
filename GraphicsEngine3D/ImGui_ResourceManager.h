#pragma once
#include "ImGui_Window.h"

#include "ResourceManager.h"

class ImGui_ResourceManager :
    public ImGui_Window
{
public:
	ImGui_ResourceManager();
	virtual ~ImGui_ResourceManager();

protected:
	ResourceManager* m_ResourceManager = nullptr;

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

