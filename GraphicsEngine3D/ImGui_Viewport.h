#pragma once
#include "ImGui_Window.h"

// --- AIE --- 
#include "RenderTarget.h"

class ImGui_Viewport :
    public ImGui_Window
{
public:
	ImGui_Viewport();
	ImGui_Viewport(aie::RenderTarget* a_RenderTarget);
	virtual ~ImGui_Viewport();

	void SetRenderTarget(aie::RenderTarget* a_NewRenderTarget) { m_RenderTarget = a_NewRenderTarget; }

	int m_Target = 0;

protected:
	aie::RenderTarget* m_RenderTarget;

protected:
	virtual void BeginDraw();
	virtual void OnDraw();
	virtual void EndDraw();
};

