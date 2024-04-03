#pragma once
#include "ImGui_Window.h"

// --- AIE --- 
#include "AIERenderTarget.h"

class ImGui_Viewport :
    public ImGui_Window
{
public:
	ImGui_Viewport();
	ImGui_Viewport(aie::AIERenderTarget* a_RenderTarget);
	virtual ~ImGui_Viewport();

	void SetRenderTarget(aie::AIERenderTarget* a_NewRenderTarget) { m_RenderTarget = a_NewRenderTarget; }

protected:
	aie::AIERenderTarget* m_RenderTarget;

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

